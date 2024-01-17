/*
 * Arm SCP/MCP Software
 * Copyright (c) 2022, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* SCP framework ID macros */
#include <fwk_id.h>

/* Base set of DT generator macros - from Zephyr project */
#include <fwk_devicetree.h>

/*
 * Generated macros from DT processing - generally reproduced in
 * source files using the generator macros
 */
#include <fwk_dt_config.h>

/* Generated API calls for modules using dynamic table type */
#include <fwk_dt_config_dyn.h>

/**
 * @brief fwk_dt_config_common.h API
 *
 * Set of common SCP specific DT generator macros for use within fwk modules
 * and product components.
 *
 * Common usage patterns built from the base set in fwk_devicetree.h
 *
 * Generator macros in the C source are used to reproduce macros generated in the DT processing
 * thus extracting the value in the device tree.
 *
 * @defgroup devicetree-scp-fwk Devicetree extensions for SCP firmware
 * @ingroup devicetree-scp-fwk
 * @{
 */

/**
 * @brief Extract a single property for a compatible instance
 * @param compat Compatible name
 * @param inst instance number
 * @param prop property name
 */
#define FWK_DT_INST_PROP(compat, inst, prop) DT_PROP(DT_INST(inst, compat), prop)

/**
  * @brief property value from a node referenced by a phandle 
  * 
  *  n1: node-1@0 {
  *     compatible = "mr-bob";
  *     bob = <&ph_ref>;
  *  }
  * 
  *  ph_ref: node-2 {
  *     dick = <42>;
  *  }
  * 
  *  usage:
  *  FWK_DT_PH_INST_PROP(mr_bob, 0, bob, dick)
  * 
  * @param ph_compat compatible name for the node the contains the phandle reference
  * @param ph_inst   instance of compatible with phandle reference 
  * @param ph_name   name of property of type phandle
  * @param ref_prop  name of property whose value is required from node referenced by phandle 
  */
#define FWK_DT_PH_INST_PROP(ph_compat, ph_inst, ph_name, ref_prop) DT_PROP(DT_PHANDLE(DT_INST(ph_inst, ph_compat), ph_name), ref_prop)

#define FWK_DT_PH_INST_REG(ph_compat, ph_inst, ph_name, reg_idx) DT_REG_ADDR_BY_IDX(DT_PHANDLE(DT_INST(ph_inst, ph_compat), ph_name), reg_idx)


/**
 * @brief Extract a single property if it exists, or set default
 * @param compat Compatible name
 * @param inst instance number
 * @param prop property name
 * @param default default value
 *
 * Checks that 'prop' exists, if not the supplied default value will be used.
 * Typical use is to populate structures where optional device tree attibutes are
 * used. 
 *
 * Example usage:
 *
 *     static const struct mod_bootloader_config bootloader_module_config = {
 *          ...
 *          .sds_struct_id = FWK_DT_INST_PROP_DEFAULT(BOOTLOADER_COMPAT, 0, sds_id, 0),
 *     };
 */
#define FWK_DT_INST_PROP_DEFAULT(compat, inst, prop, default) \
    COND_CODE_1(DT_NODE_HAS_PROP(DT_INST(inst ,compat), prop), \
    (FWK_DT_INST_PROP(compat, inst, prop)), \
    (default) \
    )

/**
 * @brief Extract an array indexed property for a compatible instance
 * @param compat Compatible name
 * @param inst instance number
 * @param prop property name
 * @param idx array index
 */
#define FWK_DT_INST_IDX_PROP(compat, inst, prop, idx) \
    DT_PROP_BY_IDX(DT_INST(inst, compat), prop, idx)

