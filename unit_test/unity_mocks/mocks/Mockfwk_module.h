/* AUTOGENERATED FILE. DO NOT EDIT. */
#ifndef _MOCKFWK_MODULE_H
#define _MOCKFWK_MODULE_H

#include "unity.h"
#include "fwk_module.h"

/* Ignore the following warnings, since we are copying code */
#if defined(__GNUC__) && !defined(__ICC) && !defined(__TMS470__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && (__GNUC_MINOR__ > 6 || (__GNUC_MINOR__ == 6 && __GNUC_PATCHLEVEL__ > 0)))
#pragma GCC diagnostic push
#endif
#if !defined(__clang__)
#pragma GCC diagnostic ignored "-Wpragmas"
#endif
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wduplicate-decl-specifier"
#endif

void Mockfwk_module_Init(void);
void Mockfwk_module_Destroy(void);
void Mockfwk_module_Verify(void);




#define fwk_module_is_valid_module_id_IgnoreAndReturn(cmock_retval) fwk_module_is_valid_module_id_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void fwk_module_is_valid_module_id_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, bool cmock_to_return);
#define fwk_module_is_valid_module_id_StopIgnore() fwk_module_is_valid_module_id_CMockStopIgnore()
void fwk_module_is_valid_module_id_CMockStopIgnore(void);
#define fwk_module_is_valid_module_id_ExpectAnyArgsAndReturn(cmock_retval) fwk_module_is_valid_module_id_CMockExpectAnyArgsAndReturn(__LINE__, cmock_retval)
void fwk_module_is_valid_module_id_CMockExpectAnyArgsAndReturn(UNITY_LINE_TYPE cmock_line, bool cmock_to_return);
#define fwk_module_is_valid_module_id_ExpectAndReturn(id, cmock_retval) fwk_module_is_valid_module_id_CMockExpectAndReturn(__LINE__, id, cmock_retval)
void fwk_module_is_valid_module_id_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, fwk_id_t id, bool cmock_to_return);
typedef bool (* CMOCK_fwk_module_is_valid_module_id_CALLBACK)(fwk_id_t id, int cmock_num_calls);
void fwk_module_is_valid_module_id_AddCallback(CMOCK_fwk_module_is_valid_module_id_CALLBACK Callback);
void fwk_module_is_valid_module_id_Stub(CMOCK_fwk_module_is_valid_module_id_CALLBACK Callback);
#define fwk_module_is_valid_module_id_StubWithCallback fwk_module_is_valid_module_id_Stub
#define fwk_module_is_valid_module_id_IgnoreArg_id() fwk_module_is_valid_module_id_CMockIgnoreArg_id(__LINE__)
void fwk_module_is_valid_module_id_CMockIgnoreArg_id(UNITY_LINE_TYPE cmock_line);
#define fwk_module_is_valid_element_id_IgnoreAndReturn(cmock_retval) fwk_module_is_valid_element_id_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void fwk_module_is_valid_element_id_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, bool cmock_to_return);
#define fwk_module_is_valid_element_id_StopIgnore() fwk_module_is_valid_element_id_CMockStopIgnore()
void fwk_module_is_valid_element_id_CMockStopIgnore(void);
#define fwk_module_is_valid_element_id_ExpectAnyArgsAndReturn(cmock_retval) fwk_module_is_valid_element_id_CMockExpectAnyArgsAndReturn(__LINE__, cmock_retval)
void fwk_module_is_valid_element_id_CMockExpectAnyArgsAndReturn(UNITY_LINE_TYPE cmock_line, bool cmock_to_return);
#define fwk_module_is_valid_element_id_ExpectAndReturn(id, cmock_retval) fwk_module_is_valid_element_id_CMockExpectAndReturn(__LINE__, id, cmock_retval)
void fwk_module_is_valid_element_id_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, fwk_id_t id, bool cmock_to_return);
typedef bool (* CMOCK_fwk_module_is_valid_element_id_CALLBACK)(fwk_id_t id, int cmock_num_calls);
void fwk_module_is_valid_element_id_AddCallback(CMOCK_fwk_module_is_valid_element_id_CALLBACK Callback);
void fwk_module_is_valid_element_id_Stub(CMOCK_fwk_module_is_valid_element_id_CALLBACK Callback);
#define fwk_module_is_valid_element_id_StubWithCallback fwk_module_is_valid_element_id_Stub
#define fwk_module_is_valid_element_id_IgnoreArg_id() fwk_module_is_valid_element_id_CMockIgnoreArg_id(__LINE__)
void fwk_module_is_valid_element_id_CMockIgnoreArg_id(UNITY_LINE_TYPE cmock_line);
#define fwk_module_is_valid_sub_element_id_IgnoreAndReturn(cmock_retval) fwk_module_is_valid_sub_element_id_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void fwk_module_is_valid_sub_element_id_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, bool cmock_to_return);
#define fwk_module_is_valid_sub_element_id_StopIgnore() fwk_module_is_valid_sub_element_id_CMockStopIgnore()
void fwk_module_is_valid_sub_element_id_CMockStopIgnore(void);
#define fwk_module_is_valid_sub_element_id_ExpectAnyArgsAndReturn(cmock_retval) fwk_module_is_valid_sub_element_id_CMockExpectAnyArgsAndReturn(__LINE__, cmock_retval)
void fwk_module_is_valid_sub_element_id_CMockExpectAnyArgsAndReturn(UNITY_LINE_TYPE cmock_line, bool cmock_to_return);
#define fwk_module_is_valid_sub_element_id_ExpectAndReturn(id, cmock_retval) fwk_module_is_valid_sub_element_id_CMockExpectAndReturn(__LINE__, id, cmock_retval)
void fwk_module_is_valid_sub_element_id_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, fwk_id_t id, bool cmock_to_return);
typedef bool (* CMOCK_fwk_module_is_valid_sub_element_id_CALLBACK)(fwk_id_t id, int cmock_num_calls);
void fwk_module_is_valid_sub_element_id_AddCallback(CMOCK_fwk_module_is_valid_sub_element_id_CALLBACK Callback);
void fwk_module_is_valid_sub_element_id_Stub(CMOCK_fwk_module_is_valid_sub_element_id_CALLBACK Callback);
#define fwk_module_is_valid_sub_element_id_StubWithCallback fwk_module_is_valid_sub_element_id_Stub
#define fwk_module_is_valid_sub_element_id_IgnoreArg_id() fwk_module_is_valid_sub_element_id_CMockIgnoreArg_id(__LINE__)
void fwk_module_is_valid_sub_element_id_CMockIgnoreArg_id(UNITY_LINE_TYPE cmock_line);
#define fwk_module_is_valid_entity_id_IgnoreAndReturn(cmock_retval) fwk_module_is_valid_entity_id_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void fwk_module_is_valid_entity_id_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, bool cmock_to_return);
#define fwk_module_is_valid_entity_id_StopIgnore() fwk_module_is_valid_entity_id_CMockStopIgnore()
void fwk_module_is_valid_entity_id_CMockStopIgnore(void);
#define fwk_module_is_valid_entity_id_ExpectAnyArgsAndReturn(cmock_retval) fwk_module_is_valid_entity_id_CMockExpectAnyArgsAndReturn(__LINE__, cmock_retval)
void fwk_module_is_valid_entity_id_CMockExpectAnyArgsAndReturn(UNITY_LINE_TYPE cmock_line, bool cmock_to_return);
#define fwk_module_is_valid_entity_id_ExpectAndReturn(id, cmock_retval) fwk_module_is_valid_entity_id_CMockExpectAndReturn(__LINE__, id, cmock_retval)
void fwk_module_is_valid_entity_id_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, fwk_id_t id, bool cmock_to_return);
typedef bool (* CMOCK_fwk_module_is_valid_entity_id_CALLBACK)(fwk_id_t id, int cmock_num_calls);
void fwk_module_is_valid_entity_id_AddCallback(CMOCK_fwk_module_is_valid_entity_id_CALLBACK Callback);
void fwk_module_is_valid_entity_id_Stub(CMOCK_fwk_module_is_valid_entity_id_CALLBACK Callback);
#define fwk_module_is_valid_entity_id_StubWithCallback fwk_module_is_valid_entity_id_Stub
#define fwk_module_is_valid_entity_id_IgnoreArg_id() fwk_module_is_valid_entity_id_CMockIgnoreArg_id(__LINE__)
void fwk_module_is_valid_entity_id_CMockIgnoreArg_id(UNITY_LINE_TYPE cmock_line);
#define fwk_module_is_valid_api_id_IgnoreAndReturn(cmock_retval) fwk_module_is_valid_api_id_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void fwk_module_is_valid_api_id_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, bool cmock_to_return);
#define fwk_module_is_valid_api_id_StopIgnore() fwk_module_is_valid_api_id_CMockStopIgnore()
void fwk_module_is_valid_api_id_CMockStopIgnore(void);
#define fwk_module_is_valid_api_id_ExpectAnyArgsAndReturn(cmock_retval) fwk_module_is_valid_api_id_CMockExpectAnyArgsAndReturn(__LINE__, cmock_retval)
void fwk_module_is_valid_api_id_CMockExpectAnyArgsAndReturn(UNITY_LINE_TYPE cmock_line, bool cmock_to_return);
#define fwk_module_is_valid_api_id_ExpectAndReturn(id, cmock_retval) fwk_module_is_valid_api_id_CMockExpectAndReturn(__LINE__, id, cmock_retval)
void fwk_module_is_valid_api_id_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, fwk_id_t id, bool cmock_to_return);
typedef bool (* CMOCK_fwk_module_is_valid_api_id_CALLBACK)(fwk_id_t id, int cmock_num_calls);
void fwk_module_is_valid_api_id_AddCallback(CMOCK_fwk_module_is_valid_api_id_CALLBACK Callback);
void fwk_module_is_valid_api_id_Stub(CMOCK_fwk_module_is_valid_api_id_CALLBACK Callback);
#define fwk_module_is_valid_api_id_StubWithCallback fwk_module_is_valid_api_id_Stub
#define fwk_module_is_valid_api_id_IgnoreArg_id() fwk_module_is_valid_api_id_CMockIgnoreArg_id(__LINE__)
void fwk_module_is_valid_api_id_CMockIgnoreArg_id(UNITY_LINE_TYPE cmock_line);
#define fwk_module_is_valid_event_id_IgnoreAndReturn(cmock_retval) fwk_module_is_valid_event_id_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void fwk_module_is_valid_event_id_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, bool cmock_to_return);
#define fwk_module_is_valid_event_id_StopIgnore() fwk_module_is_valid_event_id_CMockStopIgnore()
void fwk_module_is_valid_event_id_CMockStopIgnore(void);
#define fwk_module_is_valid_event_id_ExpectAnyArgsAndReturn(cmock_retval) fwk_module_is_valid_event_id_CMockExpectAnyArgsAndReturn(__LINE__, cmock_retval)
void fwk_module_is_valid_event_id_CMockExpectAnyArgsAndReturn(UNITY_LINE_TYPE cmock_line, bool cmock_to_return);
#define fwk_module_is_valid_event_id_ExpectAndReturn(id, cmock_retval) fwk_module_is_valid_event_id_CMockExpectAndReturn(__LINE__, id, cmock_retval)
void fwk_module_is_valid_event_id_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, fwk_id_t id, bool cmock_to_return);
typedef bool (* CMOCK_fwk_module_is_valid_event_id_CALLBACK)(fwk_id_t id, int cmock_num_calls);
void fwk_module_is_valid_event_id_AddCallback(CMOCK_fwk_module_is_valid_event_id_CALLBACK Callback);
void fwk_module_is_valid_event_id_Stub(CMOCK_fwk_module_is_valid_event_id_CALLBACK Callback);
#define fwk_module_is_valid_event_id_StubWithCallback fwk_module_is_valid_event_id_Stub
#define fwk_module_is_valid_event_id_IgnoreArg_id() fwk_module_is_valid_event_id_CMockIgnoreArg_id(__LINE__)
void fwk_module_is_valid_event_id_CMockIgnoreArg_id(UNITY_LINE_TYPE cmock_line);
#define fwk_module_is_valid_notification_id_IgnoreAndReturn(cmock_retval) fwk_module_is_valid_notification_id_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void fwk_module_is_valid_notification_id_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, bool cmock_to_return);
#define fwk_module_is_valid_notification_id_StopIgnore() fwk_module_is_valid_notification_id_CMockStopIgnore()
void fwk_module_is_valid_notification_id_CMockStopIgnore(void);
#define fwk_module_is_valid_notification_id_ExpectAnyArgsAndReturn(cmock_retval) fwk_module_is_valid_notification_id_CMockExpectAnyArgsAndReturn(__LINE__, cmock_retval)
void fwk_module_is_valid_notification_id_CMockExpectAnyArgsAndReturn(UNITY_LINE_TYPE cmock_line, bool cmock_to_return);
#define fwk_module_is_valid_notification_id_ExpectAndReturn(id, cmock_retval) fwk_module_is_valid_notification_id_CMockExpectAndReturn(__LINE__, id, cmock_retval)
void fwk_module_is_valid_notification_id_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, fwk_id_t id, bool cmock_to_return);
typedef bool (* CMOCK_fwk_module_is_valid_notification_id_CALLBACK)(fwk_id_t id, int cmock_num_calls);
void fwk_module_is_valid_notification_id_AddCallback(CMOCK_fwk_module_is_valid_notification_id_CALLBACK Callback);
void fwk_module_is_valid_notification_id_Stub(CMOCK_fwk_module_is_valid_notification_id_CALLBACK Callback);
#define fwk_module_is_valid_notification_id_StubWithCallback fwk_module_is_valid_notification_id_Stub
#define fwk_module_is_valid_notification_id_IgnoreArg_id() fwk_module_is_valid_notification_id_CMockIgnoreArg_id(__LINE__)
void fwk_module_is_valid_notification_id_CMockIgnoreArg_id(UNITY_LINE_TYPE cmock_line);
#define fwk_module_get_element_count_IgnoreAndReturn(cmock_retval) fwk_module_get_element_count_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void fwk_module_get_element_count_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, int cmock_to_return);
#define fwk_module_get_element_count_StopIgnore() fwk_module_get_element_count_CMockStopIgnore()
void fwk_module_get_element_count_CMockStopIgnore(void);
#define fwk_module_get_element_count_ExpectAnyArgsAndReturn(cmock_retval) fwk_module_get_element_count_CMockExpectAnyArgsAndReturn(__LINE__, cmock_retval)
void fwk_module_get_element_count_CMockExpectAnyArgsAndReturn(UNITY_LINE_TYPE cmock_line, int cmock_to_return);
#define fwk_module_get_element_count_ExpectAndReturn(module_id, cmock_retval) fwk_module_get_element_count_CMockExpectAndReturn(__LINE__, module_id, cmock_retval)
void fwk_module_get_element_count_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, fwk_id_t module_id, int cmock_to_return);
typedef int (* CMOCK_fwk_module_get_element_count_CALLBACK)(fwk_id_t module_id, int cmock_num_calls);
void fwk_module_get_element_count_AddCallback(CMOCK_fwk_module_get_element_count_CALLBACK Callback);
void fwk_module_get_element_count_Stub(CMOCK_fwk_module_get_element_count_CALLBACK Callback);
#define fwk_module_get_element_count_StubWithCallback fwk_module_get_element_count_Stub
#define fwk_module_get_element_count_IgnoreArg_module_id() fwk_module_get_element_count_CMockIgnoreArg_module_id(__LINE__)
void fwk_module_get_element_count_CMockIgnoreArg_module_id(UNITY_LINE_TYPE cmock_line);
#define fwk_module_get_sub_element_count_IgnoreAndReturn(cmock_retval) fwk_module_get_sub_element_count_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void fwk_module_get_sub_element_count_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, int cmock_to_return);
#define fwk_module_get_sub_element_count_StopIgnore() fwk_module_get_sub_element_count_CMockStopIgnore()
void fwk_module_get_sub_element_count_CMockStopIgnore(void);
#define fwk_module_get_sub_element_count_ExpectAnyArgsAndReturn(cmock_retval) fwk_module_get_sub_element_count_CMockExpectAnyArgsAndReturn(__LINE__, cmock_retval)
void fwk_module_get_sub_element_count_CMockExpectAnyArgsAndReturn(UNITY_LINE_TYPE cmock_line, int cmock_to_return);
#define fwk_module_get_sub_element_count_ExpectAndReturn(element_id, cmock_retval) fwk_module_get_sub_element_count_CMockExpectAndReturn(__LINE__, element_id, cmock_retval)
void fwk_module_get_sub_element_count_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, fwk_id_t element_id, int cmock_to_return);
typedef int (* CMOCK_fwk_module_get_sub_element_count_CALLBACK)(fwk_id_t element_id, int cmock_num_calls);
void fwk_module_get_sub_element_count_AddCallback(CMOCK_fwk_module_get_sub_element_count_CALLBACK Callback);
void fwk_module_get_sub_element_count_Stub(CMOCK_fwk_module_get_sub_element_count_CALLBACK Callback);
#define fwk_module_get_sub_element_count_StubWithCallback fwk_module_get_sub_element_count_Stub
#define fwk_module_get_sub_element_count_IgnoreArg_element_id() fwk_module_get_sub_element_count_CMockIgnoreArg_element_id(__LINE__)
void fwk_module_get_sub_element_count_CMockIgnoreArg_element_id(UNITY_LINE_TYPE cmock_line);
#define fwk_module_get_element_name_IgnoreAndReturn(cmock_retval) fwk_module_get_element_name_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void fwk_module_get_element_name_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, const char* cmock_to_return);
#define fwk_module_get_element_name_StopIgnore() fwk_module_get_element_name_CMockStopIgnore()
void fwk_module_get_element_name_CMockStopIgnore(void);
#define fwk_module_get_element_name_ExpectAnyArgsAndReturn(cmock_retval) fwk_module_get_element_name_CMockExpectAnyArgsAndReturn(__LINE__, cmock_retval)
void fwk_module_get_element_name_CMockExpectAnyArgsAndReturn(UNITY_LINE_TYPE cmock_line, const char* cmock_to_return);
#define fwk_module_get_element_name_ExpectAndReturn(id, cmock_retval) fwk_module_get_element_name_CMockExpectAndReturn(__LINE__, id, cmock_retval)
void fwk_module_get_element_name_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, fwk_id_t id, const char* cmock_to_return);
typedef const char* (* CMOCK_fwk_module_get_element_name_CALLBACK)(fwk_id_t id, int cmock_num_calls);
void fwk_module_get_element_name_AddCallback(CMOCK_fwk_module_get_element_name_CALLBACK Callback);
void fwk_module_get_element_name_Stub(CMOCK_fwk_module_get_element_name_CALLBACK Callback);
#define fwk_module_get_element_name_StubWithCallback fwk_module_get_element_name_Stub
#define fwk_module_get_element_name_IgnoreArg_id() fwk_module_get_element_name_CMockIgnoreArg_id(__LINE__)
void fwk_module_get_element_name_CMockIgnoreArg_id(UNITY_LINE_TYPE cmock_line);
#define fwk_module_get_data_IgnoreAndReturn(cmock_retval) fwk_module_get_data_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void fwk_module_get_data_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, const void* cmock_to_return);
#define fwk_module_get_data_StopIgnore() fwk_module_get_data_CMockStopIgnore()
void fwk_module_get_data_CMockStopIgnore(void);
#define fwk_module_get_data_ExpectAnyArgsAndReturn(cmock_retval) fwk_module_get_data_CMockExpectAnyArgsAndReturn(__LINE__, cmock_retval)
void fwk_module_get_data_CMockExpectAnyArgsAndReturn(UNITY_LINE_TYPE cmock_line, const void* cmock_to_return);
#define fwk_module_get_data_ExpectAndReturn(id, cmock_retval) fwk_module_get_data_CMockExpectAndReturn(__LINE__, id, cmock_retval)
void fwk_module_get_data_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, fwk_id_t id, const void* cmock_to_return);
typedef const void* (* CMOCK_fwk_module_get_data_CALLBACK)(fwk_id_t id, int cmock_num_calls);
void fwk_module_get_data_AddCallback(CMOCK_fwk_module_get_data_CALLBACK Callback);
void fwk_module_get_data_Stub(CMOCK_fwk_module_get_data_CALLBACK Callback);
#define fwk_module_get_data_StubWithCallback fwk_module_get_data_Stub
#define fwk_module_get_data_IgnoreArg_id() fwk_module_get_data_CMockIgnoreArg_id(__LINE__)
void fwk_module_get_data_CMockIgnoreArg_id(UNITY_LINE_TYPE cmock_line);
#define fwk_module_bind_IgnoreAndReturn(cmock_retval) fwk_module_bind_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void fwk_module_bind_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, int cmock_to_return);
#define fwk_module_bind_StopIgnore() fwk_module_bind_CMockStopIgnore()
void fwk_module_bind_CMockStopIgnore(void);
#define fwk_module_bind_ExpectAnyArgsAndReturn(cmock_retval) fwk_module_bind_CMockExpectAnyArgsAndReturn(__LINE__, cmock_retval)
void fwk_module_bind_CMockExpectAnyArgsAndReturn(UNITY_LINE_TYPE cmock_line, int cmock_to_return);
#define fwk_module_bind_ExpectAndReturn(target_id, api_id, api, cmock_retval) fwk_module_bind_CMockExpectAndReturn(__LINE__, target_id, api_id, api, cmock_retval)
void fwk_module_bind_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, fwk_id_t target_id, fwk_id_t api_id, const void* api, int cmock_to_return);
typedef int (* CMOCK_fwk_module_bind_CALLBACK)(fwk_id_t target_id, fwk_id_t api_id, const void* api, int cmock_num_calls);
void fwk_module_bind_AddCallback(CMOCK_fwk_module_bind_CALLBACK Callback);
void fwk_module_bind_Stub(CMOCK_fwk_module_bind_CALLBACK Callback);
#define fwk_module_bind_StubWithCallback fwk_module_bind_Stub
#define fwk_module_bind_ExpectWithArrayAndReturn(target_id, api_id, api, api_Depth, cmock_retval) fwk_module_bind_CMockExpectWithArrayAndReturn(__LINE__, target_id, api_id, api, api_Depth, cmock_retval)
void fwk_module_bind_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, fwk_id_t target_id, fwk_id_t api_id, const void* api, int api_Depth, int cmock_to_return);
#define fwk_module_bind_IgnoreArg_target_id() fwk_module_bind_CMockIgnoreArg_target_id(__LINE__)
void fwk_module_bind_CMockIgnoreArg_target_id(UNITY_LINE_TYPE cmock_line);
#define fwk_module_bind_IgnoreArg_api_id() fwk_module_bind_CMockIgnoreArg_api_id(__LINE__)
void fwk_module_bind_CMockIgnoreArg_api_id(UNITY_LINE_TYPE cmock_line);
#define fwk_module_bind_IgnoreArg_api() fwk_module_bind_CMockIgnoreArg_api(__LINE__)
void fwk_module_bind_CMockIgnoreArg_api(UNITY_LINE_TYPE cmock_line);
#define fwk_module_adapter_IgnoreAndReturn(cmock_retval) fwk_module_adapter_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void fwk_module_adapter_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, int cmock_to_return);
#define fwk_module_adapter_StopIgnore() fwk_module_adapter_CMockStopIgnore()
void fwk_module_adapter_CMockStopIgnore(void);
#define fwk_module_adapter_ExpectAnyArgsAndReturn(cmock_retval) fwk_module_adapter_CMockExpectAnyArgsAndReturn(__LINE__, cmock_retval)
void fwk_module_adapter_CMockExpectAnyArgsAndReturn(UNITY_LINE_TYPE cmock_line, int cmock_to_return);
#define fwk_module_adapter_ExpectAndReturn(adapter, id, cmock_retval) fwk_module_adapter_CMockExpectAndReturn(__LINE__, adapter, id, cmock_retval)
void fwk_module_adapter_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, const struct fwk_io_adapter** adapter, fwk_id_t id, int cmock_to_return);
typedef int (* CMOCK_fwk_module_adapter_CALLBACK)(const struct fwk_io_adapter** adapter, fwk_id_t id, int cmock_num_calls);
void fwk_module_adapter_AddCallback(CMOCK_fwk_module_adapter_CALLBACK Callback);
void fwk_module_adapter_Stub(CMOCK_fwk_module_adapter_CALLBACK Callback);
#define fwk_module_adapter_StubWithCallback fwk_module_adapter_Stub
#define fwk_module_adapter_ExpectWithArrayAndReturn(adapter, adapter_Depth, id, cmock_retval) fwk_module_adapter_CMockExpectWithArrayAndReturn(__LINE__, adapter, adapter_Depth, id, cmock_retval)
void fwk_module_adapter_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, const struct fwk_io_adapter** adapter, int adapter_Depth, fwk_id_t id, int cmock_to_return);
#define fwk_module_adapter_ReturnThruPtr_adapter(adapter) fwk_module_adapter_CMockReturnMemThruPtr_adapter(__LINE__, adapter, sizeof(const struct fwk_io_adapter*))
#define fwk_module_adapter_ReturnArrayThruPtr_adapter(adapter, cmock_len) fwk_module_adapter_CMockReturnMemThruPtr_adapter(__LINE__, adapter, cmock_len * sizeof(*adapter))
#define fwk_module_adapter_ReturnMemThruPtr_adapter(adapter, cmock_size) fwk_module_adapter_CMockReturnMemThruPtr_adapter(__LINE__, adapter, cmock_size)
void fwk_module_adapter_CMockReturnMemThruPtr_adapter(UNITY_LINE_TYPE cmock_line, const struct fwk_io_adapter** adapter, size_t cmock_size);
#define fwk_module_adapter_IgnoreArg_adapter() fwk_module_adapter_CMockIgnoreArg_adapter(__LINE__)
void fwk_module_adapter_CMockIgnoreArg_adapter(UNITY_LINE_TYPE cmock_line);
#define fwk_module_adapter_IgnoreArg_id() fwk_module_adapter_CMockIgnoreArg_id(__LINE__)
void fwk_module_adapter_CMockIgnoreArg_id(UNITY_LINE_TYPE cmock_line);
#define fwk_module_init_Ignore() fwk_module_init_CMockIgnore()
void fwk_module_init_CMockIgnore(void);
#define fwk_module_init_StopIgnore() fwk_module_init_CMockStopIgnore()
void fwk_module_init_CMockStopIgnore(void);
#define fwk_module_init_Expect() fwk_module_init_CMockExpect(__LINE__)
void fwk_module_init_CMockExpect(UNITY_LINE_TYPE cmock_line);
typedef void (* CMOCK_fwk_module_init_CALLBACK)(int cmock_num_calls);
void fwk_module_init_AddCallback(CMOCK_fwk_module_init_CALLBACK Callback);
void fwk_module_init_Stub(CMOCK_fwk_module_init_CALLBACK Callback);
#define fwk_module_init_StubWithCallback fwk_module_init_Stub

#if defined(__GNUC__) && !defined(__ICC) && !defined(__TMS470__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && (__GNUC_MINOR__ > 6 || (__GNUC_MINOR__ == 6 && __GNUC_PATCHLEVEL__ > 0)))
#pragma GCC diagnostic pop
#endif
#endif

#endif
