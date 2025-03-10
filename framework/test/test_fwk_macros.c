/*
 * Arm SCP/MCP Software
 * Copyright (c) 2015-2024, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <fwk_assert.h>
#include <fwk_macros.h>
#include <fwk_test.h>

static void test_fwk_macros_array_size(void);
static void test_fwk_macros_align_next(void);
static void test_fwk_macros_align_previous(void);
static void test_fwk_macros_power_of_two_check(void);
static void test_fwk_macros_align_check(void);
static void test_fwk_macros_bit(void);
static void test_fwk_macros_bit_mask(void);
static void test_fwk_macros_gen_mask(void);

static const struct fwk_test_case_desc test_case_table[] = {
    FWK_TEST_CASE(test_fwk_macros_array_size),
    FWK_TEST_CASE(test_fwk_macros_align_next),
    FWK_TEST_CASE(test_fwk_macros_align_previous),
    FWK_TEST_CASE(test_fwk_macros_power_of_two_check),
    FWK_TEST_CASE(test_fwk_macros_align_check),
    FWK_TEST_CASE(test_fwk_macros_bit),
    FWK_TEST_CASE(test_fwk_macros_bit_mask),
    FWK_TEST_CASE(test_fwk_macros_gen_mask),
};

struct fwk_test_suite_desc test_suite = {
    .name = "fwk_macros",
    .test_case_count = FWK_ARRAY_SIZE(test_case_table),
    .test_case_table = test_case_table,
};

static void test_fwk_macros_array_size(void)
{
    unsigned int element_no;
    unsigned int array_uint[5];
    char array_char[3];

    element_no = FWK_ARRAY_SIZE(array_uint);
    assert(element_no == 5);

    element_no = FWK_ARRAY_SIZE(array_char);
    assert(element_no == 3);
}

static void test_fwk_macros_align_next(void)
{
    unsigned int value;
    unsigned int interval;
    unsigned int result;

    value = 0;
    interval = 32;

    /* Precedence test */
    result = FWK_ALIGN_NEXT(value | 1, interval + 32);
    assert(result == 64);

    value = 0;
    interval = 32;
    result = FWK_ALIGN_NEXT(value, interval);
    assert(result == 0);

    value = 8;
    interval = 8;
    result = FWK_ALIGN_NEXT(value, interval);
    assert(result == 8);

    value = 9;
    interval = 8;
    result = FWK_ALIGN_NEXT(value, interval);
    assert(result == 16);
}

static void test_fwk_macros_align_previous(void)
{
    unsigned int value;
    unsigned int interval;
    unsigned int result;

    value = 65;
    interval = 32;

    /* Precedence test */
    result = FWK_ALIGN_PREVIOUS(value & 1, interval + 32);
    assert(result == 0);

    value = 0;
    interval = 32;
    result = FWK_ALIGN_PREVIOUS(value, interval);
    assert(result == 0);

    value = 8;
    interval = 8;
    result = FWK_ALIGN_PREVIOUS(value, interval);
    assert(result == 8);

    value = 9;
    interval = 8;
    result = FWK_ALIGN_PREVIOUS(value, interval);
    assert(result == 8);
}

static void test_fwk_macros_power_of_two_check(void)
{
    bool result;
    unsigned int value;
    struct power_of_two_test_data {
        unsigned long long value;
        bool verdict;
    } test_data[] = {
        { 0x00000, false }, { 0x00017, false },   { 0x0006C, false },
        { 0x0004D, false }, { 0x00017, false },   { 0x002aC, false },
        { 0x102aC, false }, { 0x00001, true },    { 0x00100, true },
        { 0x00004, true },  { 0x00080, true },    { 0x100000, true },
        { 0x400000, true }, { 0x80000000, true }, { 0xFFFFFFFF, false },
    };
    unsigned int test_count = sizeof(test_data) / sizeof(test_data[0]);

    /* Precedence tests */
    value = 3;
    result = FWK_IS_VALUE_POWER_OF_TWO(value & 1);
    assert(result == true);

    value = 3;
    result = FWK_IS_VALUE_POWER_OF_TWO(value & 2);
    assert(result == true);

    value = 5;
    result = FWK_IS_VALUE_POWER_OF_TWO(value & 1);
    assert(result == true);

    value = 5;
    result = FWK_IS_VALUE_POWER_OF_TWO(value & 4);
    assert(result == true);

    for (unsigned int i = 0; i < test_count; ++i) {
        result = FWK_IS_VALUE_POWER_OF_TWO(test_data[i].value);
        assert(result == test_data[i].verdict);
    }
}