/**
 * @defgroup devicetree-scp-fwk-scp-ids Framework IDs API
 *
 * The SCP framework uses 32 bit ID values as selectors / references between modules and
 * framework elements. There are base scp_id elements defined to allow structured definition
 * of the IDs. Specific ID numbers in module devicetree blocks use phandle refs to the base 
 * definitions, with the correct index numbers to define the SCP ID.
 *
 * The framework API begins with .yaml bindings:
 *
 *     description: SCP software framework module element ID 
 *
 *     compatible: "arm,scp-elem-id"
 *
 *     include: base.yaml
 *
 *     scp-id-cells:
 *        - mod-idx       # module index
 *        - elem-idx      # element index
 * 
 * These are then used in the baseline .dtsi include scp-fwk.dtsi
 *
 * e.g.:
 *
 *     scp-fwk {
 *
 *         elem_id: fwk-id@0 {
 *             compatible = "arm,scp-elem-id";
 *             #scp-id-cells = <2>;        
 *         };
 *     };
 *
 * These are then referenced in module objects where a SCP ID is required.
 *
 * e.g:
 *
 *     scp-fwk {
 *      
 *         scp-clk-dev@0 {
 *             compatible = "scp,clk-dev";
 *             name = "I2SCLK";
 *             scp-ids = <&elem_id FWK_MODULE_IDX_JUNO_CDCEL937 JUNO_CLOCK_CDCEL937_IDX_I2SCLK>,
 *                       <&elem_id FWK_MODULE_IDX_JUNO_CDCEL937 MOD_JUNO_CDCEL937_API_IDX_CLOCK_DRIVER>;
 *          
 *         };
 *     };
 *
 * This mechanism allows the hybrid build containing both devicetree configured and
 * earlier directly written standard C configuration
 *
 *  Where a fwk_id_t structure is then needed in the source code the following macro can be used:
 * 
 * struct fwk_module_config config_dt_clk_dev = {
 *   .data =
 *       &(struct mod_clock_dev){
 *           .alarm_id = FWK_DT_PH_IDX_SCP_ID_BY_TYPE(scp_clk_dev, 0, 0),
 *       },
 *  };
 * 
 * @ingroup devicetree-scp-fwk
 * @{
 */

/* list of compatible and attribute names from bindings */
#define SCP_DT_IDS_ATTR_N  scp_ids
#define SCP_DT_MOD_ID_COMPAT_N      scp_mod_id
#define SCP_DT_ELEM_ID_COMPAT_N     scp_elem_id
#define SCP_DT_SUB_ELEM_ID_COMPAT_N scp_sub_elem_id    
#define SCP_DT_API_ID_COMPAT_N      scp_api_id
#define SCP_DT_NTFY_ID_COMPAT_N     scp_notify_id

/**
 * @brief Generate SCP Module ID from phandle scp-ids reference.
 * @param compat Compatible name
 * @param inst Instance number
 * @param array_idx index into phandle array
 * 
 * Use the Phandle references to the SCP ID compatibles to create Module ID types
 */
#define FWK_DT_PH_IDX_SCP_ID_MOD(compat, inst, array_idx) \
        _FWK_DT_PH_IDX_SCP_ID_MOD(DT_INST(inst, compat), array_idx, SCP_DT_IDS_ATTR_N)

#define _FWK_DT_PH_IDX_SCP_ID_MOD(node_id, array_idx, scp_ids_n) FWK_ID_MODULE_INIT( \
        DT_PHA_BY_IDX(node_id, scp_ids_n, array_idx, mod_idx))

/**
 * @brief Generate SCP Module ID from phandle scp-ids reference.
 * @param node_id node id containing the scp-ids
 * @param array_idx index into phandle array
 * 
 * Use the Phandle references to the SCP ID compatibles to create Module ID types
 */

#define FWK_DT_NODE_PH_IDX_SCP_ID_MOD(node_id, array_idx) \
        _FWK_DT_PH_IDX_SCP_ID_MOD(node_id, array_idx, SCP_DT_IDS_ATTR_N)

/**
 * @brief Generate SCP element ID from phandle scp-ids reference.
 * @param compat Compatible name
 * @param inst Instance number
 * @param array_idx index into phandle array
 *
 *  Use the Phandle references to the SCP ID compatibles to create Element ID types.
 */
#define FWK_DT_PH_IDX_SCP_ID_ELEM(compat, inst, array_idx) \
    _FWK_DT_NODE_PH_IDX_SCP_ID_ELEM(DT_INST(inst, compat), array_idx, SCP_DT_IDS_ATTR_N)
    /*_FWK_DT_PH_IDX_SCP_ID_ELEM(compat, inst, array_idx, SCP_DT_IDS_ATTR_N)*/

/* need extra level to allow scp_ids_n to be a define - otherwise not evaluated before pasting */
/*#define _FWK_DT_PH_IDX_SCP_ID_ELEM(compat, inst, array_idx, scp_ids_n) FWK_ID_ELEMENT_INIT( \
        DT_PHA_BY_IDX(DT_INST(inst, compat), scp_ids_n, array_idx, mod_idx), \
        DT_PHA_BY_IDX(DT_INST(inst, compat), scp_ids_n, array_idx, elem_idx))*/
/**
 * @brief Generate SCP element ID from phandle scp-ids reference and node_id
 * @param node_id Node ID for scp-ids ref
 * @param array_idx index into phandle array
 *
 *  Use the Phandle references to the SCP ID compatibles to create Element ID types.
 */
