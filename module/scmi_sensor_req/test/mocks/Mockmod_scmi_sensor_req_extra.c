/* AUTOGENERATED FILE. DO NOT EDIT. */
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include "cmock.h"
#include "Mockmod_scmi_sensor_req_extra.h"

static const char* CMockString_fake_message_handler = "fake_message_handler";
static const char* CMockString_id = "id";
static const char* CMockString_payload = "payload";
static const char* CMockString_payload_size = "payload_size";
static const char* CMockString_reading_complete = "reading_complete";
static const char* CMockString_request_ack_by_interrupt = "request_ack_by_interrupt";
static const char* CMockString_response = "response";
static const char* CMockString_response_message_handler = "response_message_handler";
static const char* CMockString_scmi_message_id = "scmi_message_id";
static const char* CMockString_scmi_protocol_id = "scmi_protocol_id";
static const char* CMockString_scmi_send_message = "scmi_send_message";
static const char* CMockString_service_id = "service_id";
static const char* CMockString_token = "token";

typedef struct _CMOCK_reading_complete_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  char ExpectAnyArgsBool;
  fwk_id_t Expected_id;
  struct mod_sensor_driver_resp_params* Expected_response;
  int Expected_response_Depth;
  char ReturnThruPtr_response_Used;
  struct mod_sensor_driver_resp_params* ReturnThruPtr_response_Val;
  size_t ReturnThruPtr_response_Size;

} CMOCK_reading_complete_CALL_INSTANCE;

typedef struct _CMOCK_scmi_send_message_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  char ExpectAnyArgsBool;
  int ReturnVal;
  uint8_t Expected_scmi_message_id;
  uint8_t Expected_scmi_protocol_id;
  uint8_t Expected_token;
  fwk_id_t Expected_service_id;
  const void* Expected_payload;
  size_t Expected_payload_size;
  bool Expected_request_ack_by_interrupt;
  int Expected_payload_Depth;

} CMOCK_scmi_send_message_CALL_INSTANCE;

typedef struct _CMOCK_response_message_handler_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  char ExpectAnyArgsBool;
  int ReturnVal;
  fwk_id_t Expected_service_id;

} CMOCK_response_message_handler_CALL_INSTANCE;

typedef struct _CMOCK_fake_message_handler_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  char ExpectAnyArgsBool;
  int ReturnVal;
  fwk_id_t Expected_service_id;
  const uint32_t* Expected_payload;
  size_t Expected_payload_size;
  int Expected_payload_Depth;

} CMOCK_fake_message_handler_CALL_INSTANCE;

static struct Mockmod_scmi_sensor_req_extraInstance
{
  char reading_complete_CallbackBool;
  CMOCK_reading_complete_CALLBACK reading_complete_CallbackFunctionPointer;
  int reading_complete_CallbackCalls;
  CMOCK_MEM_INDEX_TYPE reading_complete_CallInstance;
  char scmi_send_message_CallbackBool;
  CMOCK_scmi_send_message_CALLBACK scmi_send_message_CallbackFunctionPointer;
  int scmi_send_message_CallbackCalls;
  CMOCK_MEM_INDEX_TYPE scmi_send_message_CallInstance;
  char response_message_handler_CallbackBool;
  CMOCK_response_message_handler_CALLBACK response_message_handler_CallbackFunctionPointer;
  int response_message_handler_CallbackCalls;
  CMOCK_MEM_INDEX_TYPE response_message_handler_CallInstance;
  char fake_message_handler_CallbackBool;
  CMOCK_fake_message_handler_CALLBACK fake_message_handler_CallbackFunctionPointer;
  int fake_message_handler_CallbackCalls;
  CMOCK_MEM_INDEX_TYPE fake_message_handler_CallInstance;
} Mock;

extern jmp_buf AbortFrame;

