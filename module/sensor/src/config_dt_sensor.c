/*
 * Arm SCP/MCP Software
 * Copyright (c) 2019-2021, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

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

/* helpers for sensor DT module */
#define SENS_COMPAT scp_sensor
#define SENS_NODE_ID(n) DT_INST(n, SENS_COMPAT)
#define SENS_TABLE_NAME(n) DT_STRING_TOKEN(SENS_NODE_ID(n), label)
#define SENS_PROP(n, prop) FWK_DT_INST_PROP(SENS_COMPAT, n, prop)

/* each sensor element table formed from name and array of child elements */
#define SENS_GEN_TABLE_ENTRY(child_node_id) \
    [DT_PROP(child_node_id, elem_idx)] = { \
        .name = DT_PROP(child_node_id, sensor_name), \
        .data = &(struct mod_sensor_dev_config) { \
            .driver_id = FWK_DT_NODE_PH_IDX_SCP_ID_BY_NAME(child_node_id, driver), \
            .driver_api_id = FWK_DT_NODE_PH_IDX_SCP_ID_BY_NAME(child_node_id, driver_api), \
        }, \
    },

#define SENS_GEN_SINGLE_TABLE(n) \
    static struct fwk_element SENS_TABLE_NAME(n)[] = { \
        DT_FOREACH_CHILD(SENS_NODE_ID(n), SENS_GEN_TABLE_ENTRY) \
    };

DT_FOREACH_OKAY_INST_scp_sensor(SENS_GEN_SINGLE_TABLE)

/* entry to the array of table names. */
struct sensor_table {
    const struct fwk_element *table;
    size_t table_size;
};

#define SENS_ELEM_TABLES_ENTRY(n) \
    [SENS_PROP(n, table_idx)] = { \
        .table = SENS_TABLE_NAME(n), \
        .table_size = FWK_ARRAY_SIZE(SENS_TABLE_NAME(n)), \
    },

/* list of sensor element tables */
static struct sensor_table element_tables[] = {
    DT_FOREACH_OKAY_INST_scp_sensor(SENS_ELEM_TABLES_ENTRY)
};


/* access data table from dynamic element callback */
#ifdef FWK_MODULE_GEN_DYNAMIC_SENSOR
const struct fwk_element *_static_get_element_table_sensor() {
    return element_tables[0].table;
}

/* special function for sensor module to access the table of tables */
const struct fwk_element *mod_sensor_get_table_by_index(int idx)
{
    if (idx < DT_N_INST_scp_sensor_NUM_OKAY)
        return element_tables[idx].table;
    return NULL;
}

size_t mod_sensor_get_table_size_by_index(int idx)
{
    if (idx < DT_N_INST_scp_sensor_NUM_OKAY)
        return element_tables[idx].table_size;
    return 0;
}

#endif

/* config structure
   we can have multiple tables, but always return the first one 
   if not using the dynamic access.  
*/
struct fwk_module_config config_dt_sensor = {
    #ifdef FWK_MODULE_GEN_DYNAMIC_SENSOR
    .elements = FWK_MODULE_DYNAMIC_ELEMENTS(dyn_get_element_table_sensor),
    #else
    .elements = FWK_MODULE_STATIC_ELEMENTS_PTR(element_tables[0].table),
    #endif
};