#define FWK_DT_NODE_PH_IDX_SCP_ID_ELEM(node_id, array_idx) \
    _FWK_DT_NODE_PH_IDX_SCP_ID_ELEM(node_id, array_idx, SCP_DT_IDS_ATTR_N)

#define _FWK_DT_NODE_PH_IDX_SCP_ID_ELEM(node_id, array_idx, scp_ids_n) FWK_ID_ELEMENT_INIT( \
        DT_PHA_BY_IDX(node_id, scp_ids_n, array_idx, mod_idx), \
        DT_PHA_BY_IDX(node_id, scp_ids_n, array_idx, elem_idx))



/**
 * @brief Shortcut to get the first SCP element ID in an array
 * @param compat Compatible name
 * @param inst Instance number
 */
#define FWK_DT_PH_SCP_ID_ELEM(compat, inst) FWK_DT_PH_IDX_SCP_ID_ELEM(compat, inst, 0)

/**
 * @brief Generate SCP sub-element ID from phandle scp-ids reference.
 * @param compat Compatible name
 * @param inst Instance number
 * @param array_idx index into phandle array
 *
 *  Use the Phandle references to the SCP ID compatibles to create sub-Element ID types.
 */
#define FWK_DT_PH_IDX_SCP_ID_SUB_ELEM(compat, inst, array_idx)  \
        _FWK_DT_NODE_PH_IDX_SCP_ID_SUB_ELEM(DT_INST(inst, compat), array_idx, SCP_DT_IDS_ATTR_N)

/**
 * @brief Generate SCP sub-element ID from phandle scp-ids reference and node id
 * @param node_id Node ID for SCP ids ref
 * @param array_idx index into phandle array
 *
 *  Use the Phandle references to the SCP ID compatibles to create sub-Element ID types.
 */
#define FWK_DT_NODE_PH_IDX_SCP_ID_SUB_ELEM(node_id, array_idx)  \
        _FWK_DT_NODE_PH_IDX_SCP_ID_SUB_ELEM(node_id, array_idx, SCP_DT_IDS_ATTR_N)



/*#define _FWK_DT_PH_IDX_SCP_ID_SUB_ELEM(compat, inst, array_idx, scp_ids_n) FWK_ID_SUB_ELEMENT_INIT( \
        DT_PHA_BY_IDX(DT_INST(inst, compat), scp_ids_n, array_idx, mod_idx), \
        DT_PHA_BY_IDX(DT_INST(inst, compat), scp_ids_n, array_idx, elem_idx), \
        DT_PHA_BY_IDX(DT_INST(inst, compat), scp_ids_n, array_idx, sub_elem_idx))*/

#define _FWK_DT_NODE_PH_IDX_SCP_ID_SUB_ELEM(node_id, array_idx, scp_ids_n) FWK_ID_SUB_ELEMENT_INIT( \
        DT_PHA_BY_IDX(node_id, scp_ids_n, array_idx, mod_idx), \
        DT_PHA_BY_IDX(node_id, scp_ids_n, array_idx, elem_idx), \
        DT_PHA_BY_IDX(node_id, scp_ids_n, array_idx, sub_elem_idx))


/**
 * @brief Shortcut to get the first SCP sub element ID in an array
 * @param compat Compatible name
 * @param inst Instance number
 */
#define FWK_DT_PH_SCP_ID_SUB_ELEM(compat, inst) FWK_DT_PH_IDX_SCP_ID_SUB_ELEM(compat, inst, 0)


/**
 * @brief Generate SCP API ID from phandle scp-ids reference.
 * @param compat Compatible name
 * @param inst Instance number
 * @param array_idx  index into phandle array
 *
 * Use the Phandle references to the SCP ID compatibles to create API ID types.
 */
#define FWK_DT_PH_IDX_SCP_ID_API(compat, inst, array_idx) \
        _FWK_DT_NODE_PH_IDX_SCP_ID_API(DT_INST(inst, compat), array_idx, SCP_DT_IDS_ATTR_N)
        
        /*_FWK_DT_PH_IDX_SCP_ID_API(compat, inst, array_idx, SCP_DT_IDS_ATTR_N)*/

/**
 * @brief Generate SCP API ID from phandle scp-ids reference.
 * @param node_id node ID for node with SCP ids ref
 * @param array_idx  index into phandle array
 *
 * Use the Phandle references to the SCP ID compatibles to create API ID types.
 */
#define FWK_DT_NODE_PH_IDX_SCP_ID_API(node_id, array_idx) \
        _FWK_DT_NODE_PH_IDX_SCP_ID_API(node_id, array_idx, SCP_DT_IDS_ATTR_N)

