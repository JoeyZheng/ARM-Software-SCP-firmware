/*
 * Arm SCP/MCP Software
 * Copyright (c) 2024, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "ccsm_drv.h"

#include <mod_ccsm.h>
#include <mod_clock.h>
#include <mod_power_domain.h>

#include <fwk_id.h>
#include <fwk_macros.h>
#include <fwk_mm.h>
#include <fwk_module.h>
#include <fwk_status.h>

#include <stddef.h>
#include <stdint.h>

/* Device context */
struct ccsm_dev_ctx {
    bool initialized;
    uint64_t current_rate_nom;
    uint64_t current_rate_fb;
    enum mod_clock_state current_state;
    struct mod_ccsm_pll_api *pll_api;
    uint32_t base_address;
    const struct mod_ccsm_dev_config *config;
};

/* Module context */
struct ccsm_ctx {
    struct ccsm_dev_ctx *dev_ctx_table;
    unsigned int dev_count;
};

static struct ccsm_ctx module_ctx;

/*
 * Static helper functions
 */
static int compare_rate_entry(const void *a, const void *b)
{
    struct mod_ccsm_clock_rate *key = (struct mod_ccsm_clock_rate *)a;
    struct mod_ccsm_clock_rate *element = (struct mod_ccsm_clock_rate *)b;

    return (key->rate_nom - element->rate_nom);
}

static int get_indexed_rates(
    struct ccsm_dev_ctx *ctx,
    uint64_t target_rate,
    struct mod_ccsm_clock_rate *clock_rates)
{
    struct mod_ccsm_clock_rate *current_rate_entry;

    if (ctx == NULL)
        return FWK_E_PARAM;
    if (clock_rates == NULL)
        return FWK_E_PARAM;

    /* Perform a binary search to find the entry matching the requested rate */
    current_rate_entry = (struct mod_ccsm_clock_rate *)bsearch(
        &target_rate,
        ctx->config->rate_table,
        ctx->config->rate_count,
        sizeof(struct mod_ccsm_clock_rate),
        compare_rate_entry);

    if (current_rate_entry == NULL)
        return FWK_E_PARAM;

    clock_rates->rate_nom = current_rate_entry->rate_nom;
    clock_rates->rate_fb = current_rate_entry->rate_fb;
    return FWK_SUCCESS;
}

static int ccsm_clock_select_rates(
    struct ccsm_dev_ctx *ctx,
    uint64_t rate,
    enum mod_clock_round_mode round_mode,
    struct mod_ccsm_clock_rate *clock_rates)
{
    uint64_t rate_rounded, rate_rounded_alt;

    if (ctx->config->clock_type == MOD_CCSM_CLOCK_TYPE_INDEXED) {
        return get_indexed_rates(ctx, rate, clock_rates);
    }

    if ((rate % ctx->config->min_step) > 0) {
        switch (round_mode) {
        case MOD_CLOCK_ROUND_MODE_NONE:
            return FWK_E_RANGE;
        case MOD_CLOCK_ROUND_MODE_NEAREST:
            rate_rounded = FWK_ALIGN_PREVIOUS(rate, ctx->config->min_step);
            rate_rounded_alt = FWK_ALIGN_NEXT(rate, ctx->config->min_step);

            /* Select the rounded rate that is closest to the given rate */
            if ((rate_rounded - rate_rounded_alt) >
                (rate_rounded_alt - rate_rounded)) {
                clock_rates->rate_nom = rate_rounded_alt;
            } else {
                clock_rates->rate_nom = rate_rounded;
            }
            break;
        case MOD_CLOCK_ROUND_MODE_DOWN:
            clock_rates->rate_nom =
                FWK_ALIGN_PREVIOUS(rate, ctx->config->min_step);
            break;
        case MOD_CLOCK_ROUND_MODE_UP:
            clock_rates->rate_nom = FWK_ALIGN_NEXT(rate, ctx->config->min_step);
            break;
        default:
            return FWK_E_SUPPORT;
        }
    } else
        clock_rates->rate_nom = rate;

