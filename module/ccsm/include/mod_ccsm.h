/*
 * Arm SCP/MCP Software
 * Copyright (c) 2024, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MOD_CCSM_H
#define MOD_CCSM_H

#include <fwk_element.h>
#include <fwk_id.h>
#include <fwk_macros.h>

#include <stdbool.h>
#include <stdint.h>

/*!
 * \ingroup GroupModules Modules
 * \defgroup GroupSystemCCSM Module
 *
 * \details A driver for Clock Control State Machines.
 *
 * \{
 */

/*! The slowest rate at which the PLL hardware can operate. */
#define MOD_CCSM_CLOCK_MIN_RATE (50UL * FWK_MHZ)

/*! The fastest rate at which the PLL hardware can operate. */
#define MOD_CCSM_CLOCK_MAX_RATE (4UL * FWK_GHZ)

/*! The maximum precision that can be used when setting the PLL rate. */
#define MOD_CCSM_CLOCK_MIN_INTERVAL (1UL * FWK_KHZ)

/*!
 * \brief Type of the clock (indexed vs non-indexed).
 */
enum mod_ccsm_clock_type {
    /*! A lookup table is used to find the settings for a given rate. */
    MOD_CCSM_CLOCK_TYPE_INDEXED,
    /*! The clock rate is set without looking up settings in a table. */
    MOD_CCSM_CLOCK_TYPE_NON_INDEXED,
};

/*! Indexes of APIs that the module offers for binding. */
enum mod_ccsm_api_types {
    MOD_CCSM_CLOCK_API,
    MOD_CCSM_API_COUNT,
};

/*!
 * \brief CCSM PLL API types for communication with PLL driver.
 */
enum mod_ccsm_pll_api_types {
    MOD_CCSM_PLL_API_TYPE_DEFAULT,
    MOD_CCSM_PLL_API_COUNT
};

/*!
 * \brief Rate lookup table entry,
 */
struct mod_ccsm_clock_rate {
    /*! Rate of nominal clock in Hertz. */
    uint64_t rate_nom;

    /* Rate of fallback clock in Hertz. */
    uint64_t rate_fb;
};

/*!
 * \brief Register values for configuration of PLLs.
 */
struct mod_ccsm_pll_reg_values {
    uint32_t pll_0_static;
    uint32_t pll_1_static;
    uint32_t pll_nom_settings_0;
    uint32_t pll_nom_settings_1;
    uint32_t pll_fb_settings_0;
    uint32_t pll_fb_settings_1;
};

/*!
 * \brief Config values for droop mitigation strategies.
 */
enum mod_ccsm_dm_strategy {
    MOD_CCSM_DM_STOP_CLK = 0,
    MOD_CCSM_DM_NOM_ONLY = 1 << 17,
    MOD_CCSM_DM_SW_FB = 1 << 16,
    MOD_CCSM_DM_CLK_ON_DVFS = 1 << 20
};

/*!
 * \brief DM strategy flags for droop detector config.
 */
enum mod_ccsm_dm_dd { MOD_CCSM_DM_ARM_DD = 0, MOD_CCSM_DM_TPIP = 1 << 28 };

enum mod_ccsm_dm_dd_soff {
    MOD_CCSM_DM_SW_SOFF_IGNORE = 0,
    MOD_CCSM_DM_SW_SOFF_STOP = 1 << 24,
};

/*!
 * \brief Droop mitigation configuration options.
 */
struct mod_ccsm_dm_config {
    enum mod_ccsm_dm_strategy strategy;
    enum mod_ccsm_dm_dd dd;
    enum mod_ccsm_dm_dd_soff soff;
    uint8_t transition_pause;
    uint16_t mitigation_duration;
};

/*!
 * \brief Modulator configuration options.
 */
struct mod_ccsm_mod_config {
    uint8_t numerator_oc;
    uint8_t numerator_regular;
    uint8_t denominator;
};

/*!
 * \brief CCSM PLL interface for implementation defined PLL functions
 */
struct mod_ccsm_pll_api {
    const char *name;

    /*!
     * \brief Generate values for the PLL dynamic configuration registers.
     *
     * \param dev_id CCSM PLL device identifier.
     *
     * \param[in] config Target DVFS configuration.
     *
     * \param[inout] pll_reg_values Updated register values.
     *
     */
    int (*gen_dynamic_registers)(
        fwk_id_t dev_id,
        const struct mod_ccsm_clock_rate *config,
        struct mod_ccsm_pll_reg_values *pll_reg_values);

    /*!
     * \brief Generate values for the PLL static configuration registers.
     *
     * \param dev_id CCSM PLL device identifier.
     *
     * \param[inout] pll_reg_values Updated register values.
     *
     */
    int (*gen_static_registers)(
        fwk_id_t dev_id,
        struct mod_ccsm_pll_reg_values *pll_reg_values);
};

/*!
 * \brief PLL device configuration.
 */
struct mod_ccsm_dev_config {
    /*! The type of the clock (indexed vs non-indexed). */
    enum mod_ccsm_clock_type clock_type;

    /*! The minimum rate at which to enable the fallback clock. */
    uint64_t min_rate_fb_enable;

    /*!
     * The slowest rate either PLL can be set to. This may be different from the
     * hardware-imposed limit.
     */
    uint64_t min_rate;

    /*!
     * The fastest rate either PLL can be set to. This may be different from the
     * hardware-imposed limit.
     */
    uint64_t max_rate;

    /*!
     * The maximum precision that can be used when setting the PLL rates. This
     * may be different from the hardware-imposed limit.
     */
    uint64_t min_step;

    /*!
     * Base address for the CCSM instance.
     */
    uint32_t base_address;

    /*! The default clock rate for the CCSM PLLs. */
    const struct mod_ccsm_clock_rate *clock_default;

    /*! The initial droop mitigation strategy to enable. */
    const struct mod_ccsm_dm_config *dm_config;

    /*! The initial modulator configuration to set. */
    const struct mod_ccsm_mod_config *mod_config;

    /*! The number of modulators implemented in this CCSM. */
    uint8_t modulator_count;

    /*! Element identifier for the associated PLL. */
    fwk_id_t pll_id;

    /*! Reference to the API provided by the PLL driver. */
    fwk_id_t pll_api_id;

    /*! Pointer to the clock's rate lookup table. */
    struct mod_ccsm_clock_rate const *rate_table;

    /*! The number of rates in the rate lookup table. */
    uint32_t rate_count;
};

#endif /* MOD_CCSM_H */