void Mockmod_scmi_sensor_req_extra_Verify(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_MEM_INDEX_TYPE call_instance;
  call_instance = Mock.reading_complete_CallInstance;
  if (CMOCK_GUTS_NONE != call_instance)
  {
    UNITY_SET_DETAIL(CMockString_reading_complete);
    UNITY_TEST_FAIL(cmock_line, CMockStringCalledLess);
  }
  if (Mock.reading_complete_CallbackFunctionPointer != NULL)
  {
    call_instance = CMOCK_GUTS_NONE;
    (void)call_instance;
  }
  call_instance = Mock.scmi_send_message_CallInstance;
  if (CMOCK_GUTS_NONE != call_instance)
  {
    UNITY_SET_DETAIL(CMockString_scmi_send_message);
    UNITY_TEST_FAIL(cmock_line, CMockStringCalledLess);
  }
  if (Mock.scmi_send_message_CallbackFunctionPointer != NULL)
  {
    call_instance = CMOCK_GUTS_NONE;
    (void)call_instance;
  }
  call_instance = Mock.response_message_handler_CallInstance;
  if (CMOCK_GUTS_NONE != call_instance)
  {
    UNITY_SET_DETAIL(CMockString_response_message_handler);
    UNITY_TEST_FAIL(cmock_line, CMockStringCalledLess);
  }
  if (Mock.response_message_handler_CallbackFunctionPointer != NULL)
  {
    call_instance = CMOCK_GUTS_NONE;
    (void)call_instance;
  }
  call_instance = Mock.fake_message_handler_CallInstance;
  if (CMOCK_GUTS_NONE != call_instance)
  {
    UNITY_SET_DETAIL(CMockString_fake_message_handler);
    UNITY_TEST_FAIL(cmock_line, CMockStringCalledLess);
  }
  if (Mock.fake_message_handler_CallbackFunctionPointer != NULL)
  {
    call_instance = CMOCK_GUTS_NONE;
    (void)call_instance;
  }
}

void Mockmod_scmi_sensor_req_extra_Init(void)
{
  Mockmod_scmi_sensor_req_extra_Destroy();
}

void Mockmod_scmi_sensor_req_extra_Destroy(void)
{
  CMock_Guts_MemFreeAll();
  memset(&Mock, 0, sizeof(Mock));
}

void reading_complete(fwk_id_t id, struct mod_sensor_driver_resp_params* response)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_reading_complete_CALL_INSTANCE* cmock_call_instance;
  UNITY_SET_DETAIL(CMockString_reading_complete);
  cmock_call_instance = (CMOCK_reading_complete_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.reading_complete_CallInstance);
  Mock.reading_complete_CallInstance = CMock_Guts_MemNext(Mock.reading_complete_CallInstance);
  if (!Mock.reading_complete_CallbackBool &&
      Mock.reading_complete_CallbackFunctionPointer != NULL)
  {
    Mock.reading_complete_CallbackFunctionPointer(id, response, Mock.reading_complete_CallbackCalls++);
    UNITY_CLR_DETAILS();
    return;
  }
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringCalledMore);
  cmock_line = cmock_call_instance->LineNumber;
  if (!cmock_call_instance->ExpectAnyArgsBool)
  {
  {
    UNITY_SET_DETAILS(CMockString_reading_complete,CMockString_id);
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_id), (void*)(&id), sizeof(fwk_id_t), cmock_line, CMockStringMismatch);
  }
  {
    UNITY_SET_DETAILS(CMockString_reading_complete,CMockString_response);
    if (cmock_call_instance->Expected_response == NULL)
      { UNITY_TEST_ASSERT_NULL(response, cmock_line, CMockStringExpNULL); }
    else
      { UNITY_TEST_ASSERT_EQUAL_MEMORY_ARRAY((void*)(cmock_call_instance->Expected_response), (void*)(response), sizeof(struct mod_sensor_driver_resp_params), cmock_call_instance->Expected_response_Depth, cmock_line, CMockStringMismatch); }
  }
  }
  if (Mock.reading_complete_CallbackFunctionPointer != NULL)
  {
    Mock.reading_complete_CallbackFunctionPointer(id, response, Mock.reading_complete_CallbackCalls++);
  }
  if (cmock_call_instance->ReturnThruPtr_response_Used)
  {
    UNITY_TEST_ASSERT_NOT_NULL(response, cmock_line, CMockStringPtrIsNULL);
    memcpy((void*)response, (void*)cmock_call_instance->ReturnThruPtr_response_Val,
      cmock_call_instance->ReturnThruPtr_response_Size);
  }
  UNITY_CLR_DETAILS();
}

