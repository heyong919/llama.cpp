#ifndef CNPX_DEFS_H_
#define CNPX_DEFS_H_
#include <dlfcn.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef CNPX_ENABLED
#error Never include this file directly -- it is automatically included by cnpx.h
#endif
#include <stdalign.h>
#if __cplusplus
#define CNPX_ALIGN(a) alignof(a)
#else
#define CNPX_ALIGN(a) _Alignof(a)
#endif

#define CNPX_UNINIT 0
#define CNPX_UNDERINIT 1
#define CNPX_INITED 2
#define CNPX_WEAK __attribute__((weak))
#define CNPX_HIDDEN __attribute__((visibility("hidden")))
#define CNPX_SYNC() __sync_synchronize()
#define CNPX_YIELD() sched_yield()
#define CNPX_ATOMIC_SET(addr, value) \
  __sync_synchronize();              \
  __sync_lock_test_and_set(addr, value)
#define CNPX_ATOMIC_CAS(cur, addr, value, comp) \
  __sync_synchronize();                         \
  cur = __sync_val_compare_and_swap(addr, value, comp)

CNPX_HIDDEN CNPX_WEAK cnpxApi_t g_cnpx_apis = {
    CNPX_UNINIT, CNPX_VERSION,{}};

typedef void (*cnpxInitInternal_t)(cnpxApi_t *);

/**
 * \brief:  used to initialize the CNPX apis
 *          and enable internal profiling feature
 * \param:  apis  -- CNPX API attributes and function pointers
 * \return:  n/a
 * */
CNPX_HIDDEN CNPX_WEAK void cnpxInitApis(cnpxApi_t *apis) {
  cnpxInitInternal_t init_ptr;
  init_ptr = (cnpxInitInternal_t)dlsym(NULL, "cnpxInitInternal");
  if (init_ptr) {
    init_ptr(apis);
  } else {
    const char *lib_path = getenv("CNPX_LIB_PATH");
    if (!lib_path) return;
    void *lib_ptr = dlopen(lib_path, RTLD_LAZY);
    if (!lib_ptr) return;
    init_ptr = (cnpxInitInternal_t)dlsym(lib_ptr, "cnpxInitInternal");
    if (init_ptr) init_ptr(apis);
  }
  {
    cnpxInitInternalV1_t local_ptr_v1 = apis->f_cnpxInitInternalV1;
    if (local_ptr_v1) {
      uint64_t sizeof_types[CNPX_PAYLOAD_ENTRY_TYPE_META_ARRAY_SIZE] = {0,
                                                                        sizeof(signed char),
                                                                        sizeof(unsigned char),
                                                                        sizeof(short),
                                                                        sizeof(unsigned short),
                                                                        sizeof(int),
                                                                        sizeof(unsigned int),
                                                                        sizeof(long),
                                                                        sizeof(unsigned long),
                                                                        sizeof(long long),
                                                                        sizeof(unsigned long long),
                                                                        sizeof(void *),
                                                                        sizeof(char),
                                                                        sizeof(size_t)};
      uint64_t alignof_types[CNPX_PAYLOAD_ENTRY_TYPE_META_ARRAY_SIZE] = {0,
                                                                         CNPX_ALIGN(signed char),
                                                                         CNPX_ALIGN(unsigned char),
                                                                         CNPX_ALIGN(short),
                                                                         CNPX_ALIGN(unsigned short),
                                                                         CNPX_ALIGN(int),
                                                                         CNPX_ALIGN(unsigned int),
                                                                         CNPX_ALIGN(long),
                                                                         CNPX_ALIGN(unsigned long),
                                                                         CNPX_ALIGN(long long),
                                                                         CNPX_ALIGN(unsigned long long),
                                                                         CNPX_ALIGN(void *),
                                                                         CNPX_ALIGN(char),
                                                                         CNPX_ALIGN(size_t)};
      local_ptr_v1(sizeof_types, alignof_types, CNPX_PAYLOAD_ENTRY_TYPE_META_ARRAY_SIZE);
    }
  }
}

#define CNPX_CHECK_INIT(apis)                                      \
  while (apis.init != CNPX_INITED) {                               \
    unsigned int cur;                                              \
    CNPX_ATOMIC_CAS(cur, &apis.init, CNPX_UNDERINIT, CNPX_UNINIT); \
    if (cur == CNPX_UNINIT) {                                      \
      cnpxInitApis(&apis);                                         \
      CNPX_ATOMIC_SET(&apis.init, CNPX_INITED);                    \
    } else if (cur == CNPX_UNDERINIT) {                            \
      CNPX_YIELD();                                                \
      CNPX_SYNC();                                                 \
    }                                                              \
  }

CNPX_DECLDPEC void cnpxInit(const void *reserved) {
  CNPX_CHECK_INIT(g_cnpx_apis);
  cnpxInit_t local_ptr = g_cnpx_apis.f_cnpxInit;
  if (local_ptr) {
    local_ptr(reserved);
  }
}

