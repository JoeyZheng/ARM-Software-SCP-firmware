/*
 * Arm SCP/MCP Software
 * Copyright (c) 2024, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "scp_unity.h"
#include "unity.h"

#ifdef TEST_ON_TARGET
#    include <fwk_id.h>
#    include <fwk_module.h>
#else
#    include <Mockfwk_id.h>
#    include <Mockfwk_module.h>
#endif

#include <Mockmod_ccsm_extra.h>

#include <mod_ccsm.h>

#include <fwk_element.h>
#include <fwk_macros.h>

#include UNIT_TEST_SRC

#define BUILD_HAS_BASE_PROTOCOL

#define FAKE_MODULE_ID 0x5

enum fake_ccsm {
    FAKE_CCSM_IDX_CONTINUOUS,
    FAKE_CCSM_IDX_INDEXED,
    FAKE_CCSM_IDX_COUNT,
};

#define FAKE_API_IDX_CCSM_SC_PLL 0x3

static const struct mod_ccsm_clock_rate clock_config_default = { 1537 * FWK_MHZ,
                                                                 0 };

static const struct mod_ccsm_dm_config dm_config_default = {
    .strategy = MOD_CCSM_DM_NOM_ONLY,
    .dd = MOD_CCSM_DM_ARM_DD,
    .soff = MOD_CCSM_DM_SW_SOFF_STOP,
    .transition_pause = 6,
    .mitigation_duration = 144
};

static const struct mod_ccsm_mod_config mod_config_default = {
    .numerator_oc = 1,
    .numerator_regular = 1,
    .denominator = 1
};

static const struct mod_ccsm_clock_rate rate_table_fake[] = {
    { /* Super Underdrive */
      .rate_nom = 300 * FWK_MHZ,
      .rate_fb = 0 },
    { /* Nominal */
      .rate_nom = 1537 * FWK_MHZ,
      .rate_fb = 0 },
    { /* Overdrive */
      .rate_nom = 1844 * FWK_MHZ,
      .rate_fb = 1660 * FWK_MHZ },
    { /* Super Overdrive */
      .rate_nom = 2000 * FWK_MHZ,
      .rate_fb = 1800 * FWK_MHZ },
};

static const struct fwk_element element_table[] = {
    [FAKE_CCSM_IDX_CONTINUOUS] = { .name = "CCSM continuous",
                                   .data =
                                       &(struct mod_ccsm_dev_config){
                                           .clock_type =
                                               MOD_CCSM_CLOCK_TYPE_NON_INDEXED,
                                           .min_rate_fb_enable = 1700 * FWK_MHZ,
                                           .min_rate = MOD_CCSM_CLOCK_MIN_RATE,
                                           .max_rate = MOD_CCSM_CLOCK_MAX_RATE,
                                           .min_step = 1 * FWK_MHZ,
                                           .base_address = 0xBA5E,
                                           .clock_default =
                                               &clock_config_default,
                                           .dm_config = &dm_config_default,
                                           .mod_config = &mod_config_default,
                                           .modulator_count = 1,
                                           .pll_id = FWK_ID_ELEMENT_INIT(
                                               FAKE_MODULE_ID,
                                               FAKE_API_IDX_CCSM_SC_PLL),
                                           .pll_api_id = FWK_ID_API_INIT(
                                               FAKE_MODULE_ID,
                                               FAKE_API_IDX_CCSM_SC_PLL),
                                       } },
    [FAKE_CCSM_IDX_INDEXED] = { .name = "CCSM indexed",
                                .data =
                                    &(struct mod_ccsm_dev_config){
                                        .clock_type =
                                            MOD_CCSM_CLOCK_TYPE_INDEXED,
                                        .base_address = 0x1DBA5E,
                                        .clock_default = &clock_config_default,
                                        .dm_config = &dm_config_default,
                                        .mod_config = &mod_config_default,
                                        .modulator_count = 1,
                                        .pll_id = FWK_ID_ELEMENT_INIT(
                                            FAKE_MODULE_ID,
                                            FAKE_API_IDX_CCSM_SC_PLL),
                                        .pll_api_id = FWK_ID_API_INIT(
                                            FAKE_MODULE_ID,
                                            FAKE_API_IDX_CCSM_SC_PLL),
                                        .rate_table = rate_table_fake,
                                        .rate_count = 4 } },
    [FAKE_CCSM_IDX_COUNT] = { 0 },
};

static const struct fwk_element *get_element_table(fwk_id_t module_id)
{
    return element_table;
}

struct fwk_module_config config_ccsm = {
    .elements = FWK_MODULE_DYNAMIC_ELEMENTS(get_element_table),
};

struct mod_ccsm_pll_api *pll_api = &(struct mod_ccsm_pll_api){
    .name = "Dummy PLL",
    .gen_dynamic_registers = mod_ccsm_pll_gen_dynamic_registers,
    .gen_static_registers = mod_ccsm_pll_gen_static_registers
};

void setUp(void)
{
    struct ccsm_dev_ctx *ctx;

    module_ctx.dev_ctx_table =
        fwk_mm_calloc(FAKE_CCSM_IDX_COUNT, sizeof(module_ctx.dev_ctx_table[0]));
    module_ctx.dev_count = FAKE_CCSM_IDX_COUNT;

    ctx = &module_ctx.dev_ctx_table[FAKE_CCSM_IDX_CONTINUOUS];
    ctx->config =
        (struct mod_ccsm_dev_config *)element_table[FAKE_CCSM_IDX_CONTINUOUS]
            .data;
    ctx->initialized = true;
    ctx->current_rate_nom = ctx->config->clock_default->rate_nom;
    ctx->current_rate_fb = ctx->config->clock_default->rate_fb;
    ctx->pll_api = pll_api;
    ctx->base_address = ctx->config->base_address;
    ctx->current_state = MOD_CLOCK_STATE_RUNNING;

    ctx = &module_ctx.dev_ctx_table[FAKE_CCSM_IDX_INDEXED];
    ctx->config =
        (struct mod_ccsm_dev_config *)element_table[FAKE_CCSM_IDX_INDEXED].data;
    ctx->initialized = true;
    ctx->current_rate_nom = ctx->config->clock_default->rate_nom;
    ctx->current_rate_fb = ctx->config->clock_default->rate_fb;
    ctx->pll_api = pll_api;
    ctx->base_address = ctx->config->base_address;
    ctx->current_state = MOD_CLOCK_STATE_RUNNING;
}