void CMockExpectParameters_reading_complete(CMOCK_reading_complete_CALL_INSTANCE* cmock_call_instance, fwk_id_t id, struct mod_sensor_driver_resp_params* response, int response_Depth);
void CMockExpectParameters_reading_complete(CMOCK_reading_complete_CALL_INSTANCE* cmock_call_instance, fwk_id_t id, struct mod_sensor_driver_resp_params* response, int response_Depth)
{
  memcpy((void*)(&cmock_call_instance->Expected_id), (void*)(&id),
         sizeof(fwk_id_t[sizeof(id) == sizeof(fwk_id_t) ? 1 : -1])); /* add fwk_id_t to :treat_as_array if this causes an error */
  cmock_call_instance->Expected_response = response;
  cmock_call_instance->Expected_response_Depth = response_Depth;
  cmock_call_instance->ReturnThruPtr_response_Used = 0;
}

void reading_complete_CMockExpectAnyArgs(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_reading_complete_CALL_INSTANCE));
  CMOCK_reading_complete_CALL_INSTANCE* cmock_call_instance = (CMOCK_reading_complete_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.reading_complete_CallInstance = CMock_Guts_MemChain(Mock.reading_complete_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ExpectAnyArgsBool = (char)0;
  cmock_call_instance->ExpectAnyArgsBool = (char)1;
}

void reading_complete_CMockExpect(UNITY_LINE_TYPE cmock_line, fwk_id_t id, struct mod_sensor_driver_resp_params* response)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_reading_complete_CALL_INSTANCE));
  CMOCK_reading_complete_CALL_INSTANCE* cmock_call_instance = (CMOCK_reading_complete_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.reading_complete_CallInstance = CMock_Guts_MemChain(Mock.reading_complete_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ExpectAnyArgsBool = (char)0;
  CMockExpectParameters_reading_complete(cmock_call_instance, id, response, 1);
}

void reading_complete_AddCallback(CMOCK_reading_complete_CALLBACK Callback)
{
  Mock.reading_complete_CallbackBool = (char)1;
  Mock.reading_complete_CallbackFunctionPointer = Callback;
}

void reading_complete_Stub(CMOCK_reading_complete_CALLBACK Callback)
{
  Mock.reading_complete_CallbackBool = (char)0;
  Mock.reading_complete_CallbackFunctionPointer = Callback;
}

void reading_complete_CMockExpectWithArray(UNITY_LINE_TYPE cmock_line, fwk_id_t id, struct mod_sensor_driver_resp_params* response, int response_Depth)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_reading_complete_CALL_INSTANCE));
  CMOCK_reading_complete_CALL_INSTANCE* cmock_call_instance = (CMOCK_reading_complete_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.reading_complete_CallInstance = CMock_Guts_MemChain(Mock.reading_complete_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ExpectAnyArgsBool = (char)0;
  CMockExpectParameters_reading_complete(cmock_call_instance, id, response, response_Depth);
}

void reading_complete_CMockReturnMemThruPtr_response(UNITY_LINE_TYPE cmock_line, struct mod_sensor_driver_resp_params* response, size_t cmock_size)
{
  CMOCK_reading_complete_CALL_INSTANCE* cmock_call_instance = (CMOCK_reading_complete_CALL_INSTANCE*)CMock_Guts_GetAddressFor(CMock_Guts_MemEndOfChain(Mock.reading_complete_CallInstance));
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringPtrPreExp);
  cmock_call_instance->ReturnThruPtr_response_Used = 1;
  cmock_call_instance->ReturnThruPtr_response_Val = response;
  cmock_call_instance->ReturnThruPtr_response_Size = cmock_size;
}

