#include "memory_trace.h"
#ifdef HOOK_ENABLE
#include "common_def.h"
#include "musl_preinit_common.h"
#endif

void memtrace(void* addr, size_t size, const char* tag, bool is_using)
{
#ifdef HOOK_ENABLE
	volatile const struct MallocDispatchType* dispatch_table = (struct MallocDispatchType *)atomic_load_explicit(
		&__musl_libc_globals.current_dispatch_table, memory_order_acquire);
	if (__predict_false(dispatch_table != NULL)) {
		if (__get_memleak_hook_flag()) {
			return;
		}
		if (!__get_global_hook_flag()) {
			return;
		}
		else if (!__get_hook_flag()) {
			return;
		}
		return dispatch_table->memtrace(addr, size, tag, is_using);
	}
#endif
    return;
}
