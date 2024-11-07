/* AUTOGENERATED FILE. DO NOT EDIT. */
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include "cmock.h"
#include "Mockmod_mpmm_v2_extra.h"

static const char* CMockString_amu_mmap_get_counters = "amu_mmap_get_counters";
static const char* CMockString_counter_buff = "counter_buff";
static const char* CMockString_num_counter = "num_counter";
static const char* CMockString_start_counter_id = "start_counter_id";

typedef struct _CMOCK_amu_mmap_get_counters_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  char ExpectAnyArgsBool;
  int ReturnVal;
  fwk_id_t Expected_start_counter_id;
  uint64_t* Expected_counter_buff;
  size_t Expected_num_counter;
  int Expected_counter_buff_Depth;
  char ReturnThruPtr_counter_buff_Used;
  uint64_t* ReturnThruPtr_counter_buff_Val;
  size_t ReturnThruPtr_counter_buff_Size;
  char IgnoreArg_start_counter_id;
  char IgnoreArg_counter_buff;
  char IgnoreArg_num_counter;

} CMOCK_amu_mmap_get_counters_CALL_INSTANCE;

static struct Mockmod_mpmm_v2_extraInstance
{
  char amu_mmap_get_counters_IgnoreBool;
  int amu_mmap_get_counters_FinalReturn;
  char amu_mmap_get_counters_CallbackBool;
  CMOCK_amu_mmap_get_counters_CALLBACK amu_mmap_get_counters_CallbackFunctionPointer;
  int amu_mmap_get_counters_CallbackCalls;
  CMOCK_MEM_INDEX_TYPE amu_mmap_get_counters_CallInstance;
} Mock;

extern jmp_buf AbortFrame;

void Mockmod_mpmm_v2_extra_Verify(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_MEM_INDEX_TYPE call_instance;
  call_instance = Mock.amu_mmap_get_counters_CallInstance;
  if (Mock.amu_mmap_get_counters_IgnoreBool)
    call_instance = CMOCK_GUTS_NONE;
  if (CMOCK_GUTS_NONE != call_instance)
  {
    UNITY_SET_DETAIL(CMockString_amu_mmap_get_counters);
    UNITY_TEST_FAIL(cmock_line, CMockStringCalledLess);
  }
  if (Mock.amu_mmap_get_counters_CallbackFunctionPointer != NULL)
  {
    call_instance = CMOCK_GUTS_NONE;
    (void)call_instance;
  }
}

void Mockmod_mpmm_v2_extra_Init(void)
{
  Mockmod_mpmm_v2_extra_Destroy();
}

void Mockmod_mpmm_v2_extra_Destroy(void)
{
  CMock_Guts_MemFreeAll();
  memset(&Mock, 0, sizeof(Mock));
}