int scmi_send_message(uint8_t scmi_message_id, uint8_t scmi_protocol_id, uint8_t token, fwk_id_t service_id, const void* payload, size_t payload_size, bool request_ack_by_interrupt)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_scmi_send_message_CALL_INSTANCE* cmock_call_instance;
  UNITY_SET_DETAIL(CMockString_scmi_send_message);
  cmock_call_instance = (CMOCK_scmi_send_message_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.scmi_send_message_CallInstance);
  Mock.scmi_send_message_CallInstance = CMock_Guts_MemNext(Mock.scmi_send_message_CallInstance);
  if (!Mock.scmi_send_message_CallbackBool &&
      Mock.scmi_send_message_CallbackFunctionPointer != NULL)
  {
    int cmock_cb_ret = Mock.scmi_send_message_CallbackFunctionPointer(scmi_message_id, scmi_protocol_id, token, service_id, payload, payload_size, request_ack_by_interrupt, Mock.scmi_send_message_CallbackCalls++);
    UNITY_CLR_DETAILS();
    return cmock_cb_ret;
  }
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringCalledMore);
  cmock_line = cmock_call_instance->LineNumber;
  if (!cmock_call_instance->ExpectAnyArgsBool)
  {
  {
    UNITY_SET_DETAILS(CMockString_scmi_send_message,CMockString_scmi_message_id);
    UNITY_TEST_ASSERT_EQUAL_HEX8(cmock_call_instance->Expected_scmi_message_id, scmi_message_id, cmock_line, CMockStringMismatch);
  }
  {
    UNITY_SET_DETAILS(CMockString_scmi_send_message,CMockString_scmi_protocol_id);
    UNITY_TEST_ASSERT_EQUAL_HEX8(cmock_call_instance->Expected_scmi_protocol_id, scmi_protocol_id, cmock_line, CMockStringMismatch);
  }
  {
    UNITY_SET_DETAILS(CMockString_scmi_send_message,CMockString_token);
    UNITY_TEST_ASSERT_EQUAL_HEX8(cmock_call_instance->Expected_token, token, cmock_line, CMockStringMismatch);
  }
  {
    UNITY_SET_DETAILS(CMockString_scmi_send_message,CMockString_service_id);
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_service_id), (void*)(&service_id), sizeof(fwk_id_t), cmock_line, CMockStringMismatch);
  }
  {
    UNITY_SET_DETAILS(CMockString_scmi_send_message,CMockString_payload);
    if (cmock_call_instance->Expected_payload == NULL)
      { UNITY_TEST_ASSERT_NULL(payload, cmock_line, CMockStringExpNULL); }
    else
      { UNITY_TEST_ASSERT_EQUAL_HEX8_ARRAY(cmock_call_instance->Expected_payload, payload, cmock_call_instance->Expected_payload_Depth, cmock_line, CMockStringMismatch); }
  }
  {
    UNITY_SET_DETAILS(CMockString_scmi_send_message,CMockString_payload_size);
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_payload_size), (void*)(&payload_size), sizeof(size_t), cmock_line, CMockStringMismatch);
  }
  {
    UNITY_SET_DETAILS(CMockString_scmi_send_message,CMockString_request_ack_by_interrupt);
    UNITY_TEST_ASSERT_EQUAL_INT(cmock_call_instance->Expected_request_ack_by_interrupt, request_ack_by_interrupt, cmock_line, CMockStringMismatch);
  }
  }
  if (Mock.scmi_send_message_CallbackFunctionPointer != NULL)
  {
    cmock_call_instance->ReturnVal = Mock.scmi_send_message_CallbackFunctionPointer(scmi_message_id, scmi_protocol_id, token, service_id, payload, payload_size, request_ack_by_interrupt, Mock.scmi_send_message_CallbackCalls++);
  }
  UNITY_CLR_DETAILS();
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_scmi_send_message(CMOCK_scmi_send_message_CALL_INSTANCE* cmock_call_instance, uint8_t scmi_message_id, uint8_t scmi_protocol_id, uint8_t token, fwk_id_t service_id, const void* payload, int payload_Depth, size_t payload_size, bool request_ack_by_interrupt);
void CMockExpectParameters_scmi_send_message(CMOCK_scmi_send_message_CALL_INSTANCE* cmock_call_instance, uint8_t scmi_message_id, uint8_t scmi_protocol_id, uint8_t token, fwk_id_t service_id, const void* payload, int payload_Depth, size_t payload_size, bool request_ack_by_interrupt)
{
  cmock_call_instance->Expected_scmi_message_id = scmi_message_id;
  cmock_call_instance->Expected_scmi_protocol_id = scmi_protocol_id;
  cmock_call_instance->Expected_token = token;
  memcpy((void*)(&cmock_call_instance->Expected_service_id), (void*)(&service_id),
         sizeof(fwk_id_t[sizeof(service_id) == sizeof(fwk_id_t) ? 1 : -1])); /* add fwk_id_t to :treat_as_array if this causes an error */
  cmock_call_instance->Expected_payload = payload;
  cmock_call_instance->Expected_payload_Depth = payload_Depth;
  memcpy((void*)(&cmock_call_instance->Expected_payload_size), (void*)(&payload_size),
         sizeof(size_t[sizeof(payload_size) == sizeof(size_t) ? 1 : -1])); /* add size_t to :treat_as_array if this causes an error */
  cmock_call_instance->Expected_request_ack_by_interrupt = request_ack_by_interrupt;
}