void tearDown(void)
{
}

void test_function_ccsm_clock_select_rates_pass(void)
{
    int status;
    struct mod_ccsm_clock_rate clock_rates;
    struct ccsm_dev_ctx *ctx;

    ctx = &module_ctx.dev_ctx_table[FAKE_CCSM_IDX_CONTINUOUS];

    status = ccsm_clock_select_rates(
        ctx,
        ((1844 * FWK_MHZ) - (900 * FWK_KHZ)),
        MOD_CLOCK_ROUND_MODE_UP,
        &clock_rates);

    TEST_ASSERT_EQUAL(FWK_SUCCESS, status);
    TEST_ASSERT_EQUAL(1844 * FWK_MHZ, clock_rates.rate_nom);
    TEST_ASSERT_EQUAL(1659 * FWK_MHZ, clock_rates.rate_fb);

    status = ccsm_clock_select_rates(
        ctx,
        ((1844 * FWK_MHZ) - (100 * FWK_KHZ)),
        MOD_CLOCK_ROUND_MODE_DOWN,
        &clock_rates);

    TEST_ASSERT_EQUAL(FWK_SUCCESS, status);
    TEST_ASSERT_EQUAL(1843 * FWK_MHZ, clock_rates.rate_nom);
    TEST_ASSERT_EQUAL(1658 * FWK_MHZ, clock_rates.rate_fb);

    status = ccsm_clock_select_rates(
        ctx,
        ((1844 * FWK_MHZ) - (400 * FWK_KHZ)),
        MOD_CLOCK_ROUND_MODE_NEAREST,
        &clock_rates);

    TEST_ASSERT_EQUAL(FWK_SUCCESS, status);
    TEST_ASSERT_EQUAL(1844 * FWK_MHZ, clock_rates.rate_nom);
    TEST_ASSERT_EQUAL(1659 * FWK_MHZ, clock_rates.rate_fb);
}

void test_function_ccsm_clock_select_rates_fail_round_none(void)
{
    int status;
    struct mod_ccsm_clock_rate clock_rates;
    struct ccsm_dev_ctx *ctx;

    ctx = &module_ctx.dev_ctx_table[FAKE_CCSM_IDX_CONTINUOUS];

    status = ccsm_clock_select_rates(
        ctx,
        ((1844 * FWK_MHZ) - (100 * FWK_KHZ)),
        MOD_CLOCK_ROUND_MODE_NONE,
        &clock_rates);

    TEST_ASSERT_EQUAL(FWK_E_RANGE, status);
}

void test_function_ccsm_clock_set_rate_pass_continuous_uninitialized(void)
{
    int status;
    uint64_t rate = 1844 * FWK_MHZ;
    uint64_t read_rate;
    struct ccsm_dev_ctx *ctx;

    fwk_id_t dev_id =
        FWK_ID_ELEMENT_INIT(FAKE_MODULE_ID, FAKE_CCSM_IDX_CONTINUOUS);

    ctx = &module_ctx.dev_ctx_table[FAKE_CCSM_IDX_CONTINUOUS];
    ctx->initialized = false;

    fwk_module_is_valid_element_id_ExpectAndReturn(dev_id, true);
    fwk_id_get_element_idx_ExpectAndReturn(dev_id, FAKE_CCSM_IDX_CONTINUOUS);

    ccsm_drv_get_mod_configuration_ExpectAnyArgsAndReturn(FWK_SUCCESS);
    ccsm_drv_set_mod_configuration_ExpectAnyArgsAndReturn(FWK_SUCCESS);
    ccsm_drv_get_dm_configuration_ExpectAnyArgsAndReturn(FWK_SUCCESS);
    ccsm_drv_set_dm_configuration_ExpectAnyArgsAndReturn(FWK_SUCCESS);

    ccsm_drv_req_set_mod_ExpectAndReturn(ctx->base_address, FWK_SUCCESS);
    ccsm_drv_req_set_dm_ExpectAndReturn(ctx->base_address, FWK_SUCCESS);

    mod_ccsm_pll_gen_static_registers_ExpectAnyArgsAndReturn(FWK_SUCCESS);
    ccsm_drv_set_pll_static_settings_ExpectAnyArgsAndReturn(FWK_SUCCESS);
    ccsm_drv_set_pll_static_settings_ExpectAnyArgsAndReturn(FWK_SUCCESS);

    mod_ccsm_pll_gen_dynamic_registers_ExpectAnyArgsAndReturn(FWK_SUCCESS);
    ccsm_drv_set_pll_dynamic_settings_ExpectAnyArgsAndReturn(FWK_SUCCESS);
    ccsm_drv_set_pll_dynamic_settings_ExpectAnyArgsAndReturn(FWK_SUCCESS);
    ccsm_drv_req_set_dvfs_ExpectAndReturn(ctx->base_address, FWK_SUCCESS);

    status = ccsm_clock_set_rate(dev_id, rate, MOD_CLOCK_ROUND_MODE_NEAREST);
    TEST_ASSERT_EQUAL(FWK_SUCCESS, status);

    fwk_module_is_valid_element_id_ExpectAndReturn(dev_id, true);
    fwk_id_get_element_idx_ExpectAndReturn(dev_id, FAKE_CCSM_IDX_CONTINUOUS);

    status = ccsm_clock_get_rate(dev_id, &read_rate);
    TEST_ASSERT_EQUAL(FWK_SUCCESS, status);
    TEST_ASSERT_EQUAL(1844 * FWK_MHZ, read_rate);

    ctx->initialized = true;
}

void test_function_ccsm_clock_set_rate_pass_indexed_initialized(void)
{
    int status;
    uint64_t rate = 1844 * FWK_MHZ;
    uint64_t read_rate;
    struct ccsm_dev_ctx *ctx;
    fwk_id_t dev_id =
        FWK_ID_ELEMENT_INIT(FAKE_MODULE_ID, FAKE_CCSM_IDX_INDEXED);

    ctx = &module_ctx.dev_ctx_table[FAKE_CCSM_IDX_INDEXED];
    ctx->initialized = true;

    fwk_module_is_valid_element_id_ExpectAndReturn(dev_id, true);
    fwk_id_get_element_idx_ExpectAndReturn(dev_id, FAKE_CCSM_IDX_INDEXED);

    mod_ccsm_pll_gen_dynamic_registers_ExpectAnyArgsAndReturn(FWK_SUCCESS);
    ccsm_drv_set_pll_dynamic_settings_ExpectAnyArgsAndReturn(FWK_SUCCESS);
    ccsm_drv_set_pll_dynamic_settings_ExpectAnyArgsAndReturn(FWK_SUCCESS);
    ccsm_drv_req_dvfs_go_up_ExpectAndReturn(ctx->base_address, FWK_SUCCESS);

    status = ccsm_clock_set_rate(dev_id, rate, MOD_CLOCK_ROUND_MODE_NEAREST);
    TEST_ASSERT_EQUAL(FWK_SUCCESS, status);

    fwk_module_is_valid_element_id_ExpectAndReturn(dev_id, true);
    fwk_id_get_element_idx_ExpectAndReturn(dev_id, FAKE_CCSM_IDX_INDEXED);

    status = ccsm_clock_get_rate(dev_id, &read_rate);
    TEST_ASSERT_EQUAL(FWK_SUCCESS, status);
    TEST_ASSERT_EQUAL(1844 * FWK_MHZ, read_rate);
}