/*#define _FWK_DT_PH_IDX_SCP_ID_API(compat, inst, array_idx, scp_ids_n) FWK_ID_API_INIT( \
        DT_PHA_BY_IDX(DT_INST(inst, compat), scp_ids_n, array_idx, mod_idx), \
        DT_PHA_BY_IDX(DT_INST(inst, compat), scp_ids_n, array_idx, api_idx))*/

#define _FWK_DT_NODE_PH_IDX_SCP_ID_API(node_id, array_idx, scp_ids_n) FWK_ID_API_INIT( \
        DT_PHA_BY_IDX(node_id, scp_ids_n, array_idx, mod_idx), \
        DT_PHA_BY_IDX(node_id, scp_ids_n, array_idx, api_idx))

/**
 * @brief Shortcut to get the first SCP API ID in an array
 * @param compat Compatible name
 * @param inst Instance number
 */
#define FWK_DT_PH_SCP_ID_API(compat, inst) FWK_DT_PH_IDX_SCP_ID_API(compat, inst, 0)

/**
 * @brief Generate SCP Notification ID from phandle sds_ids reference.
 * @param compat Compatible name
 * @param inst Instance number
 * @param array_idx index into phandle array
 *
 * Use the Phandle references to the SCP ID compatibles to create Notification ID types.
 */
#define FWK_DT_PH_IDX_SCP_ID_NTFY(compat, inst, array_idx) \
        _FWK_DT_NODE_PH_IDX_SCP_ID_NTFY(DT_INST(inst, compat), array_idx, SCP_DT_IDS_ATTR_N)

        /*_FWK_DT_PH_IDX_SCP_ID_NTFY(compat, inst, array_idx, SCP_DT_IDS_ATTR_N)*/

/**
 * @brief Generate SCP Notification ID from phandle sds_ids reference.
 * @param node_id node of SCP ids ref
 * @param array_idx index into phandle array
 *
 * Use the Phandle references to the SCP ID compatibles to create Notification ID types.
 */
#define FWK_DT_NODE_PH_IDX_SCP_ID_NTFY(node_id, array_idx) \
        _FWK_DT_NODE_PH_IDX_SCP_ID_NTFY(DT_INST(inst, compat), array_idx, SCP_DT_IDS_ATTR_N)


/*#define _FWK_DT_PH_IDX_SCP_ID_NTFY(compat, inst, array_idx, scp_ids_n) FWK_ID_NOTIFICATION_INIT( \
        DT_PHA_BY_IDX(DT_INST(inst, compat), scp_ids_n, array_idx, mod_idx), \
        DT_PHA_BY_IDX(DT_INST(inst, compat), scp_ids_n, array_idx, ntfy_idx))        */

#define _FWK_DT_NODE_PH_IDX_SCP_ID_NTFY(node_id, array_idx, scp_ids_n) FWK_ID_NOTIFICATION_INIT( \
        DT_PHA_BY_IDX(node_id, scp_ids_n, array_idx, mod_idx), \
        DT_PHA_BY_IDX(node_id, scp_ids_n, array_idx, ntfy_idx))        

/* 
 * Create a chain of type checking macros to init an ID by compatible type
 * uses compat, inst reference.
 */

/* create an scp-ids helper */
#define FWK_DT_PHA_IDX_HAS_COMPAT(compat, inst, array_idx, pha_compat) \
    DT_NODE_HAS_COMPAT(DT_PHANDLE_BY_IDX(DT_INST(inst, compat), scp_ids, array_idx), pha_compat)

/* first MOD or NONE */
#define FWK_DT_PH_IDX_SCP_ID_MOD_OR_NONE(compat, inst, array_idx) \
    COND_CODE_1(FWK_DT_PHA_IDX_HAS_COMPAT(compat, inst, array_idx, SCP_DT_MOD_ID_COMPAT_N),  \
    (FWK_DT_PH_IDX_SCP_ID_MOD(compat, inst, array_idx)), \
    (FWK_ID_NONE_INIT) \
    )

/* next ELEM or MOD */   
#define FWK_DT_PH_IDX_SCP_ID_ELEM_OR_MOD(compat, inst, array_idx) \
    COND_CODE_1(FWK_DT_PHA_IDX_HAS_COMPAT(compat, inst, array_idx, SCP_DT_ELEM_ID_COMPAT_N), \
    (FWK_DT_PH_IDX_SCP_ID_ELEM(compat, inst, array_idx)), \
    (FWK_DT_PH_IDX_SCP_ID_MOD_OR_NONE(compat, inst, array_idx)) \
    )

