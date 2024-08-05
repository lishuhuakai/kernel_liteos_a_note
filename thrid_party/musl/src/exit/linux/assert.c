#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <info/fatal_message.h>
#include <stddef.h>
#include <signal.h>
#include "musl_log.h"
#define ASSERT_FATAL_MESSAGE_SIZE 1024

static assert_call g_cb = NULL;
void set_assert_callback(assert_call cb)
{
    g_cb = cb;
}

void __assert_fail(const char *expr, const char *file, int line, const char *func)
{
    // Concatenate information for assert failures and save the final result in assert_fatal_message
    char assert_fatal_message[ASSERT_FATAL_MESSAGE_SIZE];

    snprintf(assert_fatal_message, sizeof(assert_fatal_message),
             "Assertion failed: %s (%s: %s: %d)", expr, file, func, line);

    // call set_fatal_message to set assert_fatal_message
    set_fatal_message(assert_fatal_message);
    AssertFailureInfo assert_fail = {
        expr, file, func, line
    };

    Assert_Status assert_status = ASSERT_ABORT;
    if (g_cb) {
        assert_status = g_cb(assert_fail);
    }
    if (assert_status == ASSERT_RETRY) {
        MUSL_LOGE("CallbackFunction return ASSERT_RETRY:\n");
        raise(SIGUSR1);
    } else if (assert_status == ASSERT_IGNORE) {
        MUSL_LOGE("CallbackFunction return ASSERT_IGNORE:\n");
        return;
    } else {
        MUSL_LOGE("CallbackFunction return ASSERT_ABORT:\n");
        fprintf(stderr, "%s\n", assert_fatal_message);
        abort();
    }
}