int amu_mmap_get_counters(fwk_id_t start_counter_id, uint64_t* counter_buff, size_t num_counter)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_amu_mmap_get_counters_CALL_INSTANCE* cmock_call_instance;
  UNITY_SET_DETAIL(CMockString_amu_mmap_get_counters);
  cmock_call_instance = (CMOCK_amu_mmap_get_counters_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.amu_mmap_get_counters_CallInstance);
  Mock.amu_mmap_get_counters_CallInstance = CMock_Guts_MemNext(Mock.amu_mmap_get_counters_CallInstance);
  if (Mock.amu_mmap_get_counters_IgnoreBool)
  {
    UNITY_CLR_DETAILS();
    if (cmock_call_instance == NULL)
      return Mock.amu_mmap_get_counters_FinalReturn;
    Mock.amu_mmap_get_counters_FinalReturn = cmock_call_instance->ReturnVal;
    return cmock_call_instance->ReturnVal;
  }
  if (!Mock.amu_mmap_get_counters_CallbackBool &&
      Mock.amu_mmap_get_counters_CallbackFunctionPointer != NULL)
  {
    int cmock_cb_ret = Mock.amu_mmap_get_counters_CallbackFunctionPointer(start_counter_id, counter_buff, num_counter, Mock.amu_mmap_get_counters_CallbackCalls++);
    UNITY_CLR_DETAILS();
    return cmock_cb_ret;
  }
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringCalledMore);
  cmock_line = cmock_call_instance->LineNumber;
  if (!cmock_call_instance->ExpectAnyArgsBool)
  {
  if (!cmock_call_instance->IgnoreArg_start_counter_id)
  {
    UNITY_SET_DETAILS(CMockString_amu_mmap_get_counters,CMockString_start_counter_id);
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_start_counter_id), (void*)(&start_counter_id), sizeof(fwk_id_t), cmock_line, CMockStringMismatch);
  }
  if (!cmock_call_instance->IgnoreArg_counter_buff)
  {
    UNITY_SET_DETAILS(CMockString_amu_mmap_get_counters,CMockString_counter_buff);
    if (cmock_call_instance->Expected_counter_buff == NULL)
      { UNITY_TEST_ASSERT_NULL(counter_buff, cmock_line, CMockStringExpNULL); }
    else
      { UNITY_TEST_ASSERT_EQUAL_MEMORY_ARRAY((void*)(cmock_call_instance->Expected_counter_buff), (void*)(counter_buff), sizeof(uint64_t), cmock_call_instance->Expected_counter_buff_Depth, cmock_line, CMockStringMismatch); }
  }
  if (!cmock_call_instance->IgnoreArg_num_counter)
  {
    UNITY_SET_DETAILS(CMockString_amu_mmap_get_counters,CMockString_num_counter);
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_num_counter), (void*)(&num_counter), sizeof(size_t), cmock_line, CMockStringMismatch);
  }
  }
  if (Mock.amu_mmap_get_counters_CallbackFunctionPointer != NULL)
  {
    cmock_call_instance->ReturnVal = Mock.amu_mmap_get_counters_CallbackFunctionPointer(start_counter_id, counter_buff, num_counter, Mock.amu_mmap_get_counters_CallbackCalls++);
  }
  if (cmock_call_instance->ReturnThruPtr_counter_buff_Used)
  {
    UNITY_TEST_ASSERT_NOT_NULL(counter_buff, cmock_line, CMockStringPtrIsNULL);
    memcpy((void*)counter_buff, (void*)cmock_call_instance->ReturnThruPtr_counter_buff_Val,
      cmock_call_instance->ReturnThruPtr_counter_buff_Size);
  }
  UNITY_CLR_DETAILS();
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_amu_mmap_get_counters(CMOCK_amu_mmap_get_counters_CALL_INSTANCE* cmock_call_instance, fwk_id_t start_counter_id, uint64_t* counter_buff, int counter_buff_Depth, size_t num_counter);
void CMockExpectParameters_amu_mmap_get_counters(CMOCK_amu_mmap_get_counters_CALL_INSTANCE* cmock_call_instance, fwk_id_t start_counter_id, uint64_t* counter_buff, int counter_buff_Depth, size_t num_counter)
{
  memcpy((void*)(&cmock_call_instance->Expected_start_counter_id), (void*)(&start_counter_id),
         sizeof(fwk_id_t[sizeof(start_counter_id) == sizeof(fwk_id_t) ? 1 : -1])); /* add fwk_id_t to :treat_as_array if this causes an error */
  cmock_call_instance->IgnoreArg_start_counter_id = 0;
  cmock_call_instance->Expected_counter_buff = counter_buff;
  cmock_call_instance->Expected_counter_buff_Depth = counter_buff_Depth;
  cmock_call_instance->IgnoreArg_counter_buff = 0;
  cmock_call_instance->ReturnThruPtr_counter_buff_Used = 0;
  memcpy((void*)(&cmock_call_instance->Expected_num_counter), (void*)(&num_counter),
         sizeof(size_t[sizeof(num_counter) == sizeof(size_t) ? 1 : -1])); /* add size_t to :treat_as_array if this causes an error */
  cmock_call_instance->IgnoreArg_num_counter = 0;
}

void amu_mmap_get_counters_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, int cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_amu_mmap_get_counters_CALL_INSTANCE));
  CMOCK_amu_mmap_get_counters_CALL_INSTANCE* cmock_call_instance = (CMOCK_amu_mmap_get_counters_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.amu_mmap_get_counters_CallInstance = CMock_Guts_MemChain(Mock.amu_mmap_get_counters_CallInstance, cmock_guts_index);
  Mock.amu_mmap_get_counters_IgnoreBool = (char)0;
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ExpectAnyArgsBool = (char)0;
  cmock_call_instance->ReturnVal = cmock_to_return;
  Mock.amu_mmap_get_counters_IgnoreBool = (char)1;
}

void amu_mmap_get_counters_CMockStopIgnore(void)
{
  if(Mock.amu_mmap_get_counters_IgnoreBool)
    Mock.amu_mmap_get_counters_CallInstance = CMock_Guts_MemNext(Mock.amu_mmap_get_counters_CallInstance);
  Mock.amu_mmap_get_counters_IgnoreBool = (char)0;
}

void amu_mmap_get_counters_CMockExpectAnyArgsAndReturn(UNITY_LINE_TYPE cmock_line, int cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_amu_mmap_get_counters_CALL_INSTANCE));
  CMOCK_amu_mmap_get_counters_CALL_INSTANCE* cmock_call_instance = (CMOCK_amu_mmap_get_counters_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.amu_mmap_get_counters_CallInstance = CMock_Guts_MemChain(Mock.amu_mmap_get_counters_CallInstance, cmock_guts_index);
  Mock.amu_mmap_get_counters_IgnoreBool = (char)0;
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ExpectAnyArgsBool = (char)0;
  cmock_call_instance->ReturnVal = cmock_to_return;
  cmock_call_instance->ExpectAnyArgsBool = (char)1;
}