/* next SUB_ELEM or ELEM */
#define FWK_DT_PH_IDX_SCP_ID_SUB_ELEM_OR_ELEM(compat, inst, array_idx) \
    COND_CODE_1(FWK_DT_PHA_IDX_HAS_COMPAT(compat, inst, array_idx, SCP_DT_SUB_ELEM_ID_COMPAT_N), \
    (FWK_DT_PH_IDX_SCP_ID_SUB_ELEM(compat, inst, array_idx)), \
    (FWK_DT_PH_IDX_SCP_ID_ELEM_OR_MOD(compat, inst, array_idx)) \
    )

/* next API or SUB_ELEM */
#define FWK_DT_PH_IDX_SCP_ID_API_OR_SUB_ELEM(compat, inst, array_idx) \
    COND_CODE_1(FWK_DT_PHA_IDX_HAS_COMPAT(compat, inst, array_idx, SCP_DT_API_ID_COMPAT_N),  \
    (FWK_DT_PH_IDX_SCP_ID_API(compat, inst, array_idx)), \
    (FWK_DT_PH_IDX_SCP_ID_SUB_ELEM_OR_ELEM(compat, inst, array_idx)) \
    )

/* next NOTIFY or API */
#define FWK_DT_PH_IDX_SCP_ID_NOTIFY_OR_API(compat, inst, array_idx) \
    COND_CODE_1(FWK_DT_PHA_IDX_HAS_COMPAT(compat, inst, array_idx, SCP_DT_NTFY_ID_COMPAT_N),  \
    (FWK_DT_PH_IDX_SCP_ID_NTFY(compat, inst, array_idx)), \
    (FWK_DT_PH_IDX_SCP_ID_API_OR_SUB_ELEM(compat, inst, array_idx)) \
    )

/*
 * end of chain 
 */

/* 
 * Create a chain of type checking macros to init an ID by compatible type
 * uses node_id reference.
 */

/* create an scp-ids helper */
#define FWK_DT_NODE_PHA_IDX_HAS_COMPAT(node_id, array_idx, pha_compat) \
    DT_NODE_HAS_COMPAT(DT_PHANDLE_BY_IDX(node_id, scp_ids, array_idx), pha_compat)

/* first MOD or NONE */
#define FWK_DT_NODE_PH_IDX_SCP_ID_MOD_OR_NONE(node_id, array_idx) \
    COND_CODE_1(FWK_DT_NODE_PHA_IDX_HAS_COMPAT(node_id, array_idx, SCP_DT_MOD_ID_COMPAT_N),  \
    (FWK_DT_NODE_PH_IDX_SCP_ID_MOD(node_id, array_idx)), \
    (FWK_ID_NONE_INIT) \
    )

/* next ELEM or MOD */   
#define FWK_DT_NODE_PH_IDX_SCP_ID_ELEM_OR_MOD(node_id, array_idx) \
    COND_CODE_1(FWK_DT_NODE_PHA_IDX_HAS_COMPAT(node_id, array_idx, SCP_DT_ELEM_ID_COMPAT_N), \
    (FWK_DT_NODE_PH_IDX_SCP_ID_ELEM(node_id, array_idx)), \
    (FWK_DT_NODE_PH_IDX_SCP_ID_MOD_OR_NONE(node_id, array_idx)) \
    )

/* next SUB_ELEM or ELEM */
#define FWK_DT_NODE_PH_IDX_SCP_ID_SUB_ELEM_OR_ELEM(node_id, array_idx) \
    COND_CODE_1(FWK_DT_NODE_PHA_IDX_HAS_COMPAT(node_id, array_idx, SCP_DT_SUB_ELEM_ID_COMPAT_N), \
    (FWK_DT_NODE_PH_IDX_SCP_ID_SUB_ELEM(node_id, array_idx)), \
    (FWK_DT_NODE_PH_IDX_SCP_ID_ELEM_OR_MOD(node_id, array_idx)) \
    )

/* next API or SUB_ELEM */
#define FWK_DT_NODE_PH_IDX_SCP_ID_API_OR_SUB_ELEM(node_id, array_idx) \
    COND_CODE_1(FWK_DT_NODE_PHA_IDX_HAS_COMPAT(node_id, array_idx, SCP_DT_API_ID_COMPAT_N),  \
    (FWK_DT_NODE_PH_IDX_SCP_ID_API(node_id, array_idx)), \
    (FWK_DT_NODE_PH_IDX_SCP_ID_SUB_ELEM_OR_ELEM(node_id, array_idx)) \
    )