static void test_fwk_macros_align_check(void)
{
    bool result;
    struct alignment_test_data {
        unsigned int value;
        unsigned int alignment;
        bool verdict;
    } test_data[] = {
        { 0x0000, 0, false },     { 0x0000, 4, true },
        { 0x0000, 32, true },     { 0x4000, 4, true },
        { 0x4000, 8, true },      { 0x2004, 8, false },
        { 0x1000, 32, true },     { 0x4020, 32, true },
        { 0x4000, 15, false },    { 0x4000, 17, false },
        { 0x3008, 16, false },    { 0x9000, 64, true },
        { 0x9040, 64, true },     { 0x9048, 64, false },
        { 0x9040, 67, false },    { 0xFFFFFFFF, 0, false },
        { 0xFFFFFFFF, 4, false }, { 0xFFFFFFFF, 32, false },
    };
    unsigned int test_count = sizeof(test_data) / sizeof(test_data[0]);
    struct alignment_test_data test;
    /* Precedence tests */
    test.value = 3;
    test.alignment = 4;
    result = FWK_IS_ALIGNED(test.value & 1, test.alignment + 1);
    assert(result == false);

    test.value = 0x4000;
    test.alignment = 3;
    result = FWK_IS_ALIGNED(test.value, test.alignment + 1);
    assert(result == true);

    for (unsigned int i = 0; i < test_count; ++i) {
        result = FWK_IS_ALIGNED(test_data[i].value, test_data[i].alignment);
        assert(result == test_data[i].verdict);
    }
}

static void test_fwk_macros_bit(void)
{
    assert(FWK_BIT(0) == 0x1);
    assert(FWK_BIT(1) == 0x2);
    assert(FWK_BIT(30) == 0x40000000);
    assert(FWK_BIT(31) == 0x80000000);

    assert(FWK_BIT_64(0) == 0x1);
    assert(FWK_BIT_64(1) == 0x2);
    assert(FWK_BIT_64(62) == 0x4000000000000000);
    assert(FWK_BIT_64(63) == 0x8000000000000000);
}

static void test_fwk_macros_bit_mask(void)
{
    assert(FWK_BIT_MASK(0) == 0x0);
    assert(FWK_BIT_MASK(1) == 0x1);
    assert(FWK_BIT_MASK(31) == 0x7FFFFFFF);

    assert(FWK_BIT_MASK_64(0) == 0x0);
    assert(FWK_BIT_MASK_64(1) == 0x1);
    assert(FWK_BIT_MASK_64(63) == 0x7FFFFFFFFFFFFFFF);
}

static void test_fwk_macros_gen_mask(void)
{
    assert(FWK_GEN_MASK(0, 0) == 0x1);
    assert(FWK_GEN_MASK(1, 0) == 0x3);
    assert(FWK_GEN_MASK(2, 1) == 0x6);
    assert(FWK_GEN_MASK(30, 1) == 0x7FFFFFFE);
    assert(FWK_GEN_MASK(31, 0) == 0xFFFFFFFF);
    assert(FWK_GEN_MASK(31, 31) == 0x80000000);

    assert(FWK_GEN_MASK_64(0, 0) == 0x1);
    assert(FWK_GEN_MASK_64(1, 0) == 0x3);
    assert(FWK_GEN_MASK_64(2, 1) == 0x6);
    assert(FWK_GEN_MASK_64(62, 1) == 0x7FFFFFFFFFFFFFFE);
    assert(FWK_GEN_MASK_64(63, 0) == 0xFFFFFFFFFFFFFFFF);
    assert(FWK_GEN_MASK_64(63, 63) == 0x8000000000000000);
}
