#ifndef SANITIZER_ASAN_INTERFACE_H
#define SANITIZER_ASAN_INTERFACE_H

#include <sanitizer/asan_interface.h>

#ifdef __cplusplus
extern "C" {
#endif

// Macros provided for convenience.
#if __has_feature(address_sanitizer) || defined(__SANITIZE_ADDRESS__)
/// postgresql memory pool interface
#define ASAN_MEMPOOL_ALLOC(addr, size) \
  __interceptor_palloc((size))
#else
#define ASAN_MEMPOOL_ALLOC(addr, size) \
  ((void)(addr), (void)(size))
#endif

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // SANITIZER_ASAN_INTERFACE_H
