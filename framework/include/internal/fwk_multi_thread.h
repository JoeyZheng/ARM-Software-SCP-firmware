/*
 * Arm SCP/MCP Software
 * Copyright (c) 2015-2020, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef FWK_INTERNAL_MULTI_THREAD_H
#define FWK_INTERNAL_MULTI_THREAD_H

#include <cmsis_os2.h>

#include <internal/fwk_thread.h>

#include <fwk_event.h>
#include <fwk_id.h>
#include <fwk_list.h>
#include <fwk_multi_thread.h>

#include <stdbool.h>

/*
 * Module/element thread context.
 */
struct __fwk_thread_ctx {
    /*
     * Link for the queue of ready threads, threads having at least one event
     * in their event queue and ready to process the first one in their queue.
     * Those threads are not waiting for the completion of the processing of
     * an event by another thread.
     */
    struct fwk_slist_node slist_node;

    /* Identifier of the module or element that created the thread */
    fwk_id_t id;

    /*
     * Identifier given by the underlying OS to the thread created by the
     * module or element.
     */
    osThreadId_t os_thread_id;

    /* Thread queue of events */
    struct fwk_slist event_queue;

    /*
     * Flag indicating if the thread is waiting for the completion of the
     * processing of an event by another thread (true) or not (false). A thread
     * enters this waiting state when calling the fwk_thread_put_and_wait()
     * framework API requesting another thread to process an event. A thread
     * leaves this waiting state when the processing of the aforementioned
     * event is completed. The execution of the thread is then resumed
     * immediately: no other event processing occurs between the end of the
     * event processing and the waiting thread execution being resumed.
     */
    bool waiting_event_processing_completion;

    /*
     * Pointer to storage for the response event when waiting for the
     * completion of the processing of an event by another thread. This field
     * has a valid value if and only if the flag
     * 'waiting_event_processing_completion' is equal to 'true'. The storage
     * is allocated by the module or element having the identifier 'id'.
     * The pointer to the storage is saved in the present context to be
     * passed in to the 'process_event' framework API of the module/element
     * processing the event which an event response is expected from.
     */
    struct fwk_event *response_event;
};

/*
 * Multi-thread component context. Exposed for testing only.
 */
struct __fwk_multi_thread_ctx {
    /* Initialization completed flag */
    bool initialized;

    /* Execution running flag */
    bool running;

    /* Waiting for an ISR event flag */
    bool waiting_for_isr_event;

    /* Common default thread context */
    struct __fwk_thread_ctx common_thread_ctx;

    /* Context of the thread being currently executed */
    struct __fwk_thread_ctx *current_thread_ctx;

    /* Event being currently processed */
    struct fwk_event *current_event;

    /*
     * Counter used to generate event cookies.
     */
    uint32_t event_cookie_counter;

    /*
     * Queue of free events, event structures free to be filled in and linked
     * to the queue of ISR events or to one of the thread event queues.
     */
    struct fwk_slist event_free_queue;

    /*
     * Queue of events generated by ISRs and not dispatched yet to the
     * threads.
     */
    struct fwk_slist event_isr_queue;

    /*
     * Queue of the threads with pending events in their queue and not waiting
     * for the completion of the processing of an event following the call to
     * the fwk_thread_put_event_and_wait() interface function. Those threads are
     * ready to execute as soon as the CPU becomes available for them.
     */
    struct fwk_slist thread_ready_queue;
};

/*
 * \brief Get multi thread component context.
 *
 * \note Only for testing.
 */
struct __fwk_multi_thread_ctx *__fwk_multi_thread_get_ctx(void);

#endif /* FWK_INTERNAL_MULTI_THREAD_H */