    if (clock_rates->rate_nom < ctx->config->min_rate)
        return FWK_E_RANGE;
    if (clock_rates->rate_nom > ctx->config->max_rate)
        return FWK_E_RANGE;

    /* Set fallback as maximum of 90% nominal freq. */
    if (clock_rates->rate_nom > ctx->config->min_rate_fb_enable) {
        clock_rates->rate_fb = FWK_ALIGN_PREVIOUS(
            9 * (clock_rates->rate_nom / 10), ctx->config->min_step);
    } else {
        clock_rates->rate_fb = 0;
    }
    return FWK_SUCCESS;
}

static int ccsm_mod_set_configuration(
    struct ccsm_dev_ctx *ctx,
    uint32_t mod_id,
    const struct mod_ccsm_mod_config *config)
{
    uint32_t value;

    if (mod_id > ctx->config->modulator_count) {
        return FWK_E_RANGE;
    }

    if ((config->numerator_regular < config->numerator_oc) |
        (config->denominator < config->numerator_oc) |
        (config->denominator < config->numerator_regular)) {
        return FWK_E_PARAM;
    }

    value =
        (((config->numerator_regular << MOD_CCSM_MOD_NUMERATOR) &
          MOD_CCSM_MOD_NUMERATOR_MASK) |
         ((config->numerator_oc << MOD_CCSM_MOD_NUMERATOR_PMIC_OC) &
          MOD_CCSM_MOD_NUMERATOR_PMIC_OC_MASK) |
         ((config->denominator << MOD_CCSM_MOD_DENOMINATOR) &
          MOD_CCSM_MOD_DENOMINATOR_MASK));

    return ccsm_drv_set_mod_configuration(ctx->base_address, mod_id, value);
}

static int ccsm_mod_get_configuration(
    struct ccsm_dev_ctx *ctx,
    uint32_t mod_id,
    struct mod_ccsm_mod_config *config)
{
    int status;
    uint32_t data;

    if (mod_id > ctx->config->modulator_count) {
        return FWK_E_PARAM;
    }

    status = ccsm_drv_get_mod_configuration(ctx->base_address, mod_id, &data);

    if (status != FWK_SUCCESS) {
        return status;
    }

    config->numerator_regular =
        ((data & MOD_CCSM_MOD_NUMERATOR_MASK) >> MOD_CCSM_MOD_NUMERATOR);
    config->numerator_oc =
        ((data & MOD_CCSM_MOD_NUMERATOR_PMIC_OC_MASK) >>
         MOD_CCSM_MOD_NUMERATOR_PMIC_OC);
    config->denominator =
        ((data & MOD_CCSM_MOD_DENOMINATOR_MASK) >> MOD_CCSM_MOD_DENOMINATOR);

    return FWK_SUCCESS;
}

static int ccsm_dm_set_configuration(
    struct ccsm_dev_ctx *ctx,
    const struct mod_ccsm_dm_config *config)
{
    uint32_t value;

    value =
        (config->strategy | config->dd |
         ((config->transition_pause << MOD_CCSM_DM_TRANSITION_PAUSE) &
          MOD_CCSM_DM_TRANSITION_PAUSE_MASK) |
         ((config->mitigation_duration << MOD_CCSM_DM_MITIGATION_DURATION) &
          MOD_CCSM_DM_MITIGATION_DURATION_MASK));

    return ccsm_drv_set_dm_configuration(ctx->base_address, value);
}

static int ccsm_dm_get_configuration(
    struct ccsm_dev_ctx *ctx,
    struct mod_ccsm_dm_config *config)
{
    int status;
    uint32_t value;

    status = ccsm_drv_get_dm_configuration(ctx->base_address, &value);
    if (status != FWK_SUCCESS) {
        return status;
    }

    if (value & MOD_CCSM_DM_NOM_ONLY) {
        config->strategy = MOD_CCSM_DM_NOM_ONLY;
    } else if (value & MOD_CCSM_DM_SW_FB) {
        config->strategy = MOD_CCSM_DM_SW_FB;
    } else if (value & MOD_CCSM_DM_CLK_ON_DVFS) {
        config->strategy = MOD_CCSM_DM_CLK_ON_DVFS;
    } else {
        config->strategy = MOD_CCSM_DM_STOP_CLK;
    }