/* next NOTIFY or API */
#define FWK_DT_NODE_PH_IDX_SCP_ID_NOTIFY_OR_API(node_id, array_idx) \
    COND_CODE_1(FWK_DT_NODE_PHA_IDX_HAS_COMPAT(node_id, array_idx, SCP_DT_NTFY_ID_COMPAT_N),  \
    (FWK_DT_NODE_PH_IDX_SCP_ID_NTFY(node_id, array_idx)), \
    (FWK_DT_NODE_PH_IDX_SCP_ID_API_OR_SUB_ELEM(node_id, array_idx)) \
    )

/*
 * end of chain 
 */


/**
 * @brief Generate an fwk_id_t object from a phandle to an scp_ids array entry
 * 
 * @param compat Compatible name
 * @param inst Instance number
 * @param array_idx index into phandle array
 * 
 * Uses the compatible type of the phandle referenced to determine the type if fwk_id_t
 * to construct.
 */
#define FWK_DT_PH_IDX_SCP_ID_BY_TYPE(compat, inst, array_idx) \
    FWK_DT_PH_IDX_SCP_ID_NOTIFY_OR_API(compat, inst, array_idx)

#define FWK_DT_NODE_PH_IDX_SCP_ID_BY_TYPE(node_id, array_idx) \
    FWK_DT_NODE_PH_IDX_SCP_ID_NOTIFY_OR_API(node_id, array_idx)



/**
 * @brief Generate an fwk_id_t object from a phandle to an scp_ids array entry
 * 
 * @param compat Compatible name
 * @param inst Instance number
 * @param array_idx index into phandle array
 * 
 * Uses the compatible type of the phandle referenced to determine the type if fwk_id_t
 * to construct. If the index not present in the array then a FWK_ID_NONE_INIT ID generated.
 */

#define FWK_DT_PH_IDX_SCP_ID_BY_TYPE_OR_NONE(compat, inst, array_idx) \
    COND_CODE_1(DT_PROP_HAS_IDX(DT_INST(inst, compat), SCP_DT_IDS_ATTR_N, array_idx), \
    (FWK_DT_PH_IDX_SCP_ID_BY_TYPE(compat, inst, array_idx)), \
    (FWK_ID_NONE_INIT) \
     )

#define FWK_DT_NODE_PH_IDX_SCP_ID_BY_TYPE_OR_NONE(node_id, array_idx) \
    COND_CODE_1(DT_PROP_HAS_IDX(node_id, SCP_DT_IDS_ATTR_N, array_idx), \
    (FWK_DT_NODE_PH_IDX_SCP_ID_BY_TYPE(node_id, array_idx)), \
    (FWK_ID_NONE_INIT) \
     )

/**
 * @brief Get the index of a phandle name for a phandle array property.
 * 
 * @param compat Compatible name
 * @param inst Instance number
 * @param pha_prop phandle array property 
 * @param name phandle name 
 * 
 * For a phandle array that has an associated names property, get the index
 * of the name "name" in the array
 * 
 * e.g for scp-ids property:-
 * 
 * scp-ids = <&sub_elem_id FWK_MODULE_IDX_TIMER JUNO_ALARM_ELEMENT_IDX JUNO_SYSTEM_POWER_ALARM_IDX>, 
 *                         <&elem_id FWK_MODULE_IDX_SYSTEM_POWER MOD_SYSTEM_POWER_API_IDX_PD_DRIVER>;
 * scp-id-names = "alarm", "wakeup";
 * 
 * in instance 'inst', of compatible 'compat'
 * 
 * FWK_DT_PH_NAME_HAS_IDX(compat, inst, scp_ids, alarm) will return 0  
 * FWK_DT_PH_NAME_HAS_IDX(compat, inst, scp_ids, wakeup) will return 1
 */