void test_function_ccsm_clock_set_rate_pass_indexed_uninitialized(void)
{
    int status;
    uint64_t rate = 1844 * FWK_MHZ;
    uint64_t read_rate;
    struct ccsm_dev_ctx *ctx;
    fwk_id_t dev_id =
        FWK_ID_ELEMENT_INIT(FAKE_MODULE_ID, FAKE_CCSM_IDX_INDEXED);

    ctx = &module_ctx.dev_ctx_table[FAKE_CCSM_IDX_INDEXED];
    ctx->initialized = false;

    fwk_module_is_valid_element_id_ExpectAndReturn(dev_id, true);
    fwk_id_get_element_idx_ExpectAndReturn(dev_id, FAKE_CCSM_IDX_INDEXED);

    ccsm_drv_get_mod_configuration_ExpectAnyArgsAndReturn(FWK_SUCCESS);
    ccsm_drv_set_mod_configuration_ExpectAnyArgsAndReturn(FWK_SUCCESS);
    ccsm_drv_get_dm_configuration_ExpectAnyArgsAndReturn(FWK_SUCCESS);
    ccsm_drv_set_dm_configuration_ExpectAnyArgsAndReturn(FWK_SUCCESS);

    ccsm_drv_req_set_mod_ExpectAndReturn(ctx->base_address, FWK_SUCCESS);
    ccsm_drv_req_set_dm_ExpectAndReturn(ctx->base_address, FWK_SUCCESS);

    mod_ccsm_pll_gen_static_registers_ExpectAnyArgsAndReturn(FWK_SUCCESS);
    ccsm_drv_set_pll_static_settings_ExpectAnyArgsAndReturn(FWK_SUCCESS);
    ccsm_drv_set_pll_static_settings_ExpectAnyArgsAndReturn(FWK_SUCCESS);

    mod_ccsm_pll_gen_dynamic_registers_ExpectAnyArgsAndReturn(FWK_SUCCESS);
    ccsm_drv_set_pll_dynamic_settings_ExpectAnyArgsAndReturn(FWK_SUCCESS);
    ccsm_drv_set_pll_dynamic_settings_ExpectAnyArgsAndReturn(FWK_SUCCESS);
    ccsm_drv_req_set_dvfs_ExpectAndReturn(ctx->base_address, FWK_SUCCESS);

    status = ccsm_clock_set_rate(dev_id, rate, MOD_CLOCK_ROUND_MODE_NEAREST);
    TEST_ASSERT_EQUAL(FWK_SUCCESS, status);

    fwk_module_is_valid_element_id_ExpectAndReturn(dev_id, true);
    fwk_id_get_element_idx_ExpectAndReturn(dev_id, FAKE_CCSM_IDX_INDEXED);

    status = ccsm_clock_get_rate(dev_id, &read_rate);
    TEST_ASSERT_EQUAL(FWK_SUCCESS, status);
    TEST_ASSERT_EQUAL(1844 * FWK_MHZ, read_rate);
}

void test_function_ccsm_clock_set_rate_pass_continuous_initialized(void)
{
    int status;
    uint64_t rate = 1844 * FWK_MHZ;
    uint64_t read_rate;
    struct ccsm_dev_ctx *ctx;
    fwk_id_t dev_id =
        FWK_ID_ELEMENT_INIT(FAKE_MODULE_ID, FAKE_CCSM_IDX_CONTINUOUS);

    ctx = &module_ctx.dev_ctx_table[FAKE_CCSM_IDX_CONTINUOUS];

    fwk_module_is_valid_element_id_ExpectAndReturn(dev_id, true);
    fwk_id_get_element_idx_ExpectAndReturn(dev_id, FAKE_CCSM_IDX_CONTINUOUS);

    mod_ccsm_pll_gen_dynamic_registers_ExpectAnyArgsAndReturn(FWK_SUCCESS);
    ccsm_drv_set_pll_dynamic_settings_ExpectAnyArgsAndReturn(FWK_SUCCESS);
    ccsm_drv_set_pll_dynamic_settings_ExpectAnyArgsAndReturn(FWK_SUCCESS);
    ccsm_drv_req_dvfs_go_up_ExpectAndReturn(ctx->base_address, FWK_SUCCESS);

    status = ccsm_clock_set_rate(dev_id, rate, MOD_CLOCK_ROUND_MODE_NEAREST);
    TEST_ASSERT_EQUAL(FWK_SUCCESS, status);

    fwk_module_is_valid_element_id_ExpectAndReturn(dev_id, true);
    fwk_id_get_element_idx_ExpectAndReturn(dev_id, FAKE_CCSM_IDX_CONTINUOUS);

    status = ccsm_clock_get_rate(dev_id, &read_rate);
    TEST_ASSERT_EQUAL(FWK_SUCCESS, status);
    TEST_ASSERT_EQUAL(1844 * FWK_MHZ, read_rate);
}

