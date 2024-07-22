/*
 * Arm SCP/MCP Software
 * Copyright (c) 2024, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MOD_PERF_CONTROLLER_H_
#define MOD_PERF_CONTROLLER_H_

#include <fwk_id.h>

#include <stdint.h>

/*!
 * \ingroup GroupModules
 * \defgroup GroupPERF_CONTROLLER performance controller
 * \{
 */

/*!
 * \brief Core performance API.
 */
struct mod_perf_controller_perf_api {
    /*!
     * \brief Set performance level for a controller.
     *
     * \param cluster_id Cluster identifier.
     * \param performance_level Desirable performance level.
     *
     * \retval ::FWK_E_ACCESS Wrong id.
     * \retval ::FWK_SUCCESS If the call is successful.
     * \return One of the standard framework error codes.
     */
    int (*set_performance_level)(
        fwk_id_t cluster_id,
        uintptr_t cookie,
        uint32_t performance_level);
};

/*!
 * \brief Performance driver interface.
 */
struct mod_perf_controller_drv_api {
    /*! Name of the driver */
    const char *name;

    /*!
     * \brief Set performance level for a performance domain.
     *
     * \param domain_id Domain identifier.
     * \param performance_level Desirable performance level.
     *
     * \retval ::FWK_E_ACCESS Wrong id.
     * \retval ::FWK_SUCCESS If the call is successful.
     * \return One of the standard framework error codes.
     */
    int (*set_performance_level)(
        fwk_id_t domain_id,
        uintptr_t cookie,
        uint32_t performance_level);
};

/*!
 * \brief Cluster apply performance granted API.
 */
struct mod_perf_controller_apply_performance_granted_api {
    /*!
     * \brief Provides the means to apply the output performance level after
     *      setting the power limits for each cluster.
     *
     * \retval ::FWK_SUCCESS If the call is successful.
     * \return One of the standard framework error codes.
     */
    int (*apply_performance_granted)(void);
};

/*!
 * \brief Power Model API
 */
struct mod_perf_controller_power_model_api {
    /*!
     * \brief Converts from power value to the corresponding
     *        performance level.
     *
     * \param model_id Power model identifier.
     * \param power Power value.
     * \param[out] perfomance_level
     *
     * \retval ::FWK_SUCCESS If the call is successful.
     * \return One of the standard framework error codes.
     */
    int (*power_to_performance)(
        fwk_id_t model_id,
        uint32_t power,
        uint32_t *performance_level);
};

#ifdef BUILD_HAS_NOTIFICATION
/*!
 * \brief Perf controller Notification indices.
 */
enum mod_perf_controller_notification_index {
    /* DVFS Complete */
    MOD_PERF_CONTROLLER_NOTIFICATION_IDX_DVFS_COMPLETE,

    /* Total number of perf controllers notification. */
    MOD_PERF_CONTROLLER_NOTIFICATION_COUNT,
};

/*!
 * \brief Perf controller Notification identifiers.
 */
#    ifdef BUILD_HAS_MOD_PERF_CONTROLLER
/*! Identifier of the power state transition notification */
static const fwk_id_t mod_perf_controller_notification_dvfs_complete =
    FWK_ID_NOTIFICATION_INIT(
        FWK_MODULE_IDX_PERF_CONTROLLER,
        MOD_PERF_CONTROLLER_NOTIFICATION_IDX_DVFS_COMPLETE);
#    endif /* BUILD_HAS_MOD_PERF_CONTROLLER */
#endif /* BUILD_HAS_NOTIFICATION */

struct mod_perf_controller_cluster_config {
    /*! Module or element identifier of the performance driver. */
    fwk_id_t performance_driver_id;

    /*! API identifier of the performance driver. */
    fwk_id_t performance_driver_api_id;

    /*! Module or element identifier of the power model driver. */
    fwk_id_t power_model_id;

    /*! API identifier of the power model driver. */
    fwk_id_t power_model_api_id;

    /*! Initial performance limit. */
    uint32_t initial_performance_limit;
};

/*!
 * \brief Performance controller API IDs.
 */
enum mod_perf_controller_api_idx {
    /*! Index for the cluster performance adjustments API. */
    MOD_PERF_CONTROLLER_CLUSTER_PERF_API = 0U,

    /*! Index for the core power adjustments API */
    MOD_PERF_CONTROLLER_CORE_POWER_API,

    /*! Index for the controller apply performance granted API. */
    MOD_PERF_CONTROLLER_APPLY_PERFORMANCE_GRANTED_API,

    /*! Number of APIs. */
    MOD_PERF_CONTROLLER_API_COUNT
};

/*!
 * \brief Performance controller events IDs.
 */
enum mod_perf_controller_event_idx {
    /*! Driver response event. */
    MOD_PERF_CONTROLLER_EVENT_IDX_DRIVER_RESPONSE,

    /*! Number of events. */
    MOD_PERF_CONTROLLER_EVENT_IDX_COUNT,
};

enum mod_perf_controller_notification_idx {
    /*! Notification for performance set. */
    MOD_PERF_CONTROLLER_NOTIFICATION_IDX_PERF_SET,

    /*! Number of events. */
    MOD_PERF_CONTROLLER_NOTIFICATION_IDX_COUNT,
};

/*!
 * \brief Performance controller driver response event parameters.
 */
struct mod_perf_controller_event_drv_resp_params {
    /*! Performance level. */
    uint32_t performance_level;

    /*! HAL request specific cookie. */
    uint32_t cookie;
};

/*!
 * \brief Performance controller core notification.
 */
struct mod_perf_controller_notification_params {
    /*! Performance_level. */
    uint32_t performance_level;
};

/*!
 * \}
 */

#endif /* MOD_PERF_CONTROLLER_H_ */