#define FWK_DT_PH_NAME_HAS_IDX(compat, inst, pha_prop, name) \
    DT_PROP(DT_INST(inst, compat), pha_prop##_NAME_##name##_HASIDX)

#define FWK_DT_NODE_PH_NAME_HAS_IDX(node_id, pha_prop, name) \
    DT_PROP(node_id, pha_prop##_NAME_##name##_HASIDX)    

/**
 * @brief Get the index of a phandle name for a phandle array property.
 * 
 * As FWK_DT_PH_NAME_HAS_IDX() except the input filds can be parameterise
 * defines themselves. Forces extra layer of expansion
 * 
 * e.g. 
 * #define SCP_IDS_ATTR_NAME scp_ids
 * #define SCP_ID_ARRAY_NAME alarm
 * 
 * FWK_DT_PH_NAME_HAS_IDX_PARAM(compat, inst, SCP_IDS_ATTR_NAME, SCP_ID_ARRAY_NAME)
 * will expand to 
 * FWK_DT_PH_NAME_HAS_IDX(compat, inst, scp_ids, alarm)
 */
#define FWK_DT_PH_NAME_HAS_IDX_PARAM(compat, inst, pha_prop_param, name) \
    FWK_DT_PH_NAME_HAS_IDX(compat, inst, pha_prop_param, name)

#define FWK_DT_NODE_PH_NAME_HAS_IDX_PARAM(node_id, pha_prop_param, name) \
    FWK_DT_NODE_PH_NAME_HAS_IDX(node_id, pha_prop_param, name)

/**
 * @brief Determine if a phandle name exists for a phandle array property
 *  
 * @param compat Compatible name
 * @param inst Instance number
 * @param pha_prop phandle array property 
 * @param name phandle name 
 * 
 * For a phandle array that has an associated names property, see if a given
 * "name" exists. Can be used as conditional in COND_CODE_1 macro.
 * 
 * e.g for scp-ids property:-
 * 
 * scp-ids = <&sub_elem_id FWK_MODULE_IDX_TIMER JUNO_ALARM_ELEMENT_IDX JUNO_SYSTEM_POWER_ALARM_IDX>, 
 *                         <&elem_id FWK_MODULE_IDX_SYSTEM_POWER MOD_SYSTEM_POWER_API_IDX_PD_DRIVER>;
 * scp-id-names = "alarm", "wakeup";
 * 
 * in instance 'inst', of compatible 'compat'
 * 
 * FWK_DT_PH_NAME_EXISTS(compat, inst, scp-ids, alarm) will return 1  
 * FWK_DT_PH_NAME_EXISTS(compat, inst, scp-ids, sleep) will return 0
 */
#define FWK_DT_PH_NAME_EXISTS(compat, inst, pha_prop, name) \
    IS_ENABLED(DT_PROP(DT_INST(inst, compat), pha_prop##_NAME_##name##_EXISTS))

#define FWK_DT_NODE_PH_NAME_EXISTS(node_id, pha_prop, name) \
    IS_ENABLED(DT_PROP(node_id, pha_prop##_NAME_##name##_EXISTS))


/*DT_PHA_IDX_OF_NAME(node_id, pha, name) \
	DT_PROP(node_id, pha##_NAME_##name##_HASIDX)*/

#define FWK_DT_PH_SCP_ID_NAME_NODE_ID(compat, inst, name) \
    UTIL_CAT(DT_INST(inst, compat), _P_scp_ids##_NAME_##name)

#define FWK_DT_NODE_PH_SCP_ID_NAME_NODE_ID(node_id, name) \
    UTIL_CAT(node_id, _P_scp_ids##_NAME_##name)

/* get the name index */
#define FWK_DT_PH_SCP_ID_NAME_TO_IDX(compat, inst, name) \
    UTIL_CAT(FWK_DT_PH_SCP_ID_NAME_NODE_ID(compat, inst, name), _HASIDX)

#define FWK_DT_NODE_PH_SCP_ID_NAME_TO_IDX(node_id, name) \
    UTIL_CAT(FWK_DT_NODE_PH_SCP_ID_NAME_NODE_ID(node_id, name), _HASIDX)


/* check the name exists */
#define FWK_DT_PH_SCP_ID_NAME_EXISTS(compat, inst, name) \
    IS_ENABLED(UTIL_CAT(FWK_DT_PH_SCP_ID_NAME_NODE_ID(compat, inst, name), _EXISTS))

#define FWK_DT_NODE_PH_SCP_ID_NAME_EXISTS(node_id, name) \
    IS_ENABLED(UTIL_CAT(FWK_DT_NODE_PH_SCP_ID_NAME_NODE_ID(node_id, name), _EXISTS))

/* by name -> convert name to idx - then call index macro */
#define FWK_DT_PH_IDX_SCP_ID_BY_NAME(compat, inst, name) \
    FWK_DT_PH_IDX_SCP_ID_BY_TYPE(compat, inst, FWK_DT_PH_SCP_ID_NAME_TO_IDX(compat, inst, name))

#define FWK_DT_NODE_PH_IDX_SCP_ID_BY_NAME(node_id, name) \
    FWK_DT_NODE_PH_IDX_SCP_ID_BY_TYPE(node_id, FWK_DT_NODE_PH_SCP_ID_NAME_TO_IDX(node_id, name))


/*#define FWK_DT_PH_IDX_SCP_ID_BY_NAME_X(compat, inst, name) \
    FWK_DT_PH_IDX_SCP_ID_BY_TYPE(compat, inst, FWK_DT_PH_NAME_HAS_IDX_PARAM(compat, inst, SCP_DT_IDS_ATTR_N, name))
*/

/* by name or none - if name exists select by name  */
#define FWK_DT_PH_IDX_SCP_ID_BY_NAME_OR_NONE(compat, inst, name) \
    COND_CODE_1(FWK_DT_PH_SCP_ID_NAME_EXISTS(compat, inst, name), \
        (FWK_DT_PH_IDX_SCP_ID_BY_NAME(compat, inst, name)), \
        (FWK_ID_NONE_INIT) \
        )

#define FWK_DT_NODE_PH_IDX_SCP_ID_BY_NAME_OR_NONE(node_id, name) \
    COND_CODE_1(FWK_DT_NODE_PH_SCP_ID_NAME_EXISTS(node_id, name), \
        (FWK_DT_NODE_PH_IDX_SCP_ID_BY_NAME(node_id, name)), \
        (FWK_ID_NONE_INIT) \
        )        
/****/


/**
 * @brief Create an Element ID if a phandle present at array index
 * @param compat Compatible name
 * @param inst Instance number
 * @param array_idx
 *
 * use the phandle elements to create an SCP Framework Element ID if index is present,
 * set to FWK_ID_NONE_INIT if not present.
 */
#define FWK_DT_PH_OPT_IDX_SCP_ID_ELEM(compat, inst, array_idx) \
    COND_CODE_1(DT_PROP_HAS_IDX(DT_INST(inst, compat), scp_ids, array_idx),    \
        (FWK_DT_PH_IDX_SCP_ID_ELEM(compat, inst, array_idx)), \
        (FWK_ID_NONE_INIT) \
    )

/**
 * @brief Create an Element or a Module ID
 * @param compat Compatible name
 * @param inst Instance number
 * @param array_idx
 *
 * Create a element ID if the phandle refers to an object with an elem_idx cell,
 * otherwise treat as module ID
 */
#define FWK_DT_PH_IDX_SCP_ID_MOD_OR_ELEM(compat, inst, array_idx) \
    COND_CODE_1(DT_PHA_HAS_CELL_AT_IDX(DT_INST(inst, compat), scp_ids, array_idx, elem_idx), \
        (FWK_DT_PH_IDX_SCP_ID_ELEM(compat, inst, array_idx)), \
        (FWK_DT_PH_IDX_SCP_ID_MOD(compat, inst, array_idx)) \
    )

/**
 * @}
 */

/**
 * @brief Create a framework element id from array
 * @param compat Compatible name
 * @param inst Instance number
 * @param prop Array property 
 *
 * Intialise a framework element id from a device tree 2 element property array - without a phandle reference.
 * Only use if the phandle reference cannot be used. e.g. extra ID required as a build time optional
 * addition.
 */
#define FWK_DT_FWK_ELEM_ID(compat, inst, prop) \
    FWK_ID_ELEMENT_INIT( \
        FWK_DT_INST_IDX_PROP(compat, inst, prop, 0), \
        FWK_DT_INST_IDX_PROP(compat, inst, prop, 1) \
        )
/** 
 * @brief Optionally Create a framework element id from array
 * @param compat Compatible name
 * @param inst Instance number
 * @param prop Array property 
 *
 * Intialise a framework id from an optional device tree 2 element property array - without a phandle reference.
 * Uses a default FWK_ID_NONE if the property is not present on the node instance.
 * Only use if the phandle reference cannot be used. e.g. extra ID required as a build time optional
 * addition
 */
#define FWK_DT_OPT_FWK_ELEM_ID(compat, inst, prop) \
    COND_CODE_1(DT_NODE_HAS_PROP(DT_INST(inst ,compat), prop), \
        (FWK_DT_FWK_ELEM_ID(compat, inst, prop)), \
        (FWK_ID_NONE_INIT) \
    )

/**
 * @brief Check that a table is of expected size
 * @param table_array Table array source element
 * @param count Expected number of elements in the table
 *
 * check that the table array matches the count of elements from DT
 */
#define FWK_DT_TABLE_CHECK(table_array, element_count) \
    ((sizeof(table_array) / sizeof(table_array[0])) == (element_count))

/**
 * @brief Compile time check for condition
 * @param condition The condition to validate
 *
 * Force a compile time error if 'condition' is false.
 * results in no generated code if 'condition' is true.
 *
 * Use to validate some generated elements that cannot be checked by C
 * pre-processor.
 *
 * example usage:
 *
 *     FWT_DT_BUILD_BUG_ON(!FWK_DT_TABLE_CHECK(my_table, has_4_elements))
 */
#define FWK_DT_BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2*!!(condition)]))

/**
 * @}
 */