void test_function_ccsm_clock_set_rate_pass_continuous_initialized_dn(void)
{
    int status;
    uint64_t rate = 1844 * FWK_MHZ;
    uint64_t read_rate;
    struct ccsm_dev_ctx *ctx;
    fwk_id_t dev_id =
        FWK_ID_ELEMENT_INIT(FAKE_MODULE_ID, FAKE_CCSM_IDX_CONTINUOUS);

    ctx = &module_ctx.dev_ctx_table[FAKE_CCSM_IDX_CONTINUOUS];
    ctx->current_rate_nom = 2000 * FWK_MHZ;

    fwk_module_is_valid_element_id_ExpectAndReturn(dev_id, true);
    fwk_id_get_element_idx_ExpectAndReturn(dev_id, FAKE_CCSM_IDX_CONTINUOUS);

    mod_ccsm_pll_gen_dynamic_registers_ExpectAnyArgsAndReturn(FWK_SUCCESS);
    ccsm_drv_set_pll_dynamic_settings_ExpectAnyArgsAndReturn(FWK_SUCCESS);
    ccsm_drv_set_pll_dynamic_settings_ExpectAnyArgsAndReturn(FWK_SUCCESS);
    ccsm_drv_req_dvfs_go_dn_ExpectAndReturn(ctx->base_address, FWK_SUCCESS);

    status = ccsm_clock_set_rate(dev_id, rate, MOD_CLOCK_ROUND_MODE_NEAREST);
    TEST_ASSERT_EQUAL(FWK_SUCCESS, status);

    fwk_module_is_valid_element_id_ExpectAndReturn(dev_id, true);
    fwk_id_get_element_idx_ExpectAndReturn(dev_id, FAKE_CCSM_IDX_CONTINUOUS);

    status = ccsm_clock_get_rate(dev_id, &read_rate);
    TEST_ASSERT_EQUAL(FWK_SUCCESS, status);
    TEST_ASSERT_EQUAL(1844 * FWK_MHZ, read_rate);
}

void test_function_ccsm_clock_set_rate_fail_stopped(void)
{
    int status;
    uint64_t rate = 1844 * FWK_MHZ;
    struct ccsm_dev_ctx *ctx;
    fwk_id_t dev_id =
        FWK_ID_ELEMENT_INIT(FAKE_MODULE_ID, FAKE_CCSM_IDX_CONTINUOUS);

    ctx = &module_ctx.dev_ctx_table[FAKE_CCSM_IDX_CONTINUOUS];
    ctx->current_state = MOD_CLOCK_STATE_STOPPED;

    fwk_module_is_valid_element_id_ExpectAndReturn(dev_id, true);
    fwk_id_get_element_idx_ExpectAndReturn(dev_id, FAKE_CCSM_IDX_CONTINUOUS);

    status = ccsm_clock_set_rate(dev_id, rate, MOD_CLOCK_ROUND_MODE_NEAREST);
    TEST_ASSERT_EQUAL(FWK_E_PWRSTATE, status);
}

void test_function_ccsm_clock_set_rate_pass_same_rate(void)
{
    int status;
    uint64_t rate;
    uint64_t read_rate;
    struct ccsm_dev_ctx *ctx;
    fwk_id_t dev_id =
        FWK_ID_ELEMENT_INIT(FAKE_MODULE_ID, FAKE_CCSM_IDX_CONTINUOUS);

    ctx = &module_ctx.dev_ctx_table[FAKE_CCSM_IDX_CONTINUOUS];
    rate = ctx->config->clock_default->rate_nom;

    fwk_module_is_valid_element_id_ExpectAndReturn(dev_id, true);
    fwk_id_get_element_idx_ExpectAndReturn(dev_id, FAKE_CCSM_IDX_CONTINUOUS);

    status = ccsm_clock_set_rate(dev_id, rate, MOD_CLOCK_ROUND_MODE_NEAREST);
    TEST_ASSERT_EQUAL(FWK_SUCCESS, status);

    fwk_module_is_valid_element_id_ExpectAndReturn(dev_id, true);
    fwk_id_get_element_idx_ExpectAndReturn(dev_id, FAKE_CCSM_IDX_CONTINUOUS);

    status = ccsm_clock_get_rate(dev_id, &read_rate);
    TEST_ASSERT_EQUAL(FWK_SUCCESS, status);
    TEST_ASSERT_EQUAL(rate, read_rate);
}

void test_function_ccsm_clock_get_rate_pass(void)
{
    int status;
    uint64_t rate;
    struct ccsm_dev_ctx *ctx;

    fwk_id_t dev_id =
        FWK_ID_ELEMENT_INIT(FAKE_MODULE_ID, FAKE_CCSM_IDX_CONTINUOUS);

    ctx = &module_ctx.dev_ctx_table[FAKE_CCSM_IDX_CONTINUOUS];

    fwk_module_is_valid_element_id_ExpectAndReturn(dev_id, true);
    fwk_id_get_element_idx_ExpectAndReturn(dev_id, FAKE_CCSM_IDX_CONTINUOUS);

    status = ccsm_clock_get_rate(dev_id, &rate);
    TEST_ASSERT_EQUAL(FWK_SUCCESS, status);
    TEST_ASSERT_EQUAL(ctx->config->clock_default->rate_nom, rate);
}

void test_function_ccsm_clock_get_rate_fail_null_rate(void)
{
    int status;
    uint64_t *rate = NULL;
    fwk_id_t dev_id =
        FWK_ID_ELEMENT_INIT(FAKE_MODULE_ID, FAKE_CCSM_IDX_CONTINUOUS);

    fwk_module_is_valid_element_id_ExpectAndReturn(dev_id, true);

    status = ccsm_clock_get_rate(dev_id, rate);
    TEST_ASSERT_EQUAL(FWK_E_PARAM, status);
}

void test_function_ccsm_clock_get_rate_from_index_pass(void)
{
    int status;
    uint64_t rate;
    fwk_id_t dev_id =
        FWK_ID_ELEMENT_INIT(FAKE_MODULE_ID, FAKE_CCSM_IDX_INDEXED);

    fwk_module_is_valid_element_id_ExpectAndReturn(dev_id, true);
    fwk_id_get_element_idx_ExpectAndReturn(dev_id, FAKE_CCSM_IDX_INDEXED);

    status = ccsm_clock_get_rate_from_index(dev_id, 0, &rate);
    TEST_ASSERT_EQUAL(FWK_SUCCESS, status);
    TEST_ASSERT_EQUAL(300 * FWK_MHZ, rate);
}

void test_function_ccsm_clock_get_rate_from_index_fail_rate_null(void)
{
    int status;
    uint64_t *rate = NULL;
    fwk_id_t dev_id =
        FWK_ID_ELEMENT_INIT(FAKE_MODULE_ID, FAKE_CCSM_IDX_INDEXED);

    fwk_module_is_valid_element_id_ExpectAndReturn(dev_id, true);

    status = ccsm_clock_get_rate_from_index(dev_id, 0, rate);
    TEST_ASSERT_EQUAL(FWK_E_PARAM, status);
}