    if (value & MOD_CCSM_DM_TPIP) {
        config->dd = MOD_CCSM_DM_TPIP;
    } else {
        config->dd = MOD_CCSM_DM_ARM_DD;
    }

    if (value & MOD_CCSM_DM_SW_SOFF_STOP) {
        config->soff = MOD_CCSM_DM_SW_SOFF_STOP;
    } else {
        config->soff = MOD_CCSM_DM_SW_SOFF_IGNORE;
    }

    config->transition_pause =
        ((value & MOD_CCSM_DM_TRANSITION_PAUSE_MASK) >>
         MOD_CCSM_DM_TRANSITION_PAUSE);

    config->mitigation_duration =
        ((value & MOD_CCSM_DM_MITIGATION_DURATION_MASK) >>
         MOD_CCSM_DM_MITIGATION_DURATION);
    return FWK_SUCCESS;
}

/*! Get register values from PLL driver and update CCSM. */
static int ccsm_clock_update_registers(
    struct ccsm_dev_ctx *ctx,
    struct mod_ccsm_clock_rate *clock_rates)
{
    int status;
    struct mod_ccsm_pll_reg_values pll_reg_values;

    if (!ctx->initialized) {
        status = ctx->pll_api->gen_static_registers(
            ctx->config->pll_id, &pll_reg_values);
        if (status != FWK_SUCCESS) {
            return status;
        }

        status = ccsm_drv_set_pll_static_settings(
            ctx->base_address, MOD_CCSM_PLL_0, pll_reg_values.pll_0_static);
        if (status != FWK_SUCCESS) {
            return status;
        }
        status = ccsm_drv_set_pll_static_settings(
            ctx->base_address, MOD_CCSM_PLL_1, pll_reg_values.pll_1_static);
        if (status != FWK_SUCCESS) {
            return status;
        }
    }

    status = ctx->pll_api->gen_dynamic_registers(
        ctx->config->pll_id, clock_rates, &pll_reg_values);
    if (status != FWK_SUCCESS) {
        return status;
    }

    status = ccsm_drv_set_pll_dynamic_settings(
        ctx->base_address,
        MOD_CCSM_PLL_NOMINAL,
        pll_reg_values.pll_nom_settings_0,
        pll_reg_values.pll_nom_settings_1);
    if (status != FWK_SUCCESS) {
        return status;
    }

    if (clock_rates->rate_fb != 0) {
        status = ccsm_drv_set_pll_dynamic_settings(
            ctx->base_address,
            MOD_CCSM_PLL_FALLBACK,
            pll_reg_values.pll_fb_settings_0,
            pll_reg_values.pll_fb_settings_1);
    }

    return status;
}

/*! Handle CCSM initialization. */
static int ccsm_device_setup(struct ccsm_dev_ctx *ctx, uint64_t rate)
{
    int status;
    uint32_t mod_n;
    struct mod_ccsm_clock_rate clock_rates;
    struct mod_ccsm_mod_config mod_config;
    struct mod_ccsm_dm_config dm_config;

    // Select appropriate rates for nominal and fallback
    status = ccsm_clock_select_rates(
        ctx, rate, MOD_CLOCK_ROUND_MODE_NEAREST, &clock_rates);
    if (status != FWK_SUCCESS) {
        return status;
    }

    status = ccsm_mod_get_configuration(ctx, 0, &mod_config);
    if (status != FWK_SUCCESS) {
        return status;
    }
    status = ccsm_dm_get_configuration(ctx, &dm_config);
    if (status != FWK_SUCCESS) {
        return status;
    }

    status = ccsm_dm_set_configuration(ctx, ctx->config->dm_config);
    if (status != FWK_SUCCESS) {
        return status;
    }

    status = ccsm_drv_req_set_dm(ctx->base_address);
    if (status != FWK_SUCCESS) {
        return status;
    }

    for (mod_n = 0; mod_n < ctx->config->modulator_count; mod_n++) {
        status =
            ccsm_mod_set_configuration(ctx, mod_n, ctx->config->mod_config);
        if (status != FWK_SUCCESS) {
            return status;
        }
    }
    status = ccsm_drv_req_set_mod(ctx->base_address);
    if (status != FWK_SUCCESS) {
        return status;
    }

    status = ccsm_clock_update_registers(ctx, &clock_rates);
    if (status != FWK_SUCCESS) {
        return status;
    }

    status = ccsm_drv_req_set_dvfs(ctx->base_address);
    if (status != FWK_SUCCESS) {
        return status;
    }

    ctx->initialized = true;
    ctx->current_rate_nom = clock_rates.rate_nom;
    ctx->current_rate_fb = clock_rates.rate_fb;

    return FWK_SUCCESS;
}