CNPX_DECLDPEC void cnpxMark(const char *message) {
  CNPX_CHECK_INIT(g_cnpx_apis);
  cnpxMark_t local_ptr = g_cnpx_apis.f_cnpxMark;
  if (local_ptr) {
    local_ptr(message);
  }
}

CNPX_DECLDPEC int cnpxRangePush(const char *message) {
  CNPX_CHECK_INIT(g_cnpx_apis);
  cnpxRangePush_t local_ptr = g_cnpx_apis.f_cnpxRangePush;
  if (local_ptr) {
    return local_ptr(message);
  }
  return -1;
}

CNPX_DECLDPEC int cnpxRangePop() {
  CNPX_CHECK_INIT(g_cnpx_apis);
  cnpxRangePop_t local_ptr = g_cnpx_apis.f_cnpxRangePop;
  if (local_ptr) {
    return local_ptr();
  }
  return -1;
}

CNPX_DECLDPEC void cnpxNameOsThread(const char *name) {
  CNPX_CHECK_INIT(g_cnpx_apis);
  cnpxNameOsThread_t local_ptr = g_cnpx_apis.f_cnpxNameOsThread;
  if (local_ptr) {
    local_ptr(name);
  }
}

CNPX_DECLDPEC void cnpxNameQueue(const char *name, CNqueue queue) {
  CNPX_CHECK_INIT(g_cnpx_apis);
  cnpxNameQueue_t local_ptr = g_cnpx_apis.f_cnpxNameQueue;
  if (local_ptr) {
    local_ptr(name, queue);
  }
}

CNPX_DECLDPEC int cnpxRangeStart(const char *message, void **handle) {
  CNPX_CHECK_INIT(g_cnpx_apis);
  cnpxRangeStart_t local_ptr = g_cnpx_apis.f_cnpxRangeStart;
  if (local_ptr) {
    return local_ptr(message, handle);
  }
  return -1;
}

CNPX_DECLDPEC void cnpxRangeEnd(void *handle) {
  CNPX_CHECK_INIT(g_cnpx_apis);
  cnpxRangeEnd_t local_ptr = g_cnpx_apis.f_cnpxRangeEnd;
  if (local_ptr) {
    local_ptr(handle);
  }
}
CNPX_DECLDPEC cnpxDomainHandle_t cnpxDomainCreate(const char *name) {
  CNPX_CHECK_INIT(g_cnpx_apis);
  cnpxDomainCreate_t local_ptr = g_cnpx_apis.f_cnpxDomainCreate;
  if (local_ptr) {
    return local_ptr(name);
  }
  return NULL;
}
CNPX_DECLDPEC void cnpxDomainDestroy(cnpxDomainHandle_t domain) {
  CNPX_CHECK_INIT(g_cnpx_apis);
  cnpxDomainDestroy_t local_ptr = g_cnpx_apis.f_cnpxDomainDestroy;
  if (local_ptr) {
    local_ptr(domain);
  }
}
CNPX_DECLDPEC void cnpxDomainMark(cnpxDomainHandle_t domain, const char *message) {
  CNPX_CHECK_INIT(g_cnpx_apis);
  cnpxDomainMark_t local_ptr = g_cnpx_apis.f_cnpxDomainMark;
  if (local_ptr) {
    local_ptr(domain, message);
  }
}
CNPX_DECLDPEC int cnpxDomainRangePush(cnpxDomainHandle_t domain, const char *message) {
  CNPX_CHECK_INIT(g_cnpx_apis);
  cnpxDomainRangePush_t local_ptr = g_cnpx_apis.f_cnpxDomainRangePush;
  if (local_ptr) {
    return local_ptr(domain, message);
  }
  return -1;
}
CNPX_DECLDPEC int cnpxDomainRangePop(cnpxDomainHandle_t domain) {
  CNPX_CHECK_INIT(g_cnpx_apis);
  cnpxDomainRangePop_t local_ptr = g_cnpx_apis.f_cnpxDomainRangePop;
  if (local_ptr) {
    return local_ptr(domain);
  }
  return -1;
}
CNPX_DECLDPEC int cnpxDomainRangeStart(cnpxDomainHandle_t domain, const char *message, void **handle) {
  CNPX_CHECK_INIT(g_cnpx_apis);
  cnpxDomainRangeStart_t local_ptr = g_cnpx_apis.f_cnpxDomainRangeStart;
  if (local_ptr) {
    return local_ptr(domain, message, handle);
  }
  return -1;
}
CNPX_DECLDPEC void cnpxDomainRangeEnd(cnpxDomainHandle_t domain, void *handle) {
  CNPX_CHECK_INIT(g_cnpx_apis);
  cnpxDomainRangeEnd_t local_ptr = g_cnpx_apis.f_cnpxDomainRangeEnd;
  if (local_ptr) {
    local_ptr(domain, handle);
  }
}