void test_function_ccsm_clock_get_rate_from_index_fail_count_exceeded(void)
{
    int status;
    uint64_t rate;
    fwk_id_t dev_id =
        FWK_ID_ELEMENT_INIT(FAKE_MODULE_ID, FAKE_CCSM_IDX_INDEXED);

    fwk_module_is_valid_element_id_ExpectAndReturn(dev_id, true);
    fwk_id_get_element_idx_ExpectAndReturn(dev_id, FAKE_CCSM_IDX_INDEXED);

    status = ccsm_clock_get_rate_from_index(dev_id, 4, &rate);
    TEST_ASSERT_EQUAL(FWK_E_PARAM, status);
}
void test_function_ccsm_clock_get_rate_from_index_fail_continuous(void)
{
    int status;
    uint64_t rate;
    fwk_id_t dev_id =
        FWK_ID_ELEMENT_INIT(FAKE_MODULE_ID, FAKE_CCSM_IDX_CONTINUOUS);

    fwk_module_is_valid_element_id_ExpectAndReturn(dev_id, true);
    fwk_id_get_element_idx_ExpectAndReturn(dev_id, FAKE_CCSM_IDX_CONTINUOUS);

    status = ccsm_clock_get_rate_from_index(dev_id, 0, &rate);
    TEST_ASSERT_EQUAL(FWK_E_SUPPORT, status);
}

void test_function_ccsm_dm_get_configuration_pass(void)
{
    int status;
    uint32_t ccsm_strategy_in;
    struct mod_ccsm_dm_config dm_config = { 0 };
    struct ccsm_dev_ctx *ctx;

    ccsm_strategy_in = 0x11011FFFu;

    ctx = &module_ctx.dev_ctx_table[FAKE_CCSM_IDX_CONTINUOUS];

    ccsm_drv_get_dm_configuration_ExpectAnyArgsAndReturn(FWK_SUCCESS);
    ccsm_drv_get_dm_configuration_ReturnThruPtr_config(&ccsm_strategy_in);

    status = ccsm_dm_get_configuration(ctx, &dm_config);

    TEST_ASSERT_EQUAL(status, FWK_SUCCESS);

    TEST_ASSERT_EQUAL(MOD_CCSM_DM_SW_FB, dm_config.strategy);
    TEST_ASSERT_EQUAL(MOD_CCSM_DM_TPIP, dm_config.dd);
    TEST_ASSERT_EQUAL(MOD_CCSM_DM_SW_SOFF_STOP, dm_config.soff);
    TEST_ASSERT_EQUAL(15, dm_config.transition_pause);
    TEST_ASSERT_EQUAL(511, dm_config.mitigation_duration);

    ccsm_strategy_in = 0x11021FFFu;

    ccsm_drv_get_dm_configuration_ExpectAnyArgsAndReturn(FWK_SUCCESS);
    ccsm_drv_get_dm_configuration_ReturnThruPtr_config(&ccsm_strategy_in);

    status = ccsm_dm_get_configuration(ctx, &dm_config);

    TEST_ASSERT_EQUAL(status, FWK_SUCCESS);

    TEST_ASSERT_EQUAL(MOD_CCSM_DM_NOM_ONLY, dm_config.strategy);
    TEST_ASSERT_EQUAL(MOD_CCSM_DM_TPIP, dm_config.dd);
    TEST_ASSERT_EQUAL(MOD_CCSM_DM_SW_SOFF_STOP, dm_config.soff);
    TEST_ASSERT_EQUAL(15, dm_config.transition_pause);
    TEST_ASSERT_EQUAL(511, dm_config.mitigation_duration);

    ccsm_strategy_in = 0x01001FFFu;

    ccsm_drv_get_dm_configuration_ExpectAnyArgsAndReturn(FWK_SUCCESS);
    ccsm_drv_get_dm_configuration_ReturnThruPtr_config(&ccsm_strategy_in);

    status = ccsm_dm_get_configuration(ctx, &dm_config);

    TEST_ASSERT_EQUAL(status, FWK_SUCCESS);

    TEST_ASSERT_EQUAL(MOD_CCSM_DM_STOP_CLK, dm_config.strategy);
    TEST_ASSERT_EQUAL(MOD_CCSM_DM_ARM_DD, dm_config.dd);
    TEST_ASSERT_EQUAL(MOD_CCSM_DM_SW_SOFF_STOP, dm_config.soff);
    TEST_ASSERT_EQUAL(15, dm_config.transition_pause);
    TEST_ASSERT_EQUAL(511, dm_config.mitigation_duration);

    ccsm_strategy_in = 0x00101FFFu;

    ccsm_drv_get_dm_configuration_ExpectAnyArgsAndReturn(FWK_SUCCESS);
    ccsm_drv_get_dm_configuration_ReturnThruPtr_config(&ccsm_strategy_in);

    status = ccsm_dm_get_configuration(ctx, &dm_config);

    TEST_ASSERT_EQUAL(status, FWK_SUCCESS);

    TEST_ASSERT_EQUAL(MOD_CCSM_DM_CLK_ON_DVFS, dm_config.strategy);
    TEST_ASSERT_EQUAL(MOD_CCSM_DM_ARM_DD, dm_config.dd);
    TEST_ASSERT_EQUAL(MOD_CCSM_DM_SW_SOFF_IGNORE, dm_config.soff);
    TEST_ASSERT_EQUAL(15, dm_config.transition_pause);
    TEST_ASSERT_EQUAL(511, dm_config.mitigation_duration);
}

void test_function_ccsm_dm_set_configuration_pass(void)
{
    int status;

    struct ccsm_dev_ctx *ctx;

    struct mod_ccsm_dm_config dm_config = { .strategy = MOD_CCSM_DM_NOM_ONLY,
                                            .dd = MOD_CCSM_DM_ARM_DD,
                                            .soff = MOD_CCSM_DM_SW_SOFF_IGNORE,
                                            .transition_pause = 6,
                                            .mitigation_duration = 705 };

    ctx = &module_ctx.dev_ctx_table[FAKE_CCSM_IDX_CONTINUOUS];

    ccsm_drv_set_dm_configuration_ExpectAndReturn(
        ctx->base_address, 0x00022C16, FWK_SUCCESS);

    status = ccsm_dm_set_configuration(ctx, &dm_config);
    TEST_ASSERT_EQUAL(status, FWK_SUCCESS);
}