void scmi_send_message_CMockExpectAnyArgsAndReturn(UNITY_LINE_TYPE cmock_line, int cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_scmi_send_message_CALL_INSTANCE));
  CMOCK_scmi_send_message_CALL_INSTANCE* cmock_call_instance = (CMOCK_scmi_send_message_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.scmi_send_message_CallInstance = CMock_Guts_MemChain(Mock.scmi_send_message_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ExpectAnyArgsBool = (char)0;
  cmock_call_instance->ReturnVal = cmock_to_return;
  cmock_call_instance->ExpectAnyArgsBool = (char)1;
}

void scmi_send_message_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t scmi_message_id, uint8_t scmi_protocol_id, uint8_t token, fwk_id_t service_id, const void* payload, size_t payload_size, bool request_ack_by_interrupt, int cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_scmi_send_message_CALL_INSTANCE));
  CMOCK_scmi_send_message_CALL_INSTANCE* cmock_call_instance = (CMOCK_scmi_send_message_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.scmi_send_message_CallInstance = CMock_Guts_MemChain(Mock.scmi_send_message_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ExpectAnyArgsBool = (char)0;
  CMockExpectParameters_scmi_send_message(cmock_call_instance, scmi_message_id, scmi_protocol_id, token, service_id, payload, payload_size, payload_size, request_ack_by_interrupt);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

void scmi_send_message_AddCallback(CMOCK_scmi_send_message_CALLBACK Callback)
{
  Mock.scmi_send_message_CallbackBool = (char)1;
  Mock.scmi_send_message_CallbackFunctionPointer = Callback;
}

void scmi_send_message_Stub(CMOCK_scmi_send_message_CALLBACK Callback)
{
  Mock.scmi_send_message_CallbackBool = (char)0;
  Mock.scmi_send_message_CallbackFunctionPointer = Callback;
}

void scmi_send_message_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t scmi_message_id, uint8_t scmi_protocol_id, uint8_t token, fwk_id_t service_id, const void* payload, int payload_Depth, size_t payload_size, bool request_ack_by_interrupt, int cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_scmi_send_message_CALL_INSTANCE));
  CMOCK_scmi_send_message_CALL_INSTANCE* cmock_call_instance = (CMOCK_scmi_send_message_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.scmi_send_message_CallInstance = CMock_Guts_MemChain(Mock.scmi_send_message_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ExpectAnyArgsBool = (char)0;
  CMockExpectParameters_scmi_send_message(cmock_call_instance, scmi_message_id, scmi_protocol_id, token, service_id, payload, payload_Depth, payload_size, request_ack_by_interrupt);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

int response_message_handler(fwk_id_t service_id)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_response_message_handler_CALL_INSTANCE* cmock_call_instance;
  UNITY_SET_DETAIL(CMockString_response_message_handler);
  cmock_call_instance = (CMOCK_response_message_handler_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.response_message_handler_CallInstance);
  Mock.response_message_handler_CallInstance = CMock_Guts_MemNext(Mock.response_message_handler_CallInstance);
  if (!Mock.response_message_handler_CallbackBool &&
      Mock.response_message_handler_CallbackFunctionPointer != NULL)
  {
    int cmock_cb_ret = Mock.response_message_handler_CallbackFunctionPointer(service_id, Mock.response_message_handler_CallbackCalls++);
    UNITY_CLR_DETAILS();
    return cmock_cb_ret;
  }
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringCalledMore);
  cmock_line = cmock_call_instance->LineNumber;
  if (!cmock_call_instance->ExpectAnyArgsBool)
  {
  {
    UNITY_SET_DETAILS(CMockString_response_message_handler,CMockString_service_id);
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_service_id), (void*)(&service_id), sizeof(fwk_id_t), cmock_line, CMockStringMismatch);
  }
  }
  if (Mock.response_message_handler_CallbackFunctionPointer != NULL)
  {
    cmock_call_instance->ReturnVal = Mock.response_message_handler_CallbackFunctionPointer(service_id, Mock.response_message_handler_CallbackCalls++);
  }
  UNITY_CLR_DETAILS();
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_response_message_handler(CMOCK_response_message_handler_CALL_INSTANCE* cmock_call_instance, fwk_id_t service_id);
void CMockExpectParameters_response_message_handler(CMOCK_response_message_handler_CALL_INSTANCE* cmock_call_instance, fwk_id_t service_id)
{
  memcpy((void*)(&cmock_call_instance->Expected_service_id), (void*)(&service_id),
         sizeof(fwk_id_t[sizeof(service_id) == sizeof(fwk_id_t) ? 1 : -1])); /* add fwk_id_t to :treat_as_array if this causes an error */
}

