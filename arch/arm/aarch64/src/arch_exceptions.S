/*
 * Arm SCP/MCP Software
 * Copyright (c) 2024, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

.macro ventry label
.align	7
b  \label
.endm

.global exception_vectors
exception_vectors:
    /* current EL, SP_EL0 */
	ventry	err_exception	/* synchronous */
	ventry	err_exception	/* IRQ */
	ventry	err_exception	/* FIQ */
	ventry	err_exception	/* SError */

err_exception:
    wfe
    b err_exception