void test_function_ccsm_mod_get_configuration_pass(void)
{
    int status;
    uint32_t ccsm_mod_in;
    struct ccsm_dev_ctx *ctx;
    struct mod_ccsm_mod_config mod_config = { 0 };

    ccsm_mod_in = 0x001FFFF0;
    ctx = &module_ctx.dev_ctx_table[FAKE_CCSM_IDX_CONTINUOUS];

    ccsm_drv_get_mod_configuration_ExpectAnyArgsAndReturn(FWK_SUCCESS);
    ccsm_drv_get_mod_configuration_ReturnThruPtr_config(&ccsm_mod_in);

    status = ccsm_mod_get_configuration(ctx, 0, &mod_config);
    TEST_ASSERT_EQUAL(FWK_SUCCESS, status);

    TEST_ASSERT_EQUAL(mod_config.numerator_regular, 240);
    TEST_ASSERT_EQUAL(mod_config.numerator_oc, 31);
    TEST_ASSERT_EQUAL(mod_config.denominator, 255);
}

void test_function_ccsm_mod_set_configuration_pass(void)
{
    int status;
    struct ccsm_dev_ctx *ctx;
    struct mod_ccsm_mod_config mod_config = { .numerator_regular = 240,
                                              .numerator_oc = 31,
                                              .denominator = 255 };

    ctx = &module_ctx.dev_ctx_table[FAKE_CCSM_IDX_CONTINUOUS];

    ccsm_drv_set_mod_configuration_ExpectAndReturn(
        ctx->base_address, 0, 0x001FFFF0, FWK_SUCCESS);

    status = ccsm_mod_set_configuration(ctx, 0, &mod_config);
    TEST_ASSERT_EQUAL(FWK_SUCCESS, status);
}

void test_function_ccsm_mod_set_configuration_fail(void)
{
    int status;
    struct ccsm_dev_ctx *ctx;
    struct mod_ccsm_mod_config mod_config = { .numerator_regular = 255,
                                              .numerator_oc = 31,
                                              .denominator = 100 };

    ctx = &module_ctx.dev_ctx_table[FAKE_CCSM_IDX_CONTINUOUS];

    status = ccsm_mod_set_configuration(ctx, 0, &mod_config);
    TEST_ASSERT_EQUAL(FWK_E_PARAM, status);
}

void test_function_ccsm_clock_set_state_pass(void)
{
    int status;
    struct ccsm_dev_ctx *ctx;
    fwk_id_t dev_id =
        FWK_ID_ELEMENT_INIT(FAKE_MODULE_ID, FAKE_CCSM_IDX_CONTINUOUS);

    ctx = &module_ctx.dev_ctx_table[FAKE_CCSM_IDX_CONTINUOUS];

    status = ccsm_clock_set_state(dev_id, MOD_CLOCK_STATE_RUNNING);
    TEST_ASSERT_EQUAL(FWK_SUCCESS, status);
    TEST_ASSERT_EQUAL(MOD_CLOCK_STATE_RUNNING, ctx->current_state);
}

void test_function_ccsm_clock_set_state_fail(void)
{
    int status;
    struct ccsm_dev_ctx *ctx;
    fwk_id_t dev_id =
        FWK_ID_ELEMENT_INIT(FAKE_MODULE_ID, FAKE_CCSM_IDX_CONTINUOUS);

    ctx = &module_ctx.dev_ctx_table[FAKE_CCSM_IDX_CONTINUOUS];

    status = ccsm_clock_set_state(dev_id, MOD_CLOCK_STATE_STOPPED);
    TEST_ASSERT_EQUAL(FWK_E_SUPPORT, status);
    TEST_ASSERT_EQUAL(MOD_CLOCK_STATE_RUNNING, ctx->current_state);
}

void test_function_ccsm_clock_get_state_pass(void)
{
    int status;
    enum mod_clock_state cur_state;
    fwk_id_t dev_id =
        FWK_ID_ELEMENT_INIT(FAKE_MODULE_ID, FAKE_CCSM_IDX_CONTINUOUS);

    fwk_module_is_valid_element_id_ExpectAndReturn(dev_id, true);
    fwk_id_get_element_idx_ExpectAndReturn(dev_id, FAKE_CCSM_IDX_CONTINUOUS);

    status = ccsm_clock_get_state(dev_id, &cur_state);
    TEST_ASSERT_EQUAL(FWK_SUCCESS, status);
    TEST_ASSERT_EQUAL(MOD_CLOCK_STATE_RUNNING, cur_state);
}

void test_function_ccsm_clock_get_state_fail(void)
{
    int status;
    enum mod_clock_state *cur_state = NULL;
    fwk_id_t dev_id =
        FWK_ID_ELEMENT_INIT(FAKE_MODULE_ID, FAKE_CCSM_IDX_CONTINUOUS);

    fwk_module_is_valid_element_id_ExpectAndReturn(dev_id, true);

    status = ccsm_clock_get_state(dev_id, cur_state);
    TEST_ASSERT_EQUAL(FWK_E_PARAM, status);
}

void test_function_ccsm_clock_get_range_continuous_pass(void)
{
    int status;
    struct mod_clock_range range;
    struct ccsm_dev_ctx *ctx;
    fwk_id_t dev_id =
        FWK_ID_ELEMENT_INIT(FAKE_MODULE_ID, FAKE_CCSM_IDX_CONTINUOUS);

    ctx = &module_ctx.dev_ctx_table[FAKE_CCSM_IDX_CONTINUOUS];

    fwk_module_is_valid_element_id_ExpectAndReturn(dev_id, true);
    fwk_id_get_element_idx_ExpectAndReturn(dev_id, FAKE_CCSM_IDX_CONTINUOUS);

    status = ccsm_clock_get_range(dev_id, &range);
    TEST_ASSERT_EQUAL(FWK_SUCCESS, status);
    TEST_ASSERT_EQUAL(MOD_CLOCK_RATE_TYPE_CONTINUOUS, range.rate_type);
    TEST_ASSERT_EQUAL(ctx->config->min_rate, range.min);
    TEST_ASSERT_EQUAL(ctx->config->max_rate, range.max);
    TEST_ASSERT_EQUAL(ctx->config->min_step, range.step);
}

void test_function_ccsm_clock_get_range_fail_null(void)
{
    int status;
    struct mod_clock_range *range = NULL;
    fwk_id_t dev_id =
        FWK_ID_ELEMENT_INIT(FAKE_MODULE_ID, FAKE_CCSM_IDX_CONTINUOUS);

    fwk_module_is_valid_element_id_ExpectAndReturn(dev_id, true);

    status = ccsm_clock_get_range(dev_id, range);
    TEST_ASSERT_EQUAL(FWK_E_PARAM, status);
}

