/*
 * Arm SCP/MCP Software
 * Copyright (c) 2019-2021, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "config_sensor.h"
#include "juno_id.h"

#include <mod_juno_adc.h>
#include <mod_juno_pvt.h>
#include <mod_juno_xrp7724.h>
#include <mod_scmi_sensor.h>
#include <mod_sensor.h>

#include <fwk_assert.h>
#include <fwk_element.h>
#include <fwk_id.h>
#include <fwk_macros.h>
#include <fwk_mm.h>
#include <fwk_module.h>
#include <fwk_module_idx.h>
#include <fwk_status.h>
#include <fwk_string.h>

#include <fwk_dt_config_common.h>

#include <string.h>

#ifdef FWK_MODULE_GEN_DYNAMIC_SENSOR

/* individual table access from common sensor module */
extern const struct fwk_element *mod_sensor_get_table_by_index(int idx);
extern size_t mod_sensor_get_table_size_by_index(int idx);

const struct fwk_element *dyn_get_element_table_sensor(fwk_id_t module_id)
{    

    #if USE_FULL_SET_SENSORS
    int status;
    enum juno_idx_revision rev;
    size_t pvt_sensor_elem_table_size, r0_full_elem_table_size;
    const struct fwk_element *element_table_pvt, element_table_r0_full;
    #endif

    size_t sensor_elem_table_size;
    struct fwk_element *element_table_out;
    const struct fwk_element *element_table;

#if (PLATFORM_VARIANT == JUNO_VARIANT_FVP)
    /* in the device tree we set the FVP table index as 0 */
    element_table = mod_sensor_get_table_by_index(0);
    sensor_elem_table_size = mod_sensor_get_table_size_by_index(0);
    element_table_out = fwk_mm_calloc(
        (sensor_elem_table_size + 1),
        sizeof(struct fwk_element));
    if (element_table == NULL) {
        return NULL;
    }

    fwk_str_memcpy(
        element_table_out,
        element_table,
        sizeof(element_table));

#else /* Platform variant BOARD */ 
    

    element_table = mod_sensor_get_table_by_index(0); /* r0 element table*/
    if (element_table == NULL)
        return NULL;
    sensor_elem_table_size = mod_sensor_get_table_size_by_index(0);

    #if USE_FULL_SET_SENSORS

    /* r0 full sensors element table */
    element_table_r0_full = mod_sensor_get_table_by_index(1); 
    if (element_table_r0_full == NULL)
        return NULL:
    r0_full_elem_table_size = mod_sensor_get_table_size_by_index(1);

    /* r1 r2 full sensors table */
    element_table_pvt = mod_sensor_get_table_by_index(2);
    if (element_table_pvt == NULL)
        return NULL:
    pvt_sensor_elem_table_size = mod_sensor_get_table_size_by_index(2);

    status = juno_id_get_revision(&rev);
    if (status != FWK_SUCCESS)
        return NULL;

    /* allocate table with null termination entry */
    if (rev == JUNO_IDX_REVISION_R0) {
        /* r0 table + r0 full sensors */
        element_table_out = fwk_mm_calloc(
            (sensor_elem_table_size + r0_full_elem_table_size + 1),
            sizeof(struct fwk_element));

    } else {
        /* r0 table + r0 full sensors + r1 / r2 full table */
        element_table_out = fwk_mm_calloc(
            (sensor_elem_table_size + r0_full_elem_table_size + 
            pvt_sensor_elem_table_size  + 1),
            sizeof(struct fwk_element)); 
    }

    /* copy base tables */
    memcpy(element_table_out,
        element_table,
        sizeof(element_table));

    memcpy(element_table_out + sensor_elem_table_size,
            element_table_r0_full,
            sizeof(element_table_r0_full));

    if (rev != JUNO_IDX_REVISION_R0) {
        /* copy r1 r2 tables */
        memcpy(element_table_out + sensor_elem_table_size + r0_full_elem_table_size,
            element_table_pvt,
            sizeof(element_table_pvt));
    }    

        
    #else /* not full set sensors */

        /* Just add the termination description */
        element_table_out = fwk_mm_calloc(
            (sensor_elem_table_size + 1),
            sizeof(struct fwk_element));
        if (element_table_out == NULL) {
            return NULL;
        }

        fwk_str_memcpy(
            element_table_out,
            element_table,
            sizeof(element_table));
            
    #endif /* full set sensors*/

    
#endif /* platform variant */
    return element_table_out;
}

#else
#error("Error: Juno scp_ramfw must use dynamic initialization for sensor module access to override values");
#endif