void response_message_handler_CMockExpectAnyArgsAndReturn(UNITY_LINE_TYPE cmock_line, int cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_response_message_handler_CALL_INSTANCE));
  CMOCK_response_message_handler_CALL_INSTANCE* cmock_call_instance = (CMOCK_response_message_handler_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.response_message_handler_CallInstance = CMock_Guts_MemChain(Mock.response_message_handler_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ExpectAnyArgsBool = (char)0;
  cmock_call_instance->ReturnVal = cmock_to_return;
  cmock_call_instance->ExpectAnyArgsBool = (char)1;
}

void response_message_handler_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, fwk_id_t service_id, int cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_response_message_handler_CALL_INSTANCE));
  CMOCK_response_message_handler_CALL_INSTANCE* cmock_call_instance = (CMOCK_response_message_handler_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.response_message_handler_CallInstance = CMock_Guts_MemChain(Mock.response_message_handler_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ExpectAnyArgsBool = (char)0;
  CMockExpectParameters_response_message_handler(cmock_call_instance, service_id);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

void response_message_handler_AddCallback(CMOCK_response_message_handler_CALLBACK Callback)
{
  Mock.response_message_handler_CallbackBool = (char)1;
  Mock.response_message_handler_CallbackFunctionPointer = Callback;
}

void response_message_handler_Stub(CMOCK_response_message_handler_CALLBACK Callback)
{
  Mock.response_message_handler_CallbackBool = (char)0;
  Mock.response_message_handler_CallbackFunctionPointer = Callback;
}

int fake_message_handler(fwk_id_t service_id, const uint32_t* payload, size_t payload_size)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_fake_message_handler_CALL_INSTANCE* cmock_call_instance;
  UNITY_SET_DETAIL(CMockString_fake_message_handler);
  cmock_call_instance = (CMOCK_fake_message_handler_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.fake_message_handler_CallInstance);
  Mock.fake_message_handler_CallInstance = CMock_Guts_MemNext(Mock.fake_message_handler_CallInstance);
  if (!Mock.fake_message_handler_CallbackBool &&
      Mock.fake_message_handler_CallbackFunctionPointer != NULL)
  {
    int cmock_cb_ret = Mock.fake_message_handler_CallbackFunctionPointer(service_id, payload, payload_size, Mock.fake_message_handler_CallbackCalls++);
    UNITY_CLR_DETAILS();
    return cmock_cb_ret;
  }
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringCalledMore);
  cmock_line = cmock_call_instance->LineNumber;
  if (!cmock_call_instance->ExpectAnyArgsBool)
  {
  {
    UNITY_SET_DETAILS(CMockString_fake_message_handler,CMockString_service_id);
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_service_id), (void*)(&service_id), sizeof(fwk_id_t), cmock_line, CMockStringMismatch);
  }
  {
    UNITY_SET_DETAILS(CMockString_fake_message_handler,CMockString_payload);
    if (cmock_call_instance->Expected_payload == NULL)
      { UNITY_TEST_ASSERT_NULL(payload, cmock_line, CMockStringExpNULL); }
    else
      { UNITY_TEST_ASSERT_EQUAL_HEX32_ARRAY(cmock_call_instance->Expected_payload, payload, cmock_call_instance->Expected_payload_Depth, cmock_line, CMockStringMismatch); }
  }
  {
    UNITY_SET_DETAILS(CMockString_fake_message_handler,CMockString_payload_size);
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_payload_size), (void*)(&payload_size), sizeof(size_t), cmock_line, CMockStringMismatch);
  }
  }
  if (Mock.fake_message_handler_CallbackFunctionPointer != NULL)
  {
    cmock_call_instance->ReturnVal = Mock.fake_message_handler_CallbackFunctionPointer(service_id, payload, payload_size, Mock.fake_message_handler_CallbackCalls++);
  }
  UNITY_CLR_DETAILS();
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_fake_message_handler(CMOCK_fake_message_handler_CALL_INSTANCE* cmock_call_instance, fwk_id_t service_id, const uint32_t* payload, int payload_Depth, size_t payload_size);
void CMockExpectParameters_fake_message_handler(CMOCK_fake_message_handler_CALL_INSTANCE* cmock_call_instance, fwk_id_t service_id, const uint32_t* payload, int payload_Depth, size_t payload_size)
{
  memcpy((void*)(&cmock_call_instance->Expected_service_id), (void*)(&service_id),
         sizeof(fwk_id_t[sizeof(service_id) == sizeof(fwk_id_t) ? 1 : -1])); /* add fwk_id_t to :treat_as_array if this causes an error */
  cmock_call_instance->Expected_payload = payload;
  cmock_call_instance->Expected_payload_Depth = payload_Depth;
  memcpy((void*)(&cmock_call_instance->Expected_payload_size), (void*)(&payload_size),
         sizeof(size_t[sizeof(payload_size) == sizeof(size_t) ? 1 : -1])); /* add size_t to :treat_as_array if this causes an error */
}