void test_function_ccsm_clock_get_range_indexed_pass(void)
{
    int status;
    struct mod_clock_range range;
    struct ccsm_dev_ctx *ctx;
    fwk_id_t dev_id =
        FWK_ID_ELEMENT_INIT(FAKE_MODULE_ID, FAKE_CCSM_IDX_INDEXED);

    ctx = &module_ctx.dev_ctx_table[FAKE_CCSM_IDX_INDEXED];

    fwk_module_is_valid_element_id_ExpectAndReturn(dev_id, true);
    fwk_id_get_element_idx_ExpectAndReturn(dev_id, FAKE_CCSM_IDX_INDEXED);

    status = ccsm_clock_get_range(dev_id, &range);
    TEST_ASSERT_EQUAL(FWK_SUCCESS, status);
    TEST_ASSERT_EQUAL(MOD_CLOCK_RATE_TYPE_DISCRETE, range.rate_type);
    TEST_ASSERT_EQUAL(ctx->config->rate_table[0].rate_nom, range.min);
    TEST_ASSERT_EQUAL(
        ctx->config->rate_table[ctx->config->rate_count - 1].rate_nom,
        range.max);
    TEST_ASSERT_EQUAL(ctx->config->rate_count, range.rate_count);
}

void test_function_ccsm_power_state_change_off_pass(void)
{
    int status;
    fwk_id_t dev_id =
        FWK_ID_ELEMENT_INIT(FAKE_MODULE_ID, FAKE_CCSM_IDX_CONTINUOUS);

    fwk_id_get_element_idx_ExpectAndReturn(dev_id, FAKE_CCSM_IDX_CONTINUOUS);

    status = ccsm_clock_power_state_change(dev_id, MOD_PD_STATE_OFF);
    TEST_ASSERT_EQUAL(FWK_SUCCESS, status);
}

void test_function_ccsm_power_state_change_on_pass(void)
{
    int status;
    uint64_t rate = 1844 * FWK_MHZ;
    uint64_t read_rate;
    struct ccsm_dev_ctx *ctx;
    fwk_id_t dev_id =
        FWK_ID_ELEMENT_INIT(FAKE_MODULE_ID, FAKE_CCSM_IDX_CONTINUOUS);

    ctx = &module_ctx.dev_ctx_table[FAKE_CCSM_IDX_CONTINUOUS];
    ctx->initialized = true;
    ctx->current_rate_nom = rate;

    fwk_id_get_element_idx_ExpectAndReturn(dev_id, FAKE_CCSM_IDX_CONTINUOUS);

    ccsm_drv_get_mod_configuration_ExpectAnyArgsAndReturn(FWK_SUCCESS);
    ccsm_drv_set_mod_configuration_ExpectAnyArgsAndReturn(FWK_SUCCESS);
    ccsm_drv_get_dm_configuration_ExpectAnyArgsAndReturn(FWK_SUCCESS);
    ccsm_drv_set_dm_configuration_ExpectAnyArgsAndReturn(FWK_SUCCESS);

    ccsm_drv_req_set_mod_ExpectAndReturn(ctx->base_address, FWK_SUCCESS);
    ccsm_drv_req_set_dm_ExpectAndReturn(ctx->base_address, FWK_SUCCESS);

    mod_ccsm_pll_gen_static_registers_ExpectAnyArgsAndReturn(FWK_SUCCESS);
    ccsm_drv_set_pll_static_settings_ExpectAnyArgsAndReturn(FWK_SUCCESS);
    ccsm_drv_set_pll_static_settings_ExpectAnyArgsAndReturn(FWK_SUCCESS);

    mod_ccsm_pll_gen_dynamic_registers_ExpectAnyArgsAndReturn(FWK_SUCCESS);
    ccsm_drv_set_pll_dynamic_settings_ExpectAnyArgsAndReturn(FWK_SUCCESS);
    ccsm_drv_set_pll_dynamic_settings_ExpectAnyArgsAndReturn(FWK_SUCCESS);
    ccsm_drv_req_set_dvfs_ExpectAndReturn(ctx->base_address, FWK_SUCCESS);

    status = ccsm_clock_power_state_change(dev_id, MOD_PD_STATE_ON);
    TEST_ASSERT_EQUAL(FWK_SUCCESS, status);

    fwk_module_is_valid_element_id_ExpectAndReturn(dev_id, true);
    fwk_id_get_element_idx_ExpectAndReturn(dev_id, FAKE_CCSM_IDX_CONTINUOUS);

    status = ccsm_clock_get_rate(dev_id, &read_rate);
    TEST_ASSERT_EQUAL(FWK_SUCCESS, status);
    TEST_ASSERT_EQUAL(rate, read_rate);
}

void test_function_ccsm_clock_power_state_pending_change_pass(void)
{
    int status;
    struct ccsm_dev_ctx *ctx;
    fwk_id_t dev_id =
        FWK_ID_ELEMENT_INIT(FAKE_MODULE_ID, FAKE_CCSM_IDX_CONTINUOUS);

    ctx = &module_ctx.dev_ctx_table[FAKE_CCSM_IDX_CONTINUOUS];

    fwk_module_is_valid_element_id_ExpectAndReturn(dev_id, true);
    fwk_id_get_element_idx_ExpectAndReturn(dev_id, FAKE_CCSM_IDX_CONTINUOUS);

    status = ccsm_clock_power_state_pending_change(dev_id, 0, MOD_PD_STATE_OFF);
    TEST_ASSERT_EQUAL(FWK_SUCCESS, status);
    TEST_ASSERT_EQUAL(MOD_CLOCK_STATE_STOPPED, ctx->current_state);
}

void test_function_ccsm_init(void)
{
    int status;

    status = ccsm_init(FWK_ID_MODULE(FWK_MODULE_IDX_CCSM), 0, NULL);
    TEST_ASSERT_EQUAL(FWK_SUCCESS, status);

    status = ccsm_init(
        FWK_ID_MODULE(FWK_MODULE_IDX_CCSM), FAKE_CCSM_IDX_COUNT, NULL);
    TEST_ASSERT_EQUAL(FWK_SUCCESS, status);
}