/*
 * Clock driver API functions
 */

static int ccsm_clock_set_rate(
    fwk_id_t dev_id,
    uint64_t rate,
    enum mod_clock_round_mode round_mode)
{
    struct ccsm_dev_ctx *ctx;
    struct mod_ccsm_clock_rate clock_rates;
    struct mod_ccsm_dm_config dm_config;
    int status;

    if (!fwk_module_is_valid_element_id(dev_id))
        return FWK_E_PARAM;

    ctx = module_ctx.dev_ctx_table + fwk_id_get_element_idx(dev_id);
    if (ctx->initialized == false)
        return ccsm_device_setup(ctx, rate);

    if (ctx->current_state == MOD_CLOCK_STATE_STOPPED)
        return FWK_E_PWRSTATE;

    // Select appropriate rates for nominal and fallback
    status = ccsm_clock_select_rates(ctx, rate, round_mode, &clock_rates);
    if (status != FWK_SUCCESS) {
        return status;
    }

    if (clock_rates.rate_nom == ctx->current_rate_nom) {
        return FWK_SUCCESS;
    }

    status = ccsm_clock_update_registers(ctx, &clock_rates);
    if (status != FWK_SUCCESS) {
        return status;
    }

    if (ctx->config->dm_config->strategy == MOD_CCSM_DM_SW_FB) {
        status = ccsm_dm_get_configuration(ctx, &dm_config);
        if (status != FWK_SUCCESS) {
            return status;
        }
        if ((clock_rates.rate_fb == 0) && (dm_config.strategy == MOD_CCSM_DM_SW_FB)) {
            /* Switch to nominal only so fallback can be disabled */
            dm_config.strategy = MOD_CCSM_DM_NOM_ONLY;
            status = ccsm_dm_set_configuration(ctx, &dm_config);
            if (status != FWK_SUCCESS) {
                return status;
            }
        } else if ((ctx->current_rate_fb == 0) && (dm_config.strategy == MOD_CCSM_DM_NOM_ONLY)) {
            /* Restore switch to fallback configuration */
            dm_config.strategy = MOD_CCSM_DM_SW_FB;
            status = ccsm_dm_set_configuration(ctx, &dm_config);
            if (status != FWK_SUCCESS) {
                return status;
            }
        }
    }

    // Send DVFS command as required
    if (clock_rates.rate_nom > ctx->current_rate_nom) {
        status = ccsm_drv_req_dvfs_go_up(ctx->base_address);
    } else {
        status = ccsm_drv_req_dvfs_go_dn(ctx->base_address);
    }
    if (status != FWK_SUCCESS) {
        return status;
    }

    ctx->current_rate_nom = clock_rates.rate_nom;
    ctx->current_rate_fb = clock_rates.rate_fb;

    return FWK_SUCCESS;
}

static int ccsm_clock_get_rate(fwk_id_t dev_id, uint64_t *rate)
{
    struct ccsm_dev_ctx *ctx;

    if (!fwk_module_is_valid_element_id(dev_id))
        return FWK_E_PARAM;
    if (rate == NULL)
        return FWK_E_PARAM;

    ctx = module_ctx.dev_ctx_table + fwk_id_get_element_idx(dev_id);

    *rate = ctx->current_rate_nom;

    return FWK_SUCCESS;
}