void fake_message_handler_CMockExpectAnyArgsAndReturn(UNITY_LINE_TYPE cmock_line, int cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_fake_message_handler_CALL_INSTANCE));
  CMOCK_fake_message_handler_CALL_INSTANCE* cmock_call_instance = (CMOCK_fake_message_handler_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.fake_message_handler_CallInstance = CMock_Guts_MemChain(Mock.fake_message_handler_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ExpectAnyArgsBool = (char)0;
  cmock_call_instance->ReturnVal = cmock_to_return;
  cmock_call_instance->ExpectAnyArgsBool = (char)1;
}

void fake_message_handler_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, fwk_id_t service_id, const uint32_t* payload, size_t payload_size, int cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_fake_message_handler_CALL_INSTANCE));
  CMOCK_fake_message_handler_CALL_INSTANCE* cmock_call_instance = (CMOCK_fake_message_handler_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.fake_message_handler_CallInstance = CMock_Guts_MemChain(Mock.fake_message_handler_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ExpectAnyArgsBool = (char)0;
  CMockExpectParameters_fake_message_handler(cmock_call_instance, service_id, payload, payload_size, payload_size);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

void fake_message_handler_AddCallback(CMOCK_fake_message_handler_CALLBACK Callback)
{
  Mock.fake_message_handler_CallbackBool = (char)1;
  Mock.fake_message_handler_CallbackFunctionPointer = Callback;
}

void fake_message_handler_Stub(CMOCK_fake_message_handler_CALLBACK Callback)
{
  Mock.fake_message_handler_CallbackBool = (char)0;
  Mock.fake_message_handler_CallbackFunctionPointer = Callback;
}

void fake_message_handler_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, fwk_id_t service_id, const uint32_t* payload, int payload_Depth, size_t payload_size, int cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_fake_message_handler_CALL_INSTANCE));
  CMOCK_fake_message_handler_CALL_INSTANCE* cmock_call_instance = (CMOCK_fake_message_handler_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.fake_message_handler_CallInstance = CMock_Guts_MemChain(Mock.fake_message_handler_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ExpectAnyArgsBool = (char)0;
  CMockExpectParameters_fake_message_handler(cmock_call_instance, service_id, payload, payload_Depth, payload_size);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