void amu_mmap_get_counters_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, fwk_id_t start_counter_id, uint64_t* counter_buff, size_t num_counter, int cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_amu_mmap_get_counters_CALL_INSTANCE));
  CMOCK_amu_mmap_get_counters_CALL_INSTANCE* cmock_call_instance = (CMOCK_amu_mmap_get_counters_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.amu_mmap_get_counters_CallInstance = CMock_Guts_MemChain(Mock.amu_mmap_get_counters_CallInstance, cmock_guts_index);
  Mock.amu_mmap_get_counters_IgnoreBool = (char)0;
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ExpectAnyArgsBool = (char)0;
  CMockExpectParameters_amu_mmap_get_counters(cmock_call_instance, start_counter_id, counter_buff, 1, num_counter);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

void amu_mmap_get_counters_AddCallback(CMOCK_amu_mmap_get_counters_CALLBACK Callback)
{
  Mock.amu_mmap_get_counters_IgnoreBool = (char)0;
  Mock.amu_mmap_get_counters_CallbackBool = (char)1;
  Mock.amu_mmap_get_counters_CallbackFunctionPointer = Callback;
}

void amu_mmap_get_counters_Stub(CMOCK_amu_mmap_get_counters_CALLBACK Callback)
{
  Mock.amu_mmap_get_counters_IgnoreBool = (char)0;
  Mock.amu_mmap_get_counters_CallbackBool = (char)0;
  Mock.amu_mmap_get_counters_CallbackFunctionPointer = Callback;
}

void amu_mmap_get_counters_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, fwk_id_t start_counter_id, uint64_t* counter_buff, int counter_buff_Depth, size_t num_counter, int cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_amu_mmap_get_counters_CALL_INSTANCE));
  CMOCK_amu_mmap_get_counters_CALL_INSTANCE* cmock_call_instance = (CMOCK_amu_mmap_get_counters_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.amu_mmap_get_counters_CallInstance = CMock_Guts_MemChain(Mock.amu_mmap_get_counters_CallInstance, cmock_guts_index);
  Mock.amu_mmap_get_counters_IgnoreBool = (char)0;
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ExpectAnyArgsBool = (char)0;
  CMockExpectParameters_amu_mmap_get_counters(cmock_call_instance, start_counter_id, counter_buff, counter_buff_Depth, num_counter);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

void amu_mmap_get_counters_CMockReturnMemThruPtr_counter_buff(UNITY_LINE_TYPE cmock_line, uint64_t* counter_buff, size_t cmock_size)
{
  CMOCK_amu_mmap_get_counters_CALL_INSTANCE* cmock_call_instance = (CMOCK_amu_mmap_get_counters_CALL_INSTANCE*)CMock_Guts_GetAddressFor(CMock_Guts_MemEndOfChain(Mock.amu_mmap_get_counters_CallInstance));
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringPtrPreExp);
  cmock_call_instance->ReturnThruPtr_counter_buff_Used = 1;
  cmock_call_instance->ReturnThruPtr_counter_buff_Val = counter_buff;
  cmock_call_instance->ReturnThruPtr_counter_buff_Size = cmock_size;
}

void amu_mmap_get_counters_CMockIgnoreArg_start_counter_id(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_amu_mmap_get_counters_CALL_INSTANCE* cmock_call_instance = (CMOCK_amu_mmap_get_counters_CALL_INSTANCE*)CMock_Guts_GetAddressFor(CMock_Guts_MemEndOfChain(Mock.amu_mmap_get_counters_CallInstance));
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringIgnPreExp);
  cmock_call_instance->IgnoreArg_start_counter_id = 1;
}

void amu_mmap_get_counters_CMockIgnoreArg_counter_buff(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_amu_mmap_get_counters_CALL_INSTANCE* cmock_call_instance = (CMOCK_amu_mmap_get_counters_CALL_INSTANCE*)CMock_Guts_GetAddressFor(CMock_Guts_MemEndOfChain(Mock.amu_mmap_get_counters_CallInstance));
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringIgnPreExp);
  cmock_call_instance->IgnoreArg_counter_buff = 1;
}

void amu_mmap_get_counters_CMockIgnoreArg_num_counter(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_amu_mmap_get_counters_CALL_INSTANCE* cmock_call_instance = (CMOCK_amu_mmap_get_counters_CALL_INSTANCE*)CMock_Guts_GetAddressFor(CMock_Guts_MemEndOfChain(Mock.amu_mmap_get_counters_CallInstance));
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringIgnPreExp);
  cmock_call_instance->IgnoreArg_num_counter = 1;
}