static int ccsm_clock_get_rate_from_index(
    fwk_id_t dev_id,
    unsigned int rate_index,
    uint64_t *rate)
{
    struct ccsm_dev_ctx *ctx;

    if (!fwk_module_is_valid_element_id(dev_id))
        return FWK_E_PARAM;
    if (rate == NULL)
        return FWK_E_PARAM;

    ctx = module_ctx.dev_ctx_table + fwk_id_get_element_idx(dev_id);

    if (ctx->config->clock_type == MOD_CCSM_CLOCK_TYPE_INDEXED) {
        if (rate_index >= ctx->config->rate_count)
            return FWK_E_PARAM;
        *rate = ctx->config->rate_table[rate_index].rate_nom;
        return FWK_SUCCESS;
    } else
        return FWK_E_SUPPORT;
}

static int ccsm_clock_set_state(fwk_id_t dev_id, enum mod_clock_state state)
{
    if (state == MOD_CLOCK_STATE_RUNNING)
        return FWK_SUCCESS;

    /* PLLs can only be stopped by a parent power domain state change. */
    return FWK_E_SUPPORT;
}

static int ccsm_clock_get_state(fwk_id_t dev_id, enum mod_clock_state *state)
{
    struct ccsm_dev_ctx *ctx;

    if (!fwk_module_is_valid_element_id(dev_id))
        return FWK_E_PARAM;
    if (state == NULL)
        return FWK_E_PARAM;

    ctx = module_ctx.dev_ctx_table + fwk_id_get_element_idx(dev_id);
    *state = ctx->current_state;

    return FWK_SUCCESS;
}

static int ccsm_clock_get_range(fwk_id_t dev_id, struct mod_clock_range *range)
{
    struct ccsm_dev_ctx *ctx;

    if (!fwk_module_is_valid_element_id(dev_id))
        return FWK_E_PARAM;
    if (range == NULL)
        return FWK_E_PARAM;

    ctx = module_ctx.dev_ctx_table + fwk_id_get_element_idx(dev_id);

    if (ctx->config->clock_type == MOD_CCSM_CLOCK_TYPE_INDEXED) {
        range->rate_type = MOD_CLOCK_RATE_TYPE_DISCRETE;
        range->min = ctx->config->rate_table[0].rate_nom;
        range->max =
            ctx->config->rate_table[ctx->config->rate_count - 1].rate_nom;
        range->rate_count = ctx->config->rate_count;
    } else {
        range->rate_type = MOD_CLOCK_RATE_TYPE_CONTINUOUS;
        range->min = ctx->config->min_rate;
        range->max = ctx->config->max_rate;
        range->step = ctx->config->min_step;
    }
    return FWK_SUCCESS;
}

static int ccsm_clock_power_state_change(fwk_id_t dev_id, unsigned int state)
{
    uint64_t rate;
    struct ccsm_dev_ctx *ctx;

    ctx = module_ctx.dev_ctx_table + fwk_id_get_element_idx(dev_id);

    if (state != MOD_PD_STATE_ON)
        return FWK_SUCCESS;

    ctx->current_state = MOD_CLOCK_STATE_RUNNING;

    if (ctx->initialized) {
        /* Restore the previous rate */
        rate = ctx->current_rate_nom;
    } else {
        /* Initialize the PLL to its default rate */
        rate = ctx->config->clock_default->rate_nom;
    }

    return ccsm_device_setup(ctx, rate);
}

static int ccsm_clock_power_state_pending_change(
    fwk_id_t dev_id,
    unsigned int current_state,
    unsigned int next_state)
{
    struct ccsm_dev_ctx *ctx;

    ctx = module_ctx.dev_ctx_table + fwk_id_get_element_idx(dev_id);

    if (next_state == MOD_PD_STATE_OFF) {
        /* Just mark the PLL as stopped */
        ctx->current_state = MOD_CLOCK_STATE_STOPPED;
    }

    return FWK_SUCCESS;
}