void test_function_ccsm_element_init(void)
{
    int status;
    struct ccsm_dev_ctx *ctx;
    fwk_id_t dev_id =
        FWK_ID_ELEMENT_INIT(FAKE_MODULE_ID, FAKE_CCSM_IDX_CONTINUOUS);

    ctx = &module_ctx.dev_ctx_table[FAKE_CCSM_IDX_CONTINUOUS];

    fwk_id_get_element_idx_ExpectAndReturn(dev_id, FAKE_CCSM_IDX_CONTINUOUS);

    status = ccsm_element_init(
        dev_id, 0, element_table[FAKE_CCSM_IDX_CONTINUOUS].data);
    TEST_ASSERT_EQUAL(FWK_SUCCESS, status);

    TEST_ASSERT_EQUAL(ctx->initialized, false);
    TEST_ASSERT_EQUAL(ctx->current_state, MOD_CLOCK_STATE_STOPPED);

    dev_id = FWK_ID_ELEMENT(FAKE_MODULE_ID, FAKE_CCSM_IDX_COUNT);

    fwk_id_get_element_idx_ExpectAndReturn(dev_id, FAKE_CCSM_IDX_COUNT);

    status = ccsm_element_init(
        dev_id, 0, element_table[FAKE_CCSM_IDX_CONTINUOUS].data);
    TEST_ASSERT_EQUAL(FWK_E_DATA, status);
}

void test_function_ccsm_bind(void)
{
    int status;
    struct ccsm_dev_ctx *ctx;
    fwk_id_t module_id = FWK_ID_MODULE(FAKE_MODULE_ID);
    fwk_id_t element_id =
        FWK_ID_ELEMENT_INIT(FAKE_MODULE_ID, FAKE_CCSM_IDX_CONTINUOUS);

    ctx = &module_ctx.dev_ctx_table[FAKE_CCSM_IDX_CONTINUOUS];

    status = ccsm_bind(module_id, 1);
    TEST_ASSERT_EQUAL(FWK_SUCCESS, status);

    fwk_module_is_valid_module_id_ExpectAndReturn(module_id, true);

    status = ccsm_bind(module_id, 0);
    TEST_ASSERT_EQUAL(FWK_SUCCESS, status);

    fwk_module_is_valid_module_id_ExpectAndReturn(element_id, false);
    fwk_id_get_element_idx_ExpectAndReturn(
        element_id, FAKE_CCSM_IDX_CONTINUOUS);
    fwk_module_bind_ExpectAndReturn(
        ctx->config->pll_id, ctx->config->pll_api_id, &pll_api, FWK_SUCCESS);
    status = ccsm_bind(element_id, 0);
    TEST_ASSERT_EQUAL(FWK_SUCCESS, status);
}

void test_function_ccsm_process_bind_request(void)
{
    int status;
    struct mod_clock_drv_api *clock_api;
    fwk_id_t req_id =
        FWK_ID_ELEMENT_INIT(FAKE_MODULE_ID, FAKE_CCSM_IDX_INDEXED);
    fwk_id_t element_id =
        FWK_ID_ELEMENT_INIT(FAKE_MODULE_ID, FAKE_CCSM_IDX_CONTINUOUS);
    fwk_id_t api_type = FWK_ID_API(FAKE_MODULE_ID, MOD_CCSM_CLOCK_API);

    fwk_id_get_api_idx_ExpectAndReturn(api_type, MOD_CCSM_CLOCK_API);

    status = ccsm_process_bind_request(
        req_id, element_id, api_type, (const void **)&clock_api);
    TEST_ASSERT_EQUAL(FWK_SUCCESS, status);
    TEST_ASSERT_EQUAL_PTR(&api_ccsm_clock, clock_api);
}

int ccsm_test_main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_function_ccsm_init);
    RUN_TEST(test_function_ccsm_element_init);
    RUN_TEST(test_function_ccsm_bind);
    RUN_TEST(test_function_ccsm_process_bind_request);

    RUN_TEST(test_function_ccsm_clock_select_rates_pass);
    RUN_TEST(test_function_ccsm_clock_select_rates_fail_round_none);

    RUN_TEST(test_function_ccsm_clock_set_rate_pass_indexed_uninitialized);
    RUN_TEST(test_function_ccsm_clock_set_rate_pass_indexed_initialized);

    RUN_TEST(test_function_ccsm_clock_set_rate_pass_continuous_uninitialized);
    RUN_TEST(test_function_ccsm_clock_set_rate_pass_continuous_initialized);
    RUN_TEST(test_function_ccsm_clock_set_rate_pass_continuous_initialized_dn);

    RUN_TEST(test_function_ccsm_clock_set_rate_pass_same_rate);
    RUN_TEST(test_function_ccsm_clock_set_rate_fail_stopped);

    RUN_TEST(test_function_ccsm_clock_get_rate_pass);
    RUN_TEST(test_function_ccsm_clock_get_rate_fail_null_rate);

    RUN_TEST(test_function_ccsm_clock_get_rate_from_index_pass);
    RUN_TEST(test_function_ccsm_clock_get_rate_from_index_fail_count_exceeded);
    RUN_TEST(test_function_ccsm_clock_get_rate_from_index_fail_rate_null);
    RUN_TEST(test_function_ccsm_clock_get_rate_from_index_fail_continuous);

    RUN_TEST(test_function_ccsm_dm_get_configuration_pass);
    RUN_TEST(test_function_ccsm_dm_set_configuration_pass);

    RUN_TEST(test_function_ccsm_mod_get_configuration_pass);
    RUN_TEST(test_function_ccsm_mod_set_configuration_pass);
    RUN_TEST(test_function_ccsm_mod_set_configuration_fail);

    RUN_TEST(test_function_ccsm_clock_set_state_pass);
    RUN_TEST(test_function_ccsm_clock_set_state_fail);
    RUN_TEST(test_function_ccsm_clock_get_state_pass);
    RUN_TEST(test_function_ccsm_clock_get_state_fail);

    RUN_TEST(test_function_ccsm_clock_get_range_continuous_pass);
    RUN_TEST(test_function_ccsm_clock_get_range_fail_null);
    RUN_TEST(test_function_ccsm_clock_get_range_indexed_pass);

    RUN_TEST(test_function_ccsm_power_state_change_off_pass);
    RUN_TEST(test_function_ccsm_power_state_change_on_pass);

    RUN_TEST(test_function_ccsm_clock_power_state_pending_change_pass);

    return UNITY_END();
}

#if !defined(TEST_ON_TARGET)
int main(void)
{
    return ccsm_test_main();
}
#endif
