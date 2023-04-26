//===-- asan_interceptors.cpp ---------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file is a part of AddressSanitizer, an address sanity checker.
//
// Intercept various libc functions.
//===----------------------------------------------------------------------===//

#include "asan_allocator.h"
#include "asan_interceptors.h"
#include "asan_internal.h"
#include "asan_mapping.h"
#include "asan_poisoning.h"
#include "asan_report.h"
#include "asan_stack.h"
#include "asan_stats.h"
#include "asan_suppressions.h"
#include "lsan/lsan_common.h"
#include "sanitizer_common/sanitizer_libc.h"
// #include "asan_interceptors_postgres.h"

// There is no general interception at all on Fuchsia.
// Only the functions in asan_interceptors_memintrinsics.cpp are
// really defined to replace libc functions.
#if !SANITIZER_FUCHSIA

using namespace __asan;

static inline bool MaybeInDlsym() {
  // Fuchsia doesn't use dlsym-based interceptors.
  return !SANITIZER_FUCHSIA && asan_init_is_running;
}

static inline bool UseLocalPool() { return MaybeInDlsym(); }

#  define HAL_REAL_FUNCTION(func) \
    __attribute__((weak, visibility("default"))) int func

#  define REAL_HAL(func, ...)             \
    HAL_REAL_FUNCTION(func)(__VA_ARGS__); \
    // int func(__VA_ARGS__){ \
  //   Report("can not find '%s'\n",#func); \
  //   return -1; \
  // }

#  define HAL_INTERCEPTOR(ret_type, func, ...)   \
    DECLARE_WRAPPER(ret_type, func, __VA_ARGS__) \
    extern "C" INTERCEPTOR_ATTRIBUTE ret_type WRAP(func)(__VA_ARGS__)

INTERCEPTOR(void , pfree, void *pointer, uptr size) {
  ENSURE_ASAN_INITED();
  GET_STACK_TRACE_FREE;
  if (!flags()->replace_postgres) {
    return REAL(pfree)(pointer, size);
  }
  // Report("intercept pfree\n");
  asan_pfree(pointer, size, &stack);
}

INTERCEPTOR(void *, palloc, void *pointer, uptr size) {
  ENSURE_ASAN_INITED();
  GET_STACK_TRACE_MALLOC;
  if (!flags()->replace_postgres) {
    return REAL(palloc)(pointer, size);
  }
  // Report("intercept palloc\n");
  return asan_palloc(pointer, size, &stack);
}

INTERCEPTOR(void *, repalloc, void *pointer, uptr size) {
  ENSURE_ASAN_INITED();
  GET_STACK_TRACE_MALLOC;
  if (!flags()->replace_postgres) {
    return REAL(palloc)(pointer, size);
  }
  // Report("intercept palloc\n");
  return asan_palloc(pointer, size, &stack);
}

INTERCEPTOR(void *, AllocSetAlloc, MemoryContext context, uptr size) {
  ENSURE_ASAN_INITED();
  GET_STACK_TRACE_MALLOC;
  if (!flags()->replace_postgres) {
    return REAL(AllocSetAlloc)(context, size);
  }
  // Report("intercept AllocSetAlloc\n");
  void *addr = REAL(AllocSetAlloc)(context, size);
  return addr;
  // return asan_AllocSetAlloc(context, size, &stack);
}

INTERCEPTOR(void , AllocSetFree, void *pointer) {
  ENSURE_ASAN_INITED();
  GET_STACK_TRACE_FREE;
  if (!flags()->replace_postgres) {
    REAL(AllocSetFree)(pointer);
    return ;
  }
  // Report("intercept AllocSetFree\n");
  REAL(AllocSetFree)(pointer);
  // asan_AllocSetFree(pointer, &stack);
}

// ---------------------- InitializeAsanInterceptors ---------------- {{{1
namespace __asan {
void InitializePostgresInterceptors() {
  static bool postgres_was_called_once;
  CHECK(!postgres_was_called_once);
  postgres_was_called_once = true;

  // just need real function
//   ASAN_INTERCEPT_FUNC(halMbufGetBuffAddr);
//   ASAN_INTERCEPT_FUNC(halMbufGetBuffSize);

  // need both real and interceptor function
  ASAN_INTERCEPT_FUNC(pfree);
  ASAN_INTERCEPT_FUNC(palloc);

  ASAN_INTERCEPT_FUNC(AllocSetAlloc);
  ASAN_INTERCEPT_FUNC(AllocSetFree);

  VReport(1, "Hal AddressSanitizer: hal interceptors initialized\n");
}

}  // namespace __asan

#endif  // !SANITIZER_FUCHSIA