CNPX_DECLDPEC void cnpxNameCNdev(const char *name, CNdev device) {
  CNPX_CHECK_INIT(g_cnpx_apis);
  cnpxNameCNdev_t local_ptr = g_cnpx_apis.f_cnpxNameCNdev;
  if (local_ptr) {
    local_ptr(name, device);
  }
}

CNPX_DECLDPEC void cnpxNameCNcontext(const char *name, CNcontext context) {
  CNPX_CHECK_INIT(g_cnpx_apis);
  cnpxNameCNcontext_t local_ptr = g_cnpx_apis.f_cnpxNameCNcontext;
  if (local_ptr) {
    local_ptr(name, context);
  }
}

CNPX_DECLDPEC void cnpxNameCNnotifier(const char *name, CNnotifier notifier) {
  CNPX_CHECK_INIT(g_cnpx_apis);
  cnpxNameCNnotifier_t local_ptr = g_cnpx_apis.f_cnpxNameCNnotifier;
  if (local_ptr) {
    local_ptr(name, notifier);
  }
}

CNPX_DECLDPEC void cnpxNameCNqueue(const char *name, CNqueue queue) {
  CNPX_CHECK_INIT(g_cnpx_apis);
  cnpxNameCNqueue_t local_ptr = g_cnpx_apis.f_cnpxNameCNqueue;
  if (local_ptr) {
    local_ptr(name, queue);
  }
}

CNPX_DECLDPEC uint32_t cnpxDomainRegisterString(cnpxDomainHandle_t domain, const char *string) {
  CNPX_CHECK_INIT(g_cnpx_apis);
  cnpxDomainRegisterString_t local_ptr = g_cnpx_apis.f_cnpxDomainRegisterString;
  if (local_ptr) {
    return local_ptr(domain, string);
  }
  return 0;
}

CNPX_DECLDPEC uint64_t cnpxPayloadSchemaRegister(cnpxDomainHandle_t domain, const cnpxPayloadSchemaAttr_t *attr) {
  CNPX_CHECK_INIT(g_cnpx_apis);
  cnpxPayloadSchemaRegister_t local_ptr = g_cnpx_apis.f_cnpxPayloadSchemaRegister;
  if (local_ptr) {
    return local_ptr(domain, attr);
  }
  return 0;
}

CNPX_DECLDPEC uint64_t cnpxPayloadEnumRegister(cnpxDomainHandle_t domain, const cnpxPayloadEnumAttr_t *attr) {
  CNPX_CHECK_INIT(g_cnpx_apis);
  cnpxPayloadEnumRegister_t local_ptr = g_cnpx_apis.f_cnpxPayloadEnumRegister;
  if (local_ptr) {
    return local_ptr(domain, attr);
  }
  return 0;
}

CNPX_DECLDPEC void cnpxMarkPayload(cnpxDomainHandle_t domain, const cnpxPayloadData_t *payloadData, size_t count) {
  CNPX_CHECK_INIT(g_cnpx_apis);
  cnpxMarkPayload_t local_ptr = g_cnpx_apis.f_cnpxMarkPayload;
  if (local_ptr) {
    local_ptr(domain, payloadData, count);
  }
}

CNPX_DECLDPEC int cnpxRangePushPayload(cnpxDomainHandle_t domain, const cnpxPayloadData_t *payloadData, size_t count) {
  CNPX_CHECK_INIT(g_cnpx_apis);
  cnpxRangePushPayload_t local_ptr = g_cnpx_apis.f_cnpxRangePushPayload;
  if (local_ptr) {
    return local_ptr(domain, payloadData, count);
  }
  return -1;
}

CNPX_DECLDPEC int cnpxRangePopPayload(cnpxDomainHandle_t domain, const cnpxPayloadData_t *payloadData, size_t count) {
  CNPX_CHECK_INIT(g_cnpx_apis);
  cnpxRangePopPayload_t local_ptr = g_cnpx_apis.f_cnpxRangePopPayload;
  if (local_ptr) {
    return local_ptr(domain, payloadData, count);
  }
  return -1;
}

CNPX_DECLDPEC int cnpxRangeStartPayload(cnpxDomainHandle_t domain, void **handle, const cnpxPayloadData_t *payloadData,
                                        size_t count) {
  CNPX_CHECK_INIT(g_cnpx_apis);
  cnpxRangeStartPayload_t local_ptr = g_cnpx_apis.f_cnpxRangeStartPayload;
  if (local_ptr) {
    return local_ptr(domain, handle, payloadData, count);
  }
  return -1;
}

CNPX_DECLDPEC int cnpxRangeEndPayload(cnpxDomainHandle_t domain, void *handle, const cnpxPayloadData_t *payloadData,
                                      size_t count) {
  CNPX_CHECK_INIT(g_cnpx_apis);
  cnpxRangeEndPayload_t local_ptr = g_cnpx_apis.f_cnpxRangeEndPayload;
  if (local_ptr) {
    return local_ptr(domain, handle, payloadData, count);
  }
  return -1;
}
#endif  // CNPX_DEFS_H_
