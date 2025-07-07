#ifndef CNPX_H_
#define CNPX_H_
#if defined(__linux__) || defined(__GNUC__)
#define CNPX_ENABLED TRUE
#endif /* PLATFORM */

#ifdef CNPX_DISABLE
#undef CNPX_ENABLED
#endif

#ifdef CNPX_ENABLED
#define CNPX_DECLDPEC inline static

#ifdef __cplusplus
extern "C" {
#endif
#include "cn_api.h"
struct cnpxDomainHandle_st;
typedef struct cnpxDomainHandle_st *cnpxDomainHandle_t;
#include "cnpx_payload.h"
#include "cnpx_payload_helper.h"
#include "cnpx_internal_types.h"

/**
 * \brief:  Force cnpx library to initialize. The first call to any cnpx function will
 *          automatically initialize the entire CNPX API. This can make the first call slower
 *          than subsequent calls. Calling cnpxInit would ensure the other cnpx calls
 *          occurs at an acceptable time.
 * \param:  reserved  --  reserve for future use
 * \return:  void
 * */
CNPX_DECLDPEC void cnpxInit(const void *reserved);

/**
 * \brief:  Mark an instantaneous event in default domain.
 * \param:  message -- the message associated to this marker event.
 * \return: void
 *
 * */
CNPX_DECLDPEC void cnpxMark(const char *message);

/**
 * \brief:  Start a thread range in default domain.
 * \param:  message -- the message associated to this range event.
 * \return: Reserved. If an error occurs, a negative value is returned.
 *
 * */
CNPX_DECLDPEC int cnpxRangePush(const char *message);

/**
 * \brief:  End a thread range in default domain.
 * \return: Reserved. If an error occurs, a negative value is returned.
 *
 * */
CNPX_DECLDPEC int cnpxRangePop();

/**
 * \brief:  Name a OS thread.
 * \param:  name   --  the name associated to current thread
 * \return:  void
 * */
CNPX_DECLDPEC void cnpxNameOsThread(const char *name);

/**
 * \brief:  Name a queue.
 * \param:  name   --  the name of the CNqueue
 *          queue  --  the handle of the CNqueue to name
 * \return:  void
 *
 * **DEPRECATED** This method is deprecated as of CNToolkit 3.0.
 * Use method cnpxNameCNqueue instead.
 * */
CNPX_DECLDPEC void cnpxNameQueue(const char *name, CNqueue queue);

/**
 * \brief:  Start a process range in default domain.
 * \param:  message -- the message associated to this range event.
 *          handle -- the handle used to correlate a pair of Start and End events.
 * \return: Reserved. If an error occurs, a negative value is returned.
 *
 * */
CNPX_DECLDPEC int cnpxRangeStart(const char *message, void **handle);

/**
 * \brief:  End a process range in default domain.
 * \param:  handle -- the handle returned from a cnpxRangeStart call.
 * \return: void
 *
 * */
CNPX_DECLDPEC void cnpxRangeEnd(void *handle);

/**
 * \brief:  Register a CNPX domain.
 *          Domains are used to scope annotations.
 *          Each domain maintains its own range stack.
 * \param:  name  -- A unique string representing the domain.
 * \return: A handle representing the domain.
 *
 * */
CNPX_DECLDPEC cnpxDomainHandle_t cnpxDomainCreate(const char *name);
/**
 * \brief:  Unregister a CNPX domain.
 *          Unregister the domain and free all domain specific resources.
 * \param:  domain  -- the domain handle
 * \return: void
 *
 * */
CNPX_DECLDPEC void cnpxDomainDestroy(cnpxDomainHandle_t domain);
/**
 * \brief:  Mark an instantaneous event in a domain.
 * \param:  domain  -- the domain handle
 *          message -- the message associated to this marker event.
 * \return: void
 *
 * */
CNPX_DECLDPEC void cnpxDomainMark(cnpxDomainHandle_t domain, const char *message);
/**
 * \brief:  Start a thread range in a domain.
 * \param:  domain  -- the domain handle
 *          message -- the message associated to this range event.
 * \return: Reserved. If an error occurs, a negative value is returned.
 *
 * */
CNPX_DECLDPEC int cnpxDomainRangePush(cnpxDomainHandle_t domain, const char *message);
/**
 * \brief:  End a thread range in a domain.
 * \param:  domain  -- the domain handle
 * \return: Reserved. If an error occurs, a negative value is returned.
 *
 * */
CNPX_DECLDPEC int cnpxDomainRangePop(cnpxDomainHandle_t domain);
/**
 * \brief:  Start a process range in a domain.
 * \param:  domain  -- the domain handle
 *          message -- the message associated to this range event.
 *          handle -- the handle used to correlate a pair of Start and End events.
 * \return: Reserved. If an error occurs, a negative value is returned.
 *
 * */
CNPX_DECLDPEC int cnpxDomainRangeStart(cnpxDomainHandle_t domain, const char *message, void **handle);
/**
 * \brief:  End a process range in a domain.
 * \param:  domain  -- the domain handle
 *          handle -- the handle returned from a cnpxDomainRangeStart call.
 * \return: void
 *
 * */
CNPX_DECLDPEC void cnpxDomainRangeEnd(cnpxDomainHandle_t domain, void *handle);

/**
 * \brief:  Name a device.
 * \param:  name   --  the name of the CNdevice.
 *          device  --  the handle of the CNdevice to name
 * */
CNPX_DECLDPEC void cnpxNameCNdev(const char *name, CNdev device);

/**
 * \brief:  Name a context.
 * \param:  name   --  the name of the CNcontext
 *          context  --  the handle of CNcontext to name
 * \return:  void
 * */
CNPX_DECLDPEC void cnpxNameCNcontext(const char *name, CNcontext context);

/**
 * \brief:  Name a notifier.
 * \param:  name   --  the name of the CNnotifier.
 *          notifier  --  the handle of CNnotifier to name
 * \return:  void
 * */
CNPX_DECLDPEC void cnpxNameCNnotifier(const char *name, CNnotifier notifier);

/**
 * \brief:  Name a queue.
 * \param:  name   --  the name of the CNqueue
 *          queue  --  the handle of the CNqueue to name
 * \return:  void
 * */
CNPX_DECLDPEC void cnpxNameCNqueue(const char *name, CNqueue queue);

#include "cnpx_internal_defs.h"
#ifdef __cplusplus
}
#endif /* __cplusplus */

#else /* CNPX DISABLE */

#define cnpxInit(...)
#define cnpxMark(...)
#define cnpxRangePush(...) -1
#define cnpxRangePop(...) -1
#define cnpxNameOsThread(...)
#define cnpxNameQueue(...)
#define cnpxRangeStart(...) -1
#define cnpxRangeEnd(...)
#define cnpxDomainCreate(...) NULL
#define cnpxDomainDestroy(...)
#define cnpxDomainMark(...)
#define cnpxDomainRangePush(...) -1
#define cnpxDomainRangePop(...) -1
#define cnpxDomainRangeStart(...) -1
#define cnpxDomainRangeEnd(...)
#define cnpxNameCNdev(...)
#define cnpxNameCNcontext(...)
#define cnpxNameCNnotifier(...)
#define cnpxNameCNqueue(...)
#define cnpxDomainRegisterString(...)
#define cnpxPayloadSchemaRegister(...)
#define cnpxPayloadEnumRegister(...)
#define cnpxMarkPayload(...)
#define cnpxRangePushPayload(...)
#define cnpxRangePopPayload(...)
#define cnpxRangeStartPayload(...)
#define cnpxRangeEndPayload(...)

#endif  /* CNPX ENABLED */
#endif  // CNPX_H_