static const struct mod_clock_drv_api api_ccsm_clock = {
    .set_rate = ccsm_clock_set_rate,
    .get_rate = ccsm_clock_get_rate,
    .get_rate_from_index = ccsm_clock_get_rate_from_index,
    .set_state = ccsm_clock_set_state,
    .get_state = ccsm_clock_get_state,
    .get_range = ccsm_clock_get_range,
    .process_power_transition = ccsm_clock_power_state_change,
    .process_pending_power_transition = ccsm_clock_power_state_pending_change,
};

/*
 * Framework handler functions
 */
static int ccsm_init(
    fwk_id_t module_id,
    unsigned int element_count,
    const void *data)
{
    module_ctx.dev_count = element_count;

    if (element_count == 0)
        return FWK_SUCCESS;

    module_ctx.dev_ctx_table =
        fwk_mm_calloc(element_count, sizeof(struct ccsm_dev_ctx));
    return FWK_SUCCESS;
}

static int ccsm_element_init(
    fwk_id_t element_id,
    unsigned int unused,
    const void *data)
{
    struct ccsm_dev_ctx *ctx;
    struct mod_ccsm_clock_rate rate_entry;
    const struct mod_ccsm_dev_config *dev_config = data;
    uint32_t element_idx = fwk_id_get_element_idx(element_id);
    uint32_t rate_n;

    if (element_idx >= module_ctx.dev_count) {
        return FWK_E_DATA;
    }

    ctx = module_ctx.dev_ctx_table + element_idx;

    ctx->config = dev_config;

    if ((ctx->config->max_rate > MOD_CCSM_CLOCK_MAX_RATE) ||
        (ctx->config->min_rate < MOD_CCSM_CLOCK_MIN_RATE) ||
        (ctx->config->min_step < MOD_CCSM_CLOCK_MIN_INTERVAL)) {
        return FWK_E_SUPPORT;
    }

    for (rate_n = 0; rate_n < ctx->config->rate_count; rate_n++) {
        rate_entry = ctx->config->rate_table[rate_n];
        if (rate_entry.rate_fb > rate_entry.rate_nom) {
            return FWK_E_DATA;
        }
        if ((rate_entry.rate_nom > ctx->config->max_rate) ||
            (rate_entry.rate_nom < ctx->config->min_rate)) {
            return FWK_E_SUPPORT;
        }
    }

    ctx->base_address = dev_config->base_address;
    ctx->current_rate_nom = dev_config->clock_default->rate_nom;
    ctx->initialized = false;
    ctx->current_state = MOD_CLOCK_STATE_STOPPED;
    return FWK_SUCCESS;
}

static int ccsm_bind(fwk_id_t id, unsigned int round)
{
    int status;
    struct ccsm_dev_ctx *ctx;
    const struct mod_ccsm_dev_config *config;

    if (round == 1) {
        return FWK_SUCCESS;
    }

    if (fwk_module_is_valid_module_id(id)) {
        return FWK_SUCCESS;
    }

    ctx = module_ctx.dev_ctx_table + fwk_id_get_element_idx(id);
    config = ctx->config;

    /* Bind to the clock's PLL driver. */
    status = fwk_module_bind(config->pll_id, config->pll_api_id, &ctx->pll_api);

    return status;
}

static int ccsm_process_bind_request(
    fwk_id_t requester_id,
    fwk_id_t id,
    fwk_id_t api_type,
    const void **api)
{
    switch (fwk_id_get_api_idx(api_type)) {
    case MOD_CCSM_CLOCK_API:
        *api = &api_ccsm_clock;
        break;
    default:
        return FWK_E_PARAM;
    }

    return FWK_SUCCESS;
}

const struct fwk_module module_ccsm = {
    .type = FWK_MODULE_TYPE_DRIVER,
    .api_count = MOD_CCSM_API_COUNT,
    .event_count = 0,
    .init = ccsm_init,
    .element_init = ccsm_element_init,
    .bind = ccsm_bind,
    .process_bind_request = ccsm_process_bind_request,
};
