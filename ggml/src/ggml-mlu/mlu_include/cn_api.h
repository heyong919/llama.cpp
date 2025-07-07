/*************************************************************************
 * Copyright (C) [2023] by Cambricon, Inc.
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO NOTIFIER SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *************************************************************************/

/************************************************************************
 *
 *  @file cn_api.h
 *
 *  @brief CNDrv provides programmable interfaces for users to develop
 *  their-owned programs, which includes Device management, Context
 *  management, memory management of both sides (devices and hosts), etc.
 *
 *  This header file contains programmable interface declarations,
 *      data types and Error code definitions.
 *
 **************************************************************************/

#ifndef CN_API_H_
#define CN_API_H_

/************************************************************************
 *  Include files
 ************************************************************************/

#include <stdlib.h>

#ifdef _MSC_VER
typedef unsigned __int64 cn_uint64_t;
typedef __int64 cn_int64_t;
typedef unsigned __int32 cn_uint32_t;
#else
#include <stdint.h>
typedef uint64_t cn_uint64_t;
typedef int64_t cn_int64_t;
typedef uint32_t cn_uint32_t;
#endif

#ifndef __CN_EXPORT
#if defined(__DOXYGEN_ONLY__)
#define __CN_EXPORT
#elif defined(WIN32) || defined(WINDOWS)
#if defined(DRIVERAPI_IMPORTS)
#define __CN_EXPORT __declspec(dllimport)
#else
#define __CN_EXPORT __declspec(dllexport)
#endif
#else
#define __CN_EXPORT
#endif
#endif

#ifndef __DOXYGEN_ONLY__
#define CN_API_PERTHREAD(func)  func##_pt
#define CN_API_PERPROCESS(func) func##_pp
#else
#define CN_API_PERTHREAD(func)  func
#define CN_API_PERPROCESS(func) func
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define CN_MAJOR_VERSION 2
#define CN_MINOR_VERSION 15
#define CN_PATCH_VERSION 5

#define CN_VERSION (CN_MAJOR_VERSION * 100000 + CN_MINOR_VERSION * 1000 + CN_PATCH_VERSION)

#ifdef CN_CTX_CONFIG_PARAM_PER_PROCESS
#define cnSetCtxConfigParam CN_API_PERPROCESS(cnSetCtxConfigParam)
#define cnGetCtxConfigParam CN_API_PERPROCESS(cnGetCtxConfigParam)
#else
#define cnSetCtxConfigParam CN_API_PERTHREAD(cnSetCtxConfigParam)
#define cnGetCtxConfigParam CN_API_PERTHREAD(cnGetCtxConfigParam)
#endif

/**
 *  Invoke kernel extra type
 */
#define CN_INVOKE_PARAM_BUFFER_POINTER ((void *)0x1UL)
#define CN_INVOKE_PARAM_BUFFER_SIZE    ((void *)0x2UL)
#define CN_INVOKE_PARAM_END            ((void *)0x0UL)

#ifdef CN_SUCCESS
#if CN_SUCCESS != 0
#error "Redefined CN_SUCCESS != 0."
#endif
#undef CN_SUCCESS
#endif

/**
 * @brief The return values for CNDrv APIs.
 */
typedef enum {
    /**
     * This indicates that the API has been executed successfully.
     * In the query APIs, it also indicates the completion of asynchronous operation (see
     * ::cnQueryQueue and ::cnQueryNotifier).
     */
    CN_SUCCESS = 0,
    /**
     * This indicates that asynchronous operations have not been completed, and it is not an
     * error.
     */
    CN_ERROR_NOT_READY = 1,
    /**
     * This indicates that CNDrv has not been initialized with
     * ::cnInit or CNDrv fails to be initialized.
     */
    CN_ERROR_NOT_INITIALIZED = 100002,
    /**
     * This indicates that the MLU driver has been uninstalled
     * or broken down.
     */
    CN_ERROR_DEINITIALIZED = 100003,
    /**
     * This indicates that MLU driver cannot detect any usable MLU device.
     */
    CN_ERROR_NO_DEVICE = 100004,
    /**
     * This indicates that the device handle is invalid.
     */
    CN_ERROR_INVALID_DEVICE = 100005,
    /**
     * This indicates that parameters
     * passed to this API are not within an acceptable value range.
     */
    CN_ERROR_INVALID_VALUE = 100006,
    /**
     * This indicates that an OS call fails.
     */
    CN_ERROR_OPERATING_SYSTEM = 100007,
    /**
     * This indicates that the system is not ready to start any
     * MLU task. To continue using MLU, verify that the MLU driver has been
     * installed successfully.
     */
    CN_ERROR_SYSTEM_NOT_READY = 100008,
    /**
     * This indicates that there is a mismatch between
     * CNDrv and MLU driver.
     */
    CN_ERROR_SYSTEM_DRIVER_MISMATCH = 100009,
    /**
     * This indicates that the system cannot allocate
     * enough host memory.
     */
    CN_ERROR_SYSTEM_OUT_OF_MEMORY = 100010,
    /**
     * This indicates that the host address is invalid.
     */
    CN_ERROR_INVALID_ADDRESS = 100011,
    /**
     * This indicates that the specified file cannot be found.
     */
    CN_ERROR_FILE_NOT_FOUND = 100012,
    /**
     *This indicates compatibility problems with the system.
     *The reason may be that cross-generation MLU devices exist among visible MLU devices (the major
     *computing capabilities are different).*/
    CN_ERROR_COMPAT_NOT_SUPPORT_ON_SYSTEM = 100013,
    /**
     * This indicates that a resource handle passed to the API call is not valid.
     * Resource handles are opaque types like ::CNqueue and ::CNcontext.
     */
    CN_ERROR_INVALID_HANDLE = 100014,
    /**
     * This indicates that the resource required by the API call is not in a valid
     * state to perform the request operation.
     */
    CN_ERROR_ILLEGAL_STATE = 100015,
    /**
     * This indicates that an uncorrectable ECC error was detected during execution.
     */
    CN_ERROR_ECC_UNCORRECTABLE = 100016,
    /**
     * This indicates that the operation is not supported
     * on the current system, device or driver.
     */
    CN_OPS_ERROR_NOT_SUPPORTED = 100050,
    /**
     * This indicates that the operation is not permitted.
     */
    CN_OPS_ERROR_NOT_PERMITTED = 100051,
    /**
     * This indicates that the timeout for the operation has lapsed.
     */
    CN_OPS_ERROR_TIMEOUT = 100052,

    /**
     * This indicates that CNDrv has insufficient resources to do the operation.
     *
     * The causes include:
     * - Insufficient MLU memory.
     * - Insufficient host memory.
     */
    CN_OPS_ERROR_OUT_OF_RESOURCES = 100053,
    /**
     * This indicates that the queue handle is invalid.
     * The causes include:
     * - The queue handle is not created by ::cnCreateQueue() or ::cnCreateQueueWithPriority().
     * - The queue handle is destroyed by calling ::cnDestroyQueue(),
     *   or the context of the queue has been destroyed.
     * - The kernels of the queue fail to be executed.
     */
    CN_QUEUE_ERROR_INVALID = 100060,
    /**
     * This indicates that CNDrv has insufficient resources to create a queue.
     *
     * The causes include:
     * - Insufficient MLU memory.
     * - Insufficient host memory.
     */
    CN_QUEUE_ERROR_OUT_OF_RESOURCES = 100061,
    /**
     * This indicates that the notifier handle is invalid.
     * The causes include:
     * - The notifier handle is not created by ::cnCreateNotifier().
     * - The notifier handle has been destroyed by calling ::cnDestroyNotifier().
     * - The context of the notifier has been destroyed.
     */
    CN_NOTIFIER_ERROR_INVALID = 100080,
    /**
     * This indicates that CNDrv has insufficient resources to create a notifier.
     *
     * The causes include:
     * - Insufficient MLU memory.
     * - The number of notifiers created exceeds the limit.
     */
    CN_NOTIFIER_ERROR_OUT_OF_RESOURCES = 100081,
    /**
     * This indicates that CNDrv can not allocate
     * enough MLU memory.
     */
    CN_MEMORY_ERROR_OUT_OF_MEMORY = 100100,
    /**
     * This indicates that current MLU memory usage exceeds the
     * maximum MLU memory usage of the current process via the
     * MLU_PROCESS_MAX_MEMORY_USAG environment variable.
     */
    CN_MEMORY_ERROR_OUT_OF_PROCESS_MAX_MEMORY = 100101,
    /**
     * This indicates that the MLU address is invalid.
     *
     * The causes include:
     * - The MLU address is not allocated by ::cnMalloc, ::cnMallocNode, ::cnMallocConstant,
     *   ::cnMallocNodeConstant, ::cnZmallocNode, or ::cnZmalloc.
     * - The MLU address is released by calling ::cnFreeHost.
     */
    CN_MEMORY_ERROR_INVALID_ADDRESS = 100102,
    /**
     * This indicates that there are insufficient resources to invoke the kernel.
     *
     * The causes include:
     * - Insufficient MLU memory for the local memory of this kernel.
     * - The count of the parameters is too large.
     * - The ::KernelClass of the kernel is so large that the current device is not supported.
     *   See the device attribute ::CN_DEVICE_ATTRIBUTE_MAX_CLUSTER_COUNT_PER_UNION_TASK for more
     *   information.
     */
    CN_INVOKE_ERROR_OUT_OF_RESOURCES = 100120,
    /**
     * This indicates that the kernel takes too long to execute.
     * The causes include:
     * - The argument of the kernel is invalid.
     * - The instruction of the kernel is invalid.
     */
    CN_INVOKE_ERROR_EXECUTED_TIMEOUT = 100121,
    /**
     * This indicates that the kernel takes too long to invoke.
     *
     * The causes include:
     * - The current queue is full and there are too many kernels to execute.
     * - The argument of the kernel is invalid.
     */
    CN_INVOKE_ERROR_TIMEOUT = 100122,
    /**
     * This indicates that while executing a kernel, the device runs into
     * an illegal instruction.
     */
    CN_INVOKE_ERROR_ILLEGAL_INSTRUCTION = 100123,
    /**
     * This indicates that while executing a kernel, the device runs into
     * an instruction that will use an MLU memory address (global or local) that is not
     * allowed address space.
     */
    CN_INVOKE_ERROR_ADDRESS_SPACE = 100124,
    /**
     * This indicates that an error occurs on the device with tiny core
     * disabled when executing a kernel.
     */
    CN_INVOKE_NO_TINYCORE = 100125,
    /**
     * This indicates that the instruction is illegal when fetched.
     */
    CN_INVOKE_ERROR_INVALID_PC = 100126,
    /**
     * This indicates that while executing a kernel, the device runs into
     * an instruction that has illegal operand.
     */
    CN_INVOKE_ERROR_ILLEGAL_OPERAND = 100127,
    /**
     * This indicates that a device-side assert is triggered during kernel execution.
     */
    CN_INVOKE_ERROR_ASSERT = 100128,
    /**
     * This indicates that while executing a kernel, the device runs into
     * an instruction that uses a misaligned MLU memory address.
     */
    CN_INVOKE_ERROR_MEMORY_MISALIGNED_ADDRESS = 100130,
    /**
     * This indicates that while executing a kernel, the device runs into
     * an instruction that uses an illegal MLU memory address (global or local).
     */
    CN_INVOKE_ERROR_MEMORY_ILLEGAL_ADDRESS = 100131,

    /**
     * This indicates that while executing a kernel, the device-side code proceeds to
     * a specific trap.
     */
    CN_INVOKE_ERROR_KERNEL_TRAP = 100132,
    /**
     * This indicates that while executing a kernel, the device-side code proceeds to
     * a user specific trap.
     */
    CN_INVOKE_ERROR_KERNEL_USER_TRAP = 100133,
    /**
     * This indicates that it exceeds the stack size limit.
     */
    CN_INVOKE_ERROR_STACK_OVERFLOW = 100134,
    /**
     * This indicates that while executing a kernel, the device runs into
     * an instruction that will use an MLU memory address (global or local)
     * that is out of current space range.
     *
     * The causes include:
     * - address is out of range.
     * - address adding offset is out of range.
     */
    CN_INVOKE_ERROR_OUT_OF_RANGE_ADDRESS = 100135,
    /**
     * This indicates that the device encounters NaN.
     */
    CN_INVOKE_ERROR_NAN = 100136,
    /**
     * This indicates that the device encounters positive infinity.
     */
    CN_INVOKE_ERROR_POSITIVE_INF = 100137,
    /**
     * This indicates that the device encounters negative infinity.
     */
    CN_INVOKE_ERROR_NEGATIVE_INF = 100138,
    /**
     * This indicates that an exception occurs on the device when executing
     * a kernel.
     */
    CN_INVOKE_ERROR = 100200,
    /**
     * This mostly indicates that the current thread is not bound to
     * a valid context.
     * The causes include:
     * - It fails to call ::cnCtxCreate() or ::cnCtxSetCurrent() to bind the context
     *   to the current thread.
     * - The context bound to the current thread has been destroyed by calling
     *   ::cnCtxDestroy().
     *
     * This also indicates that the context handle is invalid.
     * The causes include:
     * - The context handle has not been created by ::cnCtxCreate().
     * - The context handle has been destroyed by ::cnCtxDestroy().
     */
    CN_CONTEXT_ERROR_INVALID = 100201,
    /**
     * This indicates that CNDrv has insufficient resources to create a context.
     *
     * The causes include:
     * - Insufficient thread key.
     * - Insufficient MLU memory.
     */
    CN_CONTEXT_ERROR_OUT_OF_RESOURCES = 100202,
    /**
     * This indicates that the fat binary compilation fails.
     */
    CN_ERROR_INVALID_FATBIN = 100300,
    /**
     * This indicates that the specified symbol is not found.
     */
    CN_ERROR_NOT_FOUND = 100301,
    /**
     * This indicates that the kernel handle is invalid.
     *
     * The cause includes:
     * - The kernel handle is not acquired by ::cnModuleGetKernel.
     */
    CN_ERROR_INVALID_KERNEL = 100302,
    /**
     * This indicates that the fat binary is compiled with an
     * unsupported toolchain.
     */
    CN_FATBIN_ERROR_UNSUPPORTED_VERSION = 100303,
    /**
     * This indicates that the fat binary source is invalid.
     */
    CN_FATBIN_ERROR_NO_KERNEL = 101312,
    /**
     * This indicates that the fat binary image is invalid.
     */
    CN_FATBIN_INVALID_IMAGE = 101313,
    /**
     * This indicates that CNBin library is not found.
     */
    CN_FATBIN_LIBCNBIN_NOT_FOUND = 101314,
    /**
     * This indicates that the architecture option passed into CNCC
     * when compiling does not match the current MLU platform.
     */
    CN_FATBIN_INVALID_ARCH = 101315,

    /**
     * This indicates that the operation is not permitted when
     * the queue is capturing.
     */
    CN_ERROR_QUEUE_CAPTURE_UNSUPPORTED = 100360,

    /**
     * This indicates that the current capture sequence on the queue
     * has been invalidated due to previous error.
     */
    CN_ERROR_QUEUE_CAPTURE_INVALIDATED = 100361,

    /**
     * This indicates that the current capture sequence on the queue
     * does not begin with the CN_QUEUE_CAPTURE_MODE_RELAXED mode, and ends
     * in a different thread.
     */
    CN_ERROR_QUEUE_CAPTURE_WRONG_THREAD = 100362,

    /**
     * This indicates that two independent capture sequences are merged.
     */
    CN_ERROR_QUEUE_CAPTURE_MERGED = 100363,

    /**
     * This indicates that the capture sequence contains at least a fork that is not
     * joined to the primary queue.
     */
    CN_ERROR_QUEUE_CAPTURE_UNJOINED = 100364,

    /**
     * This indicates that a queue in capture sequence is trying to create a dependency
     * which crosses the queue capture boundary.
     */
    CN_ERROR_QUEUE_CAPTURE_ISOLATION = 100365,

    /**
     * This indicates that the queue in capture sequence is not the initially captured one.
     */
    CN_ERROR_QUEUE_CAPTURE_UNMATCHED = 100366,

    /**
     * This indicates that the TaskTopo update is not performed because it includes
     * changes which violate constraints specific to TaskTopo entity update.
     */
    CN_ERROR_TASK_TOPO_ENTITY_UPDATE_FAILURE = 100400,

    /**
     * This indicates that the NCS task experiences timeout. You can set the timeout period.
     */
    CN_NCS_ACK_TASK_TIMEOUT = 101410,
    /**
     * This indicates that the QPair is invalid.
     */
    CN_NCS_ACK_QP_INVALID = 101411,
    /**
     * This indicates that the QPair is busy.
     */
    CN_NCS_ACK_QP_BUSY = 101412,
    /**
     * This indicates that the QPair does not exist.
     */
    CN_NCS_ACK_QP_NOT_EXISTS = 101413,
    /**
     * This indicates that the template is invalid.
     */
    CN_NCS_ACK_TEMPLATE_INVALID = 101414,
    /**
     * This indicates that the template does not exist.
     */
    CN_NCS_ACK_TEMPLATE_NOT_EXISTS = 101415,
    /**
     * This indicates that the parameter is invalid when launching a task.
     */
    CN_NCS_ACK_PARAM_INVALID = 101416,
    /**
     * This indicates that the counts of descriptions are invalid.
     * The cause includes:
     * - The counts of descriptions of receive task and send task are not equal.
     */
    CN_NCS_ACK_DECS_CNT_ERR = 101417,
    /**
     * This indicates that the source address of send task is invalid.
     */
    CN_NCS_ACK_SRC_ADDR_ERR = 101418,
    /**
     * This indicates that the destination address of receive task is invalid.
     */
    CN_NCS_ACK_DST_ADDR_ERR = 101419,
    /**
     * This indicates that the kernel is invalid.
     */
    CN_NCS_ACK_KERNEL_ERR = 101420,
    /**
     * This indicates that the trigger count of send task is not as expected.
     */
    CN_NCS_ACK_TRIGGER_CNT_ERR = 101421,
    /**
     * This indicates that the remote task of the current task experiences error.
     */
    CN_NCS_ACK_REMOTE_ERR = 101422,
    /**
     * This indicates some mistakes that cannot be corrected on link layer.
     */
    CN_NCS_ACK_LINK_UNCORR_ERR = 101423,
    /**
     * This indicates that a link error occurs when a task is running.
     * The causes include:
     * - The SerDes error.
     * - CN_NCS_ACK_LINK_UNCORR_ERR.
     */
    CN_NCS_ACK_LINK_ERR = 101424,
    /**
     * This indicates that the NCS tasks already exist.
     */
    CN_NCS_ACK_TASK_EXIST = 101425,
    /**
     * This indicates that the type of NCS tasks is invalid.
     * The valid types of NCS tasks include:
     * - CN_TASK_DIR_RX
     * - CN_TASK_DIR_TX
     */
    CN_NCS_ACK_TASK_TYPE_INVALID = 101426,
    /**
     * This indicates that descriptions do not match.
     * The causes of the mismatch include:
     * - The counts of descriptions of receive task and send task are not equal.
     * - The total data sizes of descriptions of receive task and send task are not equal.
     */
    CN_NCS_ACK_DESC_NOT_MATCH = 101427,
    /**
     * This indicates other errors.
     */
    CN_NCS_ACK_OTHER = 101449,
    /**
     * This indicates an internal fatal error of TCDP.
     */
    CN_TCDP_ACK_INTERNAL_ERROR = 101450,
    /**
     * This indicates the TCDP handle is invalid.
     */
    CN_TCDP_ACK_TCDP_HANDLE_INVALID = 101451,
    /**
     * This indicates that there is no slot resource left for TCDP.
     */
    CN_TCDP_ACK_TCDP_NO_RESOURCE = 101452,
    /**
     * This indicates that TCDP resend cnt exceeds max resend cnt.
     */
    CN_TCDP_ACK_TCDP_RESEND_EXCEED_MAX_NUM = 101453,
    /**
     * This indicates a TCDP proxy error.
     */
    CN_TCDP_ACK_TCDP_PROXY_ERROR = 101454,
    /**
     * This indicates a TCDP fatal error.
     */
    CN_TCDP_ACK_TCDP_FATAL_ERROR = 101455,
    /**
     * This indicates other TCDP errors.
     */
    CN_TCDP_ACK_OTHER = 101500,
    /**
     * This indicates hardware error.
     */
    CN_NCS_HW_ERROR = 101600,
    /**
     * This indicates that the port ID is not in the range.
     */
    CN_NCS_PORT_NOT_IN_RANGE = 101601,
    /**
     * This indicates link error when creating/modifing a QPair.
     * The causes include:
     * - The SerDes link error.
     * - The port is not enabled or configured.
     * - QP socket client connect failed.
     * - CN_NCS_ACK_LINK_UNCORR_ERR.
     */
    CN_NCS_PORT_LINK_ERR = 101602,
    /**
     * This indicates that the parameter is invalid when creating a QPair.
     */
    CN_NCS_QP_CREATE_PARAM_INVALID = 101603,
    /**
     * This indicates that the hardware has no resource when creating a QPair.
     */
    CN_NCS_QP_CREATE_NO_RESOURCE = 101604,
    /**
     * This indicates that the QPair modify timeout.
     */
    CN_NCS_QP_MODIFY_PAIRING = 101605,
    /**
     * This indicates that the local QPair is invalid.
     */
    CN_NCS_QP_MODIFY_LQP_INVALID = 101606,
    /**
     * This indicates that the remote QPair is invalid.
     */
    CN_NCS_QP_MODIFY_RQP_INVALID = 101607,
    /**
     * This indicates that the hardware has no resource when modifying a QPair.
     */
    CN_NCS_QP_MODIFY_NO_RESOURCE = 101608,
    /**
     * This indicates that the parameter is invalid when destroying a QPair.
     */
    CN_NCS_QP_DESTROY_PARAM_INVALID = 101609,
    /**
     * This indicates that the QPair does not exist.
     */
    CN_NCS_QP_DESTROY_NOT_EXISTS = 101610,
    /**
     * This indicates that the QPair is busy.
     */
    CN_NCS_QP_DESTROY_BUSY = 101611,
    /**
     * This indicates that the QPair user is not expected.
     */
    CN_NCS_QP_DESTROY_USER_INVALID = 101612,
    /**
     * This indicates that the parameter is invalid when creating a template.
     */
    CN_NCS_TEMPLATE_CREATE_PARAM_INVALID = 101613,
    /**
     * This indicates that the hardware has no resource when creating a template.
     */
    CN_NCS_TEMPLATE_CREATE_NO_RESOURCE = 101614,
    /**
     * This indicates template loading error.
     * The causes include:
     * - Hardware error.
     * - CN_NCS_TEMPLATE_CREATE_PARAM_INVALID.
     */
    CN_NCS_TEMPLATE_CREATE_LOAD_ERR = 101615,
    /**
     * This indicates that the parameter is invalid when destroying a template.
     */
    CN_NCS_TEMPLATE_DESTROY_PARAM_INVALID = 101616,
    /**
     * This indicates that the template does not exist.
     */
    CN_NCS_TEMPLATE_DESTROY_NOT_EXISTS = 101617,
    /**
     * This indicates that the template is busy.
     * The cause includes:
     * - The template is occupied by other NCS tasks.
     */
    CN_NCS_TEMPLATE_DESTROY_BUSY = 101618,
    /**
     * This indicates that the user of the template is not expected.
     */
    CN_NCS_TEMPLATE_DESTROY_USER_INVALID = 101619,
    /**
     * This indicates that the port is disabled based on external setting.
     */
    CN_NCS_PORT_OB_DISABLED = 101620,
    /**
     * This indicates that the port is disabled based on internal setting.
     */
    CN_NCS_PORT_IB_DISABLED = 101621,
    /**
     * This indicates that the port is disabled.
     */
    CN_NCS_PORT_DISABLED = 101622,
    /**
     * This indicates that NCS is not supported on current platform.
     */
    CN_NCS_UNSUPPORT = 101623,
    /**
     * This indicates that the QPair parameter passed in is invalid when QPair error is injected
     */
    CN_NCS_GET_LQP_INVALID = 101624,
    /**
     * This indicates that the physical address of the inplace buffer is not continuous.
     */
    CN_NCS_INPLACEBUFF_IS_NOT_CONTINUOUS_ERROR = 101625,
    /**
     * This indicates that the QPair error injection fails. QPair status and link should be
     * checked..
     */
    CN_NCS_QP_IS_INJECTING_ERROR = 101626,
    /**
     * This result indicates hardware error.
     */
    CN_TCDP_HW_ERROR = 101700,
    /**
     * This result indicates that there is no slot resource left for TCDP.
     */
    CN_TCDP_NO_RESOURCE = 101701,
    /**
     * This result indicates that a parameter passed to the TCDP API call is not valid.
     */
    CN_TCDP_INVALID_PARAM = 101702,
    /**
     * This result indicates that slot in TCDP handle does not exist.
     */
    CN_TCDP_SLOT_NOT_EXIST = 101703,
    /**
     * This result indicates that the TCDP handle does not belong to current user.
     */
    CN_TCDP_USER_INVALID = 101704,
    /**
     * This result indicates that the QP is busy.
     */
    CN_TCDP_QP_BUSY = 101705,
    /**
     * This result indicates that TCDP is not supported on current platform.
     */
    CN_TCDP_UNSUPPORT = 101706,
    /**
     * This result indicates that an error occurs when updating topology.
     */
    CN_NCS_UPDATE_TOPOLOGY = 101800,
    /**
     * This indicates that an error occurs when launching a kernel.
     */
    CN_NCS_LAUNCH_KERNEL = 101801,
    /**
     * This indicates that an error occurs when getting port information.
     */
    CN_NCS_GET_PORT_INFO = 101802,
    /**
     * This indicates that an error occurs when getting the device UUID.
     */
    CN_NCS_GET_DEVICE_UUID = 101803,
    /**
     * This indicates that the port ID is invalid.
     */
    CN_NCS_PORT_ID_INVALID = 101804,
    /**
     * This indicates that the task description is invalid.
     */
    CN_NCS_DESC_INVALID = 101805,
    /**
     * This indicates unknown error of NCS.
     */
    CN_NCS_ERROR = 101898,
    /**
     * This indicates internal fatal error of NCS.
     */
    CN_NCS_INTERNAL_ERROR = 101899,

    /**
     * This indicates an unknown error.
     */
    CN_ERROR_UNKNOWN = 200000
} CNresult;

/**
 * @brief The compute modes.
 */
typedef enum CNcomputemode_enum {
    CN_COMPUTEMODE_DEFAULT =
        0, /**< The default compute mode: multiple contexts are allowed per device. */
    CN_COMPUTEMODE_EXCLUSIVE_PROCESS =
        1 /**< The compute-exclusive-process mode: only one context used by a single process can be
             present on this device at a time. */
} CNcomputemode;

/**
 * @brief The properties of the device, which are used by ::cnDeviceGetAttribute.
 */
typedef enum CNdevice_attribute_enum {
    /* Computing Capabilities */
    CN_DEVICE_ATTRIBUTE_COMPUTE_CAPABILITY_MAJOR =
        0x01, /**< Major computing capability version number. */
    CN_DEVICE_ATTRIBUTE_COMPUTE_CAPABILITY_MINOR =
        0x02, /**< Minor computing capability version number. */
    CN_DEVICE_ATTRIBUTE_SPARSE_COMPUTING_SUPPORTED =
        0x03, /**< The device supports sparse convolution or matrix multiplication by convolution
                 filter function unit of the core. */
    CN_DEVICE_ATTRIBUTE_FP16_COMPUTING_SUPPORTED =
        0x04, /**< The device supports fp16 type computing by all the function units of the core. */
    CN_DEVICE_ATTRIBUTE_INT4_COMPUTING_SUPPORTED =
        0x05, /**< The device supports int4 type convolution or matrix multiplication by
                 convolution filter function unit of the core. */
    CN_DEVICE_ATTRIBUTE_INT8_COMPUTING_SUPPORTED =
        0x06, /**< The device supports int8 type convolution or matrix multiplication by
                 convolution filter function unit of the core. */
    CN_DEVICE_ATTRIBUTE_BF16_COMPUTING_SUPPORTED =
        0x07, /**< The device supports bf16 type convolution or matrix multiplication by
                 convolution filter function unit of the core. */
    CN_DEVICE_ATTRIBUTE_TF32_COMPUTING_SUPPORTED =
        0x08, /**< The device supports tf32 type convolution or matrix multiplication by
                 convolution filter function unit of the core. */
    CN_DEVICE_ATTRIBUTE_COMPUTE_MODE =
        0x09, /**< The compute modes. See ::CNcomputemode for details. */

    /* Heterogeneous Capabilities */
    CN_DEVICE_ATTRIBUTE_MAX_QUEUE_COUNT =
        0x101, /**< The maximum count of queues that can be created on a device.
                  Each creation of a TaskTopo entity will occupy some queues, which depends on the
                  parallelism of the entity. So the maximum count of queues may not be created.*/
    CN_DEVICE_ATTRIBUTE_MAX_NOTIFIER_COUNT =
        0x102, /**< The maximum count of notifiers that can be created on a device. The notifier
                  created by the flag ::CN_NOTIFIER_DISABLE_TIMING_ALL will not be counted in this
                  value. Each creation of a context will occupy some notifiers, so the maximum
                  count of notifiers might not be created. */
    CN_DEVICE_ATTRIBUTE_QUEUE_PRIORITIES_SUPPORTED =
        0x103, /**< The queue priorities that are supported by device. */
    CN_DEVICE_ATTRIBUTE_TINY_CORE_SUPPORTED =
        0x104, /**< The device supports using tiny core to accelerate inter-device or intra-device
                collective communication. */
    CN_DEVICE_ATTRIBUTE_CODEC_JPEG_SUPPORTED =
        0x105, /**< The device supports hardware JPEG Codec acceleration. */
    CN_DEVICE_ATTRIBUTE_CODEC_H264_SUPPORTED =
        0x106, /**< The device supports hardware video H.264 Codec acceleration. */
    CN_DEVICE_ATTRIBUTE_CODEC_H265_SUPPORTED =
        0x107, /**< The device supports hardware video H.265 Codec acceleration. */
    CN_DEVICE_ATTRIBUTE_AIISP_CORE_SUPPORTED = 0x108, /**< The device supports AI ISP core. */
    CN_DEVICE_ATTRIBUTE_MULTI_CTX_NOTIFIER_WAIT_SUPPORTED =
        0x109, /**< The device supports wait notifier on another context's queue. */
    CN_DEVICE_ATTRIBUTE_IPCNOTIFIER_SUPPORTED =
        0x10a, /**< The device supports ipcnotifier functions via ::cnIpcGetNotifierHandle and
                  ::cnIpcOpenNotifierHandle. */

    /* Elastic Capabilities */
    CN_DEVICE_ATTRIBUTE_MAX_BLOCK_TASK_DIM_X =
        0x201, /**< The maximum x-dimension of a block task. */
    CN_DEVICE_ATTRIBUTE_MAX_BLOCK_TASK_DIM_Y =
        0x202, /**< The maximum y-dimension of a block task. */
    CN_DEVICE_ATTRIBUTE_MAX_BLOCK_TASK_DIM_Z =
        0x203, /**< The maximum z-dimension of a block task. */
    CN_DEVICE_ATTRIBUTE_MAX_CLUSTER_COUNT_PER_UNION_TASK =
        0x204, /**< The maximum number of clusters per union task. @deprecated This flag is
                  deprecated since CNDrv v1.6.0. */
    CN_DEVICE_ATTRIBUTE_MAX_CLUSTER_COUNT = 0x205, /**< The number of clusters on device. */
    CN_DEVICE_ATTRIBUTE_MAX_CORE_COUNT_PER_CLUSTER =
        0x206, /**< The maximum number of MLU Cores per cluster. */
    CN_DEVICE_ATTRIBUTE_MAX_QUADRANT_COUNT =
        0x207, /**< The maximum count of quadrants per device. Intra-quadrant clusters have the best
                  unified memory access performance. */
    CN_DEVICE_ATTRIBUTE_MAX_UNION_TYPE_PER_QUADRANT =
        0x208, /**< The maximum union task type that can maintain unified intra-quadrant memory
                * access.
                */
    CN_DEVICE_ATTRIBUTE_MAX_CLUSTERS_PER_UNION_LIMIT_TASK =
        0x209, /**< The maximum number of clusters per union limitation task. */
    CN_DEVICE_ATTRIBUTE_MLU_ISA_VERSION =
        0x20a, /**< ISA version of the current MLU device in the form of three digit numbers (for
                  example, '270' for MLU270-X5K device). */
    CN_DEVICE_ATTRIBUTE_IS_MULTIPLE_TENSOR_PROCESSOR =
        0x20b, /**< MLU ISA version has two prefixes 'tp_xxx' and 'mtp_xxx'. 'mtp_xxx' architecture
                  contains at least one cluster, so ::cnDeviceGetAttribute() returns 1 if the
                  current MLU device adopts multi-tensor-processor architecture. */
    CN_DEVICE_ATTRIBUTE_IPCRESOURCE_PER_CTX =
        0x20c, /**< The maximum number of ipc handle resources per context. */

    /* Memory Capacities */
    CN_DEVICE_ATTRIBUTE_MAX_L2_CACHE_SIZE = 0x301, /**< The size of L2 cache in bytes. */
    CN_DEVICE_ATTRIBUTE_NRAM_SIZE_PER_CORE =
        0x302, /**< The maximum size of NRAM available per MLU Core in bytes. */
    CN_DEVICE_ATTRIBUTE_NEURAL_RAM_SIZE_PER_CORE =
        CN_DEVICE_ATTRIBUTE_NRAM_SIZE_PER_CORE, /**< @deprecated Same with
                                                   ::CN_DEVICE_ATTRIBUTE_NRAM_SIZE_PER_CORE. */
    CN_DEVICE_ATTRIBUTE_WEIGHT_RAM_SIZE_PER_CORE =
        0x303, /**< The maximum size of WRAM available per MLU Core in bytes. */
    CN_DEVICE_ATTRIBUTE_TOTAL_CONST_MEMORY_SIZE =
        0x304, /**< The memory available on device for __mlu_const__ variables in a Cambricon BANG C
                  kernel in megabytes. */
    CN_DEVICE_ATTRIBUTE_LOCAL_MEMORY_SIZE_PER_CORE =
        0x305, /**< The maximum size of local memory available per MLU Core (i.e. __ldram__) in
                  megabytes. */
    CN_DEVICE_ATTRIBUTE_MAX_SHARED_RAM_SIZE_PER_CLUSTER =
        0x306, /**< The maximum size of shared memory per cluster (i.e. __mlu_shared__) in bytes. */
    CN_DEVICE_ATTRIBUTE_GLOBAL_MEMORY_NODE_COUNT =
        0x307, /**< The number of NUMA nodes on device. */
    CN_DEVICE_ATTRIBUTE_CLUSTER_L1_CACHE_SUPPORTED = 0x308, /**< Not supported yet. */
    CN_DEVICE_ATTRIBUTE_MAX_PERSISTING_L2_CACHE_SIZE =
        0x309, /**< The maximum persisting cache size of L2 cache in bytes. */
    CN_DEVICE_ATTRIBUTE_MAX_SHARED_MEMORY_SIZE_PER_UNION_TASK = 0x30A, /**< Not supported yet. */
    CN_DEVICE_ATTRIBUTE_VIRTUAL_ADDRESS_MANAGEMENT_SUPPORTED =
        0x30B, /**< The device supports virtual address management APIs like
                  ::cnMemAddressReserve(),
                  ::cnMemCreate(), and ::cnMemMap(). */
    CN_DEVICE_ATTRIBUTE_HANDLE_TYPE_POSIX_FILE_DESCRIPTOR_SUPPORTED =
        0x30C, /**< The device supports exporting memory to a POSIX file descriptor with
                  ::cnMemExportToShareableHandle(). */
    CN_DEVICE_ATTRIBUTE_GENERIC_COMPRESSION_SUPPORTED =
        0x30D, /**< The device supports compression of memory. */
    CN_DEVICE_ATTRIBUTE_CAN_USE_HOST_POINTER_FOR_REGISTERED_MEM =
        0x30E, /**< The device can access host registered memory at the same virtual address as the
                  MLU Device. 1: The Device can access; 0: The Device cannot. */
    CN_DEVICE_ATTRIBUTE_CAN_MAP_HOST_MEMORY =
        0x30F, /**< The device can map host memory into MLU address space. 1: The Device can map
                  host memory into the MLU address space; 0: the Device cannot. */
    CN_DEVICE_ATTRIBUTE_LINEAR_MAPPING_SUPPORTED =
        0x310, /**< The device can map device memory into linear virtual address space. */
    CN_DEVICE_ATTRIBUTE_LINEAR_RECOMMEND_GRANULARITY =
        0x311, /**< The devcie can allocate linear memory with input size that is aligned with the
                  recommend granularity. */
    CN_DEVICE_ATTRIBUTE_AVAILABLE_GLOBAL_MEMORY_SIZE =
        0x312, /**< Available global memory size in megabytes. */

    CN_DEVICE_ATTRIBUTE_DMA_BUF_SUPPORTED =
        0x313, /*Device supports buffer sharing with the dma_buf mechanism.*/

    /* Hardware Properties */
    CN_DEVICE_ATTRIBUTE_ECC_ENABLED = 0x401, /**< The device has ECC support enabled. */
    CN_DEVICE_ATTRIBUTE_CLUSTER_CLOCK_RATE =
        0x402, /**< Typical cluster clock frequency in kilohertz. */
    CN_DEVICE_ATTRIBUTE_MEMORY_CLOCK_RATE = 0x403, /**< Peak memory clock frequency in kilohertz. */
    CN_DEVICE_ATTRIBUTE_GLOBAL_MEMORY_BUS_WIDTH = 0x404, /**< Global memory bus width in bits. */
    CN_DEVICE_ATTRIBUTE_GLOBAL_MEMORY_TOTAL_SIZE =
        0x405,                                   /**< Total global memory size in megabytes. */
    CN_DEVICE_ATTRIBUTE_PCI_BUS_ID = 0x406,      /**< PCI bus ID of the MLU device. */
    CN_DEVICE_ATTRIBUTE_PCI_DEVICE_ID = 0x407,   /**< PCI device ID of the MLU device. */
    CN_DEVICE_ATTRIBUTE_PCI_DOMAIN_ID = 0x408,   /**< PCI domain ID of the MLU device. */
    CN_DEVICE_ATTRIBUTE_MDR_MEMORY_SIZE = 0x409, /**< MDR memory size in megabytes. */
    CN_DEVICE_ATTRIBUTE_PCI_MPS = 0x40A,         /**< PCI maximum payload size. */
    CN_DEVICE_ATTRIBUTE_PCI_MRRS = 0x40B,        /**< PCI maximum read request size. */

    CN_DEVICE_ATTRIBUTE_MAX /**< Reserved. */
} CNdevice_attribute;

/**
 * @brief Memory types.
 */
enum CN_memory_type {
    CN_MEMORYTYPE_UNKNOWN = 0, /**< Unknown memory type. */
    CN_MEMORYTYPE_HOST = 1,    /**< Host memory. */
    CN_MEMORYTYPE_DEVICE = 2,  /**< Device memory. */
};

/**
 * @brief Host pointer cache types, which are used by CN_MEM_ATTRIBUTE_HOST_CACHE_TYPE.
 */
typedef enum CN_host_cache_type {
    CN_HOSTMEMORY_NOT_SUPPORT = -1,    /**< The current platform or driver version does not support
                                          CN_MEM_ATTRIBUTE_HOST_CACHE_TYPE. */
    CN_HOSTMEMORY_CACHE_UNKNOWN = 0x0, /**< Unknown host memory cache type. */
    CN_HOSTMEMORY_NONCACHE = 0x1,      /**< Non-cacheable host memory. */
    CN_HOSTMEMORY_CACHEABLE = 0x2,     /**< Cacheable host memory. */
} CNHostMemCachingType;

/**
 * @brief IPC memory flags, which are used by ::cnIpcOpenMemHandle.
 */
typedef enum CNipcMemFlags {
    CN_IPC_MEM_DEFAULT = 0x0,          /**< The default ipc memory flag. */
    CN_IPC_MEM_NEED_PEER_ACCESS = 0x2, /**< Need enable peer accesss between remote devices. */
} CNIpcMemFlags_t;

/**
 * @brief Memory properties.
 */
typedef enum CNmem_attribute_enum {
    CN_MEM_ATTRIBUTE_TYPE =
        0x1, /**< The ::CN_memory_type describes the physical location of a pointer. */
    CN_MEM_ATTRIBUTE_DEVICE_POINTER =
        0x2, /**< The address at which the memory of a pointer may be accessed on the device. */
    CN_MEM_ATTRIBUTE_HOST_POINTER =
        0x3, /**< The address at which the memory of a pointer may be accessed on the host. */
    CN_MEM_ATTRIBUTE_IS_CACHE_OPS =
        0x4, /**< Indicates whether the pointer points to do cache operation. */
    CN_MEM_ATTRIBUTE_CACHE_OPS =
        0x5, /**< The ::CNCacheOps describes the type of cache operation. */
    CN_MEM_ATTRIBUTE_CONTEXT =
        0x6, /**< The ::CNcontext on which a pointer is allocated or registered. */
    CN_MEM_ATTRIBUTE_DEVICE_ORDINAL =
        0x7, /**< An ordinal of a device on which a pointer is allocated or registered. */
    CN_MEM_ATTRIBUTE_RANGE_START_ADDR = 0x11, /**< Starting address for this requested pointer. */
    CN_MEM_ATTRIBUTE_RANGE_SIZE =
        0x12, /**< Size of the address range for this requested pointer. */
    CN_MEM_ATTRIBUTE_MAPPING_TYPE =
        0x13, /**< @deprecated ::CNMemoryMappingType describes the type of mapping. */
    CN_MEM_ATTRIBUTE_ACCESS_FLAGS =
        0x13, /**< The ::CNMemoryAccessFlags describes the type of mapping. */
    CN_MEM_ATTRIBUTE_CACHE_CONFIG =
        0x14, /**< The ::CNMemoryCachingType describes the cache types of MLU device. */
    CN_MEM_ATTRIBUTE_HOST_CACHE_TYPE =
        0x15, /**< The ::CNHostMemCachingType describes the cache types of host. */
    CN_MEM_ATTRIBUTE_MAPPED = 0x16, /**< Indicates that the requested pointer is mapped
                                       to an MLU physical memory. */
    CN_MEM_ATTRIBUTE_ALLOWED_HANDLE_TYPES =
        0x17, /**< A bitmask of allowed ::CNmemAllocationHandleType for this requested pointer. */
    CN_MEM_ATTRIBUTE_ISLINEAR = 0x18, /**< Indicates that the requested pointer is linear mapped to
                                         an MLU physical memory. */
    CN_MEM_ATTRIBUTE_IS_LEGACY_IPC_CAPABLE =
        0x19, /**< Indicates that the requested pointer is suitable for ::cnIpcGetMemHandle. */
    CN_MEM_ATTRIBUTE_MAX, /**< Reserved. */
} CNmem_attribute;

/**
 * @brief The status of module loading.
 */
typedef enum CNmoduleLoadingModeEnum {
    /**
     * This enum describes the current module loading mode as eager,
     * which means that the module is loaded immediately when requested.
     */
    CN_MODULE_EAGER_LOADING = 1,
    /**
     * This enum describes the current module loading mode as lazy,
     * which means that the module is loaded only when needed.
     */
    CN_MODULE_LAZY_LOADING = 2,
} CNmoduleLoadingMode;

/**
 * @brief The properties of the kernel, which are used by ::cnKernelGetAttribute.
 */
typedef enum CNkernel_attribute_enum {

    /**
     * The size in bytes of user-allocated NRAM required by this
     * kernel.
     */
    CN_KERNEL_ATTRIBUTE_NRAM_SIZE_BYTES = 0,
    /**
     * @deprecated Same with ::CN_KERNEL_ATTRIBUTE_NRAM_SIZE_BYTES.
     */
    CN_KERNEL_ATTRIBUTE_NEURAL_RAM_SIZE_BYTES = CN_KERNEL_ATTRIBUTE_NRAM_SIZE_BYTES,
    /**
     * The size in bytes of user-allocated WRAM required by this
     * kernel.
     */
    CN_KERNEL_ATTRIBUTE_WEIGHT_RAM_SIZE_BYTES = 1,
    /**
     * The size in bytes of user-allocated shared memory required by this
     * kernel.
     */
    CN_KERNEL_ATTRIBUTE_SHARED_SIZE_BYTES = 2,

    /**
     * The size in bytes of user-allocated constant memory required by this
     * kernel.
     */
    CN_KERNEL_ATTRIBUTE_CONST_SIZE_BYTES = 3,

    /**
     * The binary architecture version for which the kernel is compiled.
     * This value is the major binary version * 100 + the minor binary version,
     * and 2.1 version will return the value 201.
     */
    CN_KERNEL_ATTRIBUTE_BINARY_VERSION = 4,

    /**
     * Reserved.
     */
    CN_KERNEL_ATTRIBUTE_MAX,
} CNkernel_attribute;

/**
 * @brief The context schedule mode, which is the flag parameter passed to ::cnCtxCreate.
 */
typedef enum CNCtxSchedEnum {
    CN_CTX_SCHED_SYNC_SPIN = 0, /**< Sets spin as the current context scheduling policy. */
    CN_CTX_SCHED_SYNC_WAIT, /**< Sets wait synchronization as the current context scheduling policy.
                             */
    CN_CTX_SCHED_SYNC_YIELD, /**< Sets yield as the current context scheduling policy. */
    CN_CTX_SCHED_SYNC_AUTO,  /**< Automatic scheduling. */
    CN_CTX_SCHED_SYNC_NUM,   /**< Reserved. */
} CNCtxSched;

/**
 * @brief The context configuration type, which is used by ::cnCtxGetConfig and ::cnCtxSetConfig.
 */
typedef enum CNCtxConfigEnum {
    CN_CTX_CONFIG_RESERVED = 0,   /**< Reserved. */
    CN_CTX_CONFIG_PRINTF_MAX_NUM, /**< Sets the maximum print line number per MLU Core. The value
                                     must be larger than zero. */
    CN_CTX_CONFIG_MAX_PERSISTING_L2_CACHE_SIZE, /**< Sets the maximum persisting L2 cache size in
                                                   bytes. */
    CN_CTX_CONFIG_PRINTF_FIFO_SIZE,             /**< The MLU print line FIFO size.*/
    CN_CTX_CONFIG_STACK_SIZE,                   /**< The stack size in megabytes of each MLU core.*/
    CN_CTX_CONFIG_TYPE_NUM,                     /**< Reserved. */
} CNCtxConfig;

/**
 * @brief The status of context.
 */
typedef enum CNCtxStateEnum {
    CN_CONTEXT_STATE_INACTIVE = 0, /**< Context status is inactive. */
    CN_CONTEXT_STATE_ACTIVE,       /**< Context status is active. */
} CNCtxState;

/**
 * @brief The kernel classes, which are used by ::cnInvokeKernel.
 */
typedef enum cn_kernel_class {
    CN_KERNEL_CLASS_BLOCK = 1,    /**< Block task. One core is used. */
    CN_KERNEL_CLASS_UNION = 4,    /**< Union task. One cluster is used. */
    CN_KERNEL_CLASS_UNION2 = 8,   /**< Union2 task. Two clusters are used. */
    CN_KERNEL_CLASS_UNION4 = 16,  /**< Union4 task. Four clusters are used. */
    CN_KERNEL_CLASS_UNION8 = 32,  /**< Union8 task. Eight clusters are used. */
    CN_KERNEL_CLASS_UNION16 = 64, /**< Union16 task. Sixteen clusters are used.*/
    CN_KERNEL_CLASS_NONE = 0xFF,  /**< Reserved.*/
} KernelClass;

/**
 * @brief The context configuration types for the current thread, which are used by ::cnGetCtxConfigParam and ::cnSetCtxConfigParam.
 */
typedef enum cn_ctx_config_type {
    CN_CTX_CONFIG_VISIBLE_CLUSTER = 0, /**< Sets or gets visible cluster bitmap. */
    CN_CTX_CONFIG_VISIBLE_CLUSTER_NUM, /**< Gets visible cluster number. */
    CN_CTX_CONFIG_UNION_LIMIT,         /**< Sets or gets union type enumerated value. */
    CN_CTX_CONFIG_NUM,                 /**< Reserved.*/
} CNctxConfigParamType;

/**
 * @brief \n The context configuration parameters for the current thread, which are used by ::cnGetCtxConfigParam and ::cnSetCtxConfigParam.
 */
typedef union cn_ctx_config_param {
    cn_uint64_t visibleCluster;       /**< Visible cluster bitmap. */
    cn_uint64_t visibleClusterNumber; /**< Visible cluster number. */
    KernelClass unionLimit;           /**< Union type limitation. */
} CNctxConfigParam;

/**
 * @brief Cache operation types, which are used by ::cnCacheOperation.
 */
typedef enum cn_cache_ops {
    CN_FLUSH_CACHE = 1,   /**< Flush cache. */
    CN_INVALID_CACHE = 2, /**< Invalid cache. */
} CNCacheOps;

/**
 * @brief The atomic operation types, which are used by ::cnQueueAtomicOperation and ::cnAtomicOperation.
 */
typedef enum CNAtomicOpType {
    CN_ATOMIC_OP_REQUEST, /**< Request operation. For more information, refer to ::CNAtomicReqFlag.
                           */
    CN_ATOMIC_OP_COMPARE, /**< Compare operation. For more information, refer to ::CNAtomicCompFlag.
                           */
} CNAtomicOpType_t;

/**
 * @brief The atomic operation compare types.
 */
enum CNAtomicCompFlag {
    CN_ATOMIC_COMPARE_EQUAL = 0,  /**< Compares input data1 and opPtr until opData1 == *opPtr. */
    CN_ATOMIC_COMPARE_LESS_EQUAL, /**< Compares input data1 and opPtr until opData1 <= *opPtr. */
    CN_ATOMIC_COMPARE_LESS,       /**< Compares input data1 and opPtr until opData1 <  *opPtr. */
};

/**
 * @brief The atomic operation request type.
 */
enum CNAtomicReqFlag {
    CN_ATOMIC_REQUEST_DEFAULT =
        0, /**< Default request operation, which is the same as ::CN_ATOMIC_REQUEST_ADD. */
    CN_ATOMIC_REQUEST_ADD =
        CN_ATOMIC_REQUEST_DEFAULT, /**< Atomic add operation, which is the default operation. */
    CN_ATOMIC_REQUEST_SET,         /**< Sets operation address to input value. */
    CN_ATOMIC_REQUEST_CLEAR,       /**< Resets operation address to zero. */
};

/**
 * @brief The flags of notifier, which are used by ::cnCreateNotifier.
 */
typedef enum CNNotifierFlags {
    CN_NOTIFIER_DEFAULT = 0x0,           /**< The default notifier creation flag. */
    CN_NOTIFIER_DISABLE_TIMING_SW = 0x2, /**< The notifier will not record sw timestamp data. */
    CN_NOTIFIER_DISABLE_TIMING = CN_NOTIFIER_DISABLE_TIMING_SW, /**< @deprecated This flag is
                                                                   deprecated.*/
    CN_NOTIFIER_DISABLE_TIMING_ALL = 0x4, /**< The notifier will not record all timestamp data to
                                         reduce overhead.  */
    CN_NOTIFIER_INTERPROCESS = 0x8,       /**< The notifier is suitable for interprocess use.
                                               CN_NOTIFIER_DISABLE_TIMING_ALL must be set. */
} CNNotifierFlags_t;

/**
 * @brief The flags of notifier place, which are used by ::cnPlaceNotifierWithFlags.
 */
typedef enum CNNotifierPlaceFlags {
    CN_NOTIFIER_PLACE_DEFAULT = 0x0, /**< Default notifier place flag. */
    CN_NOTIFIER_PLACE_EXTERNAL, /**< Creates an notifier place node instead of the default behavior
                                   of ::cnPlaceNotifierWithFlags when using queue capture. This flag
                                   is invalid when used outside  of queue capture. */
} CNNotifierPlaceFlags;

/**
 * @brief The flags of notifier wait, which are used by ::cnQueueWaitNotifierWithFlags.
 */
typedef enum CNNotifierWaitFlags {
    CN_NOTIFIER_WAIT_DEFAULT = 0x0, /**< Default notifier wait flag. */
    CN_NOTIFIER_WAIT_EXTERNAL, /**< Creates an notifier wait node instead of the default behavior of
                                  ::cnQueueWaitNotifierWithFlags when using queue capture. This flag
                                  is invalid when used outside of queue capture. */
} CNNotifierWaitFlags;

#define CN_IPC_HANDLE_SIZE       8
#define CN_IPC_HANDLE_SIZE_BYTES (CN_IPC_HANDLE_SIZE * sizeof(cn_uint64_t))
/**
 * @brief IPC notifier handle.
 */
typedef struct CNIpcNotifierHandle_st {
    cn_uint64_t res[CN_IPC_HANDLE_SIZE];
    /*!< Reserved size for IPC notifier handle.*/
} CNIpcNotifierHandle_v1;
typedef struct CNqueue_st *CNqueue;
typedef struct CNnotifier_st *CNnotifier;
typedef cn_uint64_t CNaddr;
typedef struct CNipcMemHandle_st *CNIpcMemHandle;
typedef CNIpcNotifierHandle_v1 CNIpcNotifierHandle;
typedef cn_uint64_t CNdev;
typedef struct CNcontext_st *CNcontext;
typedef struct CNkernel_st *CNkernel;
typedef struct CNmodule_st *CNmodule;
typedef cn_uint64_t CNmemGenericAllocationHandle;
typedef void (*CNhostFn)(void *userData);
typedef void (*CNqueueCallback)(CNqueue hqueue, CNresult status, void *userData);
typedef struct CNmemoryPool_st *CNmemoryPool;

typedef struct CNtaskTopo_st *CNtaskTopo;             /**< TaskTopo handle. */
typedef struct CNtaskTopoNode_st *CNtaskTopoNode;     /**< TaskTopo node handle. */
typedef struct CNtaskTopoEntity_st *CNtaskTopoEntity; /**< Executable TaskTopo entity. */
typedef struct CNuserObject_st *CNuserObject;         /**< User object for TaskTopo. */

/**
 * @brief UUID(Universally Unique Identifier) of the device.
 */
typedef struct CNuuid_st {
    char uuid[16];
} CNuuid;

/**
 * @brief The offset of the address.
 */
typedef struct {
    cn_uint64_t x;
    /*!< The offset in the x direction.*/
    cn_uint64_t y;
    /*!< The offset in the y direction.*/
    cn_uint64_t z;
    /*!< The offset in the z direction.*/
} CNpos;

/**
 * @brief The pitch (the alignment) of the address. None of the parameters can be 0.
 */
typedef struct {
    cn_uint64_t pitch;
    /*!< The pitch of the memory. It cannot be less than the p->extent.width. */
    void *ptr;
    /*!< The pointer of the memory, which is the same as the p->dst.*/
    cn_uint64_t xsize;
    /*!< The memory x size, which is set to p->extent.width.
            It is not in use currently.*/
    cn_uint64_t ysize;
    /*!< The memory y size, which cannot be less than the p->extent.height.*/
} CNpitchedPtr;

/**
 * @brief The extent (the size) of the address. Unless otherwise specified,
 * none of the parameters can be 0.
 */
typedef struct {
    cn_uint64_t depth;
    /*!< The depth of the memory.*/
    cn_uint64_t height;
    /*!< The height of the memory.*/
    cn_uint64_t width;
    /*!< The width of the memory.*/
} CNextent;

/**
 * @brief The configuration parameters of 3D memcpy.
 */
typedef struct CNmemcpy3dParam_st {
    void *dst;
    /*!< The destination address.*/
    CNpos dstPos;
    /*!< The destination address position.*/
    CNpitchedPtr dstPtr;
    /*!< The pitch of the destination address.*/
    CNextent extent;
    /*!< The extent of the memory.*/
    cn_uint32_t dir;
    /*!< Reserved.*/
    void *src;
    /*!< The source address.*/
    CNpos srcPos;
    /*!< The source address position.*/
    CNpitchedPtr srcPtr;
    /*!< The pitch of the source address.*/
} CNmemcpy3dParam;

/**
 * @brief @deprecated Memory mapping type.
 */
typedef enum cn_mapping_type {
    CN_MEM_MAPPING_READWRITE = 0x0, /**< Makes the virtual address range read-write accessible. */
    CN_MEM_MAPPING_READONLY = 0x1,  /**< Makes the virtual address range read accessible. */
} CNMemoryMappingType;

/**
 * @brief Memory access flag.
 */
typedef enum cn_memory_access_flags {
    CN_MEM_ACCESS_FLAGS_PROT_READWRITE =
        0x0, /**< Makes the virtual address range read-write accessible. */
    CN_MEM_ACCESS_FLAGS_PROT_READONLY =
        0x1, /**< Makes the virtual address range read accessible. */
} CNMemoryAccessFlags;

/**
 * @brief The cache types of MLU device.
 */
typedef enum cn_caching_type {
    CN_MEM_NON_CACHEABLE = 0x0,       /**< Non-caching capability. */
    CN_MEM_CACHEABLE_READONLY = 0x1,  /**< The read caching capability. */
    CN_MEM_CACHEABLE_WRITEONLY = 0x2, /**< The write caching capability. */
    CN_MEM_CACHEABLE_READWRITE = 0x3, /**< The read-write caching capability. */
} CNMemoryCachingType;

/**
 * @brief The pinned types of device memory cache.
 */
typedef enum cn_caching_pinned {
    CN_MEM_CACHE_NON_PINNED = 0x0, /**< The non-pinned memory cache type. */
    CN_MEM_CACHE_PINNED = 0x1,     /**< The pinned memory cache type. */
} CNMemoryCachePinnedType;

/**
 * @brief The memory location types.
 */
typedef enum cn_mem_location_type {
    CN_MEM_LOCATION_TYPE_INVALID = 0x0, /**< The invalid memory location. */
    CN_MEM_LOCATION_TYPE_DEVICE = 0x1,  /**< The MLU device location. */
} CNmemLocationType;

/**
 * @brief The compression attributes of memory allocation.
 */
typedef enum cn_mem_allocation_comp_type {
    CN_MEM_ALLOCATION_COMP_NONE = 0x0,    /**< The incompressible memory allocation. */
    CN_MEM_ALLOCATION_COMP_GENERIC = 0x1, /**< The compressible memory allocation. */
} CNmemAllocationCompType;

/**
 * @brief The security attributes of memory allocation.
 */
typedef enum cn_mem_allocation_security_type {
    CN_MEM_ALLOCATION_SECUR_NONE = 0x0,    /**< Unsecure memory handle. */
    CN_MEM_ALLOCATION_SECUR_GENERIC = 0x1, /**< Secure memory handle. */
} CNmemAllocationSecurType;

/**
 * @brief The memory allocation types available.
 */
typedef enum cn_mem_allocation_type {
    CN_MEM_ALLOCATION_TYPE_INVALID = 0x0, /**< The invalid memory allocation type. */
    CN_MEM_ALLOCATION_TYPE_DEFAULT = 0x1, /**< The default memory allocation type. */
} CNmemAllocationType;

/**
 * @brief The memory allocation granularity.
 */
typedef enum cn_mem_allocation_granularity {
    CN_MEM_ALLOC_GRANULARITY_MINIMUM =
        0x0, /**< The minimum required memory allocation granularity. */
    CN_MEM_ALLOC_GRANULARITY_RECOMMENDED =
        0x1, /**< The recommended memory allocation granularity for best performance. */
} CNmemAllocationGranularity;

/**
 *  @brief The requested handle types.
 */
typedef enum cn_mem_allocation_handle_type {
    CN_MEM_HANDLE_TYPE_NONE = 0x0, /**< The handle type that does not support memory exporting. */
    CN_MEM_HANDLE_TYPE_POSIX_FILE_DESCRIPTOR =
        0x1, /**< The handle type that supports exporting memory to a POSIX file descriptor, only
                available for POSIX system. */
} CNmemAllocationHandleType;

/**
 *  @brief The memory location.
 */
typedef struct cn_mem_location {
    CNmemLocationType type; /**< The location types which define the ID of MLU device ordinal. */
    cn_uint32_t id;         /**< The ID of a given location type. If the type is
                               ::CN_MEM_LOCATION_TYPE_DEVICE, the ID means the device ordinal. */
} CNmemLocation;

/**
 * @brief The memory access descriptor.
 */
typedef struct cn_mem_access_desc {
    CNmemLocation location;             /**< The access of devices to locations. */
    CNMemoryAccessFlags accessFlags;    /**< The access flags to set on the request. */
    CNMemoryCachingType cacheType;      /**< The caching type flags to set on the request. */
    CNMemoryCachePinnedType pinnedType; /**< The pinned/non-pinned flags to set on the request. */
} CNmemAccessDesc;

/**
 *  @brief The properties of a memory allocation.
 */
typedef struct cn_mem_allocation_prop {
    CNmemAllocationType type; /**< The memory allocation type requested, which must be
                                 ::CN_MEM_ALLOCATION_TYPE_DEFAULT currently. */
    CNmemAllocationHandleType requestedHandleTypes; /**< The handle types requested. */
    CNmemLocation location;                         /**< The location of memory allocation. */
    struct {
        /**
         * Allocation hint for requesting compressible memory.
         * On devices that support Compute Data Compression, compressible memory can be used to
         * accelerate access to data. Applications are expected to query allocation properties of
         * the handle obtained using ::cnMemGetAllocationPropertiesFromHandle() to validate that the
         * obtained allocation is compressible. When ::CN_MEM_ALLOCATION_COMP_NONE is 0, the memory
         * is incompressible. When the value is ::CN_MEM_ALLOCATION_COMP_GENERIC, the memory is
         * compressible.
         */
        unsigned char compressionType;
        /**
         * Allocation hint for requesting secure memory.
         * On devices that support security feature, secure memory can only be accessed in
         * secure environment. When ::CN_MEM_ALLOCATION_SECUR_NONE is 0, the memory is insecure.
         * When the value is ::CN_MEM_ALLOCATION_SECUR_GENERIC, the memory is secure.
         */
        unsigned char securityType;
    } allocFlags;
} CNmemAllocationProp;

/**
 *  @brief The properties of allocations made from the pool.
 */
typedef struct CNmemPoolProps_st {
    CNmemAllocationType type; /**< The memory allocation type requested, which must be
                              ::CN_MEM_ALLOCATION_TYPE_DEFAULT currently. */
    CNmemAllocationHandleType requestedHandleTypes; /**< The handle types requested. */
    CNmemLocation location;                         /**< The location of memory allocation. */
    size_t maxSize; /**< Maximum pool size. When set to 0, defaults to a system dependent value. */
    unsigned char reserved[64]; /**< reserved for future use, must be 0. */
} CNmemPoolProps;

/**
 *  @brief Memory pool attributes.
 */
typedef enum CNmemPool_attribute {
    /**
     * (value type = int)
     * Allow ::cnMemAllocAsync to use memory asynchronously freed
     * in another queues as long as a queue ordering dependency
     * of the allocating queue on the free action exists.
     * Notifier interactions can create the required
     * queue ordered dependencies. (default enabled)
     */
    CN_MEMPOOL_ATTR_REUSE_FOLLOW_NOTIFIER_DEPENDENCIES = 1,
    /**
     * (value type = int)
     * Allow reuse of already completed frees when there is no dependency
     * between the free and allocation. (default enabled)
     */
    CN_MEMPOOL_ATTR_REUSE_ALLOW_OPPORTUNISTIC,
    /**
     * (value type = int)
     * Allow ::cnMemAllocAsync to insert new queue dependencies
     * in order to establish the queue ordering required to reuse
     * a piece of memory released by ::cnMemFreeAsync (default enabled).
     */
    CN_MEMPOOL_ATTR_REUSE_ALLOW_INTERNAL_DEPENDENCIES,
    /**
     * (value type = cn_int64_t)
     * Amount of reserved memory in bytes to hold onto before trying
     * to release memory back to the OS. When more than the release
     * threshold bytes of memory are held by the memory pool, the
     * allocator will try to release memory back to the OS on the
     * next call to queue, notifier or context synchronize. The default value is 0.
     */
    CN_MEMPOOL_ATTR_RELEASE_THRESHOLD,
    /**
     * (value type = cn_int64_t)
     * Amount of backing memory currently allocated for the mempool.
     */
    CN_MEMPOOL_ATTR_RESERVED_MEM_CURRENT,
    /**
     * (value type = cn_int64_t)
     * High watermark of backing memory allocated for the mempool since the
     * last time it was reset. High watermark can only be reset to zero.
     */
    CN_MEMPOOL_ATTR_RESERVED_MEM_HIGH,
    /**
     * (value type = cn_int64_t)
     * Amount of memory from the pool that is currently in use by the application.
     */
    CN_MEMPOOL_ATTR_USED_MEM_CURRENT,
    /**
     * (value type = cn_int64_t)
     * High watermark of the amount of memory from the pool that was in use by the application since
     * the last time it was reset. High watermark can only be reset to zero.
     */
    CN_MEMPOOL_ATTR_USED_MEM_HIGH,
} CNmemPoolAttribute;

/**
 * @brief The performance hint for hitProp and missProp members with ::CNaccessPolicyWindow.
 */
typedef enum CNaccessProperty_enum {
    CN_ACCESS_PROPERTY_NORMAL = 0,    /**< Normal cache persistence. */
    CN_ACCESS_PROPERTY_STREAMING = 1, /**< Streaming access is less likely to persist in cache. */
    CN_ACCESS_PROPERTY_PERSISTING = 2 /**< Persisting access is more likely to persist in cache.*/
} CNaccessProperty;

/**
 * @brief An access policy for a window.
 *
 * A contiguous extent memory begins at @p baseAddr and ends at @p baseAddr + @p numBytes.
 * The regions and ratio specifications are fitted to the capabilities of the architecture.
 */
typedef struct CNaccessPolicyWindow_st {
    /**
     * Starting address of the access policy window. MLU driver may align it.
     */
    CNaddr baseAddr;
    /**
     * Size in bytes of the access policy window.
     * MLU driver may restrict the maximum size and alignment.
     */
    size_t numBytes;
    /**
     * hitRatio specifies percentage of cache lines assigned according to hitProp, and the rest are
     * assigned missProp. The valid range is [0, 1.0].
     */
    float hitRatio;
    /**
     * The access property set for cache hit in ::CNaccessProperty.
     * Accesses in cache hit follow the hitProp access policy.
     */
    CNaccessProperty hitProp;
    /**
     * The access property set for cache miss in ::CNaccessProperty.
     * It must be either ::CN_ACCESS_PROPERTY_NORMAL or ::CN_ACCESS_PROPERTY_STREAMING.
     * Accesses in cache miss follow the missProp access policy.
     */
    CNaccessProperty missProp;
} CNaccessPolicyWindow;

/**
 * @brief Information for TCDP Kernel attribute.
 *
 */
typedef struct CNTCDPKernelInfo_st {
    /**
     * The task handle that has been created before.
     */
    cn_uint64_t handle;
    /**
     * The task bitmap created by the upon execution.
     */
    cn_uint32_t mask;
    /**
     * The extra flag for task.
     */
    cn_uint32_t flag;
} CNTCDPKernelInfo;

/**
 * @brief Invoke attributes enum.
 */
typedef enum CNinvokeAttributeID_enum {
    CN_INVOKE_ATTRIBUTE_EMPTY = 0, /**< Empty attribute entry. */
    CN_INVOKE_ATTRIBUTE_ACCESS_POLICY_WINDOW =
        1, /**< Valid for queues and task topo nodes. Identifier for ::CNaccessPolicyWindow. */
    CN_INVOKE_ATTRIBUTE_TCDP_HANDLE_INFO =
        2, /**< Valid for task topo nodes. Identifier for ::CNTCDPKernelInfo. */
    CN_INVOKE_ATTRIBUTE_KERNEL_PRELOAD =
        3, /**< Valid for invoke. Setting ::CNinvokeAttrValue::kernelPreloadAllowed to
            * non-0 signals that the kernel will use programmatic means to resolve its queue
            * dependency, so that the runtime should opportunistically allow the kernel execution to
            * overlap with the previous kernel in the queue, if that kernel requests the overlap.
            * The dependent launches can choose to wait on the dependency using the programmatic
            * sync. this flag will opportunistically execute the secondary kernel right after the
            * primary kernel execute.
            */
    CN_INVOKE_ATTRIBUTE_PROGRAMMATIC_QUEUE_SERIALIZATION =
        4, /**< Valid for invoke.
            * Setting ::CNinvokeAttrValue::programmaticQueueSerializationAllowed to non-0
            * signals to enable this function, the method of wait on the dependency kernel is same
            * as CN_INVOKE_ATTRIBUTE_KERNEL_PRELOAD, but this flag only supports secondary kernels
            * overlap, which will execute when the primary kernel finish.
            */
} CNinvokeAttributeID;

/**
 * @brief The invoke attribute union.
 */
typedef union CNinvokeAttrValue_union {
    char pad[64];                            /**< The padding. */
    CNaccessPolicyWindow accessPolicyWindow; /**< Invoke attribute value for
                                                ::CN_INVOKE_ATTRIBUTE_ACCESS_POLICY_WINDOW. */
    CNTCDPKernelInfo tcdpKernelInfo;         /**< TCDP information attribute value for
                                                ::CN_INVOKE_ATTRIBUTE_TCDP_HANDLE_INFO. */
    int kernelPreloadAllowed; /**< Value of invoke attribute ::CN_INVOKE_ATTRIBUTE_KERNEL_PRELOAD.
                               */
    int programmaticQueueSerializationAllowed; /**< Value of invoke
                                                  attribute
                                                  ::CN_INVOKE_ATTRIBUTE_PROGRAMMATIC_QUEUE_SERIALIZATION.
                                                */
} CNinvokeAttrValue;

/**
 * @brief The queue attribute id.
 */
typedef CNinvokeAttributeID CNqueueAttrID;
#define CN_QUEUE_ATTRIBUTE_ACCESS_POLICY_WINDOW CN_INVOKE_ATTRIBUTE_ACCESS_POLICY_WINDOW

/**
 * @brief The queue attribute value.
 */
typedef CNinvokeAttrValue CNqueueAttrValue;

/**
 * @brief The kernel node attribute id.
 */
typedef CNinvokeAttributeID CNkernelNodeAttrID;
#define CN_KERNEL_NODE_ATTRIBUTE_ACCESS_POLICY_WINDOW CN_INVOKE_ATTRIBUTE_ACCESS_POLICY_WINDOW
#define CN_KERNEL_NODE_ATTRIBUTE_TCDP_HANDLE_INFO     CN_INVOKE_ATTRIBUTE_TCDP_HANDLE_INFO

/**
 * @brief The kernel node attribute value.
 */
typedef CNinvokeAttrValue CNkernelNodeAttrValue;

/**
 * @brief Host node parameters.
 */
typedef struct CN_HOST_NODE_PARAMS_st {
    CNhostFn fn;    /**< The function to call when the node is being executed. */
    void *userData; /**< The argument to be passed to the function. */
} CN_HOST_NODE_PARAMS;

/**
 * @brief Kernel node parameters.
 */
typedef struct CN_KERNEL_NODE_PARAMS_st {
    CNkernel hkernel;     /**< The handle of kernel to launch. */
    unsigned int dimx;    /**< The dimension of x. */
    unsigned int dimy;    /**< The dimension of y. */
    unsigned int dimz;    /**< The dimension of z. */
    KernelClass c;        /**< The union type of kernel. */
    unsigned int reserve; /**< Reserved parameter. */
    void **kernelParams;  /**< The array of pointers to kernel parameters. */
    void **extra;         /**< Extra options, such as packaged parameters. */
} CN_KERNEL_NODE_PARAMS;

/**
 * @brief Memset node parameters.
 */
typedef struct CN_MEMSET_NODE_PARAMS_st {
    /**
     * Destination device pointer.
     */
    CNaddr dst;
    /**
     * Pitch of destination device pointer, which will be ignored if the height is 1.
     */
    size_t pitch;
    /**
     * The value to be set.
     */
    unsigned int value;
    /**
     * The size of each element in bytes, which must be 1, 2, or 4.
     */
    unsigned int elementSize;
    /**
     * The width of the row in elements.
     */
    size_t width;
    /**
     * The number of rows.
     */
    size_t height;
} CN_MEMSET_NODE_PARAMS;

/**
 * @brief TaskTopo node types.
 */
typedef enum CNtaskTopoNodeType_enum {
    CN_TASK_TOPO_NODE_TYPE_EMPTY = 0,          /**< Empty node. */
    CN_TASK_TOPO_NODE_TYPE_KERNEL = 1,         /**< Kernel node. */
    CN_TASK_TOPO_NODE_TYPE_HOST = 2,           /**< Host function node. */
    CN_TASK_TOPO_NODE_TYPE_MEMCPY = 3,         /**< Memory copy node. */
    CN_TASK_TOPO_NODE_TYPE_MEMSET = 4,         /**< Memory set node. */
    CN_TASK_TOPO_NODE_TYPE_TASK_TOPO = 5,      /**< Child TaskTopo node. */
    CN_TASK_TOPO_NODE_TYPE_NOTIFIER_PLACE = 6, /**< External notifier place node. */
    CN_TASK_TOPO_NODE_TYPE_NOTIFIER_WAIT = 7,  /**< External notifier wait node. */
    CN_TASK_TOPO_NODE_TYPE_NUM,                /**< Reserved. */
} CNtaskTopoNodeType;

/**
 * The additional write flags to create DOT file.
 */
typedef enum CNtaskTopoDebugDot_flags_enum {
    /**
     * Outputs all debug data as if every debug flag is enabled.
     */
    CN_TASK_TOPO_DEBUG_DOT_FLAGS_VERBOSE = (1 << 0),
    /**
     * Uses runtime structures for output.
     */
    CN_TASK_TOPO_DEBUG_DOT_FLAGS_RUNTIME_TYPES = (1 << 1),
    /**
     * Adds handles to output.
     */
    CN_TASK_TOPO_DEBUG_DOT_FLAGS_HANDLES = (1 << 2),
    /**
     * Adds CN_KERNEL_NODE_PARAMS values to output.
     */
    CN_TASK_TOPO_DEBUG_DOT_FLAGS_KERNEL_NODE_PARAMS = (1 << 3),
    /**
     * Adds CNmemcpy3dParam values to output.
     */
    CN_TASK_TOPO_DEBUG_DOT_FLAGS_MEMCPY_NODE_PARAMS = (1 << 4),
    /**
     * Adds CN_MEMSET_NODE_PARAMS values to output.
     */
    CN_TASK_TOPO_DEBUG_DOT_FLAGS_MEMSET_NODE_PARAMS = (1 << 5),
    /**
     * Adds CN_HOST_NODE_PARAMS values to output.
     */
    CN_TASK_TOPO_DEBUG_DOT_FLAGS_HOST_NODE_PARAMS = (1 << 6),
    /**
     * Adds CNkernelNodeAttrValue values to output.
     */
    CN_TASK_TOPO_DEBUG_DOT_FLAGS_KERNEL_NODE_ATTRIBUTES = (1 << 7),
    /**
     * Adds CNNotifier handle values to output.
     */
    CN_TASK_TOPO_DEBUG_DOT_FLAGS_NOTIFIER_NODE_PARAMS = (1 << 8)
} CNtaskTopoDebugDotFlags;

/**
 * @brief Flags for acquiring user object references for TaskTopo.
 */
typedef enum CNuserObjectAcquire_flags_enum {
    /**
     * Transfers references from the caller rather than creating new references.
     */
    CN_TASK_TOPO_USER_OBJECT_MOVE = 1,
} CNuserObjectAcquireFlags;

/**
 * @brief TaskTopo entity update error result.
 */
typedef enum CNtaskTopoEntityUpdateResult_enum {
    /**
     * The update succeeds.
     */
    CN_TASK_TOPO_ENTITY_UPDATE_SUCCESS = 0x0,
    /**
     * The update fails for an unexpected reason.
     */
    CN_TASK_TOPO_ENTITY_UPDATE_ERROR = 0x1,
    /**
     * The update fails because the topology changed.
     */
    CN_TASK_TOPO_ENTITY_UPDATE_ERROR_TOPOLOGY_CHANGED = 0x2,
    /**
     * The update fails because a node type changed.
     */
    CN_TASK_TOPO_ENTITY_UPDATE_ERROR_NODE_TYPE_CHANGED = 0x3,
    /**
     * The update fails because the parameters changed in a way that is not supported.
     */
    CN_TASK_TOPO_ENTITY_UPDATE_ERROR_PARAMETERS_CHANGED = 0x4,
    /**
     * The update fails because something about the node is not supported.
     */
    CN_TASK_TOPO_ENTITY_UPDATE_ERROR_NOT_SUPPORTED = 0x5,
    /**
     * The update fails because the function of a kernel node changed in an unsupported way.
     */
    CN_TASK_TOPO_ENTITY_UPDATE_ERROR_UNSUPPORTED_FUNCTION_CHANGE = 0x6,
    /**
     * The update fails because the node attributes changed in a way that is not supported.
     */
    CN_TASK_TOPO_ENTITY_UPDATE_ERROR_ATTRIBUTES_CHANGED = 0x7
} CNtaskTopoEntityUpdateResult;

/**
 * @brief Queue capturing status.
 */
typedef enum CNqueueCaptureStatus_enum {
    CN_QUEUE_CAPTURE_STATUS_NONE = 0,        /**< Queue is not capturing. */
    CN_QUEUE_CAPTURE_STATUS_ACTIVE = 1,      /**< Queue is actively capturing. */
    CN_QUEUE_CAPTURE_STATUS_INVALIDATED = 2, /**< Queue is partly capturing sequence that
                                                  has been invalidated, but not terminated. */
} CNqueueCaptureStatus;

/**
 * @brief Queue capture modes.
 *
 * When a queue is capturing, it may affect potentially unsafe APIs.
 *
 * The potentially unsafe APIs refer to memory allocation and queue synchronization related APIs,
 * such as ::cnQueueSync(), ::cnQueryQueue(), ::cnMalloc() and so on. They may cause unexpected
 * results when any queue is capturing.
 */
typedef enum CNqueueCaptureMode_enum {
    /**
     * If any queue is actively capturing under ::CN_QUEUE_CAPTURE_MODE_GLOBAL mode,
     * all the potentially unsafe APIs are prohibited from calling.
     */
    CN_QUEUE_CAPTURE_MODE_GLOBAL = 0,
    /**
     * If any queue is actively capturing under ::CN_QUEUE_CAPTURE_MODE_THREAD_LOCAL mode,
     * all the potentially unsafe APIs in local thread will be prohibited from calling.
     */
    CN_QUEUE_CAPTURE_MODE_THREAD_LOCAL = 1,
    /**
     * If there are only queue captures activated under ::CN_QUEUE_CAPTURE_MODE_RELAXED mode,
     * no potentially unsafe APIs will be prohibited from calling.
     */
    CN_QUEUE_CAPTURE_MODE_RELAXED = 2,
} CNqueueCaptureMode;

/**
 * @brief Flags for ::cnQueueUpdateCaptureDependencies().
 */
typedef enum CNqueueUpdateCaptureDependenciesFlags_enum {
    CN_QUEUE_ADD_CAPTURE_DEPENDENCIES = 0, /**< Adds new nodes to the dependency set. */
    CN_QUEUE_SET_CAPTURE_DEPENDENCIES = 1, /**< Replaces dependency set with new nodes. */
} CNqueueUpdateCaptureDependenciesFlags;

/**
 * @brief Invoke attribute
 */
typedef struct {
    CNinvokeAttributeID id;  /**< Attribute to set. */
    CNinvokeAttrValue value; /**< Value of the attribute. */
} CNinvokeAttribute;

/**
 * @brief Extensible invoke configuration
 */
typedef struct {
    unsigned int dimx;     /**< The dimesion of x. */
    unsigned int dimy;     /**< The dimesion of y. */
    unsigned int dimz;     /**< The dimesion of z. */
    KernelClass c;         /**< The union type of kernel. */
    unsigned int reserved; /**< Reserved parameter. */
    CNqueue hqueue;        /**< The handle of a queue. */
    CNinvokeAttribute
        *attrs; /**< List of attributes. The value is null, if ::CNinvokeConfig::numAttrs is 0. */
    unsigned int numAttrs; /**< Number of attributes populated in ::CNinvokeConfig::attrs */
} CNinvokeConfig;

/**
 *  Default queue type
 */
#define CN_QUEUE_TYPE_PER_THREAD ((CNqueue)1)
#define CN_QUEUE_TYPE_LEGACY     ((CNqueue)2)

/**
 * @brief Handle types for an address range, which define the type and size of
 * the handle output parameter.
 */
typedef enum cn_mem_range_handle_type {
    CN_MEM_RANGE_HANDLE_TYPE_DMA_BUF_FD = 0x1, /**< The dma_buf handle type. */
    CN_MEM_RANGE_HANDLE_TYPE_MAX = 0x7FFFFFFF, /**< The max handle type. */
} CNmemRangeHandleType;

/*************************************************************************
 * Error Handling Management
 *************************************************************************/
// Group: Error Handling Management
/**
 * @brief Gets the string description of an error code.
 *
 * Sets ``*pStr`` to the address of a NULL-terminated string description
 * of the error code. If the error code is not recognized, ``CN_SUCCESS``
 * will be returned and ``*pStr`` will return
 * "This error code is not in the scope of the CNresult enumeration".
 *
 * @param[in] error Error code to convert to string.
 * @param[out] pStr Address of the string pointer.
 * @return
 * ::CN_SUCCESS
 */
__CN_EXPORT extern CNresult cnGetErrorString(CNresult error, const char **pStr);
// Group: Error Handling Management
/**
 * @brief Gets the string representation of an error code enum name.
 *
 * Gets the NULL-terminated string representation
 * of an error code enum name. Returns the error code enum name in @p pStr.
 * "This error code is not in the scope of the CNresult enumeration".
 *
 * @param[in] error Error code to convert to string.
 * @param[out] pStr Address of the string pointer.
 * @return
 * ::CN_SUCCESS
 */
__CN_EXPORT extern CNresult cnGetErrorName(CNresult error, const char **pStr);

/*************************************************************************
 * Initialization Management
 *************************************************************************/
// Group: Initialization Management
/**
 * @brief Initializes CNDrv environment in the current process space.
 *
 * If ::cnInit() has not been called, execution of any CNDrv API
 * returns CN_ERROR_NOT_INITIALIZED. Currently, the @p flags parameter must be 0.
 * If cross-generation MLU devices exist among the visible MLU devices,
 * their major computing capabilities are different, and ::CN_ERROR_COMPAT_NOT_SUPPORT_ON_SYSTEM is
 * returned.
 *
 * @param[in] flags Initialization flag for CNDrv.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_NO_DEVICE,
 * ::CN_ERROR_SYSTEM_DRIVER_MISMATCH,
 * ::CN_ERROR_COMPAT_NOT_SUPPORT_ON_SYSTEM
 *
 * @note
 * - After calling ::cnInit(), it is forbidden to call system function
 * fork(); otherwise, an unknown error will occur.
 */
__CN_EXPORT extern CNresult cnInit(unsigned int flags);

/******************************************************************************
 * Version Management
 ******************************************************************************/
// Group: Version Management
/**
 * @brief Returns the latest CNDrv version supported by MLU driver.
 *
 * Returns the major version in ``major``, minor version in ``minor``,
 * and patch version in ``patch`` of CNDrv supported by the driver.
 *
 * @param[out] major The major version of CNDrv supported by the driver.
 * @param[out] minor The minor version of CNDrv supported by the driver.
 * @param[out] patch The patch version of CNDrv supported by the driver.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE
 */
__CN_EXPORT extern CNresult cnDriverGetVersion(int *major, int *minor, int *patch);

// Group: Version Management
/**
 * @brief Returns the CNDrv version.
 *
 * Returns the major version in ``major``, minor version in ``minor``,
 * 	and patch version in ``patch`` of the current CNDrv.
 *
 * @param[out] major The CNDrv major version.
 * @param[out] minor The CNDrv minor version.
 * @param[out] patch The CNDrv patch version.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE
 */
__CN_EXPORT extern CNresult cnGetLibVersion(int *major, int *minor, int *patch);

// Group: Version Management
/**
 * @brief Returns the version of the MLU driver.
 *
 * Returns the major version in ``major``, minor version in ``minor``,
 *          and patch version in ``patch`` of the MLU driver.
 *
 * @param[out] major The driver major version.
 * @param[out] minor The driver minor version.
 * @param[out] patch The driver patch version.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE
 */
__CN_EXPORT extern CNresult cnGetDriverVersion(int *major, int *minor, int *patch);

/******************************************************************************
 * Device Management
 ******************************************************************************/
// Group: Device Management
/**
 * @brief Returns a handle that is descriptor of MLU device.
 *
 * Returns in ``*pdev`` a device handle given an ordinal in the range [0,
 * ::cnDeviceGetCount()-1].
 *
 * @param[out] pdev The device handle.
 * @param[in]  ordinal The device number to get handle for.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_INVALID_DEVICE
 */
__CN_EXPORT extern CNresult cnDeviceGet(CNdev *pdev, int ordinal);
// Group: Device Management
/**
 * @brief Returns the number of compute-capable MLU devices in the current system.
 *
 * If no compute-capable MLU device is available, cnDeviceGetCount() returns 0.
 *
 * @param[out] pcount The number of compute-capable MLU devices.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_INVALID_DEVICE
 */
__CN_EXPORT extern CNresult cnDeviceGetCount(int *pcount);

// Group: Device Management
/**
 * @brief Returns an identifier string for the MLU device.
 *
 * Returns an ASCII string identifying the MLU device in the NULL-terminated
 * string pointed to by an MLU name.
 *
 * @param[out] name The identifier string for the MLU device.
 * @param[in]  len  The maximum length of string to store in name.
 * @param[in]  dev  The device handle.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_INVALID_DEVICE
 *
 * @note
 * - The output is just an identifier string. Do not parse this information.
 * - @p len is maximum length of string to store. ::CN_ERROR_INVALID_VALUE will be returned
 *   when device name length is greater than @p len .
 */
__CN_EXPORT extern CNresult cnDeviceGetName(char *name, unsigned int len, CNdev dev);
// Group: Device Management
/**
 * @brief Returns in ``*pbytes`` the total amount of memory available on the device
 * ``dev`` in bytes.
 *
 * @param[out] pbytes The memory available on MLU device in bytes.
 * @param[in] dev The device handle.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_INVALID_DEVICE
 */
__CN_EXPORT extern CNresult cnDeviceTotalMem(cn_uint64_t *pbytes, CNdev dev);
// Group: Device Management
/**
 * @brief Returns information about the MLU device.
 *
 * Returns the integer value of the attribute on device.
 * See ::CNdevice_attribute for supported attributes.
 *
 * @param[out] pi The device attribute value.
 * @param[in]  attrib The device attribute type to query.
 * @param[in]  dev The device handle.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_INVALID_DEVICE,
 * ::CN_OPS_ERROR_NOT_SUPPORTED
 */
__CN_EXPORT extern CNresult cnDeviceGetAttribute(int *pi, CNdevice_attribute attrib, CNdev dev);
// Group: Device Management
/**
 * @brief Returns a handle to the device.
 *
 * Returns in `*dev`  a device handle by giving a PCI bus ID string.
 * PCI bus ID string is in one of the following forms:
 * [domain]:[bus]:[Device].[function]
 * [domain]:[bus]:[Device]
 * [bus]:[Device].[function] where domain, bus, device, and function are all hexadecimal values.
 *
 * @param[out] dev     The device handle.
 * @param[in]  pciBusId PCI bus ID string.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_INVALID_DEVICE
 */
__CN_EXPORT extern CNresult cnDeviceGetByPCIBusId(CNdev *dev, const char *pciBusId);
// Group: Device Management
/**
 * @brief Returns a PCI bus ID string for the device.
 *
 * Returns an ASCII string identifying the device in the NULL-terminated
 * string pointed to by @p pciBusId. @p len specifies the maximum length of the
 * string that may be returned. @p pciBusId is a string for the device in the following format
 * [domain]:[bus]:[Device].[function] where domain, bus, device, and function are all hexadecimal
 * values. @p pciBusId should be large enough to store 13 characters including the NULL-terminator.
 *
 * @param[out] pciBusId PCI bus ID string.
 * @param[in]  len       The maximum length of string to store in @p pciBusId.
 * @param[in]  dev       The device handle.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_INVALID_DEVICE
 */
__CN_EXPORT extern CNresult cnDeviceGetPCIBusId(char *pciBusId, int len, CNdev dev);
// Group: Device Management
/**
 * @brief Returns an UUID for the device.
 *
 * Returns a 16-octet @p uuid to identify the device @p dev.
 *
 * @param[out] uuid uuid The 16-octet UUID.
 * @param[in]  dev  The device handle.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_INVALID_DEVICE
 */
__CN_EXPORT extern CNresult cnDeviceGetUuid(CNuuid *uuid, CNdev dev);
// Group: Device Management
/**
 * @brief Returns a 36-byte UUID string for the device.
 *
 * Returns a 36 bytes @p uuid_str to identify the device @p dev.
 *
 * @param[out] uuid_str The 36-byte UUID string.
 * @param[in]  len      The maximum length of string to store in @p uuid_str.
 * @param[in]  dev      The device handle.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_INVALID_DEVICE
 */
__CN_EXPORT extern CNresult cnDeviceGetUuidStr(char *uuid_str, int len, CNdev dev);
// Group: Device Management
/**
 * @brief Returns a handle to the device.
 *
 * Returns the device handle @p dev through the 36-byte UUID string,
 * which is obtained through ::cnDeviceGetUuidStr() or CNDEV APIs.
 *
 * @param[out] dev      The device handle.
 * @param[in]  uuid_str UUID string.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_INVALID_DEVICE
 */
__CN_EXPORT extern CNresult cnDeviceGetByUuidStr(CNdev *dev, const char *uuid_str);
// Group: Device Management
/**
 * @brief Returns the default mempool of a device.
 *
 * The default mempool of a device contains device memory from the device.
 *
 * @param[out] pool      The memory pool.
 * @param[in]  dev       The device.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_INVALID_DEVICE
 */
__CN_EXPORT extern CNresult cnDeviceGetDefaultMemPool(CNmemoryPool *pool, CNdev dev);
// Group: Device Management
/**
 * @brief Gets the current mempool for a device.
 *
 * Returns the last pool provided to ::cnDeviceSetMemPool for this device or the device's
 * default memory pool if ::cnDeviceSetMemPool has never been called. By default the current
 * mempool is the default mempool for a device. Otherwise the returned pool must have been set
 * with ::cnDeviceSetMemPool.
 *
 * @param[out] pool      The memory pool.
 * @param[in]  dev       The device.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_INVALID_DEVICE
 */
__CN_EXPORT extern CNresult cnDeviceGetMemPool(CNmemoryPool *pool, CNdev dev);
// Group: Device Management
/**
 * @brief Sets the current mempool for a device.
 *
 * The memory pool must be local to the specified device. ::cnMemAllocAsync allocates
 * from the current mempool of the provided queue's device. By default, a device's
 * current memory pool is its default memory pool.
 *
 * @param[in] pool      The memory pool.
 * @param[in]  dev       The device.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_INVALID_DEVICE
 */
__CN_EXPORT extern CNresult cnDeviceSetMemPool(CNdev dev, CNmemoryPool pool);

/******************************************************************************
 * Context Management
 ******************************************************************************/
// Group: Context Management
/**
 * @brief Creates a new context.
 *
 * Creates a new CNDrv context and associates it with the caller.
 *
 * The flags of input parameters determine the context scheduler patterns, which include:
 * - CN_CTX_SCHED_SYNC_SPIN: Allows to spin actively when waiting for results from device. This
 * pattern decreases latency but may lower the performance of CPU threads and high CPU usage.
 * - CN_CTX_SCHED_SYNC_WAIT: The thread is blocked with low CPU usage, and enters sleep mode.
 * - CN_CTX_SCHED_SYNC_YIELD: Allows to yield the current thread when waiting for results from
 * device. This pattern may increase latency but increase the performance of CPU threads.
 * - CN_CTX_SCHED_SYNC_AUTO: Automatic scheduling. According to the device number and the number of
 * processors in the system, choose the mode CN_CTX_SCHED_SYNC_SPIN or CN_CTX_SCHED_SYNC_WAIT.
 *
 * You can manage the lifecycle of context. Call ::cnCtxDestroy() to destroy the context.
 *
 * @param[out] pcontext The handle of the new context.
 * @param[in]  flags Context creation flags.
 * @param[in]  dev The device to create context on.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_INVALID_DEVICE,
 * ::CN_CONTEXT_ERROR_OUT_OF_RESOURCES,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY
 */
__CN_EXPORT extern CNresult cnCtxCreate(CNcontext *pcontext, unsigned int flags, CNdev dev);
// Group: Context Management
/**
 * @brief Destroys a context.
 *
 * Destroys the CNDrv context specified by context. The context will be
 * destroyed regardless of how many threads there are currently.
 * The calling API shall ensure that no API
 * call issues occur using context while ::cnCtxDestroy() is executing.
 *
 * @param[in] context The context handle created by calling ::cnCtxCreate().
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_CONTEXT_ERROR_INVALID
 */
__CN_EXPORT extern CNresult cnCtxDestroy(CNcontext context);
// Group: Context Management
/**
 * @brief Returns the flags of the current context.
 *
 * Gets the schedule flags of the current context. See ::cnCtxCreate()
 * for flag values.
 *
 * @param[out] pflags The context schedule flag.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID
 */
__CN_EXPORT extern CNresult cnCtxGetFlags(unsigned int *pflags);
// Group: Context Management
/**
 * @brief Gets context API version.
 *
 * Returns a version number corresponding to the capabilities of
 * the context (for example, 1020 or 1030) that you can use to direct
 * them to a specific API version. If context is NULL, returns the API version
 * used to create the currently bound context.
 *
 * @param[in] context Context handle created by calling ::cnCtxCreate().
 * @param[out] pversion The context API version.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID
 */
__CN_EXPORT extern CNresult cnCtxGetApiVersion(CNcontext context, unsigned int *pversion);
// Group: Context Management
/**
 * @brief Returns the context bound to the calling CPU thread.
 *
 * If no context is bound to the calling CPU thread, then @p context is
 * set to NULL and ::CN_SUCCESS is returned.
 *
 * @param[out] pcontext The context handle of the current thread.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE
 */
__CN_EXPORT extern CNresult cnCtxGetCurrent(CNcontext *pcontext);
// Group: Context Management
/**
 * @brief Binds the specified context to the calling CPU thread.
 *
 * If @p context is NULL, then the context previously bound to the
 * calling CPU thread is unbound and CN_SUCCESS is returned.
 *
 * @param[in] context The context handle which will be set to the current thread.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_CONTEXT_ERROR_INVALID
 */
__CN_EXPORT extern CNresult cnCtxSetCurrent(CNcontext context);
// Group: Context Management
/**
 * @brief Returns the device handle of the current context.
 *
 * Each device has several contexts and a context belongs to a
 * device. So the device handle that context of the current thread belongs to
 * will be returned.
 *
 * @param[out] pdev The handle of the current context's device.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID
 */
__CN_EXPORT extern CNresult cnCtxGetDevice(CNdev *pdev);
// Group: Context Management
/**
 * @brief Gets queue priority range in the current context.
 *
 * If you attempt to create a queue with a priority value that is
 * outside the meaningful range as specified by this API, the priority is
 * automatically clamped down or up to either *pminPriority or *pmaxPriority
 * respectively. See ::cnCreateQueueWithPriority() for details on creating a
 * priority queue.
 *
 * @param[out] pminPriority The minimum priority of queue in the current context.
 * @param[out] pmaxPriority The maximum Priority of queue in the current context.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID
 */
__CN_EXPORT extern CNresult cnCtxGetQueuePriorityRange(int *pminPriority, int *pmaxPriority);
// Group: Context Management
/**
 * @brief Gets the configuration value of the current context according to type.
 *
 * Returns the current context configuration by @p pconfig
 * according to the configuration type.
 *
 * @param[out] pconfig Configuration in the current context.
 * @param[in]  type The type of configuration.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID
 */
__CN_EXPORT extern CNresult cnCtxGetConfig(long long int *pconfig, CNCtxConfig type);
// Group: Context Management
/**
 * @brief Sets the current context configuration
 * according to the configuration type. @p pconfig can be limited to the minimum and maximum range,
 * and aligned with the nearest element size. Use ::cnCtxGetConfig() to get the exact configuration
 * value.
 *
 * @param[in] type Configuration type in the current context.
 * @param[in] config Configuration in the current context.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID
 */
__CN_EXPORT extern CNresult cnCtxSetConfig(CNCtxConfig type, long long int config);
// Group: Context Management
/**
 * @brief Sets per-context configuration parameters.
 *
 * Sets context configuration of the current thread
 * according to the configuration type. Configurations are independent in each thread and context.
 *
 * @param[in] context The context handle created by calling ::cnCtxCreate().
 * @param[in] type The configuration parameter type ::CNctxConfigParamType.
 * @param[in] param The value of configuration parameter ::CNctxConfigParam.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID
 */
__CN_EXPORT extern CNresult cnSetCtxConfigParam(CNcontext context, CNctxConfigParamType type,
                                                CNctxConfigParam *param);
// Group: Context Management
/**
 * @brief Gets per-context configuration parameters.
 *
 * Gets context configuration of the current thread
 * according to the configuration type. Configurations are independent in each thread and context.
 * Default configuration parameters will be given if not set before.
 *
 * @param[in] context The context handle created by calling ::cnCtxCreate().
 * @param[in] type The configuration parameter type ::CNctxConfigParamType.
 * @param[out] param The return value of configuration parameter ::CNctxConfigParam.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID
 */
__CN_EXPORT extern CNresult cnGetCtxConfigParam(CNcontext context, CNctxConfigParamType type,
                                                CNctxConfigParam *param);
// Group: Context Management
/**
 * @brief Gets the maximum number of parallel jobs according to the visible cluster and union type (for example, kernel class).
 *
 * Gets the maximum number of parallel jobs of specific union type.
 * Parallel job number means the number of jobs which can be executed simultaneously
 * under specific visible cluster and union type.
 *
 * @param[in] context The context handle created by calling ::cnCtxCreate().
 * @param[in] c The union type of kernel.
 * @param[out] number The maximum number of parallel jobs.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID
 */
__CN_EXPORT extern CNresult cnGetCtxMaxParallelUnionTasks(CNcontext context, KernelClass c,
                                                          int *number);
// Group: Context Management
/**
 * @brief Synchronizes the current context.
 *
 * The context synchronization blocks until the device has completed all preceding requested tasks.
 * cnCtxSync() returns an error if one of the preceding tasks fails.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_CONTEXT_ERROR_INVALID
 */
__CN_EXPORT extern CNresult cnCtxSync(void);

// Group: Context Management
/**
 * @brief Resets all persisting cache lines in cache normal status.
 *
 * This interface changes all persisting cache line status to normal immediately.
 * The cache line status has been changed when it returns ::CN_SUCCESS.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_OPS_ERROR_NOT_SUPPORTED
 * ::CN_ERROR_NOT_INITIALIZED
 *
 * @note
 * - The interface does not flush cache line, but just changes the cache line status.
 */
__CN_EXPORT extern CNresult cnCtxResetPersistingL2Cache(void);

/******************************************************************************
 * Shared Context management
 ******************************************************************************/
// Group: Context Management
/**
 * @brief Gets shared context state and flag according to device.
 *
 * Returns state and flag of the shared context by input device.
 * The default flag value is CN_CTX_SCHED_SYNC_AUTO, and the state will be CN_CONTEXT_STATE_INACTIVE
 * when releasing the Shared context by calling ::cnSharedContextRelease() or
 * ::cnSharedContextReset().
 *
 * See ::cnCtxCreate() for flag values.
 *
 * The states of output parameters indicate whether the Shared context is active or not, which
 * include:
 * - CN_CONTEXT_STATE_INACTIVE: This state means the shared context is inactive after
 * ::cnSharedContextRelease() or ::cnSharedContextReset() is called.
 * - CN_CONTEXT_STATE_ACTIVE: This state means the shared context is active after
 * ::cnSharedContextAcquire() is called.
 *
 * @param[in] dev The device that shared context belongs to.
 * @param[out] flags Shared context flags.
 * @param[out] state Shared context state.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID
 */
__CN_EXPORT extern CNresult cnSharedContextGetState(CNdev dev, unsigned int *flags, int *state);
// Group: Context Management
/**
 * @brief Sets flag for shared context.
 *
 * Sets shared context flag according to device. Make sure the shared context is
 * inactive when this API is called, otherwise setting flag will fail and error code is returned.
 *
 * @param[in] dev The device that shared context belongs to.
 * @param[in] flags Shared context flags. See ::cnCtxCreate() for flags values.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID
 */
__CN_EXPORT extern CNresult cnSharedContextSetFlags(CNdev dev, unsigned int flags);
// Group: Context Management
/**
 * @brief Acquires shared context for device.
 *
 * Increases shared context reference count and returns shared context handle.
 * It will activate shared context if reference count is zero,
 * otherwise it just increases reference count.
 *
 * @param[out] pctx The shared context handle.
 * @param[in] dev The device that shared context belongs to.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_CONTEXT_ERROR_OUT_OF_RESOURCES
 */
__CN_EXPORT extern CNresult cnSharedContextAcquire(CNcontext *pctx, CNdev dev);
// Group: Context Management
/**
 * @brief Releases shared context for device.
 *
 * Decreases shared context reference count. It will inactivate shared context if reference
 * count is zero.
 *
 * @param[in] dev The device that the shared context belongs to.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID
 */
__CN_EXPORT extern CNresult cnSharedContextRelease(CNdev dev);
// Group: Context Management
/**
 * @brief Resets the shared context for device.
 *
 * Resets all states of the shared context, no matter of the reference count.
 * The shared context state will be inactive and the schedule flag is 0.
 *
 * @param[in] dev The device that the shared context belongs to.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID
 */
__CN_EXPORT extern CNresult cnSharedContextReset(CNdev dev);

/******************************************************************************
 * Memory Management
 ******************************************************************************/
// Group: Memory Management
/**
 * @brief Retrieves the memory information of the current device.
 *
 * Returns @p pfreeBytes and *ptotalBytes respectively. @p pfreeBytes and
 * @p ptotalBytes are the free memory and total memory allocated by the CNDrv
 * context respectively in bytes.
 *
 * @param[out] pfreeBytes  The free memory in bytes.
 * @param[out] ptotalBytes The total memory in bytes.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID
 */
__CN_EXPORT extern CNresult cnMemGetInfo(cn_uint64_t *pfreeBytes, cn_uint64_t *ptotalBytes);
// Group: Memory Management
/**
 * @brief Retrieves the memory information of specific node of the current device.
 *
 * Returns @p pfreeBytes and @p ptotalBytes respectively. @p pfreeBytes and
 * @p ptotalBytes are the free memory and total memory of specific node allocated
 * by the CNDrv context respectively in bytes.
 *
 * @param[out] pfreeBytes  The free memory in bytes.
 * @param[out] ptotalBytes The total memory in bytes.
 * @param[in]  node The memory node to be queried.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID
 */
__CN_EXPORT extern CNresult cnMemGetNodeInfo(cn_uint64_t *pfreeBytes, cn_uint64_t *ptotalBytes,
                                             int node);
// Group: Memory Management
/**
 * @brief Allocates global MLU memory with given bytes.
 *
 * Allocates global MLU memory in bytes on the device and returns
 * @p pmluAddr pointer to the allocated memory. The allocated memory supports
 * variables of any type. The allocated memory is not zero.
 *
 * @param[out] pmluAddr The allocated MLU memory.
 * @param[in]  bytes The number of bytes of MLU memory to be queried.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_MEMORY_ERROR_OUT_OF_PROCESS_MAX_MEMORY,
 * ::CN_MEMORY_ERROR_OUT_OF_MEMORY
 */
__CN_EXPORT extern CNresult cnMalloc(CNaddr *pmluAddr, cn_uint64_t bytes);
// Group: Memory Management
/**
 * @brief Allocates security MLU memory with given bytes.
 *
 * Allocates security MLU memory in bytes on the device and returns
 * @p pmluAddr pointer to the allocated memory. The allocated memory supports
 * variables of any type. The memory is not cleared.
 *
 * @param[out] pmluAddr The allocated MLU memory.
 * @param[in]  bytes The number of bytes of MLU memory to be allocated.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_MEMORY_ERROR_OUT_OF_PROCESS_MAX_MEMORY,
 * ::CN_MEMORY_ERROR_OUT_OF_MEMORY
 */
__CN_EXPORT extern CNresult cnMallocSecurity(CNaddr *pmluAddr, cn_uint64_t bytes);
// Group: Memory Management
/**
 * @brief Allocates global MLU memory of given node.
 *
 * Allocates global MLU memory in bytes on the device and returns
 * @p pmluAddr pointer to the allocated memory. The allocated memory supports
 * variables of any type. The memory is not cleared.
 *
 * @param[out] pmluAddr The allocated MLU memory.
 * @param[in]  bytes The number of bytes of MLU memory to be allocated.
 * @param[in]  node The node of NUMA memory. The maximum value of node can be got from
 * ::cnDeviceGetAttribute().
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_MEMORY_ERROR_OUT_OF_PROCESS_MAX_MEMORY,
 * ::CN_MEMORY_ERROR_OUT_OF_MEMORY
 */
__CN_EXPORT extern CNresult cnMallocNode(CNaddr *pmluAddr, cn_uint64_t bytes, int node);
// Group: Memory Management
/**
 * @brief Allocates MLU memory of given bytes and sets the initial memory value
 *        as 0 to avoid abnormality.
 *
 * Allocates global MLU memory in bytes on the device and returns
 * @p pmluAddr pointer to the allocated memory. The allocated memory supports
 * variables of any type. The memory is cleared.
 *
 * @param[out] pmluAddr The allocated MLU memory.
 * @param[in]  bytes The number of bytes MLU memory to be allocated.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_MEMORY_ERROR_OUT_OF_PROCESS_MAX_MEMORY,
 * ::CN_MEMORY_ERROR_OUT_OF_MEMORY
 */
__CN_EXPORT extern CNresult cnZmalloc(CNaddr *pmluAddr, cn_uint64_t bytes);
// Group: Memory Management
/**
 * @brief Allocates MLU memory of given node and sets the initial memory value
 *        as 0 to avoid abnormality.
 *
 * Allocates global MLU memory in bytes on the device and returns
 * pmluAddr pointer to the allocated memory. The allocated memory supports
 * variables of any type. The memory is cleared.
 *
 * @param[out] pmluAddr The allocated MLU memory.
 * @param[in]  bytes The number of bytes of allocated MLU memory.
 * @param[in]  node The node of NUMA memory. The maximum value of node can be got from
 * ::cnDeviceGetAttribute().
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_MEMORY_ERROR_OUT_OF_PROCESS_MAX_MEMORY,
 * ::CN_MEMORY_ERROR_OUT_OF_MEMORY
 */
__CN_EXPORT extern CNresult cnZmallocNode(CNaddr *pmluAddr, cn_uint64_t bytes, int node);
// Group: Memory Management
/**
 * @brief Allocates constant MLU memory of given bytes.
 *
 * Allocates constant MLU memory in bytes on the device and returns
 * pmluAddr pointer to the allocated memory. The allocated memory supports
 * variables of any type. The memory is not cleared.
 *
 * @param[out] pmluAddr The allocated constant MLU memory.
 * @param[in]  bytes The number of bytes of allocated constant MLU memory.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_MEMORY_ERROR_OUT_OF_PROCESS_MAX_MEMORY,
 * ::CN_MEMORY_ERROR_OUT_OF_MEMORY
 */
__CN_EXPORT extern CNresult cnMallocConstant(CNaddr *pmluAddr, cn_uint64_t bytes);
// Group: Memory Management
/**
 * @brief Allocates constant MLU memory of given node.
 *
 * Allocates constant MLU memory in bytes on the device and returns
 * pmluAddr pointer to the allocated memory. The allocated memory supports
 * variables of any type. The memory is not cleared.
 *
 * @param[out] pmluAddr The allocated constant MLU memory.
 * @param[in]  bytes The number of bytes of allocated constant MLU memory.
 * @param[in]  node The node of NUMA memory. The maximum value of node can be got from
 * ::cnDeviceGetAttribute().
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_MEMORY_ERROR_OUT_OF_PROCESS_MAX_MEMORY,
 * ::CN_MEMORY_ERROR_OUT_OF_MEMORY
 */
__CN_EXPORT extern CNresult cnMallocNodeConstant(CNaddr *pmluAddr, cn_uint64_t bytes, int node);
// Group: Memory Management
/**
 * @brief Allocates frame buffer of given bytes.
 *
 * Allocates MLU memory in bytes on the device and returns @p pmluAddr pointer to
 * the allocated memory. The allocated memory supports variables of any type.
 * The memory is not cleared, and is only used for CNCodec.
 *
 *
 * @param[out] pmluAddr The allocated frame buffer.
 * @param[in]  bytes The number of bytes of allocated frame buffer.
 * @param[in]  node The node of NUMA memory.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_MEMORY_ERROR_OUT_OF_PROCESS_MAX_MEMORY,
 * ::CN_MEMORY_ERROR_OUT_OF_MEMORY
 */
__CN_EXPORT extern CNresult cnMallocFrameBuffer(CNaddr *pmluAddr, cn_uint64_t bytes, int node);
// Group: Memory Management
/**
 * @brief Allocates peerable memory.
 *
 * Allocates bytes of peerable MLU memory on the device and returns in
 * *pmluAddr a pointer to the allocated memory. The memory is not cleared.
 *
 * @param[out] pmluAddr The allocated peerable MLU memory.
 * @param[in]  bytes The number of bytes of allocated peerable MLU memory.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_MEMORY_ERROR_OUT_OF_PROCESS_MAX_MEMORY,
 * ::CN_MEMORY_ERROR_OUT_OF_MEMORY
 */
__CN_EXPORT extern CNresult cnMallocPeerAble(CNaddr *pmluAddr, cn_uint64_t bytes);
// Group: Memory Management
/**
 * @brief Frees the MLU memory.
 *
 * Frees the memory space pointed to by @p mluAddr, which must have been returned
 * by a previous call to ::cnMalloc(), ::cnMallocNode(), ::cnZmalloc(),
 * ::cnMallocConstant(), ::cnMallocNodeConstant(), cnMallocFrameBuffer() or ::cnZmallocNode().
 *
 * @param[in] mluAddr The freed MLU address.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS
 */
__CN_EXPORT extern CNresult cnFree(CNaddr mluAddr);
// Group: Memory Management
/**
 * @brief Merges several MLU memories to an MLU memory.
 *
 * @param[out] pmergeAddr The merged memory.
 * @param[in]  count The count of MLU memory that needs to be merged.
 * @param[in]  mluAddrs The addresses of MLU memory that need to be merged.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID
 *
 * @note
 * - After merging several MLU memory to an MLU memory, the address meta of such memory will be
 *   invalid.
 * - It does not support merging device memories that are allocated by different Contexts.
 * - This interface is not supported in MLU300 series or later.
 */
__CN_EXPORT extern CNresult cnMemMerge(CNaddr *pmergeAddr, unsigned int count,
                                       const CNaddr *mluAddrs);

// Group: Memory Management
/**
 * @brief Gets MLU memory information through an input MLU address.
 *
 * Both parameters @p pbaseAddr and @p pbytes are optional. If one
 * of them is NULL, it is ignored.
 *
 * @param[out] pbaseAddr The base address.
 * @param[out] pbytes The bytes of MLU memory allocation.
 * @param[in]  mluAddr The MLU address to query.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS
 */
__CN_EXPORT extern CNresult cnMemGetAddressRange(CNaddr *pbaseAddr, cn_uint64_t *pbytes,
                                                 const CNaddr mluAddr);

// Group: Memory Management
/**
 * @brief Allocates bytes of host memory that is page-locked.
 *
 * @param[out] paddr Pointer to a host address which is allocated.
 * @param[in]  bytes The bytes count to be allocated.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY
 */
__CN_EXPORT extern CNresult cnMallocHost(void **paddr, cn_uint64_t bytes);
// Group: Memory Management
/**
 * @brief Frees page-locked host memory.
 *
 * Frees the memory space pointed to by address, which has been returned by
 * a previous call to ::cnMallocHost().
 *
 * @param[in] addr The pointer to a host address which is allocated.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID
 */
__CN_EXPORT extern CNresult cnFreeHost(void *addr);
// Group: Memory Management
/**
 * @brief Gets an interprocess memory handle for an existing device memory
 * allocation.
 *
 * Takes a pointer to the base of an existing device memory allocation created
 * with ::cnMalloc() and exports it for use in another process. This is a
 * lightweight operation and may be called multiple times on an allocation
 * without adverse effects.
 *
 * @param[out] pHandle The pointer to the user allocated
 *                     ::CNIpcMemHandle, which stores the handle.
 * @param[in]  addr The MLU address which is already allocated.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS,
 * ::CN_OPS_ERROR_NOT_SUPPORTED
 */
__CN_EXPORT extern CNresult cnIpcGetMemHandle(CNIpcMemHandle *pHandle, CNaddr addr);
// Group: Memory Management
/**
 * @brief Opens an interprocess memory handle exported from another process
 * and returns a device pointer usable in the local process.
 *
 * Maps memory exported from another process with ::cnIpcGetMemHandle() into
 * the current device address space.
 *
 * Calling ::cnFree() on an exported memory region before calling ::cnIpcCloseMemHandle() in the
 * importing context will result in undefined behavior.
 *
 * @param[out] pdptr Pointer to a shared MLU memory.
 * @param[in]  handle ::CNIpcMemHandle which is exported from another process.
 * @param[in]  flag IPC share memory flags.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_OPS_ERROR_NOT_SUPPORTED
 */
__CN_EXPORT extern CNresult cnIpcOpenMemHandle(CNaddr *pdptr, CNIpcMemHandle handle,
                                               unsigned int flag);
// Group: Memory Management
/**
 * @brief Closes memory mapped with ::cnIpcOpenMemHandle().
 *
 * Unmaps memory returned by ::cnIpcOpenMemHandle(). The original allocation
 * in the exporting process as well as imported mappings in other processes
 * will be unaffected.
 *
 * @param[in] dptr Requires an MLU memory pointer returned by ::cnIpcOpenMemHandle()
 * which needs to be closed and freed.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_OPS_ERROR_NOT_SUPPORTED,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS
 */
__CN_EXPORT extern CNresult cnIpcCloseMemHandle(CNaddr dptr);
// Group: Memory Management
/**
 * @brief Copies data from source address to destination address.
 *
 * @param[in] dstAddr The required unified virtual destination address.
 * @param[in] srcAddr The required unified virtual source address.
 * @param[in] bytes The number of bytes to be copied.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS
 *
 * @note
 * - This API infers the type of the transfer (host to host, host to
 * device, device to device, or device to host) from the pointer values. This
 * API is only allowed in Contexts which support unified addressing.
 * It does not support memory copy by different Contexts
 * which are created by the same device.
 */
__CN_EXPORT extern CNresult cnMemcpy(CNaddr dstAddr, CNaddr srcAddr, cn_uint64_t bytes);
// Group: Memory Management
/**
 * @brief Copies MLU memory between two Contexts
 *
 * Copies from MLU memory in one context to MLU memory in another
 * context.
 *
 * @param[in] dstAddr The destination device pointer.
 * @param[in] dstCtx The destination context.
 * @param[in] srcAddr The source device pointer.
 * @param[in] srcCtx The source context.
 * @param[in] bytes The size of memory copy in bytes.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS
 *
 * @note
 * - It does not support memory copy by different Contexts
 *  which are created by the same device.
 */
__CN_EXPORT extern CNresult cnMemcpyPeer(CNaddr dstAddr, CNcontext dstCtx, CNaddr srcAddr,
                                         CNcontext srcCtx, cn_uint64_t bytes);

// Group: Memory Management
/**
 * @brief Copies bytes from source host address to destination MLU address.
 *
 * @param[in] dstAddr The required destination MLU address.
 * @param[in] srcAddr The required source host address.
 * @param[in] bytes The number of bytes to be copied.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS,
 * ::CN_ERROR_INVALID_ADDRESS
 */
__CN_EXPORT extern CNresult cnMemcpyHtoD(CNaddr dstAddr, const void *srcAddr, cn_uint64_t bytes);
// Group: Memory Management
/**
 * @brief Copies bytes from source MLU address to destination host address.

 *
 * @param[in] srcAddr The required source MLU address.
 * @param[in] dstAddr The required destination host address.
 * @param[in] bytes The number of bytes to be copied.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS,
 * ::CN_ERROR_INVALID_ADDRESS
 */
__CN_EXPORT extern CNresult cnMemcpyDtoH(void *dstAddr, CNaddr srcAddr, cn_uint64_t bytes);
// Group: Memory Management
/**
 * @brief Copies memory from source MLU address to destination MLU address.
 *
 * @param[in] srcAddr The required source MLU address.
 * @param[in] dstAddr The required destination MLU address.
 * @param[in] bytes The number of bytes to be copied.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS
 *
 * @note
 * - It does not support memory copy by different Contexts
 *  which are created by the same device
 */
__CN_EXPORT extern CNresult cnMemcpyDtoD(CNaddr dstAddr, CNaddr srcAddr, cn_uint64_t bytes);
// Group: Memory Management
/**
 * @brief Copies memory from source MLU address to destination MLU address in 2D.
 *
 * Synchronously reads ``spitch * width`` bytes of data from
 * the source address pointed by ``srcAddr``, and writes it to the
 * ``dpitch * width`` bytes of destination address pointed by ``dstAddr``.
 *
 * @param[in] dstAddr Pointer to the destination address.
 * @param[in] dpitch The pitch of the destination memory. It must be
 *  greater than or equal to the width.
 * @param[in] srcAddr Pointer to the source address.
 * @param[in] spitch The pitch of the source memory. It must be
 *  greater than or equal to the width.
 * @param[in] width The width of memory to be copied.
 * @param[in] height The height of memory to be copied.
 * @return
 * ::CN_SUCCESS,
 * ::CN_OPS_ERROR_NOT_SUPPORTED,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID
 *
 * @note
 * - It only supports MLU300 series or above.
 * - It does not support memory copy by different Contexts
 *   which are created by the same device.
 * - None of the parameters can be 0.
 */
__CN_EXPORT extern CNresult cnMemcpyDtoD2D(CNaddr dstAddr, cn_uint64_t dpitch, CNaddr srcAddr,
                                           cn_uint64_t spitch, cn_uint64_t width,
                                           cn_uint64_t height);
// Group: Memory Management
/**
 * @brief Copies from source MLU address to destination MLU address in 3D.
 *
 * Synchronously reads ``src_size`` bytes of data from the
 * source address pointed by ``p->src`` or ``p->srcPtr.ptr`` , and
 * writes it to the ``dst_size`` bytes of data from the
 * destination address pointed by ``p->dst`` or ``p->dstPtr.ptr`` .
 * The ``src_size`` is configured by ``p->extent`` and ``p->srcPtr`` ,
 * the ``dst_size`` is configured by ``p->extent`` and ``p->dstPtr`` .
 *
 * @param[in] p The parameter of 3D memory copy.
 * @return
 * ::CN_SUCCESS,
 * ::CN_OPS_ERROR_NOT_SUPPORTED,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID
 *
 * @note
 * - It only supports MLU300 series or above.
 * - It does not support memory copy by different Contexts
 *   which are created by the same device.
 */
__CN_EXPORT extern CNresult cnMemcpyDtoD3D(CNmemcpy3dParam *p);
// Group: Memory Management
/**
 * @brief Copies from source address to destination address in 2D.
 *
 * Synchronously reads ``spitch * width`` bytes of data from
 * the source address pointed by ``src_addr``, and writes it to the
 * ``dpitch * width`` bytes of destination address pointed by ``dst_addr``.
 *
 * @param[in] dst_addr Pointer to the destination address.
 * @param[in] dpitch The pitch of the destination memory. It must be
 *  greater than or equal to the width.
 * @param[in] src_addr Pointer to the source address.
 * @param[in] spitch The pitch of the source memory. It must be
 *  greater than or equal to the width.
 * @param[in] width The width of memory to be copied.
 * @param[in] height The height of memory to be copied.
 * @return
 * ::CN_SUCCESS,
 * ::CN_OPS_ERROR_NOT_SUPPORTED,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID
 *
 * @note
 * - It only supports MLU300 series or above.
 * - It does not support memory copy by different Contexts
 *   which are created by the same device.
 * - None of the parameters can be 0.
 */
__CN_EXPORT extern CNresult cnMemcpy2D(CNaddr dst_addr, cn_uint64_t dpitch, CNaddr src_addr,
                                       cn_uint64_t spitch, cn_uint64_t width, cn_uint64_t height);
// Group: Memory Management
/**
 * @brief Copies from source address to destination address in 2D asynchronously.
 *
 * Asynchronously reads ``spitch * width`` bytes of data from
 * the source address pointed by ``src_addr``, and writes it to the
 * ``dpitch * width`` bytes of destination address pointed by ``dst_addr``.
 *
 * @param[in] dst_addr Pointer to the destination address.
 * @param[in] dpitch The pitch of the destination memory. It must be
 *  greater than or equal to the width.
 * @param[in] src_addr Pointer to the source address.
 * @param[in] spitch The pitch of the source memory. It must be
 *  greater than or equal to the width.
 * @param[in] width The width of memory to be copied.
 * @param[in] height The height of memory to be copied.
 * @param[in] hqueue The queue handle created by calling ::cnCreateQueue() or
 * ::cnCreateQueueWithPriority().
 * @return
 * ::CN_SUCCESS,
 * ::CN_OPS_ERROR_NOT_SUPPORTED,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID
 *
 * @note
 * - It only supports MLU300 series or above.
 * - It does not support memory copy by different Contexts
 *   which are created by the same device.
 * - Parameters cannot be 0.
 */
__CN_EXPORT extern CNresult cnMemcpy2DAsync(CNaddr dst_addr, cn_uint64_t dpitch, CNaddr src_addr,
                                            cn_uint64_t spitch, cn_uint64_t width,
                                            cn_uint64_t height, CNqueue hqueue);
// Group: Memory Management
/**
 * @brief Copies from source MLU address to destination MLU address in 3D.
 *
 * Synchronously reads ``src_size`` bytes of data from the
 * source address pointed by ``p->src`` or ``p->srcPtr.ptr``, and
 * writes it to the ``dst_size`` bytes of data from the
 * destination address pointed by ``p->dst`` or ``p->dstPtr.ptr``.
 * The ``src_size`` is configured by ``p->extent`` and ``p->srcPtr``,
 * the ``dst_size`` is configured by ``p->extent`` and ``p->dstPtr``.
 *
 * @param[in] p The parameter of 3D memory copy.
 * @return
 * ::CN_SUCCESS,
 * ::CN_OPS_ERROR_NOT_SUPPORTED,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID
 *
 * @note
 * - It only supports MLU300 series or above.
 * - It does not support memory copy by different Contexts
 *  which are created by the same device.
 */
__CN_EXPORT extern CNresult cnMemcpy3D(CNmemcpy3dParam *p);
// Group: Memory Management
/**
 * @brief Copies from source address to destination address in 3D asynchronously.
 *
 * Asynchronously reads ``src_size`` bytes of data from the
 * source address pointed by ``p->src`` or ``p->srcPtr.ptr``, and
 * writes it to the ``dst_size`` bytes of data from the
 * destination address pointed by ``p->dst`` or ``p->dstPtr.ptr``.
 * The ``src_size`` is configured by ``p->extent`` and ``p->srcPtr``,
 * the ``dst_size`` is configured by ``p->extent`` and ``p->dstPtr``.
 *
 * @param[in] p The parameter of 3D memory copy.
 * @param[in] hqueue The queue handle created by calling ::cnCreateQueue() or
 * ::cnCreateQueueWithPriority().
 * @return
 * ::CN_SUCCESS,
 * ::CN_OPS_ERROR_NOT_SUPPORTED,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID
 *
 * @note
 * - It only supports MLU300 series or above.
 * - It does not support memory copy by different Contexts
 *  which are created by the same device.
 */
__CN_EXPORT extern CNresult cnMemcpy3DAsync(const CNmemcpy3dParam *p, CNqueue hqueue);
// Group: Memory Management
/**
 * @brief Copies memory asynchronously.
 *
 * Copies data between two pointers.
 * @p dstAddr and @p srcAddr are base pointers of the destination and source, respectively.
 * @p bytes specifies the number of bytes to copy.
 *
 * @param[in] dstAddr The required unified virtual destination address.
 * @param[in] srcAddr The required unified virtual source address.
 * @param[in] bytes The number of bytes to be copied.
 * @param[in] hqueue The queue handle created by calling ::cnCreateQueue() or
 * ::cnCreateQueueWithPriority().
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_ERROR_INVALID_ADDRESS,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS,
 * ::CN_QUEUE_ERROR_INVALID,
 * ::CN_INVOKE_ERROR_TIMEOUT
 *
 * @note
 * - This API infers the type of the transfer (host to host, host to device, device to device, or
 * device to host) from the pointer values.
 * - This API is only allowed in Contexts which support unified addressing.
 * - Whether it is @p dstAddr or @p srcAddr, you must ensure the memory will not be released
 *   until calling ::cnQueueSync or ::cnCtxSync is done.
 * - This function will be deprecated in a future release. Use ::cnMemcpyAsync_V3() instead.
 */
__CN_EXPORT extern CNresult cnMemcpyAsync(CNaddr dstAddr, CNaddr srcAddr, cn_uint64_t bytes,
                                          CNqueue hqueue);
// Group: Memory Management
/**
 * @brief Copies data between two pointers asynchronously.
 * @p dstAddr and @p srcAddr are base pointers of the destination and source, respectively.
 * @p bytes specifies the number of bytes to copy.
 *
 * @param[in] dstAddr The required unified virtual destination address.
 * @param[in] srcAddr The required unified virtual source address.
 * @param[in] bytes The number of bytes to be copied.
 * @param[in] hqueue The queue handle created by calling ::cnCreateQueue() or
 * ::cnCreateQueueWithPriority().
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_ERROR_INVALID_ADDRESS,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS,
 * ::CN_QUEUE_ERROR_INVALID,
 * ::CN_INVOKE_ERROR_TIMEOUT
 *
 * @note
 * - This API infers the type of the transfer (host to host, host to device, device to device, or
 * device to host) from the pointer values.
 * - This API is only allowed in Contexts which support unified addressing.
 * - Whether it is @p dstAddr or @p srcAddr, you must ensure the memory will not be released
 *   until calling ::cnQueueSync or ::cnCtxSync is done.
 * - This API exhibits asynchronous behavior for most use cases.
 * - This function will be deprecated in a future release. Use ::cnMemcpyAsync_V3() instead.
 */
__CN_EXPORT extern CNresult cnMemcpyAsync_V2(CNaddr dstAddr, CNaddr srcAddr, cn_uint64_t bytes,
                                             CNqueue hqueue);
// Group: Memory Management
/**
 * @brief Copies data between two pointers asynchronously.
 * @p dstAddr and @p srcAddr are base pointers of the destination and source, respectively.
 * @p bytes specifies the number of bytes to copy.
 *
 * @param[in] dstAddr The required unified virtual destination address.
 * @param[in] srcAddr The required unified virtual source address.
 * @param[in] bytes The number of bytes to be copied.
 * @param[in] hqueue The queue handle created by calling ::cnCreateQueue() or
 * ::cnCreateQueueWithPriority().
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_ERROR_INVALID_ADDRESS,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS,
 * ::CN_QUEUE_ERROR_INVALID,
 * ::CN_INVOKE_ERROR_TIMEOUT
 *
 * @note
 * - This API infers the type of the transfer (host to host, host to device, device to device, or
 * device to host) from the pointer values.
 * - This API is only allowed in Contexts which support unified addressing.
 * - Whether it is @p dstAddr or @p srcAddr, you must ensure the memory will not be released
 *   until calling ::cnQueueSync or ::cnCtxSync is done.
 * - This API exhibits asynchronous behavior for most use cases.
 */
__CN_EXPORT extern CNresult cnMemcpyAsync_V3(CNaddr dstAddr, CNaddr srcAddr, cn_uint64_t bytes,
                                             CNqueue hqueue);
// Group: Memory Management
/**
 * @brief Copies MLU memory between two Contexts asynchronously
 *
 * Copies from MLU memory in one context to MLU memory in another
 * context. @p dstAddr is the base MLU pointer of the destination memory
 * and @p dstCtx is the destination context. @p srcAddr is the base
 * device pointer of the source memory and srcCtx is the source pointer.
 * bytes specifies the number of bytes to copy.
 *
 * @param[in] dstAddr The destination device pointer.
 * @param[in] dstCtx The destination context.
 * @param[in] srcAddr The source device pointer.
 * @param[in] srcCtx The source context.
 * @param[in] bytes The number of memory copy in bytes.
 * @param[in] hqueue The queue handle created by calling ::cnCreateQueue() or
 * ::cnCreateQueueWithPriority().
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_QUEUE_ERROR_INVALID,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS,
 * ::CN_INVOKE_ERROR_TIMEOUT
 *
 * @note
 * - The srcAddr and hqueue must be allocated by the same context.
 * - Whether it is @p dstAddr or @p srcAddr, you must ensure the memory will not be released
 *   until calling ::cnQueueSync or ::cnCtxSync is done.
 */
__CN_EXPORT extern CNresult cnMemcpyPeerAsync(CNaddr dstAddr, CNcontext dstCtx, CNaddr srcAddr,
                                              CNcontext srcCtx, cn_uint64_t bytes, CNqueue hqueue);
// Group: Memory Management
/**
 * @brief Copies bytes from source host address to destination MLU address asynchronously.
 *
 * Copies data from host memory to MLU memory. @p dstAddr and @p srcAddr are
 * the base addresses of the destination and source respectively. Bytes
 * specifies the number of bytes to copy.
 *
 * @param[in] dstAddr The required destination MLU address.
 * @param[in] srcAddr The required source host address.
 * @param[in] bytes The number of bytes to be copied.
 * @param[in] hqueue The queue handle created by calling ::cnCreateQueue() or
 * ::cnCreateQueueWithPriority().
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_QUEUE_ERROR_INVALID,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS,
 * ::CN_ERROR_INVALID_ADDRESS,
 * ::CN_INVOKE_ERROR_TIMEOUT
 *
 * @note
 * - @p dstAddr and @p hqueue must be allocated by the same context.
 * - Whether it is @p dstAddr or @p srcAddr, you must ensure the memory will not be released
 *   until calling ::cnQueueSync or ::cnCtxSync is done.
 * - This function will be deprecated in a future release. Use ::cnMemcpyAsync_V3() instead.
 *
 */
__CN_EXPORT extern CNresult cnMemcpyHtoDAsync(CNaddr dstAddr, const void *srcAddr,
                                              cn_uint64_t bytes, CNqueue hqueue);
// Group: Memory Management
/**
 * @brief Copies bytes from source host address to destination MLU address asynchronously.
 * @p dstAddr and @p srcAddr are
 * the base addresses of the destination and source respectively. Bytes
 * specifies the number of bytes to copy.
 *
 * @param[in] dstAddr The required destination MLU address.
 * @param[in] srcAddr The required source host address.
 * @param[in] bytes The number of bytes to be copied.
 * @param[in] hqueue The queue handle created by calling ::cnCreateQueue() or
 * ::cnCreateQueueWithPriority().
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_QUEUE_ERROR_INVALID,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS,
 * ::CN_ERROR_INVALID_ADDRESS,
 * ::CN_INVOKE_ERROR_TIMEOUT
 *
 * @note
 * - @p dstAddr and @p hqueue must be allocated by the same context.
 * - Whether it is @p dstAddr or @p srcAddr, you must ensure the memory will not be released
 *   until calling ::cnQueueSync or ::cnCtxSync is done.
 * - This API exhibits asynchronous behavior for most use cases.
 * - This function will be deprecated in a future release. Use ::cnMemcpyAsync_V3() instead.
 */
__CN_EXPORT extern CNresult cnMemcpyHtoDAsync_V2(CNaddr dstAddr, const void *srcAddr,
                                                 cn_uint64_t bytes, CNqueue hqueue);
// Group: Memory Management
/**
 * @brief Copies bytes from source MLU address to destination host address asynchronously.
 *
 * @param[in] srcAddr The required destination MLU address.
 * @param[in] dstAddr The required source host address.
 * @param[in] bytes The number of bytes to be copied.
 * @param[in] hqueue The queue handle created by calling ::cnCreateQueue() or
 * ::cnCreateQueueWithPriority().
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS,
 * ::CN_ERROR_INVALID_ADDRESS,
 * ::CN_QUEUE_ERROR_INVALID,
 * ::CN_INVOKE_ERROR_TIMEOUT
 *
 * @note
 * - @p srcAddr and @p hqueue must be allocated by the same context.
 * - Whether it is @p dstAddr or @p srcAddr, you must ensure the memory will not be released
 *   until calling ::cnQueueSync or ::cnCtxSync is done.
 * - This function will be deprecated in a future release. Use ::cnMemcpyAsync_V3() instead.
 */
__CN_EXPORT extern CNresult cnMemcpyDtoHAsync(void *dstAddr, CNaddr srcAddr, cn_uint64_t bytes,
                                              CNqueue hqueue);
// Group: Memory Management
/**
 * @brief Copies bytes from source MLU address to destination host address asynchronously.
 *
 * @param[in] srcAddr The required destination MLU address.
 * @param[in] dstAddr The required source host address.
 * @param[in] bytes The number of bytes to be copied.
 * @param[in] hqueue The queue handle created by calling ::cnCreateQueue() or
 * ::cnCreateQueueWithPriority().
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS,
 * ::CN_ERROR_INVALID_ADDRESS,
 * ::CN_QUEUE_ERROR_INVALID,
 * ::CN_INVOKE_ERROR_TIMEOUT
 *
 * @note
 * - @p srcAddr and @p hqueue must be allocated by the same context.
 * - Whether it is @p dstAddr or @p srcAddr, you must ensure the memory will not be released
 *   until calling ::cnQueueSync or ::cnCtxSync is done.
 * - This API exhibits asynchronous behavior for most use cases.
 * - This function will be deprecated in a future release. Use ::cnMemcpyAsync_V3() instead.
 */
__CN_EXPORT extern CNresult cnMemcpyDtoHAsync_V2(void *dstAddr, CNaddr srcAddr, cn_uint64_t bytes,
                                                 CNqueue hqueue);
// Group: Memory Management
/**
 * @brief Copies bytes from source MLU address to destination MLU address asynchronously.
 *
 * @param[in] srcAddr The required source MLU address.
 * @param[in] dstAddr The required destination MLU address.
 * @param[in] bytes The number of bytes to be copied.
 * @param[in] hqueue The queue handle created by calling ::cnCreateQueue() or
 * ::cnCreateQueueWithPriority().
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_QUEUE_ERROR_INVALID,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS,
 * ::CN_INVOKE_ERROR_TIMEOUT
 *
 * @note
 * - The @p srcAddr and @p hqueue must be allocated by the same context.
 * - Whether it is @p dstAddr or @p srcAddr, you must ensure the memory will not be released
 *   until calling ::cnQueueSync or ::cnCtxSync is done.
 */
__CN_EXPORT extern CNresult cnMemcpyDtoDAsync(CNaddr dstAddr, CNaddr srcAddr, cn_uint64_t bytes,
                                              CNqueue hqueue);
// Group: Memory Management
/**
 * @brief Initializes device memory.
 *
 * Sets the memory range of 8-bit values to the specified value.
 *
 * @param[in] mluAddr The required destination MLU address.
 * @param[in] value The value to set for each byte of specified MLU memory.
 * @param[in] count The number of elements.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS,
 * ::CN_CONTEXT_ERROR_INVALID
 */
__CN_EXPORT extern CNresult cnMemsetD8(CNaddr mluAddr, unsigned char value, cn_uint64_t count);
// Group: Memory Management
/**
 * @brief Initializes device memory.
 *
 * Sets the memory range of 16-bit values to the specified value.
 *
 * @param[in] mluAddr The required destination MLU address.
 * @param[in] value The value to set for each word of specified MLU memory.
 * @param[in] count The number of elements.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS,
 * ::CN_CONTEXT_ERROR_INVALID
 */
__CN_EXPORT extern CNresult cnMemsetD16(CNaddr mluAddr, unsigned short value, cn_uint64_t count);
// Group: Memory Management
/**
 * @brief Initializes device memory.
 *
 * Sets the memory range of 32-bit values to the specified value.
 *
 * @param[in] mluAddr The required destination MLU address.
 * @param[in] value The value to set for each dword of specified MLU memory.
 * @param[in] count The number of elements.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS,
 * ::CN_CONTEXT_ERROR_INVALID
 */
__CN_EXPORT extern CNresult cnMemsetD32(CNaddr mluAddr, unsigned int value, cn_uint64_t count);
// Group: Memory Management
/**
 * @brief Initializes device memory asynchronously.
 *
 * Sets the memory range of 8-bit values to the specified value.
 *
 * @param[in] mluAddr The required destination MLU address.
 * @param[in] value The value to set for each byte of specified MLU memory.
 * @param[in] count The number of elements.
 * @param[in] hqueue The queue handle created by calling ::cnCreateQueue() or
 * ::cnCreateQueueWithPriority().
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS,
 * ::CN_QUEUE_ERROR_INVALID,
 * ::CN_INVOKE_ERROR_TIMEOUT
 *
 * @note
 * - The mluAddr and hqueue must be allocated by the same context.
 */
__CN_EXPORT extern CNresult cnMemsetD8Async(CNaddr mluAddr, unsigned char value, cn_uint64_t count,
                                            CNqueue hqueue);
// Group: Memory Management
/**
 * @brief Initializes device memory asynchronously.
 *
 * Sets the memory range of 16-bit values to the specified value.
 *
 * @param[in] mluAddr The required destination MLU address.
 * @param[in] value The value to set for each word of specified MLU memory.
 * @param[in] count The number of elements.
 * @param[in] hqueue The queue handle created by calling ::cnCreateQueue() or
 * ::cnCreateQueueWithPriority().
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_QUEUE_ERROR_INVALID,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS,
 * ::CN_INVOKE_ERROR_TIMEOUT
 *
 * @note
 * - The mluAddr and hqueue must be allocated by the same context.
 */
__CN_EXPORT extern CNresult cnMemsetD16Async(CNaddr mluAddr, unsigned short value,
                                             cn_uint64_t count, CNqueue hqueue);
// Group: Memory Management
/**
 * @brief Initializes device memory asynchronously.
 *
 * Sets the memory range of 32-bit values to the specified value.
 *
 * @param[in] mluAddr The required destination MLU address.
 * @param[in] value The value to set for each dword of specified MLU memory.
 * @param[in] count The number of elements.
 * @param[in] hqueue The queue handle created by calling ::cnCreateQueue() or
 * ::cnCreateQueueWithPriority().
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_QUEUE_ERROR_INVALID,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS,
 * ::CN_INVOKE_ERROR_TIMEOUT
 *
 * @note
 * - The mluAddr and hqueue must be allocated by the same context.
 */
__CN_EXPORT extern CNresult cnMemsetD32Async(CNaddr mluAddr, unsigned int value, cn_uint64_t count,
                                             CNqueue hqueue);
// Group: Memory Management
/**
 * @brief Checks the peer-to-peer ability.
 *
 * @param[out] canPeer The peer-to-peer ability; the value is 1 when the peer-to-peer ability is available, otherwise the value is 0.
 * @param[in]  devSrc The src device descriptor.
 * @param[in]  devDst The dst device descriptor.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_ERROR_INVALID_DEVICE
 */
__CN_EXPORT extern CNresult cnDeviceCanPeerAble(int *canPeer, CNdev devSrc, CNdev devDst);

/******************************************************************************
 * Unified Addressing
 ******************************************************************************/
// Group: Memory Management
/**
 * @brief Returns information about the memory pointer.
 *
 * Supported attributes are:
 *
 * - CN_MEM_ATTRIBUTE_TYPE :
 *
 *   Returns the memory type. The physical memory type with ptr
 *   address as a ::CN_memory_type enumerated value.
 *
 *   CN memory type enums:
 *
 *   - CN_MEMORYTYPE_UNKNOWN: The ptr is an invalid pointer. The API will return
 *     error while data is assigned to ::CN_MEMORYTYPE_UNKNOWN.
 *   - CN_MEMORYTYPE_HOST: The ptr is host address allocated by or mapped by MLU API.
 *   - CN_MEMORYTYPE_DEVICE: The ptr is device address allocated by or mapped by MLU API.
 *   - CN_MEMORYTYPE_ARRAY: The ptr is array memory (not used now).
 *   - CN_MEMORYTYPE_UNIFIED: The ptr is unified device memory or host memory
 *     address (not used now).
 *
 *   The type of pi must be int *.
 *
 * - CN_MEM_ATTRIBUTE_HOST_POINTER :
 *
 *   Returns the host pointer that can be accessed by host. The type of pi must be void *.
 *   The ptr is device pointer. If it fails, *pi will return NULL and
 *   this API is only supported by MLU220 edge.
 *
 * - CN_MEM_ATTRIBUTE_DEVICE_POINTER :
 *
 *   Returns the device pointer that can be accessed by MLU. The type of pi must be void *.
 *   The ptr is host pointer. If it fails, *pi will return NULL and
 *   this API is only supported by MLU220 edge.
 *
 * - CN_MEM_ATTRIBUTE_IS_CACHE_OPS:
 *
 *   Checks if the memory needs to keep cache consistency.
 *   For example, before reading data from the memory, you need to invalidate cache; after writing
 *   data to the memory, you need to flush cache. The type of pi  must be int *.
 *
 * - CN_MEM_ATTRIBUTE_CONTEXT:
 *
 *   Returns the CNcontext of the allocated memory. The type of pi must be CNcontext*.
 *
 * - CN_MEM_ATTRIBUTE_DEVICE_ORDINAL:
 *
 *   Returns an integer representing the ordinal of a device against which the memory is allocated.
 *
 * - CN_MEM_ATTRIBUTE_RANGE_START_ADDR:
 *
 *   Returns the allocated starting address of the memory related to the device pointer ptr.
 *
 * - CN_MEM_ATTRIBUTE_RANGE_SIZE:
 *
 *   Returns the allocated memory size related to the device pointer ptr.
 *
 * - CN_MEM_ATTRIBUTE_HOST_CACHE_TYPE:
 *
 *   Returns the cache type of host memory of the ptr address. The cache type is an enumerated value
 *   of ::CNHostMemCachingType. This attribute is only supported in CE3226V100 and ptr must be a
 *   pointer whose memory type is CN_MEMORYTYPE_HOST.
 *   CN host memory cache type enums:
 *
 *   - CN_HOSTMEMORY_NOT_SUPPORT: The current platform or driver version does not support
 *   - CN_MEM_ATTRIBUTE_HOST_CACHE_TYPE.
 *   - CN_HOSTMEMORY_CACHE_UNKNOWN: The ptr is either an invalid
 *     pointer or valid device memory pointer; it fails to get the host cache type.
 *   - CN_HOSTMEMORY_NONCACHE: The ptr is non-cacheable host memory.
 *   - CN_HOSTMEMORY_CACHEABLE: The ptr is cacheable host memory.
 *
 * @param[out] pi Returned pointer attribute value.
 * @param[in]  attr  Pointer attribute to query.
 * @param[in]  ptr Pointer to query attribute.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID
 */
__CN_EXPORT extern CNresult cnGetMemAttribute(void *pi, CNmem_attribute attr, CNaddr ptr);
// Group: Memory Management
/**
 * @brief Returns information about the memory pointer.
 *
 * Supported attributes are:
 *
 * - ::CN_MEM_ATTRIBUTE_TYPE
 * - ::CN_MEM_ATTRIBUTE_HOST_POINTER
 * - ::CN_MEM_ATTRIBUTE_IS_CACHE_OPS
 * - ::CN_MEM_ATTRIBUTE_CONTEXT
 * - ::CN_MEM_ATTRIBUTE_DEVICE_ORDINAL
 * - ::CN_MEM_ATTRIBUTE_RANGE_START_ADDR
 * - ::CN_MEM_ATTRIBUTE_RANGE_SIZE
 * - ::CN_MEM_ATTRIBUTE_HOST_CACHE_TYPE
 *
 * @param[in] numAttributes The number of attributes to query.
 * @param[in]  attributes An array of attributes to query
 *       (@p numAttributes and the number of attributes in this array should match).
 * @param[out] data A 2D array containing pointers to memory
 *       locations where the result of each attribute query will be written to.
 * @param[in] ptr The pointer to query.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS
 */
__CN_EXPORT extern CNresult cnGetMemAttributes(unsigned int numAttributes,
                                               CNmem_attribute *attributes, void **data,
                                               CNaddr ptr);
// Group: Memory Management
/**
 * @brief Sets attribute for the memory.
 *
 * The supported attributes are:
 *
 * - CN_MEM_ATTRIBUTE_CACHE_OPS:
 *
 *   If the value is CN_FLUSH_CACHE, flush cache;
 *   If the value is CN_INVALID_CACHE, invalidate cache;
 *   For other value input, error code is returned.
 *
 * - CN_MEM_ATTRIBUTE_ACCESS_FLAGS:
 *
 *   Sets MLU memory access flags for the given MLU memory pointer;
 *   CNMemoryAccessFlags enumeration is the valid value input;
 *   For other value input, error code is returned.
 *
 * - CN_MEM_ATTRIBUTE_CACHE_CONFIG:
 *
 *   Sets MLU memory cache policy for the given MLU memory pointer;
 *   CNMemoryCachingType enumeration is the valid value input;
 *   For other value input, error code is returned.
 *
 *
 * @param[in] value Sets pointer attribute value.
 * @param[in] attr The pointer attribute to set.
 * @param[in] ptr The pointer to set.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS
 */
__CN_EXPORT extern CNresult cnSetMemAttribute(cn_uint64_t value, CNmem_attribute attr, CNaddr ptr);
// Group: Memory Management
/**
 * @brief Sets attribute for the memory.
 *
 * The supported attributes are:
 *
 * CN_MEM_ATTRIBUTE_CACHE_OPS:
 *
 * If the value is CN_FLUSH_CACHE, flush cache;
 * If the value is CN_INVALID_CACHE, invalidate cache;
 * For other value input, error code is returned.
 *
 * @param[in] value Sets pointer attribute value.
 * @param[in] attr The pointer attribute to set.
 * @param[in] ptr The pointer to set.
 * @param[in] bytes The bytes of the memory to set.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID
 */
__CN_EXPORT extern CNresult cnSetMemRangeAttribute(cn_uint64_t value, CNmem_attribute attr,
                                                   CNaddr ptr, cn_uint64_t bytes);

/******************************************************************************
 * Queue Management
 ******************************************************************************/
// Group: Queue Management
/**
 * @brief Creates a queue and returns queue handle.
 *
 * @param[out] phqueue The newly created queue handle.
 * @param[in]  flags Reserved parameter, which must be 0.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY,
 * ::CN_QUEUE_ERROR_OUT_OF_RESOURCES
 *
 * @note
 * - You can manage the lifecycle of the queue. Call ::cnDestroyQueue() to destroy the queue.
 */
__CN_EXPORT extern CNresult cnCreateQueue(CNqueue *phqueue, unsigned int flags);
// Group: Queue Management
/**
 * @brief Creates a new queue with priority.
 *
 * Creates a queue with the specified priority and returns a handle in @p phqueue.
 * This API alters the scheduler priority of work in the queue. The task in a higher
 * priority queue may preempt the task already executed in a lower priority queue.
 *
 * @param[out] phqueue The newly created queue handle.
 * @param[in]  flags Reserved parameter, which must be 0.
 * @param[in]  priority The priority of the created queue. See ::cnCtxGetQueuePriorityRange()
 * for the meaningful priorities that can be passed.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY,
 * ::CN_QUEUE_ERROR_OUT_OF_RESOURCES
 *
 * @note
 * - You can manage the lifecycle of the queue. Call ::cnDestroyQueue() to destroy the queue.
 * - If the specified priority is outside the range, it will automatically be clamped to the lowest
 *   or the highest number in the range.
 */
__CN_EXPORT extern CNresult cnCreateQueueWithPriority(CNqueue *phqueue, unsigned int flags,
                                                      int priority);
// Group: Queue Management
/**
 * @brief Queries the priority of a given queue.
 *
 * Queries the priority of a queue created using ::cnCreateQueue() or ::cnCreateQueueWithPriority()
 * and returns the priority in @p priority.
 *
 * @param[in]  hqueue The queue to be queried.
 * @param[out] priority Pointer to a signed integer in which the queue's priority is returned.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY,
 * ::CN_CONTEXT_ERROR_INVALID
 *
 * @note
 * - If the queue was created with a priority outside the numerical range returned by
 *   ::cnCtxGetQueuePriorityRange(), this API will the clamped priority.
 */
__CN_EXPORT extern CNresult cnQueueGetPriority(CNqueue hqueue, int *priority);
// Group: Queue Management
/**
 * @brief Destroys a queue.
 *
 * In case the MLU device is still doing work in the queue
 * when ::cnDestroyQueue() is called, the API will still return ::CN_SUCCESS immediately
 * and the resources associated with the queue will be released automatically.
 *
 * @param[in] hqueue The queue handle created by calling ::cnCreateQueue() or
 * ::cnCreateQueueWithPriority().
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_QUEUE_ERROR_INVALID
 *
 * @note
 * - This API is protected inside the API to ensure that resources can be released normally.
 * For better performance, complete all tasks in queue before calling ::cnDestroyQueue.
 * Attention should be paid to the asynchronous API. You can ensure all tasks have been finished by
 * calling ::cnQueryQueue() or ::cnQueueSync().
 */
__CN_EXPORT extern CNresult cnDestroyQueue(CNqueue hqueue);
// Group: Queue Management
/**
 * @brief Queries queue status on the completion of all precedent tasks.
 *
 * Returns CN_SUCCESS if all operations in the queue specified by
 * hqueue have been completed, or CN_ERROR_NOT_READY if not.
 *
 * @param[in] hqueue The queue handle created by calling cnCreateQueue() or
 * ::cnCreateQueueWithPriority().
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_QUEUE_ERROR_INVALID,
 * ::CN_ERROR_NOT_READY
 */
__CN_EXPORT extern CNresult cnQueryQueue(CNqueue hqueue);
// Group: Queue Management
/**
 * @brief Waits until a queue's tasks are completed.
 *
 * Waits until the device has completed all operations in the queue.
 *
 * @param[in] hqueue The queue handle created by calling ::cnCreateQueue() or
 * ::cnCreateQueueWithPriority().
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_QUEUE_ERROR_INVALID,
 * ::CN_INVOKE_ERROR_OUT_OF_RESOURCES,
 * ::CN_INVOKE_ERROR_EXECUTED_TIMEOUT,
 * ::CN_INVOKE_ERROR_ILLEGAL_INSTRUCTION,
 * ::CN_INVOKE_ERROR_ADDRESS_SPACE,
 * ::CN_INVOKE_ERROR_KERNEL_TRAP,
 * ::CN_INVOKE_ERROR,
 * ::CN_ERROR_QUEUE_CAPTURE_UNSUPPORTED
 */
__CN_EXPORT extern CNresult cnQueueSync(CNqueue hqueue);

// Group: Queue Management
/**
 * @brief Makes a queue wait on a notifier.
 *
 * Makes all future tasks submitted to @p hqueue wait for all tasks captured in
 * @p hnotifier. See ::cnPlaceNotifier() for details on what is captured by a notifier.
 *
 * @param[in] hqueue The queue handle created by calling ::cnCreateQueue() or
 * ::cnCreateQueueWithPriority().
 * @param[in] hnotifier The notifier to wait on (It can not be NULL).
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_QUEUE_ERROR_INVALID,
 * ::CN_NOTIFIER_ERROR_INVALID,
 * ::CN_OPS_ERROR_NOT_PERMITTED,
 * ::CN_OPS_ERROR_OUT_OF_RESOURCES,
 * ::CN_OPS_ERROR_NOT_SUPPORTED
 *
 * @note
 * - The hnotifier and hqueue must be created by the same context.
 * - This API is an asynchronous interface,
 * and CN_SUCCESS does not represent the completion of notifier execution.
 * Call ::cnQueryNotifier() or ::cnWaitNotifier() to confirm whether the notifier in queue has been
 * executed.
 */
__CN_EXPORT extern CNresult cnQueueWaitNotifier(CNqueue hqueue, CNnotifier hnotifier);

// Group: Queue Management
/**
 * @brief Makes a queue wait on a notifier with flags.
 *
 * Makes all future tasks submitted to @p hqueue wait for all tasks captured in
 * @p hnotifier. See ::cnPlaceNotifier() for details on what is captured by a notifier.
 *
 * @param[in] hnotifier The notifier handle created by calling ::cnCreateNotifier().
 * @param[in] hqueue The queue handle created by calling ::cnCreateQueue() or
 * ::cnCreateQueueWithPriority().
 * @param[in] flags See ::CNNotifierWaitFlags.
 * The flags include:
 * - ::CN_NOTIFIER_WAIT_DEFAULT: Default flag.
 * - ::CN_NOTIFIER_WAIT_EXTERNAL: Notifier is captured in the TaskTopo as an external notifier wait
 * node when performing queue capture. This flag is invalid outside of queue capture.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_NOTIFIER_ERROR_INVALID,
 * ::CN_QUEUE_ERROR_INVALID,
 * ::CN_OPS_ERROR_NOT_PERMITTED
 */
__CN_EXPORT CNresult cnQueueWaitNotifierWithFlags(CNqueue hqueue, CNnotifier hnotifier,
                                                  unsigned int flags);

// Group: Queue Management
/**
 * @brief Gets the context that is bound to a queue.
 *
 * Returns the CNDrv context that the queue is associated with.
 *
 * @param[in] hqueue The queue handle created by calling ::cnCreateQueue() or
 * ::cnCreateQueueWithPriority().
 * @param[out] pctx The context that is bound to a queue.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_QUEUE_ERROR_INVALID
 */
__CN_EXPORT extern CNresult cnQueueGetContext(CNqueue hqueue, CNcontext *pctx);

// Group: Queue Management
/**
 * @brief Sets the queue attribute.
 *
 * Sets the attribute corresponding to @p attr_id for @p hqueue from corresponding
 * attribute of @p value.
 * The updated attribute will be applied to subsequent work submitted to the queue.
 * It will not affect previously submitted tasks in the queue.
 *
 * @param[in] hqueue   The queue handle created by calling ::cnCreateQueue() or
 *                     ::cnCreateQueueWithPriority().
 * @param[in] attr_id  Attribute ID.
 * @param[in] value    The attribute value to set.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID
 */
__CN_EXPORT extern CNresult cnQueueSetAttribute(CNqueue hqueue, CNqueueAttrID attr_id,
                                                const CNqueueAttrValue *value);

// Group: Queue Management
/**
 * @brief Queries the queue attribute.
 *
 * Queries the attribute corresponding to @p attr_id for @p hqueue, and stores it in corresponding
 * member of @p value_out.
 *
 * @param[in] hqueue     The queue handle created by calling ::cnCreateQueue() or
 *                       ::cnCreateQueueWithPriority().
 * @param[in] attr_id    Attribute ID.
 * @param[out] value_out The room to store the attribute value.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID
 */
__CN_EXPORT extern CNresult cnQueueGetAttribute(CNqueue hqueue, CNqueueAttrID attr_id,
                                                CNqueueAttrValue *value_out);

// Group: Queue Management
/**
 * @brief Copies queue attributes from source queue to destination queue.
 *
 * Copies attributes from source queue @p src to destination queue @p dst.
 * Both queues must share the same context.
 *
 * @param[in] dst   The destination queue.
 * @param[in] src   The source queue.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID
 */
__CN_EXPORT extern CNresult cnQueueCopyAttributes(CNqueue dst, CNqueue src);

// Group: Queue Management
/**
 * @brief Adds a callback to @p queue.
 *
 * If you do not require the callback to execute in case of device error or
 * uncompleted queue destroyed by ::cnDestroyQueue(), consider using ::cnInvokeHostFunc().
 * Additionally, this function is not supported by ::cnQueueBeginCapture()
 * and ::cnQueueEndCapture(), unlike ::cnInvokeHostFunc().
 *
 * Adds a callback to be called on the host after all currently enqueued
 * tasks in the queue have completed. For each
 * ::cnQueueAddCallback() call, the callback will be executed exactly once.
 * The callback will block later work in the queue until it is finished.
 *
 * The callback may be passed with ::CN_SUCCESS or an error code. In the event
 * of a device error, all subsequently executed callbacks will receive an
 * appropriate ::CNresult.
 *
 * @param[in] queue    The queue to add callback to.
 * @param[in] callback The function to call once preceding queue tasks are complete.
 * @param[in] userData The user data passed to the callback function.
 * @param[in] flags    Reserved for future use, which must be 0.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_OPS_ERROR_NOT_PERMITTED,
 * ::CN_INVOKE_ERROR_OUT_OF_RESOURCES,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY,
 * ::CN_ERROR_NOT_INITIALIZED
 *
 * @note
 * - Callbacks must not make any CNRT or CNDrv API call. Attempting to use a CNRT or CNDrv API
 * will result in ::CN_OPS_ERROR_NOT_PERMITTED.
 * - Callbacks must not perform any synchronization that may depend on outstanding device work
 * or other callbacks that are not mandated to run earlier.
 * - Callbacks without a mandated order (in independent queues) execute in undefined order and may
 * be serialized.
 * - If an uncompleted queue is destroyed by ::cnDestroyQueue(), the unexecuted callback will be
 * executed in calling thread.
 *
 */
__CN_EXPORT extern CNresult cnQueueAddCallback(CNqueue queue, CNqueueCallback callback,
                                               void *userData, unsigned int flags);

// Group: Queue Management
/**
 * @brief Begins queue capture on @p hQueue.
 *
 * When a queue is capturing, all the tasks pushed to this queue will not be executed but instead
 * captured into a TaskTopo.
 * Call ::cnQueueIsCapturing() to query whether the queue is capturing.
 * Call ::cnQueueGetCaptureInfo() to query the unique ID representing the sequence number of each
 * capture.
 *
 * @param[in] hQueue    The queue to begin capture for.
 * @param[in] mode      The capture mode.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_ILLEGAL_STATE
 *
 * @note
 * - If @p mode is not ::CN_QUEUE_CAPTURE_MODE_RELAXED, ::cnQueueEndCapture() must be called on this
 *   @p hQueue from the same thread.
 * - It is not allowed to pass NULL to @p hQueue as a default queue to begin capturing.
 * - The capture must be ended on the same queue as it was started.
 */
__CN_EXPORT extern CNresult cnQueueBeginCapture(CNqueue hQueue, CNqueueCaptureMode mode);

// Group: Queue Management
/**
 * @brief Ends a queue capture, and returns the captured TaskTopo.
 *
 * @param[in] hQueue       The queue in which to end capturing.
 * @param[out] pTaskTopo   The captured TaskTopo.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_ILLEGAL_STATE,
 * ::CN_ERROR_QUEUE_CAPTURE_INVALIDATED
 *
 * @note
 * - If the queue is not capturing under ::CN_QUEUE_CAPTURE_MODE_RELAXED, ::cnQueueEndCapture()
 *   must be called on this @p hQueue from the same thread.
 * - If the capture is invalidated, then the NULL TaskTopo will be returned.
 * - Capture must have been begun on @p hQueue via ::cnQueueBeginCapture().
 */
__CN_EXPORT extern CNresult cnQueueEndCapture(CNqueue hQueue, CNtaskTopo *pTaskTopo);

// Group: Queue Management
/**
 * @brief Queries a queue's capture status.
 *
 * @param[in] hQueue           The queue to query.
 * @param[out] pCaptureStatus  The capture status.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE
 */
__CN_EXPORT extern CNresult cnQueueIsCapturing(CNqueue hQueue,
                                               CNqueueCaptureStatus *pCaptureStatus);

// Group: Queue Management
/**
 * @brief Queries a queue's capture information.
 *
 * Returns detailed information if the @p hQueue is in active capture status.
 * The parameters @p idOut, @p pTaskTopo, @p pDependenciesOut, and @p pNumDependenciesOut are
 * optional, which can be NULL, with nothing returned.
 *
 * @param[in] hQueue                 The queue to query.
 * @param[out] pCaptureStatus        The capture status.
 * @param[out] idOut                 The unique sequence number of the current capturing.
 * @param[out] pTaskTopo             The current captured TaskTopo.
 * @param[out] pDependenciesOut      A pointer to store an array of dependency nodes.
 * @param[out] pNumDependenciesOut   The count of dependency nodes.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE
 */
__CN_EXPORT extern CNresult cnQueueGetCaptureInfo(CNqueue hQueue,
                                                  CNqueueCaptureStatus *pCaptureStatus,
                                                  cn_uint64_t *idOut, CNtaskTopo *pTaskTopo,
                                                  const CNtaskTopoNode **pDependenciesOut,
                                                  size_t *pNumDependenciesOut);

// Group: Queue Management
/**
 * @brief Updates the set of dependencies in a capturing queue.
 *
 * Modifies the dependency set of capturing @p hQueue. The dependency set is the set of nodes
 * that the next captured node in the @p hQueue will depend on.
 *
 * @param[in] hQueue             The queue in capture status.
 * @param[in] dependencies       The array of dependency nodes to modify the capturing sequence dependencies.
 * @param[in] numDependencies    The node count of the @p dependencies.
 * @param[in] flags              Modification flag. Valid flags are ::CN_QUEUE_ADD_CAPTURE_DEPENDENCIES and
 *                               ::CN_QUEUE_SET_CAPTURE_DEPENDENCIES.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_ILLEGAL_STATE,
 * ::CN_ERROR_QUEUE_CAPTURE_INVALIDATED,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY
 */
__CN_EXPORT extern CNresult cnQueueUpdateCaptureDependencies(CNqueue hQueue,
                                                             CNtaskTopoNode *dependencies,
                                                             size_t numDependencies,
                                                             unsigned int flags);

// Group: Queue Management
/**
 * @brief Swaps the queue capture interaction mode for a thread.
 *
 * Sets the calling thread capture mode to the value contained in @p *mode, and overwrites
 * @p *mode with the previous mode for the thread. To facilitate deterministic behavior across function or module
 * boundaries, callers are encouraged to use the API in a push-pop fashion:
 * @par Example
 * @code
 *   CNqueueCaptureMode mode = desiredMode;
 *   cnThreadExchangeQueueCaptureMode(&mode);
 *   ...
 *   cnThreadExchangeQueueCaptureMode(&mode);  //restore previous mode
 * @endcode
 * During queue capture (see ::cnQueueBeginCapture), some actions, such as a call to ::cnMalloc, may
 * be unsafe. In the case of ::cnMalloc, the operation is not asynchronous,
 * and is not observed by queue capture. Therefore, if the sequence of operations captured via
 * ::cnQueueBeginCapture depended on the allocation being replayed whenever the topo is launched,
 * the capture graph would be invalid.
 *
 * Therefore, queue capture places restrictions on API calls that can be made within or concurrently
 * to a
 * ::cnQueueBeginCapture-::cnQueueEndCapture sequence. The behavior can be controlled via this API
 * and flag to
 * ::cnQueueBeginCapture.
 *
 * A thread's mode is one of the following:
 * - ::CN_QUEUE_CAPTURE_MODE_GLOBAL: This is the default mode. If the local thread has an ongoing
 * capture sequence that was not initiated with ::CN_QUEUE_CAPTURE_MODE_RELAXED at
 * cnQueueBeginCapture, or if any other thread has a concurrent capture sequence initiated with
 * ::CN_QUEUE_CAPTURE_MODE_GLOBAL, this thread is prohibited from potentially unsafe API calls.
 * - ::CN_QUEUE_CAPTURE_MODE_THREAD_LOCAL: If the local thread has an ongoing capture sequence
 * not initiated with ::CN_QUEUE_CAPTURE_MODE_RELAXED, it is prohibited from potentially unsafe
 * API calls. Concurrent capture sequences in other threads are ignored.
 * - ::CN_QUEUE_CAPTURE_MODE_RELAXED: The local thread is not prohibited from potentially unsafe API
 * calls. Note that the thread is still prohibited from API calls which necessarily conflict with
 * queue capture.
 *
 * @param[in,out] pmode    Pointer to mode value to swap with the current mode.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_ILLEGAL_STATE,
 * ::CN_ERROR_QUEUE_CAPTURE_INVALIDATED,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY
 */
__CN_EXPORT extern CNresult cnThreadExchangeQueueCaptureMode(CNqueueCaptureMode *pmode);

/******************************************************************************
 * Notifier Management
 ******************************************************************************/
// Group: Notifier Management
/**
 * @brief Creates a new notifier.
 *
 * Creates a notifier with flags in the current context. The notifier will belong to the current
 * context if created successfully.
 *
 * The @p flags that you can specify include:
 *
 * - CN_NOTIFIER_DEFAULT:
 *
 *   Default event creation flag. The created notifier will record timing data.
 *
 * - CN_NOTIFIER_DISABLE_TIMING_SW:
 *
 *   Specifies that the created notifier does not record software timing data.
 *
 * - CN_NOTIFIER_DISABLE_TIMING_ALL:
 *
 *   Specifies that the created notifier does not record any timing data. Notifier created
 *   with this flag specified will provide the best performance.
 *
 * - CN_NOTIFIER_INTERPROCESS:
 *
 *   Specifies that the created notifier can be used as an interprocess notifier by
 * ::cnIpcGetNotifierHandle.
 * ::CN_NOTIFIER_INTERPROCESS must be specified along with ::CN_NOTIFIER_DISABLE_TIMING_ALL.
 *
 * @param[out] phnotifier The new created notifier handle.
 * @param[in]   flags Notifier creation flags.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY,
 * ::CN_OPS_ERROR_NOT_SUPPORTED,
 * ::CN_OPS_ERROR_OUT_OF_RESOURCES
 *
 * @note
 * - You can manage the lifecycle of the notifier. Call ::cnDestroyNotifier() to destroy the
 * notifier.
 */
__CN_EXPORT extern CNresult cnCreateNotifier(CNnotifier *phnotifier, unsigned int flags);
// Group: Notifier Management
/**
 * @brief Destroys a notifier.
 *
 * A notifier may be destroyed before it is completed (for example, while
 * ::cnQueryNotifier() would return CN_ERROR_NOT_READY). In this case, the
 * call does not block on completion of the notifier, and any associated
 * resources will automatically be released asynchronously at completion.
 *
 * @param[in] hnotifier The notifier handle created by calling ::cnCreateNotifier().
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_NOTIFIER_ERROR_INVALID
 *
 * @note
 * - This interface is protected inside the API to ensure resources can be released normally.
 * For better performance, complete all tasks in a notifier before calling ::cnDestroyNotifier().
 * Attention should be paid to the asynchronous interface.
 */
__CN_EXPORT extern CNresult cnDestroyNotifier(CNnotifier hnotifier);
// Group: Notifier Management
/**
 * @brief Waits until the specified notifier object is in the signaled state or exceeds the time-out interval.
 *
 * Makes all future tasks submitted to queue wait for all tasks captured in
 * hnotifier. See ::cnPlaceNotifier() for details on what is captured by a
 * hnotifier.
 *
 * @param[in] hnotifier The notifier handle created by calling ::cnCreateNotifier().
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_NOTIFIER_ERROR_INVALID,
 * ::CN_OPS_ERROR_TIMEOUT
 */
__CN_EXPORT extern CNresult cnWaitNotifier(CNnotifier hnotifier);
// Group: Notifier Management
/**
 * @brief Queries the status of the notifier.
 *
 * Queries the status of all work currently captured by hnotifier. See
 * ::cnPlaceNotifier() for details on what is captured by a @p hnotifier.
 *
 * @param[in] hnotifier The notifier handle created by calling ::cnCreateNotifier().
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_NOTIFIER_ERROR_INVALID,
 * ::CN_ERROR_NOT_READY
 *
 * @note
 * - The API returns 1, which means the notifier has not been executed yet.
 */
__CN_EXPORT extern CNresult cnQueryNotifier(CNnotifier hnotifier);
// Group: Notifier Management
/**
 * @brief Places a notifier in specified queue.
 *
 * The @p hnotifier and @p hqueue must be created by the same context.
 * This API is an asynchronous interface,
 * and ::CN_SUCCESS does not represent the completion of notifier execution.
 * Call ::cnQueryNotifier() or ::cnWaitNotifier() to confirm whether the notifier in a queue has
 * been executed.
 *
 * @param[in] hnotifier The notifier handle created by calling ::cnCreateNotifier().
 * @param[in] hqueue The queue handle created by calling ::cnCreateQueue() or
 * ::cnCreateQueueWithPriority().
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_NOTIFIER_ERROR_INVALID,
 * ::CN_QUEUE_ERROR_INVALID,
 * ::CN_OPS_ERROR_NOT_PERMITTED
 */
__CN_EXPORT extern CNresult cnPlaceNotifier(CNnotifier hnotifier, CNqueue hqueue);
// Group: Notifier Management
/**
 * @brief Places a notifier in specified queue with flags.
 *
 * @p hnotifier and @p hqueue must be created by the same context.
 * This API is an asynchronous interface,
 * and ::CN_SUCCESS does not represent the completion of notifier execution.
 * Call ::cnQueryNotifier() or ::cnWaitNotifier() to confirm whether the notifier in a queue has
 * been executed.
 *
 * @param[in] hnotifier The notifier handle created by calling ::cnCreateNotifier().
 * @param[in] hqueue The queue handle created by calling ::cnCreateQueue() or
 * ::cnCreateQueueWithPriority().
 * @param[in] flags See ::CNNotifierPlaceFlags.
 * The flags include:
 * - ::CN_NOTIFIER_PLACE_DEFAULT: Default flag.
 * - ::CN_NOTIFIER_PLACE_EXTERNAL: Notifier is captured in the TaskTopo as an external notifier
 * place node when performing queue capture. This flag is invalid outside of queue capture.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_NOTIFIER_ERROR_INVALID,
 * ::CN_QUEUE_ERROR_INVALID,
 * ::CN_OPS_ERROR_NOT_PERMITTED
 */
__CN_EXPORT CNresult cnPlaceNotifierWithFlags(CNnotifier hnotifier, CNqueue hqueue,
                                              unsigned int flags);
// Group: Notifier Management
/**
 * @brief Computes the elapsed time between two notifiers.
 *
 * Each notifier records a global timestamp when it is executed.
 * The elapsed time is the duration between the timestamps of @p hend and @p hstart.
 * If either notifier is created with ::CN_NOTIFIER_DISABLE_TIMING_SW or
 * ::CN_NOTIFIER_DISABLE_TIMING_ALL flag, this function returns
 * ::CN_NOTIFIER_ERROR_INVALID.
 *
 * @param[out] pMilliseconds The duration between @p hend and @p hstart in ms.
 * @param[in]  hstart The handle of the start notifier created by ::cnCreateNotifier().
 * @param[in]  hend The handle of the end notifier created by ::cnCreateNotifier().
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_NOTIFIER_ERROR_INVALID,
 * ::CN_ERROR_NOT_READY
 *
 * @note
 * - @p hstart and @p hend must be created by the same context.
 */
__CN_EXPORT extern CNresult cnNotifierElapsedTime(float *pMilliseconds, CNnotifier hstart,
                                                  CNnotifier hend);
// Group: Notifier Management
/**
 * @brief Computes the total kernel hardware execution time between two notifiers in one queue.
 *
 * Each notifier records a hardware timestamp when it is executed.
 * The elapsed time is the hardware execution time difference between timesatmps of @p hend and @p
 * hstart. If either notifier is created with ::CN_NOTIFIER_DISABLE_TIMING_ALL flag, this function
 * returns ::CN_NOTIFIER_ERROR_INVALID.
 *
 * @param[out] pMilliseconds The duration between timestamps of @p hend and @p hstart in ms.
 * @param[in]  hstart The handle of the start notifier created by ::cnCreateNotifier().
 * @param[in]  hend The handle of the end notifier created by ::cnCreateNotifier().
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_NOTIFIER_ERROR_INVALID,
 * ::CN_ERROR_NOT_READY
 *
 * @note
 * - Only kernel task guarantees that hardware execution time is recorded.
 * - @p hstart and @p hend must be created by the same context.
 * - @p hstart and @p hend must be placed in same queue by calling ::cnPlaceNotifier.
 */
__CN_EXPORT extern CNresult cnNotifierElapsedExecTime(float *pMilliseconds, CNnotifier hstart,
                                                      CNnotifier hend);
// Group: Notifier Management
/**
 * @brief Gets an interprocess handle for a previously allocated notifier.
 * This handle may be copied into other processes and opened with ::cnIpcOpenNotifierHandle
 *
 * @param[out] phandle    Pointer to a user allocated ::CNIpcNotifierHandle in which to return the event ipc handle.
 * @param[in]  hnotifier  The notifier handle created by calling ::cnCreateNotifier
 *     with ::CN_NOTIFIER_INTERPROCESS and ::CN_NOTIFIER_DISABLE_TIMING_ALL.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_HANDLE,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY,
 * ::CN_ERROR_INVALID_ADDRESS,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_NOTIFIER_ERROR_INVALID,
 * ::CN_OPS_ERROR_NOT_SUPPORTED,
 * ::CN_OPS_ERROR_NOT_PERMITTED
 *
 * @note
 * - The @p phandle will be invalid after you destroy the @p hnotifier by calling
 * ::cnDestroyNotifier.
 * - The @p phandle will be invalid after you destroy the context by calling ::cnCtxDestroy that
 * contains the @p hnotifier.
 * - The new notifier allocated by ::cnIpcOpenNotifierHandle may be invalid if @p hnotifier has
 * been freed with ::cnDestroyNotifier.
 *
 */
__CN_EXPORT extern CNresult cnIpcGetNotifierHandle(CNIpcNotifierHandle *phandle,
                                                   CNnotifier hnotifier);
// Group: Notifier Management
/**
 * @brief Opens an interprocess notifier handle for use in the current process.
 * This API returns a new notifier handle like a locally created event with
 * ::CN_NOTIFIER_DISABLE_TIMING_ALL flag. This notifier must be freed with ::cnDestroyNotifier.
 *
 * @param[in]  handle      Interprocess handle got from ::cnIpcGetNotifierHandle.
 * @param[out] phnotifier  New notifier handle.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_HANDLE,
 * ::CN_ERROR_FILE_NOT_FOUND,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY,
 * ::CN_ERROR_OPERATING_SYSTEM,
 * ::CN_ERROR_COMPAT_NOT_SUPPORT_ON_SYSTEM,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_NOTIFIER_ERROR_INVALID,
 * ::CN_OPS_ERROR_NOT_SUPPORTED,
 * ::CN_OPS_ERROR_NOT_PERMITTED
 *
 * @note
 * - The @p hnotifier which is allocated by ::cnIpcOpenNotifierHandle cannot use
 * ::cnIpcGetNotifierHandle to get a new handle.
 *
 */
__CN_EXPORT extern CNresult cnIpcOpenNotifierHandle(CNnotifier *phnotifier,
                                                    CNIpcNotifierHandle handle);

/******************************************************************************
 * Atomic Operation Management
 ******************************************************************************/
// Group: Atomic Operation Management
/**
 * @brief Invokes atomic operation to @p hqueue.
 *
 * Atomic task will change @p opPtr value or wait @p opPtr value by type and flag.
 *
 * @param[in] hqueue The handle of a queue.
 * @param[in] opPtr The operation user address pointer, which shall be allocated from ::cnMallocHost().
 * @param[in] opData1 The operation data.
 * @param[in] opData2 Reserved.
 * @param[in] type The operation type.
 * @param[in] flag The operation flag ::CNAtomicCompFlag or ::CNAtomicReqFlag.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_QUEUE_ERROR_INVALID,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_INVALID_ADDRESS,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_QUEUE_ERROR_INVALID
 *
 * @note
 * - Do not access the @p opPtr address if it has been passed to ::cnQueueAtomicOperation.
 *   It is not allowed to directly read/write this @p opPtr address.
 *   You can read the latest value of @p opPtr address by ::cnAtomicReadOps or update
 *   new value by ::cnAtomicOperation.
 * - Do not call ::cnFreeHost to free @p opPtr address while this address still
 *   has ::cnQueueAtomicOperation task in queue. Otherwise, this may cause queue exception.
 */
__CN_EXPORT extern CNresult cnQueueAtomicOperation(CNqueue hqueue, cn_uint64_t opPtr,
                                                   cn_uint64_t opData1, cn_uint64_t opData2,
                                                   CNAtomicOpType_t type, cn_uint64_t flag);
// Group: Atomic Operation Management
/**
 * @brief Does atomic operation immediately.
 *
 * The task will change @p opPtr value if the type is ::CN_ATOMIC_OP_REQUEST,
 * and wake up all tasks with the same @p opPtr by ::cnQueueAtomicOperation().
 * The task will return opPtr status if the type is ::CN_ATOMIC_OP_COMPARE.
 * This API will not block.
 *
 * @param[in] opPtr The operation user address pointer, which shall be allocated from ::cnMallocHost().
 * @param[in] opData1 The operation data.
 * @param[in] opData2 Reserved.
 * @param[in] type The operation type.
 * @param[in] flag The operation flag ::CNAtomicCompFlag or ::CNAtomicReqFlag.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_NOT_READY,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_INVALID_ADDRESS,
 * ::CN_CONTEXT_ERROR_INVALID
 *
 */
__CN_EXPORT extern CNresult cnAtomicOperation(cn_uint64_t opPtr, cn_uint64_t opData1,
                                              cn_uint64_t opData2, CNAtomicOpType_t type,
                                              cn_uint64_t flag);
// Group: Atomic Operation Management
/**
 * @brief Reads input @p opPtr address value.
 *
 * This API will return immediately and will not be blocked by other
 * asynchronous interfaces.
 *
 * @param[in] opPtr The operation user address pointer, which shall be allocated from ::cnMallocHost().
 * @param[out] value Reads value from @p opPtr.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_ADDRESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID
 */
__CN_EXPORT extern CNresult cnAtomicReadOps(cn_uint64_t opPtr, cn_uint64_t *value);

/******************************************************************************
 * Module Management
 ******************************************************************************/
// Group: Module Management
/**
 * @brief Loads a Cambricon BANG fat binary.
 *
 * Takes a fat binary and loads corresponding module into the current context.
 *
 * @param[in] fatBin The pointer to Cambricon BANG fat binary data.
 * @param[out] phmod The module.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_FATBIN_ERROR_NO_KERNEL,
 * ::CN_FATBIN_ERROR_UNSUPPORTED_VERSION,
 * ::CN_FATBIN_LIBCNBIN_NOT_FOUND,
 * ::CN_FATBIN_INVALID_IMAGE,
 * ::CN_FATBIN_INVALID_ARCH,
 * ::CN_ERROR_FILE_NOT_FOUND
 *
 * @note
 * - This API does not attempt to lazily allocate the resources needed
 * by a module.
 */
__CN_EXPORT extern CNresult cnModuleLoadFatBinary(const void *fatBin, CNmodule *phmod);
// Group: Module Management
/**
 * @brief Loads a Cambricon BANG fat binary from File.
 *
 * Takes a fat binary file name and loads corresponding module into the current context.
 *
 * @param[in] fname The fat binary file name.
 * @param[out] phmod The module.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_FATBIN_ERROR_NO_KERNEL,
 * ::CN_FATBIN_ERROR_UNSUPPORTED_VERSION,
 * ::CN_FATBIN_LIBCNBIN_NOT_FOUND,
 * ::CN_FATBIN_INVALID_IMAGE,
 * ::CN_FATBIN_INVALID_ARCH,
 * ::CN_ERROR_FILE_NOT_FOUND
 *
 * @note
 * - This API does not attempt to lazily allocate the resources needed
 * by a module.
 */
__CN_EXPORT extern CNresult cnModuleLoad(const char *fname, CNmodule *phmod);

// Group: Module Management
/**
 * @brief Unloads a module.
 *
 * Unloads a module from the current context.
 * It will return error code if the module does not belong to the current context.
 *
 * @param[in] hmod The handle of module to unload.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID
 */
__CN_EXPORT extern CNresult cnModuleUnload(CNmodule hmod);

// Group: Module Management
/**
 * @brief Gets the device memory usage of a Cambricon BANG fat binary file without loading it to device.
 *
 * @param[in] path     The Cambricon BANG fat bianry file path.
 * @param[out] pbytes  The returned device memory usage in bytes.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_ERROR_OPERATING_SYSTEM,
 * ::CN_ERROR_FILE_NOT_FOUND,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY
 *
 * @note
 * - This API returns the device memory size of all kernels in fat binary file, even if the loading
 * mode is ``lazy``.
 */
__CN_EXPORT extern CNresult cnModuleQueryMemoryUsage(const char *path, size_t *pbytes);

// Group: Module Management
/**
 * @brief Gets the device memory usage of a Cambricon BANG fat binary without loading it to device.
 *
 * @param[in] fatbin   The Cambricon BANG fat binary data.
 * @param[out] pbytes  The returned device memory usage in bytes.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY
 *
 * @note
 * - This API returns the device memory size of all kernels in fat binary file, even if the loading
 * mode is ``lazy``.
 */
__CN_EXPORT extern CNresult cnModuleQueryFatBinaryMemoryUsage(const void *fatbin, size_t *pbytes);

// Group: Module Management
/**
 * @brief Returns a kernel handle by module and kernel's name.
 *
 * Gets kernel handle by module and kernel's name.
 *
 * @param[in] hmod The module to extract kernel from.
 * @param[in] name The name of kernel to extract.
 * @param[out] phkernel The kernel handle.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_ERROR_NOT_FOUND
 */
__CN_EXPORT extern CNresult cnModuleGetKernel(CNmodule hmod, const char *name, CNkernel *phkernel);

// Group: Module Management
/**
 * @brief Returns a symbol address from a module.
 *
 * Returns the base address and size of the symbol with the symbol name that is located in the
 * module. If no variable of that name exists, ::cnModuleGetSymbol() returns ::CN_ERROR_NOT_FOUND.
 * Both parameters pdptr and pbytes are optional. If one of them is
 * NULL, it is ignored.
 *
 * @param[in] hmod The module to extract symbol from.
 * @param[in] name The name of symbol to extract.
 * @param[out] pdptr The device pointer of symbol.
 * @param[out] pbytes The symbol size in bytes.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_OPS_ERROR_NOT_SUPPORTED,
 * ::CN_ERROR_NOT_FOUND
 */
__CN_EXPORT extern CNresult cnModuleGetSymbol(CNmodule hmod, const char *name, CNaddr *pdptr,
                                              cn_uint64_t *pbytes);
// Group: Module Management
/**
 * @brief Returns information about a kernel.
 *
 * Returns the integer value the attribute about a kernel.
 * See ::CNkernel_attribute for supported attributes.
 *
 * @param[out] pi Returned attribute value.
 * @param[in]  attrib Attribute requested.
 * @param[in]  hkernel Kernel to query attribute of.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE
 */
__CN_EXPORT extern CNresult cnKernelGetAttribute(cn_int64_t *pi, CNkernel_attribute attrib,
                                                 CNkernel hkernel);

// Group: Module Management
/**
 * @brief Queries loading mode of a module.
 *
 * This function returns the loading mode of a module,
 * which is controlled by the environment variable CN_MODULE_LOADING.
 *
 * @param[out] mode The loading mode.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE
 */
__CN_EXPORT extern CNresult cnModuleGetLoadingMode(CNmoduleLoadingMode *mode);

/******************************************************************************
 * Execution Control Management
 ******************************************************************************/
// Group: Execution Control Management
/**
 * @brief Invokes a kernel.
 *
 * This API is an asynchronous interface and ::CN_SUCCESS does not
 * represent the completion of kernel execution.
 * Call ::cnQueryQueue() or ::cnQueueSync() to confirm whether the kernel in queue has been
 * executed. The memory resources involved in this kernel can only be released after it is executed.
 *
 * @param[in] hkernel The handle of kernel to launch.
 * @param[in] dimx The dimension of x.
 * @param[in] dimy The dimension of y.
 * @param[in] dimz The dimension of z.
 * @param[in] c The union type of kernel.
 * @param[in] reserve Reserved parameter.
 * @param[in] hqueue The handle of a queue.
 * @param[in] kernelParams The array of pointers to kernel parameters.
 * @param[in] extra Extra options, such as packaged parameters.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_QUEUE_ERROR_INVALID,
 * ::CN_INVOKE_ERROR_OUT_OF_RESOURCES,
 * ::CN_INVOKE_ERROR_EXECUTED_TIMEOUT,
 * ::CN_INVOKE_ERROR_ILLEGAL_INSTRUCTION,
 * ::CN_INVOKE_ERROR_ADDRESS_SPACE,
 * ::CN_INVOKE_ERROR_TIMEOUT,
 * ::CN_ERROR_INVALID_HANDLE,
 * ::CN_MEMORY_ERROR_OUT_OF_MEMORY
 */
__CN_EXPORT extern CNresult cnInvokeKernel(CNkernel hkernel, unsigned int dimx, unsigned int dimy,
                                           unsigned int dimz, KernelClass c, unsigned int reserve,
                                           CNqueue hqueue, void **kernelParams, void **extra);
// Group: Execution Control Management
/**
 * @brief Invokes a kernel with configuration.
 *
 * This API is an asynchronous interface and ::CN_SUCCESS does not
 * represent the completion of kernel execution.
 * Call ::cnQueryQueue() or ::cnQueueSync() to confirm whether the kernel in queue has been
 * executed. The memory resources involved in this kernel can only be released after it is executed.
 *
 * @param[in] hkernel The handle of kernel to launch.
 * @param[in] config The configuration to invoke.
 * @param[in] kernelParams The array of pointers to kernel parameters.
 * @param[in] extra Extra options, such as packaged parameters.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_QUEUE_ERROR_INVALID,
 * ::CN_INVOKE_ERROR_OUT_OF_RESOURCES,
 * ::CN_INVOKE_ERROR_EXECUTED_TIMEOUT,
 * ::CN_INVOKE_ERROR_ILLEGAL_INSTRUCTION,
 * ::CN_INVOKE_ERROR_ADDRESS_SPACE,
 * ::CN_INVOKE_ERROR_TIMEOUT,
 * ::CN_ERROR_INVALID_HANDLE,
 * ::CN_MEMORY_ERROR_OUT_OF_MEMORY
 */
__CN_EXPORT extern CNresult cnInvokeKernelEx(const CNinvokeConfig *config, CNkernel hkernel,
                                             void **kernelParams, void **extra);

// Group: Execution Control Management
/**
 * @brief Enqueues a host function call in a queue.
 *
 * This API is an asynchronous interface and ::CN_SUCCESS does not
 * represent the completion of host function execution.
 * Call ::cnQueryQueue() or ::cnQueueSync() to confirm whether the host function call in the queue
 * has been executed. The host function call must not make any CNRT or CNDrv API call. Attempting
 * to use CNRT and CNDrv API may result in ::CN_OPS_ERROR_NOT_PERMITTED. The host function call must
 * not perform any synchronization that may depend on other asynchronous tasks not mandated to run
 * earlier, otherwise, it can cause a deadlock.
 * Without a mandated order, host function is executed (in independent queues) in undefined order
 * and may be serialized.
 *
 * @param[in] queue The handle of a queue.
 * @param[in] fn The function to call once preceding operations are complete.
 * @param[in] userData User-specified data that is passed to the function.
 * @return
 * ::CN_SUCCESS,
 * ::CN_OPS_ERROR_NOT_PERMITTED,
 * ::CN_INVOKE_ERROR_OUT_OF_RESOURCES,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY,
 * ::CN_ERROR_NOT_INITIALIZED
 */
__CN_EXPORT extern CNresult cnInvokeHostFunc(CNqueue queue, CNhostFn fn, void *userData);

/** @cond impl_private */
// Group: Impl Private
__CN_EXPORT extern CNresult cnGetExportFunction(const char *name, const void **func);
/** @endcond */

// Group: Memory Management
/**
 * @brief Flushes or invalidates cache on the host.
 *
 * @param[in] mluAddr Reserved for checking the device address legality.
 * @param[in] hostAddr Host memory address to do cache operation.
 * @param[in] size The number of bytes to do cache operation.
 * @param[in] ops The operation type on cache.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_OPS_ERROR_NOT_SUPPORTED,
 * ::CN_ERROR_INVALID_VALUE
 */
__CN_EXPORT extern CNresult cnCacheOperation(CNaddr mluAddr, void *hostAddr, cn_uint64_t size,
                                             CNCacheOps ops);
// Group: Memory Management
/**
 * @brief Maps the range of device memory address into the uncached address space of host.
 *
 * @param[in]  mluAddr Device address to map.
 * @param[out] phostAddr Pointer to the host memory address to be mapped into.
 * @param[in]  size The size of the memory in bytes to be mapped into.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS,
 * ::CN_OPS_ERROR_NOT_SUPPORTED
 */
__CN_EXPORT extern CNresult cnMmap(CNaddr mluAddr, void **phostAddr, cn_uint64_t size);
// Group: Memory Management
/**
 * @brief Maps the range of device memory address into the cached address space of host.
 *
 * @param[in]  mluAddr Device memory address to map.
 * @param[out] phostAddr Pointer to the host memory address to be mapped into.
 * @param[in]  size The size of the memory in bytes to be mapped into.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS,
 * ::CN_OPS_ERROR_NOT_SUPPORTED
 */
__CN_EXPORT extern CNresult cnMmapCached(CNaddr mluAddr, void **phostAddr, cn_uint64_t size);
// Group: Memory Management
/**
 * @brief Unmaps the host memory address. The mapping is created by ::cnMmap or ::cnMmapCached.
 *
 * @param[in] hostAddr Host memory address to be unmapped.
 * @param[in] size The size to unmap in bytes.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS,
 * ::CN_OPS_ERROR_NOT_SUPPORTED
 */
__CN_EXPORT extern CNresult cnMunmap(void *hostAddr, cn_uint64_t size);

// Group: Memory Management
/**
 * @brief Retrieves the handle for an address range.
 *
 * @param[out] handle Pointer to the location where the returned handle will be stored.
 * @param[in] dptr Pointer to a valid MLU device allocation. It must be aligned to host page size.
 * @param[in] size Length of the address range. It must be aligned to host page size.
 * @param[in] handleType Type of handle requested, which defines the type and size of the handle output parameter.
 * @param[in] flags Reserved. Now it must be zero.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_INVALID_ADDRESS,
 * ::CN_OPS_ERROR_NOT_SUPPORTED,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS,
 * ::CN_CONTEXT_ERROR_INVALID
 *
 * @note
 * - The address range must have been obtained by a prior call to either ::cnMalloc()
 * - When requesting the ::CN_MEM_RANGE_HANDLE_TYPE_DMA_BUF_FD handle type, users are expected to
 *   query for dma_buf support for the platform by using the ::CN_DEVICE_ATTRIBUTE_DMA_BUF_SUPPORTED
 *   device attribute before calling this API.
 * - The handle will be interpreted as a pointer to an integer to store the dma_buf file descriptor.
 */
__CN_EXPORT extern CNresult cnMemGetHandleForAddressRange(void *handle, CNaddr dptr, size_t size,
                                                          CNmemRangeHandleType handleType,
                                                          cn_uint64_t flags);

/******************************************************************************
 * Virtual Memory management
 ******************************************************************************/
// Group: Virtual Memory Management
/**
 * @brief Creates an MLU memory handle of given size and given properties; the handle represents a memory allocation.
 *
 * Creates a memory allocation on the target device specified by ::CNmemAllocationProp. The created
 * allocation does not have any device or host mappings. The generic memory handle can be mapped to
 * the virtual address space of calling process via ::cnMemMap(), while it cannot be transmitted
 * directly to other processes (see ::cnMemExportToShareableHandle()). The size of the allocation
 * must be the multiple of the ::CN_MEM_ALLOC_GRANULARITY_MINIMUM value of
 * ::cnMemGetAllocationGranularity().
 *
 * @param[out] phandle The value of the handle returned. All operations on this allocation are to be performed using this handle.
 * @param[in] size The size of the allocation requested in bytes.
 * @param[in] prop The properties of the allocation to create.
 * @param[in] flags The flags for future use, whose value must be zero now.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_MEMORY_ERROR_OUT_OF_MEMORY,
 * ::CN_ERROR_INVALID_DEVICE,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_OPS_ERROR_NOT_SUPPORTED
 */
__CN_EXPORT extern CNresult cnMemCreate(CNmemGenericAllocationHandle *phandle, cn_uint64_t size,
                                        CNmemAllocationProp *prop, cn_uint64_t flags);

// Group: Virtual Memory Management
/**
 * @brief Releases an MLU memory handle representing a memory allocation, which was previously
 *        created by ::cnMemCreate().
 *
 * The memory allocation represented by the handle will be freed when all mappings to the memory
 * are unmapped and when all references to the handle (including it's shareable counterparts) are
 * also released. Each time a consumer process imports a shareable handle, it needs to pair the
 * handle with ::cnMemRelease() to free the handle.
 *
 * @param[in] handle The returned value of the handle, which was previously created by ::cnMemCreate().
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_OPS_ERROR_NOT_SUPPORTED
 */
__CN_EXPORT extern CNresult cnMemRelease(CNmemGenericAllocationHandle handle);

// Group: Virtual Memory Management
/**
 * @brief Reserves a virtual MLU address range.
 *
 * Reserves a virtual MLU address range based on the given parameters. The @p size parameter
 * must be a multiple of the default alignment and the @p address parameter must be aligned with the
 * default alignment in ::CN_MEM_ALLOC_GRANULARITY_MINIMUM of ::cnMemGetAllocationGranularity(). The
 * @p alignment parameter must be a power of two of the default alignment or 0.
 *
 * @param[out] pmluAddr The starting virtual address of the range allocated.
 * @param[in]  size The size of the reserved virtual address range requested.
 * @param[in]  alignment The alignment of the reserved virtual address range requested.
 * @param[in]  start The fixed starting virtual address of the range requested.
 * @param[in]  flags The flags for future use, whose value must be zero now.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_MEMORY_ERROR_OUT_OF_MEMORY,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_OPS_ERROR_NOT_SUPPORTED
 */
__CN_EXPORT extern CNresult cnMemAddressReserve(CNaddr *pmluAddr, cn_uint64_t size,
                                                cn_uint64_t alignment, CNaddr start,
                                                cn_uint64_t flags);

// Group: Virtual Memory Management
/**
 * @brief Frees a virtual address range reserved by ::cnMemAddressReserve().
 *
 * @param[in] mluAddr The starting virtual address of the range to free, which
 *                    must match that returned from ::cnMemAddressReserve().
 * @param[in] size The size of the virtual address range to free, which must
 *                 match that given in ::cnMemAddressReserve().
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_OPS_ERROR_NOT_SUPPORTED
 */
__CN_EXPORT extern CNresult cnMemAddressFree(CNaddr mluAddr, cn_uint64_t size);

// Group: Virtual Memory Management
/**
 * @brief Maps a memory handle to a reserved virtual address range.
 *
 * Maps the [offset , offset + size) range of the handle related MLU memory to the virtual address
 * range [mluAddr, mluAddr + size). Wherein, the [mluAddr, mluAddr + size) is the previously
 * reserved range by calling ::cnMemAddressReserve().
 *
 * Neither the @p offset nor the @p offset + @p size shall exceed the MLU * memory size applied by
 * calling ::cnMemCreate(). @p mluAddr and @p offset must be aligned with
 * ::CN_MEM_ALLOC_GRANULARITY_MINIMUM value of ::cnMemGetAllocationGranularity(),
 * and @p size must be the multiple of ::CN_MEM_ALLOC_GRANULARITY_MINIMUM value of
 * ::cnMemGetAllocationGranularity().
 *
 * Once a consumer process obtains a shareable memory handle from
 * ::cnMemImportFromShareableHandle(), the process must use ::cnMemMap() to map the memory into its
 * virtual address ranges before setting accessibility with ::cnMemSetAccess().
 *
 * ::cnMemMap() can only create mappings on virtual address range reservations that are not
 * currently mapped.
 *
 * @param[in] mluAddr The address where the memory will be mapped with.
 * @param[in] size The size of the memory mapping in bytes.
 * @param[in] offset The offset into the memory represented by the handle from which to start
 *                   mapping, which must be zero currently.
 * @param[in] handle The handle to a shareable MLU memory.
 * @param[in] flags The flags for future use, whose value must be zero now.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_OPS_ERROR_NOT_SUPPORTED
 *
 * @note
 * - Calling ::cnMemMap() does not make the address accessible,
 * you need to update accessibility of a contiguous mapped virtual address
 * range by calling ::cnMemSetAccess().
 *
 */
__CN_EXPORT extern CNresult cnMemMap(CNaddr mluAddr, cn_uint64_t size, cn_uint64_t offset,
                                     CNmemGenericAllocationHandle handle, cn_uint64_t flags);

// Group: Virtual Memory Management
/**
 * @brief Sets the access flags for location specified in @p desc for the given virtual address range.
 *
 * Given the virtual address range via mluAddr and size, and the location in the desc, set the
 * access flags for the target locations. The range must be a fully mapped address range
 * created by ::cnMemMap().
 *
 * @param[in] mluAddr The starting address of the virtual address range.
 * @param[in] size The length in bytes of the virtual address range.
 * @param[in] desc The array of ::CNmemAccessDesc that describes how to change
 *                 the mapping for each location specified.
 * @param[in] count The number of ::CNmemAccessDesc in desc, which must be one currently.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS,
 * ::CN_OPS_ERROR_NOT_SUPPORTED
 */
__CN_EXPORT extern CNresult cnMemSetAccess(CNaddr mluAddr, cn_uint64_t size, CNmemAccessDesc *desc,
                                           cn_uint64_t count);

// Group: Virtual Memory Management
/**
 * @brief Unmaps the mapped MLU memory of a given virtual address range.
 *
 * The range [ @p mluAddr, @p mluAddr + @p size ) must be the entire contiguous address range that
 * was mapped to. In other words, ::cnMemUnmap() cannot unmap a sub-range of an address range mapped
 * by ::cnMemMap(). Any MLU memory allocation will be freed if there are neither the existing
 * mappings nor unreleased memory handles.
 *
 * When ::cnMemUnmap() returns successfully, the address range is converted to an
 * address reservation and can be used for future calls to ::cnMemMap(). Any new
 * mapping to this virtual address will need to have access granted through
 * ::cnMemSetAccess(), as all mappings start with no accessibility setup.
 *
 * @param[in] mluAddr The starting address for the virtual address range to unmap.
 * @param[in] size The size of the virtual address range to unmap.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_OPS_ERROR_NOT_SUPPORTED
 */
__CN_EXPORT extern CNresult cnMemUnmap(CNaddr mluAddr, cn_uint64_t size);

// Group: Virtual Memory Management
/**
 * @brief Calculates either the minimal or recommended granularity.
 *
 * Calculates either the minimal or recommended granularity for a given
 * allocation specification and returns it in granularity. This granularity
 * can be used as a multiple for alignment, size, or address mapping.
 *
 * @param[out] granularity The value of the granularity returned.
 * @param[in]  prop The properties that determine the granularity.
 * @param[in]  options The option to determine the granularity returned.
 *
 * @note
 * - The size parameter in virtual address management APIs like ::cnMemAddressReserve(),
 * ::cnMemCreate() and so on, must be a multiple of the granularity returned from
 * ::cnMemGetAllocationGranularity().
 * - The address parameter in virtual address management APIs like ::cnMemAddressReserve(),
 * ::cnMemCreate() and so on, must be aligned with the granularity returned from
 * ::cnMemGetAllocationGranularity().
 * - The alignment parameter in virtual address management APIs like ::cnMemAddressReserve(), must
 * be power of two of the granularity returned from ::cnMemGetAllocationGranularity() or zero.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_OPS_ERROR_NOT_SUPPORTED
 */
__CN_EXPORT extern CNresult cnMemGetAllocationGranularity(cn_uint64_t *granularity,
                                                          CNmemAllocationProp *prop,
                                                          CNmemAllocationGranularity options);

// Group: Virtual Memory Management
/**
 * @brief Retrieves the @p prop struct defining properties for the handle input.
 *
 * @param[out]  prop The properties to hold the information about the handle input.
 * @param[in]  handle The handle to perform the query on.
 *
 * @returns
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_OPS_ERROR_NOT_SUPPORTED
 */
__CN_EXPORT extern CNresult cnMemGetAllocationPropertiesFromHandle(
    CNmemAllocationProp *prop, CNmemGenericAllocationHandle handle);

// Group: Virtual Memory Management
/**
 * @brief Gets the access descriptor @p desc set for the given @p location and @p ptr.
 *
 * @param[out]  desc The descriptor to hold the access flags about the pointer input.
 * @param[in]  location The location to check the access flags.
 * @param[in]  ptr The pointer to check the access flags.
 *
 * @returns
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_OPS_ERROR_NOT_SUPPORTED
 */
__CN_EXPORT extern CNresult cnMemGetAccess(CNmemAccessDesc *desc, const CNmemLocation *location,
                                           void *ptr);

// Group: Virtual Memory Management
/**
 * @brief Returns an MLU memory handle representing a memory allocation, which is mapped with @p addr.
 *
 * The handle is guaranteed to be the same handle value used to map the memory. If the address input
 * is not mapped, the function will fail. The returned handle must be released with corresponding
 * number of calls to ::cnMemRelease().
 *
 * @param[out]  handle The handle holds the MLU memory handle mapped with @p addr input.
 * @param[in]  addr The virtual address to query on.
 *
 * @note
 * - The address @p addr can be any address in a range previously mapped by ::cnMemMap(), not
 * necessarily the start address.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_OPS_ERROR_NOT_SUPPORTED
 */
__CN_EXPORT extern CNresult cnMemRetainAllocationHandle(CNmemGenericAllocationHandle *handle,
                                                        CNaddr addr);

// Group: Virtual Memory Management
/**
 * @brief Exports an allocation to the shareable passed-in handle of a specific type.
 *
 * Creates a memory allocation handle that can share the memory with other processes based on a
 * given MLU memory handle. The recipient process can convert the shareable handle to an MLU memory
 * handle with ::cnMemImportFromShareableHandle() and map it with ::cnMemMap(). The implementation
 * of this handle and the way that it can be transferred are defined by the requested handle type in
 * @p handleType.
 *
 * Once all shareable handles are closed and all allocations are released, the requested memory can
 * be reclaimed.
 *
 * @param[out]  shareableHandle The pointer to the location in which to store the requested handle
type.
 * @param[in]  handle The handle for the MLU memory allocation.
 * @param[in]  handleType The type of the shareable handle requested, which defines the type and
size of @p shareableHandle.
 * @param[in]  flags Reserved. The value must be zero currently.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_OPS_ERROR_NOT_SUPPORTED
 */
__CN_EXPORT extern CNresult cnMemExportToShareableHandle(void *shareableHandle,
                                                         CNmemGenericAllocationHandle handle,
                                                         CNmemAllocationHandleType handleType,
                                                         cn_uint64_t flags);

// Group: Virtual Memory Management
/**
 * @brief Imports an allocation from the shareable passed-in handle of a specific type.
 *
 * @param[out]  handle The handle for the MLU memory allocation.
 * @param[in]  osHandle The shareable handle representing the memory allocation to be imported.
 * @param[in]  shHandleType The handle type of the exported handle ::CNmemAllocationHandleType.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_OPS_ERROR_NOT_SUPPORTED
 */
__CN_EXPORT extern CNresult cnMemImportFromShareableHandle(CNmemGenericAllocationHandle *handle,
                                                           void *osHandle,
                                                           CNmemAllocationHandleType shHandleType);

/******************************************************************************
 * Queue Ordered Memory Allocator
 ******************************************************************************/

// Group: Queue Ordered Memory Allocator
/**
 * @brief Allocates memory following the order determined by the queue.
 * @details  Places a memory allocation operation in ``hqueue``.
 * A pointer to the allocated memory returns in ``mluAddr`` right away,
 * but access to the allocated memory is restricted until the allocation operation is finished.
 * The allocation comes from the memory pool reserved for the queue on the device.
 * @param[out] mluAddr The allocated MLU memory.
 * @param[in] byteSize The number of bytes to allocate.
 * @param[in] hqueue The handle of a queue.
 *
 * @return
 * ::CN_SUCCESS
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY,
 * ::CN_OPS_ERROR_NOT_SUPPORTED,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS
 *
 * @note
 * - The memory pool of an MLU contains device memory from the MLU by default.
 * - The subsequent tasks submitted into the same queue can use the allocation according to the
 * basic queue ordering.
 * - Use queue query, queue synchronization, and notifier to guarantee that the allocation
 *   operation is completed before task submitted in a separate queue runs.
 *
 */
__CN_EXPORT extern CNresult cnMemAllocAsync(CNaddr *mluAddr, size_t byteSize, CNqueue hqueue);

// Group: Queue Ordered Memory Allocator
/**
 * @brief Allocates memory from a specified pool following the order determined by the queue.
 * @details Places a memory allocation operation in ``hqueue``.
 * A pointer to the allocated memory returns in ``mluAddr`` right away,
 * but access to the allocated memory is restricted until the allocation operation is finished.
 * The allocation comes from the memory pool reserved for the queue on the device.
 * @param[out] mluAddr The allocated MLU memory.
 * @param[in] byteSize The number of bytes to allocate.
 * @param[in] pool The memory pool from which to allocate the memory.
 * @param[in] hqueue The queue determining the order of queue.
 * @return
 * ::CN_SUCCESS
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY,
 * ::CN_OPS_ERROR_NOT_SUPPORTED,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS
 *
 * @note
 * - The specified memory pool may be from a device different than that of the specified @p hqueue.
 * - The subsequent tasks submitted into the same queue can use the allocation according to the
 * basic queue ordering.
 * - Use queue query, queue synchronization, and notifier to guarantee that the allocation
 *   operation is completed before task submitted in a separate queue runs.
 */
__CN_EXPORT extern CNresult cnMemAllocFromPoolAsync(CNaddr *mluAddr, size_t byteSize,
                                                    CNmemoryPool pool, CNqueue hqueue);

// Group: Queue Ordered Memory Allocator
/**
 * @brief  Frees memory following the order determined by the queue.
 * @details Places a free operation into ``hqueue``.
 * Access to the memory must cease once the queue reaches this free operation.
 * @param[out] mluAddr The memory to free.
 * @param[in] hqueue The queue establishing the queue ordering contract.
 *
 * @return
 * ::CN_SUCCESS
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY,
 * ::CN_OPS_ERROR_NOT_SUPPORTED,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS
 *
 * @note
 * - After calling ::cnMemFreeAsync, any access to the memory,
 *   on the device or querying its pointer attributes from subsequent tasks
 *   causes unexpected behavior.
 *
 */
__CN_EXPORT extern CNresult cnMemFreeAsync(CNaddr mluAddr, CNqueue hqueue);

// Group: Queue Ordered Memory Allocator
/**
 * @brief Establishes a memory pool.
 * @details Allocates resources and establishes a memory pool.
 *          The properties of the pool, for example, the backing and IPC capabilities, is defined by
 * ``poolProps``. The handle to this pool is stored in ``pool``.
 *
 * @param[out] pool The memory pool.
 * @param[in] poolProps The properties of allocations made from the pool.
 *
 * @return
 * ::CN_SUCCESS
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY,
 * ::CN_OPS_ERROR_NOT_SUPPORTED,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS
 *
 * @note
 * - The memory pool created with ::CN_MEM_HANDLE_TYPE_NONE does not
 *   support memory exporting.
 * - The memory of the pool is accessible from the device on which it is allocated, by default.
 * - The maximum size of the pool can be controlled by specifying a non-zero value for
 *   ::CNmemPoolProps::maxSize. If set to 0, the maximum size of the pool defaults to a
 *   system-dependent value.
 */
__CN_EXPORT extern CNresult cnMemPoolCreate(CNmemoryPool *pool, CNmemPoolProps *poolProps);

// Group: Queue Ordered Memory Allocator
/**
 * @brief Releases the reserved memory pool.
 * @details When invoked, this function immediately returns if there are any pointers from the pool
 * that have not  been freed or if there are pending free operations.
 * The associated resources will automatically be released once all outstanding allocations are
 * resolved.
 *
 * @param[in] pool      The memory pool.
 *
 * @return
 * ::CN_SUCCESS
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY,
 * ::CN_OPS_ERROR_NOT_SUPPORTED,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS
 *
 * @note
 * - The default memory pool of a device cannot be destroyed.
 * - Destroying a device's current memory pool automatically
 *   sets that device's default memory pool as its new current memory pool.
 */
__CN_EXPORT extern CNresult cnMemPoolDestroy(CNmemoryPool pool);

// Group: Queue Ordered Memory Allocator
/**
 * @brief Determines how accessible a memory is from a specific device.
 * @details Returns the degree of accessibility of a memory pool from a device.
 *
 * @param[out] flags The accessibility of a memory pool from the specified location.
 * @param[in] pool The memory pool being accessed.
 * @param[in] location The location trying to access the pool.
 *
 * @return
 * ::CN_SUCCESS
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY,
 * ::CN_OPS_ERROR_NOT_SUPPORTED,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS
 */
__CN_EXPORT extern CNresult cnMemPoolGetAccess(CNMemoryAccessFlags *flags, CNmemoryPool pool,
                                               CNmemLocation *location);

// Group: Queue Ordered Memory Allocator
/**
 * @brief Retrieves attributes of a memory pool.
 * @details
 * Here is a list of supported attributes:
 *
 * - ::CN_MEMPOOL_ATTR_RELEASE_THRESHOLD: The type of value is ``cn_uint64_t``. This attribute
 * defines the minimum amount of free memory (in bytes) that the pool maintains before attempting to
 * release memory back to the operating system. When the amount of free memory of the memory pool
 * falls below threshold, the allocator will attempt to release memory back to the operating system
 * during the next queue, notifier or context synchronize. The default value is 0.
 * - ::CN_MEMPOOL_ATTR_REUSE_FOLLOW_NOTIFIER_DEPENDENCIES: The type of value is ``int``. This
 * attribute enables
 *   ::cnMemAllocAsync to reuse memory asynchronously freed in another queue, provided there's a
 * queue ordering dependency between the allocating queue and the free action. The interaction
 * between notifiers and a null queue can establish the necessary queue ordered dependencies. This
 * attribute is enabled by default.
 * - ::CN_MEMPOOL_ATTR_REUSE_ALLOW_OPPORTUNISTIC: The type of value is ``int``. This attribute
 * allows reuse memory from already completed frees as long as there is no dependency between the
 * free and allocation. This attribute is enabled by default.
 * - ::CN_MEMPOOL_ATTR_REUSE_ALLOW_INTERNAL_DEPENDENCIES: The type of value is ``int``. This
 * attribute allows the queue to reuse memory internally by analyzing and inserting new dependency
 * relationships. For example, when the dependency established, ::cnMemAllocAsync can reuse the
 * memory released by ::cnMemFreeAsync. This attribute is enabled by default.
 * - ::CN_MEMPOOL_ATTR_RESERVED_MEM_CURRENT: The type of value is ``cn_uint64_t``. This attribute
 * indicates the current amount of backing memory allocated for the memory pool.
 * - ::CN_MEMPOOL_ATTR_RESERVED_MEM_HIGH: The type of value is ``cn_uint64_t``. This attribute
 * indicates the maximum amount of backing memory allocated for the memory pool since the last time
 * it was reset.
 * - ::CN_MEMPOOL_ATTR_USED_MEM_CURRENT: The type of value is ``cn_uint64_t``. This attribute
 * indicates the current amount of memory from the memory pool being used by the application.
 * - ::CN_MEMPOOL_ATTR_USED_MEM_HIGH: The type of value is ``cn_uint64_t``. This attribute indicates
 * the maximum amount of memory from the memory pool that was in use by the application.
 *
 * @param[in] pool The memory pool to get attributes of.
 * @param[in] attr The attribute to retrieve from the memory pool.
 * @param[out] value The returned attribute value.
 *
 * @return
 * ::CN_SUCCESS
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY,
 * ::CN_OPS_ERROR_NOT_SUPPORTED,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS
 */
__CN_EXPORT extern CNresult cnMemPoolGetAttribute(CNmemoryPool pool, CNmemPoolAttribute attr,
                                                  void *value);

// Group: Queue Ordered Memory Allocator
/**
 * @brief Sets attributes of a memory pool.
 * @details
 * Here is a list of supported attributes:
 *
 * - ::CN_MEMPOOL_ATTR_RELEASE_THRESHOLD: The type of value is ``cn_uint64_t``. This attribute
 * defines the minimum amount of free memory (in bytes) that the pool maintains before attempting to
 * release memory back to the operating system. When the amount of free memory of the memory pool
 * falls below threshold, the allocator will attempt to release memory back to the operating system
 * during the next queue, notifier or context synchronize. The default value is 0.
 * - ::CN_MEMPOOL_ATTR_REUSE_FOLLOW_NOTIFIER_DEPENDENCIES: The type of value is ``int``. This
 * attribute enables
 *   ::cnMemAllocAsync to reuse memory asynchronously freed in another queue, provided there's a
 * queue ordering dependency between the allocating queue and the free action. The interaction
 * between notifiers and a null queue can establish the necessary queue ordered dependencies. This
 * attribute is enabled by default.
 * - ::CN_MEMPOOL_ATTR_REUSE_ALLOW_OPPORTUNISTIC: The type of value is ``int``. This attribute
 * allows reuse memory from already completed frees as long as there is no dependency between the
 * free and allocation. This attribute is enabled by default.
 * - ::CN_MEMPOOL_ATTR_REUSE_ALLOW_INTERNAL_DEPENDENCIES: The type of value is ``int``. This
 * attribute allows the queue to reuse memory internally by analyzing and inserting new dependency
 * relationships. For example, when the dependency established, ::cnMemAllocAsync can reuse the
 * memory released by ::cnMemFreeAsync. This attribute is enabled by default.
 * - ::CN_MEMPOOL_ATTR_RESERVED_MEM_CURRENT: The type of value is ``cn_uint64_t``. This attribute
 * indicates the current amount of backing memory allocated for the memory pool.
 * - ::CN_MEMPOOL_ATTR_RESERVED_MEM_HIGH: The type of value is ``cn_uint64_t``. This attribute
 * indicates the maximum amount of backing memory allocated for the memory pool since the last time
 * it was reset.
 * - ::CN_MEMPOOL_ATTR_USED_MEM_CURRENT: The type of value is ``cn_uint64_t``. This attribute
 * indicates the current amount of memory from the memory pool being used by the application.
 * - ::CN_MEMPOOL_ATTR_USED_MEM_HIGH: The type of value is ``cn_uint64_t``. This attribute indicates
 * the maximum amount of memory from the memory pool that was in use by the application.
 *
 * @param[in] pool The memory pool to set its attributes.
 * @param[in] attr The attribute to be set.
 * @param[in] value The pointer to the value to assign.
 *
 * @return
 * ::CN_SUCCESS
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY,
 * ::CN_OPS_ERROR_NOT_SUPPORTED,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS
 */
__CN_EXPORT extern CNresult cnMemPoolSetAttribute(CNmemoryPool pool, CNmemPoolAttribute attr,
                                                  void *value);

// Group: Queue Ordered Memory Allocator
/**
 * @brief Releases memory back to the operating system.
 * @details Releases memory back to the operating system as long as the bytes in the pool are less than
 *           ``minByteToKeep`` reserved bytes. This function cannot release memory that is backing
 * any outstanding asynchronous allocations. The operating system allocations may happen at
 *           different granularity from the user allocations.
 *
 * @param[in] pool The memory pool to trim.
 * @param[in] minByteToKeep The minimum number of bytes to keep by memory pool.
 *
 * @return
 * ::CN_SUCCESS
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY,
 * ::CN_OPS_ERROR_NOT_SUPPORTED,
 * ::CN_MEMORY_ERROR_INVALID_ADDRESS
 *
 * @note
 * - If the pool has less than ``minByteToKeep`` bytes reserved, ::cnMemPoolTrimTo is no-op.
 *   Otherwise the pool will be guaranteed to have at least ``minByteToKeep`` bytes reserved
 *   after the operation.
 * - Allocations that have not been freed are considered outstanding.
 * - Allocations that have been asynchronously freed but whose completion has not been observed on
 *   the host(e.g. by a synchronize) can be considered outstanding.
 *
 */
__CN_EXPORT extern CNresult cnMemPoolTrimTo(CNmemoryPool pool, size_t minByteToKeep);
/******************************************************************************
 * TaskTopo Management
 ******************************************************************************/

// Group: TaskTopo Management
/**
 * @brief Creates an empty TaskTopo.
 *
 * @param[out] pTopo    The new TaskTopo.
 * @param[in] flags     The TaskTopo creation flags. It must be 0.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY,
 * ::CN_OPS_ERROR_NOT_SUPPORTED
 */
__CN_EXPORT extern CNresult cnTaskTopoCreate(CNtaskTopo *pTopo, unsigned int flags);

// Group: TaskTopo Management
/**
 * @brief Destroys a specified TaskTopo along with all its node.
 *
 * @param[in] hTopo   The TaskTopo to be destroyed.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE
 */
__CN_EXPORT extern CNresult cnTaskTopoDestroy(CNtaskTopo hTopo);

// Group: TaskTopo Management
/**
 * @brief Clones an existing TaskTopo.
 *
 * @param[out] pClonedTopo  The newly cloned TaskTopo.
 * @param[in]  hOriginTopo  The original TaskTopo to clone.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY
 */
__CN_EXPORT extern CNresult cnTaskTopoClone(CNtaskTopo *pClonedTopo, CNtaskTopo hOriginTopo);

// Group: TaskTopo Management
/**
 * @brief Finds the corresponding node in cloned TaskTopo.
 *
 * @param[out] pClonedNode  The handle of the cloned node.
 * @param[in]  hOriginNode  Origin node.
 * @param[in]  hClonedTopo  The cloned TaskTopo to query.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_NOT_FOUND
 */
__CN_EXPORT extern CNresult cnTaskTopoNodeFindInClone(CNtaskTopoNode *pClonedNode,
                                                      CNtaskTopoNode hOriginNode,
                                                      CNtaskTopo hClonedTopo);

// Group: TaskTopo Management
/**
 * @brief Destroys @p hnode from the TaskTopo.
 *
 * @param[in] hNode  The node to be removed.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE
 */
__CN_EXPORT extern CNresult cnTaskTopoDestroyNode(CNtaskTopoNode hNode);

// Group: TaskTopo Management
/**
 * @brief Retrieves the dependency edges of a TaskTopo.
 * @details Retrieves a list of dependency edges of the TaskTopo. The relationships are
 *          indicated by indices, with each edge from ``pNodesFrom[i]`` pointing to ``pNodesTo[i]``.
 *          - If both ``pNodesFrom`` and ``pNodesTo`` are NULL, this API
 *            retrieves the total number of edges in the TaskTopo and stores it in ``numEdges``.
 *          - If one of ``pNodesFrom`` and ``pNodesTo`` is NULL while the other is non-NULL, this
 *            API returns ::CN_ERROR_INVALID_VALUE.
 *          - If both ``pNodesFrom`` and ``pNodesTo`` are non-NULL, this API retrieves edges and
 fills in entries.
 *          - If the input value of ``numEdges`` is greater than the actual
 *            number of edges, this API sets the remaining entries in ``pNodesFrom`` and
 ``pNodesTo``
 *            to NULL, and stores the number of edges actually returned in
 *            ``numEdges``.

 *
 * @param[in] hTopo           The TaskTopo to get the edges from.
 * @param[out] pNodesFrom     Location to return the edge of a source node.
 * @param[out] pNodesTo       Location to return the edge of a destination node.
 * @param[in,out] numEdges    The value cannot be NULL. See the API description above for details.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE
 *
 * @note
 * - @p pNodesFrom and @p pNodeTo must be both NULL or neither NULL, otherwise
 *   ::CN_ERROR_INVALID_VALUE will be returned
 */
__CN_EXPORT extern CNresult cnTaskTopoGetEdges(CNtaskTopo hTopo, CNtaskTopoNode *pNodesFrom,
                                               CNtaskTopoNode *pNodesTo, size_t *numEdges);

// Group: TaskTopo Management
/**
 * @brief Retrieves nodes of a TaskTopo.
 *
 * @details Retrieves a list of nodes of a TaskTopo.
 * - If ``pNodes`` is NULL,
 *   this API retrieves the number of nodes and stores it in ``numNodes``.
 * - If ``pNode`` is non-NULL, ``numNodes`` entries are filled in.
 * - If the input value of ``numNodes``
 *   is greater than the actual number of nodes, this API sets the remaining entries in
 *   ``pNode`` to NULL, and stores the number of nodes actually
 *   obtained in ``numNodes``.
 * - The space of @p pNodesFrom and @p pNodesTo cannot be
 *   less than @p numEdges.

 * @param[in] hTopo          The TaskTopo to query..
 * @param[out] pNodes        The array to store the returned nodes. It can be NULL.
 * @param[in,out] numNodes   The number of nodes. It cannot be NULL. See the API description above
 for details.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE
 */
__CN_EXPORT extern CNresult cnTaskTopoGetNodes(CNtaskTopo hTopo, CNtaskTopoNode *pNodes,
                                               size_t *numNodes);

// Group: TaskTopo Management
/**
 * @brief Retrieves root nodes of a TaskTopo.
 *
 * Retrieves a list of root nodes of a TaskTopo.
 *
 * - If ``pRootNodes`` is
 *   NULL, this API returns the number of nodes in
 *   ``numRootNodes``.
 * - If ``pRootNodes`` is non-NULL, ``numRootNodes`` entries are filled in.
 * - If the input value of ``numRootNodes`` is greater than the actual number of nodes,
 *   this API sets the remaining entries in ``pRootNodes`` to NULL, and stores the
 *   number of nodes actually obtained ``pRootNodes``.
 * - The space of @p pRootNodes cannot be less than @p numRootNodes.
 *
 * @param[in] hTopo             The TaskTopo to query.
 * @param[out] pRootNodes       The array to store the returned root nodes. It can be NULL.
 * @param[in,out] numRootNodes  The number of rood nodes. It cannot be NULL. See the API description above for details.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE
 */
__CN_EXPORT extern CNresult cnTaskTopoGetRootNodes(CNtaskTopo hTopo, CNtaskTopoNode *pRootNodes,
                                                   size_t *numRootNodes);

// Group: TaskTopo Management
/**
 * @brief Adds edges between nodes in a TaskTopo to establish dependencies.
 *
 * @param[in] hTopo             The TaskTopo to add dependency edges to.
 * @param[in] from              Array of nodes that act as dependencies
 * @param[in] to                Array of nodes that depend on the ``from`` nodes.
 * @param[in] numDependencies   The number of dependency edges to be added.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY
 */
__CN_EXPORT extern CNresult cnTaskTopoAddDependencies(CNtaskTopo hTopo, const CNtaskTopoNode *from,
                                                      const CNtaskTopoNode *to,
                                                      size_t numDependencies);

// Group: TaskTopo Management
/**
 * @brief Removes dependency edges from a TaskTopo.
 *
 * @param[in] hTopo            The TaskTopo to remove dependency edges from.
 * @param[in] from              Array of nodes that act as dependencies
 * @param[in] to                Array of nodes that depend on the ``from`` nodes.
 * @param[in] numDependencies  The number of dependency edges to be removed.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_NOT_FOUND
 */
__CN_EXPORT extern CNresult cnTaskTopoRemoveDependencies(CNtaskTopo hTopo,
                                                         const CNtaskTopoNode *from,
                                                         const CNtaskTopoNode *to,
                                                         size_t numDependencies);

// Group: TaskTopo Management
/**
 * @brief Retrieves the dependencies of a node.
 * @details Retrieves a list of dependencies of ``node``.
 *
 * - If ``pDependencies`` is
 * NULL, this API returns the number of dependencies in
 * ``numDependencies``.
 * - If ``pDependencies`` is provided, this API fills in ``numDependencies``.
 * - If the input value of ``numDependencies`` is greater than the actual number of
 * dependencies, this API sets the remaining entries in ``pDependencies``
 * to NULL, and returns the number of nodes actually obtained
 * in ``numDependencies``.
 * - The space of @p pDependencies cannot be less than @p numDependencies.
 *
 * @param[in] hNode                 The node to query.
 * @param[out] pDependencies        The array to store returned dependency nodes of a node. It can be NULL.
 * @param[in,out] numDependencies   The value cannot be NULL. See the API description above for details.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE
 */
__CN_EXPORT extern CNresult cnTaskTopoNodeGetDependencies(CNtaskTopoNode hNode,
                                                          CNtaskTopoNode *pDependencies,
                                                          size_t *numDependencies);

// Group: TaskTopo Management
/**
 * @brief Retrieves the nodes that depend on the specified node.
 * @details Retrieves a list of dependent nodes that  depend on ``node``.
 *
 * - If ``pDependentNodes`` is
 *   NULL, this API returns the number of dependent nodes in ``numDependentNodes``.
 * - If ``pDependentNodes`` is provided, this API fills in ``numDependentNodes`` entries.
 * - If the input value of ``numDependentNodes`` is greater than the actual number of
 *   dependent nodes, this API sets the remaining entries in ``pDependentNodes``
 *   to NULL, and returns the number of nodes actually obtained
 *   in ``pDependentNodes``.
 *
 * @param[in] hNode                   The node to query.
 * @param[out] pDependentNodes        The array to store returned dependent nodes of a node. It can be NULL.
 * @param[in,out] numDependentNodes   The value cannot be NULL. See the API description above for details.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_NOT_INITIALIZED,
 * ::CN_ERROR_INVALID_VALUE
 */
__CN_EXPORT extern CNresult cnTaskTopoNodeGetDependentNodes(CNtaskTopoNode hNode,
                                                            CNtaskTopoNode *pDependentNodes,
                                                            size_t *numDependentNodes);

// Group: TaskTopo Management
/**
 * @brief Returns the node type.
 *
 * @param[in] hNode    The node to query.
 * @param[out] pType   Pointer to return the node type.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE
 */
__CN_EXPORT extern CNresult cnTaskTopoNodeGetType(CNtaskTopoNode hNode, CNtaskTopoNodeType *pType);

// Group: TaskTopo Management
/**
 * @brief Writes a DOT file describing TaskTopo structure.
 *
 * @param[in] hTopo    The TaskTopo to create DOT file from.
 * @param[in] path     The path to write the DOT file to.
 * @param[in] flags    Flags from ::CNtaskTopoDebugDotFlags for specifying
 *                     the additional node information to write.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_OPERATING_SYSTEM,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY
 */
__CN_EXPORT extern CNresult cnTaskTopoDebugDotPrint(CNtaskTopo hTopo, const char *path,
                                                    unsigned int flags);

// Group: TaskTopo Management
/**
 * @brief Queries the TaskTopo kernel node attribute.
 *
 * Queries the attribute corresponding to @p attrId from @p hNode, and stores it
 * in corresponding member of @p valueOut.
 *
 * @param[in] hNode       The node to query.
 * @param[in] attrId      Attribute ID.
 * @param[out] valueOut   The array to store the attribute value.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE
 *
 * @note
 * - The type of @p hNode must be ::CN_TASK_TOPO_NODE_TYPE_KERNEL.
 */
__CN_EXPORT extern CNresult cnTaskTopoKernelNodeGetAttribute(CNtaskTopoNode hNode,
                                                             CNkernelNodeAttrID attrId,
                                                             CNkernelNodeAttrValue *valueOut);

// Group: TaskTopo Management
/**
 * @brief Sets the TaskTopo kernel node attribute.
 *
 * Sets the attribute corresponding to @p attrId for @p hNode from corresponding
 * attribute of @p value.
 *
 * @param[in] hNode      The node to set attribute for.
 * @param[in] attrId     Attribute ID.
 * @param[in] value      The attribute value to set.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE
 *
 * @note
 * - The type of @p hNode must be ::CN_TASK_TOPO_NODE_TYPE_KERNEL.
 */
__CN_EXPORT extern CNresult cnTaskTopoKernelNodeSetAttribute(CNtaskTopoNode hNode,
                                                             CNkernelNodeAttrID attrId,
                                                             const CNkernelNodeAttrValue *value);

// Group: TaskTopo Management
/**
 * @brief Copies TaskTopo kernel node attributes from source node to the destination node.
 *
 * @param[in] dst    The destination node.
 * @param[in] src    The source node.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE
 *
 * @note
 * - Both nodes must belong to the same TaskTopo.
 * - The type of both nodes must be ::CN_TASK_TOPO_NODE_TYPE_KERNEL.
 */
__CN_EXPORT extern CNresult cnTaskTopoKernelNodeCopyAttributes(CNtaskTopoNode dst,
                                                               CNtaskTopoNode src);

// Group: TaskTopo Management
/**
 * @brief Creates a user object.
 *
 * Creates a user object with the specified destructor callback and initial reference count.
 * The initial references are owned by the caller.
 * Destructor callbacks cannot make CNRT or CNDrv API call and should avoid blocking behavior.
 *
 * @param[out] object_out Location to return the user object handle.
 * @param[in] ptr The pointer to pass to the destroy function.
 * @param[in] destroy Callback to free the user object when it is no longer in use.
 * @param[in] initialRefcount The initial reference count to create the user object with, which is typically 1.
 * @param[in] flags Currently it is required to pass 1.
 *            This indicates that the destroy callback cannot be waited on by any CNRT or CNDrv API.
 *            Those requiring synchronization of the callback should signal its completion manually.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_OPS_ERROR_NOT_SUPPORTED
 */
__CN_EXPORT CNresult cnUserObjectCreate(CNuserObject *object_out, void *ptr, CNhostFn destroy,
                                        unsigned int initialRefcount, unsigned int flags);

// Group: TaskTopo Management
/**
 * @brief Acquires references for a user object. The new references are owned by the caller.
 *
 * @param[in] object The user object to acquire references for.
 * @param[in] count The number of references to acquire, which is typically 1. The value must be nonzero and not larger than ``INT_MAX``.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY,
 * ::CN_ERROR_INVALID_VALUE
 */
__CN_EXPORT CNresult cnUserObjectAcquire(CNuserObject object, unsigned int count);

// Group: TaskTopo Management
/**
 * @brief Releases references for a user object.
 *
 * Releases user object references owned by the caller.
 * The user object's destructor is invoked if the reference count reaches zero.
 * It is undefined behavior to release references not owned by the caller,
 * or to use object handle after all references are released.
 *
 * @param[in] object The user object to release references for.
 * @param[in] count The number of references to release, which is typically 1.
 *                  The value must be nonzero and not larger than ``INT_MAX``.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY,
 * ::CN_ERROR_INVALID_VALUE
 */
__CN_EXPORT CNresult cnUserObjectRelease(CNuserObject object, unsigned int count);

// Group: TaskTopo Management
/**
 * @brief Acquires references for a user object from a TaskTopo.
 *
 * Creates or moves user object references that will be owned by a TaskTopo.
 *
 * @param[in] hTopo  The TaskTopo to associate the references with.
 * @param[in] object The user object to acquire references for.
 * @param[in] count  The number of references to add to the TaskTopo, which is typically 1.
 *                 The value must be nonzero and not larger than ``INT_MAX``.
 * @param[in] flags  The optional flag ::CN_TASK_TOPO_USER_OBJECT_MOVE transfers references
 *                  from the caller, rather than creating new references,
 *                  which are created by passing 0.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY,
 * ::CN_ERROR_INVALID_VALUE
 */
__CN_EXPORT CNresult cnTaskTopoAcquireUserObject(CNtaskTopo hTopo, CNuserObject object,
                                                 unsigned int count, unsigned int flags);

// Group: TaskTopo Management
/**
 * @brief Releases references for a user object from a TaskTopo.
 *
 * @param[in] hTopo  The TaskTopo that will release the references.
 * @param[in] object The user object to release references for.
 * @param[in] count  The number of references to release, which is typically 1.
 *                 The value must be nonzero and not larger than ``INT_MAX``.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY,
 * ::CN_ERROR_INVALID_VALUE
 */
__CN_EXPORT CNresult cnTaskTopoReleaseUserObject(CNtaskTopo hTopo, CNuserObject object,
                                                 unsigned int count);

// Group: TaskTopo Management
/**
 * @brief Creates an empty node and adds it to a TaskTopo.
 *
 * Creates a new node which performs no operation, and adds it to @p hTopo with
 * @p numDependencies dependencies specified via @p dependencies.
 * A handle to the new node will be returned in @p pNode.
 * @p numDependencies can be 0, in which case the node will be placed
 * at the root of the TaskTopo. @p dependencies may not have any duplicate entries.
 *
 * An empty node performs no operation during the execution, but it can be
 * transitive ordering. For example, for a phased execution TaskTopo with 2 groups of nodes, one
 * group has m nodes, and the other has n nodes,  m+n dependency edges are needed with an empty
 * node, while m*n dependency edges are needed without an empty node.
 *
 * @param[out] pNode The newly created node.
 * @param[in]  hTopo The TaskTopo to add the node to.
 * @param[in]  dependencies The dependencies of the node.
 * @param[in]  numDependencies The number of dependencies.
 *
 * @returns
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY
 */
CNresult cnTaskTopoAddEmptyNode(CNtaskTopoNode *pNode, CNtaskTopo hTopo,
                                const CNtaskTopoNode *dependencies, size_t numDependencies);

// Group: TaskTopo Management
/**
 * @brief Creates a host execution node and adds it to a TaskTopo.
 *
 * Creates a new host function node and adds it to @p hTopo with @p numDependencies
 * dependencies specified via @p dependencies and arguments specified in @p nodeParams.
 * @p numDependencies can be 0, in which case the node will be placed
 * at the root of the TaskTopo. @p dependencies may not have any duplicate entries.
 * A handle to the new node will be returned in @p pNode.
 *
 * When the TaskTopo is launched, the node will invoke the specified CPU function.
 *
 * @param[out] pNode The newly created node.
 * @param[in] hTopo The TaskTopo to add the node to.
 * @param[in] dependencies The dependencies of the node.
 * @param[in] numDependencies The number of dependencies.
 * @param[in] nodeParams Parameters for the host node.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY
 */
CNresult cnTaskTopoAddHostNode(CNtaskTopoNode *pNode, CNtaskTopo hTopo,
                               const CNtaskTopoNode *dependencies, size_t numDependencies,
                               const CN_HOST_NODE_PARAMS *nodeParams);

// Group: TaskTopo Management
/**
 * @brief Returns the parameters of a host node.
 *
 * @param[in] hNode Node to get the parameters for.
 * @param[out] nodeParams Pointer to the returned parameters.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE
 */
CNresult cnTaskTopoHostNodeGetParams(CNtaskTopoNode hNode, CN_HOST_NODE_PARAMS *nodeParams);

// Group: TaskTopo Management
/**
 * @brief Sets the parameters of a host node.
 *
 * Sets the parameters of host node @p hNode to @p nodeParams.
 *
 * @param[in] hNode Node to set the parameters for.
 * @param[in] nodeParams Parameters to copy.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE
 */
CNresult cnTaskTopoHostNodeSetParams(CNtaskTopoNode hNode, const CN_HOST_NODE_PARAMS *nodeParams);

// Group: TaskTopo Management
/**
 * @brief Creates a kernel execution node and adds it to a TaskTopo.
 *
 * Creates a new kernel execution node and adds it to @p hTopo with @p numDependencies
 * dependencies specified via @p dependencies and arguments specified in @p nodeParams.
 * @p numDependencies can be 0, in which case the node will be placed
 * at the root of the TaskTopo. @p dependencies may not have any duplicate entries.
 * A handle to the new node will be returned in @p pNode.
 *
 * When the TaskTopo is launched, the node will invoke kernel @p hkernel described
 * by @p nodeParams.
 * See ::cnInvokeKernel for the description of the kernel node param and its restrictions.
 *
 * @param[out] pNode The newly created node.
 * @param[in] hTopo The TaskTopo to add the node to.
 * @param[in] dependencies The dependencies of the node.
 * @param[in] numDependencies The number of dependencies.
 * @param[in] nodeParams Parameters for kernel node.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY
 */
CNresult cnTaskTopoAddKernelNode(CNtaskTopoNode *pNode, CNtaskTopo hTopo,
                                 const CNtaskTopoNode *dependencies, size_t numDependencies,
                                 const CN_KERNEL_NODE_PARAMS *nodeParams);

// Group: TaskTopo Management
/**
 * @brief Returns the parameters of a kernel node.
 *
 * Returns the parameters of kernel node @p hNode in @p nodeParams.
 * The @p extra array returned in @p nodeParams,
 * as well as the argument values it points to, are owned by the node.
 * This memory remains valid until the node is destroyed or its
 * parameters are modified, and should not be modified
 * directly. Use ::cnTaskTopoKernelNodeSetParams to update the
 * parameters of this node.
 *
 * @param[in] hNode Node to get the parameters for.
 * @param[out] nodeParams Pointer to return the parameters.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE
 */
CNresult cnTaskTopoKernelNodeGetParams(CNtaskTopoNode hNode, CN_KERNEL_NODE_PARAMS *nodeParams);

// Group: TaskTopo Management
/**
 * @brief Sets the parameters of a kernel node.
 *
 * Sets the parameters of kernel node @p hNode to @p nodeParams.
 *
 * @param[in] hNode Node to set the parameters for.
 * @param[in] nodeParams Parameters to copy.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY
 */
CNresult cnTaskTopoKernelNodeSetParams(CNtaskTopoNode hNode,
                                       const CN_KERNEL_NODE_PARAMS *nodeParams);

// Group: TaskTopo Management
/**
 * @brief Creates a memcpy node and adds it to a TaskTopo
 *
 * Creates a new memcpy node and adds it to @p hTopo with @p numDependencies
 * dependencies specified via @p dependencies.
 * @p numDependencies can be 0, in which case the node will be placed
 * at the root of the TaskTopo. @p dependencies may not have any duplicate entries.
 * A handle to the new node will be returned in @p pNode.
 *
 * When the TaskTopo is launched, the node will perform the memcpy described by @p copyParams.
 * See ::cnMemcpy3D() for the description of the structure and its restrictions.
 *
 * Currently, memcpy node only supports 1D memcpy, and does not support host to host memory copy.
 * Here is the restriction of @p copyParams on setting 1D memcpy node:
 *
 * @code
 *   #define N copy_size
 *
 *   CNmemcpy3dParam memcpy_param = {0};
 *
 *   memcpy_param.dstPtr.pitch = N;
 *   memcpy_param.dstPtr.ysize = 0x1;
 *
 *   memcpy_param.extent.depth = 0x1;
 *   memcpy_param.extent.height = 0x1;
 *   memcpy_param.extent.width = N;
 *
 *   memcpy_param.srcPtr.pitch = N;
 *   memcpy_param.srcPtr.ysize = 0x1;
 *
 *   memcpy_param.src = src_addr;
 *   memcpy_param.dst = dst_addr;
 * @endcode
 * Ignores other parameters when setting 1D memcpy node.
 *
 * @param[out] pNode The newly created node.
 * @param[in] hTopo The TaskTopo to add the node to.
 * @param[in] dependencies The dependencies of the node.
 * @param[in] numDependencies The number of dependencies.
 * @param[in] copyParams Parameters for the memory copy.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY
 */
CNresult cnTaskTopoAddMemcpyNode(CNtaskTopoNode *pNode, CNtaskTopo hTopo,
                                 const CNtaskTopoNode *dependencies, size_t numDependencies,
                                 const CNmemcpy3dParam *copyParams);

// Group: TaskTopo Management
/**
 * @brief Returns the parameters of a memcpy node.
 *
 * Returns the parameters of memcpy node @p hNode in @p nodeParams.
 *
 * @param[in] hNode Node to get the parameters for.
 * @param[out] nodeParams Pointer to return the parameters.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE
 */
CNresult cnTaskTopoMemcpyNodeGetParams(CNtaskTopoNode hNode, CNmemcpy3dParam *nodeParams);

// Group: TaskTopo Management
/**
 * @brief Sets the parameters of a memcpy node.
 *
 * Sets the parameters of memcpy node @p hNode to @p nodeParams.
 * The restrictions of @p nodeParams are the same as ::cnTaskTopoAddMemcpyNode().
 *
 * @param[in] hNode Node to set the parameters for.
 * @param[in] nodeParams Parameters to copy.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE
 */
CNresult cnTaskTopoMemcpyNodeSetParams(CNtaskTopoNode hNode, const CNmemcpy3dParam *nodeParams);

// Group: TaskTopo Management
/**
 * @brief Creates a memset node and adds it to a TaskTopo.
 *
 * Creates a new memset node and adds it to @p pNode with @p numDependencies
 * dependencies specified via @p dependencies.
 * @p numDependencies can be 0, in which case the node will be placed
 * at the root of the TaskTopo. @p dependencies may not have any duplicate entries.
 * A handle to the new node will be returned in @p pNode.
 *
 * The element size must be 1, 2, or 4 bytes.
 * When the TaskTopo is launched, the node will perform the memset described by @p copyParams.
 *
 * Currently, memset node only support 1D memset. Sets @p height 1 to represent 1D memset.
 *
 * @param[out] pNode The newly created node.
 * @param[in] hTopo The TaskTopo to add the node to.
 * @param[in] dependencies The dependencies of the node.
 * @param[in] numDependencies The number of dependencies.
 * @param[in] copyParams Parameters for the memory set.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY
 */
CNresult cnTaskTopoAddMemsetNode(CNtaskTopoNode *pNode, CNtaskTopo hTopo,
                                 const CNtaskTopoNode *dependencies, size_t numDependencies,
                                 const CN_MEMSET_NODE_PARAMS *copyParams);

// Group: TaskTopo Management
/**
 * @brief Returns the parameters of memset node @p hNode in @p nodeParams.
 *
 * @param[in] hNode Node to get the parameters for.
 * @param[out] nodeParams Pointer to the returned memset node parameters.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE
 */
CNresult cnTaskTopoMemsetNodeGetParams(CNtaskTopoNode hNode, CN_MEMSET_NODE_PARAMS *nodeParams);

// Group: TaskTopo Management
/**
 * @brief Sets the parameters of a memset node.
 *
 * Sets the parameters of memset node @p hNode to @p nodeParams.
 *
 * @param[in] hNode Node to set the parameters for.
 * @param[in] nodeParams Parameters to copy.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE
 */
CNresult cnTaskTopoMemsetNodeSetParams(CNtaskTopoNode hNode,
                                       const CN_MEMSET_NODE_PARAMS *nodeParams);

// Group: TaskTopo Management
/**
 * @brief Creates a child TaskTopo node and adds it to a Task Topo.
 *
 * Creates a new node which executes an embedded TaskTopo, and adds it to @p hTopo with
 * @p numDependencies dependencies specified via @p dependencies.
 * @p numDependencies can be 0, in which case the node will be placed
 * at the root of the TaskTopo. @p dependencies may not have any duplicate entries.
 * A handle to the new node will be returned in @p pNode.
 *
 * The node executes an embedded child TaskTopo. The child Task Topo is cloned in this call.
 *
 * @param[out] pNode The newly created node.
 * @param[in] hTopo The TaskTopo to add the node to.
 * @param[in] dependencies The dependencies of the node.
 * @param[in] numDependencies The number of dependencies.
 * @param[in] hChildTopo The TaskTopo to clone into this node.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY
 */
CNresult cnTaskTopoAddChildTopoNode(CNtaskTopoNode *pNode, CNtaskTopo hTopo,
                                    const CNtaskTopoNode *dependencies, size_t numDependencies,
                                    CNtaskTopo hChildTopo);

// Group: TaskTopo Management
/**
 * @brief Gets a handle to the embedded TaskTopo of a child Task Topo node.
 *
 * This call does not clone the TaskTopo. Changes to the Task Topo will be reflected in
 * the node, and the node retains ownership of the TaskTopo.
 *
 * @param[in] hNode Node to get the embedded TaskTopo for.
 * @param[out] pTopo Location to store a handle to the TaskTopo.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE
 */
CNresult cnTaskTopoChildTopoNodeGetTopo(CNtaskTopoNode hNode, CNtaskTopo *pTopo);

// Group: TaskTopo Management
/**
 * @brief Creates a notifier place node within a specified TaskTopo.
 * Adds a newly created notifier place node to ``hTopo``.
 * This node places the specified notifier when the TaskTopo is launched and its dependencies are
 * executed. If ``numDependencies`` is 0, the node is placed at the root of the TaskTopo.
 *
 * @param[out] pNode The newly created node.
 * @param[in] hTopo The TaskTopo to add the node to.
 * @param[in] dependencies The dependencies of the node.
 * @param[in] numDependencies The number of dependencies.
 * @param[in] hNotifier The notifier handle for the node.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_NOTIFIER_ERROR_INVALID,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY
 */
CNresult cnTaskTopoAddNotifierPlaceNode(CNtaskTopoNode *pNode, CNtaskTopo hTopo,
                                        const CNtaskTopoNode *dependencies, size_t numDependencies,
                                        CNnotifier hNotifier);

// Group: TaskTopo Management
/**
 * @brief Creates a notifier wait node and adds it to a TaskTopo.
 *
 * @brief Creates a notifier place node within a specified TaskTopo.
 * Adds a newly created notifier place node to ``hTopo``.
 * This node places the specified notifier when the TaskTopo is launched and its dependencies are
 * executed. If ``numDependencies`` is 0, the node is placed at the root of the TaskTopo.
 * ``dependencies`` cannot contain any duplicate entries.
 *
 * @param[out] pNode The newly created node.
 * @param[in] hTopo The TaskTopo to add the node to.
 * @param[in] dependencies The dependencies of the node.
 * @param[in] numDependencies The number of dependencies.
 * @param[in] hNotifier The notifier handle for the node.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_CONTEXT_ERROR_INVALID,
 * ::CN_NOTIFIER_ERROR_INVALID,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY
 */
CNresult cnTaskTopoAddNotifierWaitNode(CNtaskTopoNode *pNode, CNtaskTopo hTopo,
                                       const CNtaskTopoNode *dependencies, size_t numDependencies,
                                       CNnotifier hNotifier);

// Group: TaskTopo Management
/**
 * @brief Returns the notifier handle of notifier place node @p hNode in @p phNotifier.
 *
 * @param[in] hNode Node to get the parameters for.
 * @param[out] phNotifier Pointer to return the notifier handle.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE
 */
__CN_EXPORT CNresult cnTaskTopoNotifierPlaceNodeGetNotifier(CNtaskTopoNode hNode,
                                                            CNnotifier *phNotifier);
//
// Group: TaskTopo Management
/**
 * @brief Returns the notifier handle of notifier wait node @p hNode in @p phNotifier.
 *
 * @param[in] hNode Node to get the parameters for.
 * @param[out] phNotifier Pointer to return the notifier handle.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE
 */
__CN_EXPORT CNresult cnTaskTopoNotifierWaitNodeGetNotifier(CNtaskTopoNode hNode,
                                                           CNnotifier *phNotifier);

// Group: TaskTopo Management
/**
 * @brief Sets notifier handle for a notifier place node.
 *
 * @param[in] hNode Node to set the parameters for.
 * @param[in] hNotifier Notifier to use.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE
 */
__CN_EXPORT CNresult cnTaskTopoNotifierPlaceNodeSetNotifier(CNtaskTopoNode hNode,
                                                            const CNnotifier hNotifier);

// Group: TaskTopo Management
/**
 * @brief Sets notifier handle for a notifier wait node.
 *
 * @param[in] hNode Node to set the parameters for.
 * @param[in] hNotifier Notifier to use.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE
 */
__CN_EXPORT CNresult cnTaskTopoNotifierWaitNodeSetNotifier(CNtaskTopoNode hNode,
                                                           const CNnotifier hNotifier);

// Group: TaskTopo Management
/**
 * @brief Creates an executable TaskTopo from a Task Topo.
 *
 * Instantiates @p topo as an executable TaskTopo. The Task Topo is validated for any
 * structural constraints or intra-node constraints which were not previously
 * validated. If instantiation is successful, a handle to the instantiated TaskTopo
 * is returned in @p entity.
 *
 * If there are any errors, diagnostic information may be returned in @p errorNode and
 * @p logBuffer. This is the primary way to inspect instantiation errors. The output
 * will be null terminated unless the diagnostics overflow
 * the buffer. In this case, they will be truncated, and the last byte can be
 * inspected to determine if truncation occurred.
 *
 * @param[out] entity The instantiated TaskTopo.
 * @param[in] topo TaskTopo to instantiate.
 * @param[out] pErrorNode In case of an instantiation error, this may be modified to
 *                        indicate a node contributing to the error.
 * @param[out] logBuffer A character buffer to store diagnostic messages.
 * @param[in] bufferSize Size of the log buffer in bytes.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY
 */
CNresult cnTaskTopoInstantiate(CNtaskTopoEntity *entity, CNtaskTopo topo,
                               CNtaskTopoNode *pErrorNode, char *logBuffer, size_t bufferSize);

// Group: TaskTopo Management
/**
 * @brief Destroys an executable TaskTopo.
 *
 * Destroys the executable TaskTopo specified by @p hEntity, as well as all of its executable
 * nodes. If the executable TaskTopo is being executed, it will not be terminated by this API,
 * while asynchronously released on completion of the TaskTopo execution.
 *
 * @param[in] hEntity Executable TaskTopo to destroy.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE
 */
CNresult cnTaskTopoEntityDestroy(CNtaskTopoEntity hEntity);

// Group: TaskTopo Management
/**
 * @brief Invokes an executable TaskTopo in a queue.
 *
 * Executes @p hEntity in @p hqueue. Only one instance of @p hEntity may be executed at a time.
 * For each launch, the entity will be ordered after both the previously invoked entity @p hEntity
 * and the task previously invoked to a queue @p hqueue. To execute a TaskTopo concurrently, it
 * must be instantiated multiple times into multiple executable TaskTopo.
 *
 * @param[in] hEntity Executable TaskTopo to launch.
 * @param[in] hqueue Queue in which to launch the TaskTopo Entity.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_SYSTEM_OUT_OF_MEMORY
 */
CNresult cnTaskTopoEntityInvoke(CNtaskTopoEntity hEntity, CNqueue hqueue);

// Group: TaskTopo Management
/**
 * @brief Sets the parameters for a host node in the given TaskTopo entity @p hEntity.
 *
 * The host node is identified by the corresponding node @p hNode in the
 * non-executable TaskTopo, from which the executable Task Topo is instantiated.
 * Changes to to-and-from @p hNode edges are ignored.
 *
 * The changes only affect future launches of @p hEntity. Already enqueued
 * or running launches of @p hEntity are not affected by this call. @p hNode cannot
 * be modified by this call either.
 *
 * @param[in] hEntity The executable TaskTopo in which to set the specified node.
 * @param[in] hNode Host node of the TaskTopo which is used to instantiate entity.
 * @param[in] nodeParams The updated parameters to set.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE
 */
CNresult cnTaskTopoEntityHostNodeSetParams(CNtaskTopoEntity hEntity, CNtaskTopoNode hNode,
                                           const CN_HOST_NODE_PARAMS *nodeParams);

// Group: TaskTopo Management
/**
 * @brief Sets the parameters for a kernel node in the TaskTopo entity @p hEntity.
 *
 * The kernel node is identified by the corresponding node @p hNode in the
 * non-executable TaskTopo, from which the executable Task Topo entity is instantiated.
 * Changes to to-and-from @p hNode edges are ignored.
 *
 * @p hNode must not have been removed from the original TaskTopo entity. All variables
 * of @p nodeParams can be changed, while there are restrictions on changing @p hkernel.
 * The owning context of the @p hkernel cannot be change.
 *
 * The changes only affect future launches of @p hEntity. Already
 * enqueued or running launches of @p hEntity are not affected by this call.
 * @p hNode cannot be modified by this call either.
 *
 * @param[in] hEntity The executable TaskTopo entity in which to set the specified node.
 * @param[in] hNode Kernel node of the TaskTopo from which Task Topo entity is instantiated.
 * @param[in] nodeParams Updated parameters to set.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE
 */
CNresult cnTaskTopoEntityKernelNodeSetParams(CNtaskTopoEntity hEntity, CNtaskTopoNode hNode,
                                             const CN_KERNEL_NODE_PARAMS *nodeParams);

// Group: TaskTopo Management
/**
 * @brief Sets the parameters for a memcpy node in the given TaskTopo entity @p hEntity.
 *
 * The memcpy node is identified by the corresponding node @p hNode in the
 * non-executable TaskTopo, from which the executable Task Topo entity is instantiated.
 * Changes to to-and-from @p hNode edges are ignored.
 *
 * The new copy direction must match the original copy direction.
 * If origin memcpy node is DtoH or HtoD, the device memory must be allocated
 * from the same context as the original memory. If origin memcpy node is DtoD,
 * the source memory must be allocated from the same context as the original source memory.
 * Both the instantiation-time memory operands and the memory operands in @p nodeParams must be
 * 1D. Zero-length operations are not supported. The restrictions of @p nodeParams are
 * the same as ::cnTaskTopoAddMemcpyNode().
 *
 * The changes only affect future launches of @p hEntity. Already enqueued
 * or running launches of @p hEntity are not affected by this call. @p hNode cannot be modified
 *  by this call.
 *
 * Returns CN_ERROR_INVALID_VALUE if the memory operands' mappings change; or
 * either the original or new memory operands are multidimensional.
 *
 * @param[in] hEntity The executable TaskTopo entity in which to set the specified node.
 * @param[in] hNode Memcpy node of the TaskTopo which is used to instantiate Task Topo entity.
 * @param[in] nodeParams The updated parameters to set.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE
 */
CNresult cnTaskTopoEntityMemcpyNodeSetParams(CNtaskTopoEntity hEntity, CNtaskTopoNode hNode,
                                             const CNmemcpy3dParam *nodeParams);
// Group: TaskTopo Management
/**
 * @brief Sets the parameters for a memset node in the given TaskTopo entity @p hEntity.
 *
 * The memset node is identified by the corresponding node @p hNode in the
 * non-executable TaskTopo, from which the executable Task Topo entity is instantiated.
 * Changes to to-and-from @p hNode edges are ignored.
 *
 * The destination memory in @p nodeParams must be allocated from the same
 * context as the original destination memory. Both the instantiation-time
 * memory operand and the memory operand in @p nodeParams must be 1-dimensional.
 * Zero-length operations are not supported.
 *
 * The changes only affect future launches of @p hEntity. Already enqueued
 * or running launches of @p hEntity are not affected by this call. @p hNode
 * cannot be modified by this call either.
 *
 * Returns CN_ERROR_INVALID_VALUE if the memory operand's mappings changes; or
 * either the original or new memory operand are multidimensional.
 *
 * @param[in] hEntity The executable TaskTopo entity in which to set the specified node.
 * @param[in] hNode Memset node of the TaskTopo which is used to instantiate Task Topo entity.
 * @param[in] nodeParams The updated parameters to set.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE
 */
CNresult cnTaskTopoEntityMemsetNodeSetParams(CNtaskTopoEntity hEntity, CNtaskTopoNode hNode,
                                             const CN_MEMSET_NODE_PARAMS *nodeParams);

// Group: TaskTopo Management
/**
 * @brief Updates node parameters in the child TaskTopo node in the given Task Topo entity @p hEntity.
 *
 * The child node is identified by the corresponding node @p hNode in the
 * non-executable TaskTopo, from which the executable Task Topo entity is instantiated.
 * Changes to to-and-from @p hNode edges are ignored.
 *
 * The changes only affect future launches of @p hEntity. Already enqueued
 * or running launches of @p hEntity are not affected by this call. @p hNode cannot be
 * modified by this call either.
 *
 * The topology of @p childTopo, as well as the node insertion order, must match that
 * of the TaskTopo contained in @p hNode. See ::cnTaskTopoEntityUpdate() for a list of
 * restrictions on what can be updated in TaskTopo entity. The update is recursive, so child Topo
 * nodes contained within the top level child TaskTopo will also be updated.
 *
 * @param[in] hEntity The executable TaskTopo entity in which to set the specified node.
 * @param[in] hNode Child node of the TaskTopo which is used to instantiate Task Topo entity.
 * @param[in] childTopo The child TaskTopo supplying the updated parameters.
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE
 */
CNresult cnTaskTopoEntityChildTopoNodeSetParams(CNtaskTopoEntity hEntity, CNtaskTopoNode hNode,
                                                CNtaskTopo childTopo);

// Group: TaskTopo Management
/**
 * @brief Sets the notifier for a notifier place node in the given TaskTopo entity @p hEntity.
 *
 * The notifier place node is identified by the corresponding node @p hNode in the
 * non-executable TaskTopo, from which the executable Task Topo entity is instantiated.
 * Changes to to-and-from @p hNode edges are ignored.
 *
 * The changes only affect future launches of @p hEntity. Already enqueued
 * or running launches of @p hEntity are not affected by this call. @p hNode
 * cannot be modified by this call either.
 *
 * @param[in] hEntity The executable TaskTopo entity in which to set the specified node.
 * @param[in] hNode The notifier place node of the TaskTopo which is used to instantiate Task Topo entity.
 * @param[in] hNotifier The updated notifier to set.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE
 *
 * @note
 * - The notifier handle @p hNotifier must be allocated from the same context as the original
 * notifier handle.
 */
__CN_EXPORT CNresult cnTaskTopoEntityNotifierPlaceNodeSetNotifier(CNtaskTopoEntity hEntity,
                                                                  CNtaskTopoNode hNode,
                                                                  const CNnotifier hNotifier);
//
// Group: TaskTopo Management
/**
 * @brief Sets the notifier for a notifier wait node in the given TaskTopo entity @p hEntity.
 *
 * The notifier wait node is identified by the corresponding node @p hNode in the
 * non-executable TaskTopo, from which the executable Task Topo entity is instantiated.
 * Changes to to-and-from @p hNode edges are ignored.
 *
 * The changes only affect future launches of @p hEntity. Already enqueued
 * or running launches of @p hEntity are not affected by this call. @p hNode
 * cannot be modified by this call either.
 *
 * @param[in] hEntity The executable TaskTopo entity in which to set the specified node.
 * @param[in] hNode The notifier wait node of the TaskTopo which is used to instantiate Task Topo entity.
 * @param[in] hNotifier The updated notifier to set.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE
 *
 * @note
 * - The notifier handle @p hNotifier must be allocated from the same context as the original
 * notifier handle.
 */
__CN_EXPORT CNresult cnTaskTopoEntityNotifierWaitNodeSetNotifier(CNtaskTopoEntity hEntity,
                                                                 CNtaskTopoNode hNode,
                                                                 const CNnotifier hNotifier);

// Group: TaskTopo Management
/**
 * @brief Checks whether an executable TaskTopo entity can be updated with a Task Topo
 * and performs the update if possible.
 *
 * Updates the node parameters in the instantiated TaskTopo entity specified by @p hEntity with the
 * node parameters in a topologically identical TaskTopo specified by @p hTopo.
 *
 * Limitations:
 *
 * - Kernel node restrictions:
 *   - The owning context of the function cannot be changed.
 * - Memset node restrictions:
 *   - The device(s) to which the operand(s) is allocated/mapped cannot be changed.
 *   - The memory must be allocated from the same context as the original memory.
 *   - Only 1D memset is supported now.
 *   - Zero-length operations are not supported.
 * - Memcpy node restrictions:
 *   - The new copy direction must match the original copy direction.
 *   - If memcpy is DtoH or HtoD, the device memory must be allocated from the same
 *     context as the original memory.
 *   - If memcpy is DtoD, the source memory must be allocated from the same context as
 *     the original source memory.
 *   - Only 1D memcpy is supported now.
 *   - Zero-length operations are not supported.
 *
 * ::cnTaskTopoEntityUpdate() sets @p updateResult_out to CN_TASK_TOPO_ENTITY_UPDATE_ERROR_
 * TOPOLOGY_CHANGED under the following conditions:
 *
 * - The count of nodes directly in @p hEntity and @p hTopo differ, in which case @p hErrorNode_out
 *   is NULL.
 * - A node is deleted in @p hTopo but not its pair from @p hEntity, in which case @p hErrorNode_out
 *   is NULL.
 * - A node is deleted in @p hEntity but not its pair from @p hTopo, in which case @p hErrorNode_out
 *   is the pairless node from @p hTopo.
 * - The dependent nodes of a pair differ, in which case @p hErrorNode_out is the node from @p
 *   hTopo.
 *
 * ::cnTaskTopoEntityUpdate() sets @p updateResult_out to:
 * - CN_TASK_TOPO_ENTITY_UPDATE_ERROR if an invalid value is passed.
 * - CN_TASK_TOPO_ENTITY_UPDATE_ERROR_TOPOLOGY_CHANGED if the TaskTopo topology is changed.
 * - CN_TASK_TOPO_ENTITY_UPDATE_ERROR_NODE_TYPE_CHANGED if the type of a node  is changed, in which
 *   case
 *   @p hErrorNode_out is set to the node from @p hTopo.
 * - CN_TASK_TOPO_ENTITY_UPDATE_ERROR_UNSUPPORTED_FUNCTION_CHANGE if the function changed in an
 *   unsupported way(see note above), in which case @p hErrorNode_out is set to the node from @p
 *   hTopo.
 * - CN_TASK_TOPO_ENTITY_UPDATE_ERROR_PARAMETERS_CHANGED if any parameter of a node is changed in a
 *   way that is not supported, in which case @p hErrorNode_out is set to the node from @p hTopo.
 * - CN_TASK_TOPO_ENTITY_UPDATE_ERROR_ATTRIBUTES_CHANGED if any attribute of a node is changed in a
 *   way that is not supported, in which case @p hErrorNode_out is set to the node from @p hTopo.
 * - CN_TASK_TOPO_ENTITY_UPDATE_ERROR_NOT_SUPPORTED if something about a node is unsupported, in
 *   which case @p hErrorNode_out is set to the node from @p hTopo.
 *
 * If @p updateResult_out isn't set in one of the situations described above, the update check
 * passes and ::cnTaskTopoEntityUpdate() updates @p hEntity to match the contents of @p hTopo. If an
 * error happens during the update, @p updateResult_out will be set to
 * CN_TASK_TOPO_ENTITY_UPDATE_ERROR; otherwise,
 * @p updateResult_out is set to CN_TASK_TOPO_ENTITY_UPDATE_SUCCESS.
 *
 * ::cnTaskTopoEntityUpdate() returns CN_SUCCESS when the update is performed successfully. It
 * returns CN_ERROR_TASK_TOPO_ENTITY_UPDATE_FAILURE if the TaskTopo entity update is not
 * performed because it includes changes which violate constraints specific to instantiated Task
 * Topo entity update.
 *
 * @param[in] hEntity The instantiated TaskTopo entity to be updated.
 * @param[in] hTopo The TaskTopo containing the updated parameters.
 * @param[out] hErrorNode_out The node which causes the permissibility check to forbid the update, if any.
 * @param[out] updateResult_out Whether the TaskTopo update is permitted, and what the reason is if it is forbidden.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_TASK_TOPO_ENTITY_UPDATE_FAILURE
 * @note
 * - The API may add further restrictions in future releases. The return code should always be
 *   checked.
 */
CNresult cnTaskTopoEntityUpdate(CNtaskTopoEntity hEntity, CNtaskTopo hTopo,
                                CNtaskTopoNode *hErrorNode_out,
                                CNtaskTopoEntityUpdateResult *updateResult_out);

// Group: TaskTopo Management
/**
 * @brief Uploads an executable TaskTopo in a queue.
 * Uploads @p hEntity to the device in @p hqueue for later execution.
 * The upload is performed asynchronously within the specified @p hqueue. Uploads of the same @p
 * hEntity are processed serially, meaning subsequent uploads is queued behind the current one.
 *
 * @param[in] hEntity Executable TaskTopo to upload.
 * @param[in] hqueue Queue where the TaskTopo entity is uploaded.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_INVALID_VALUE,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED
 */
CNresult cnTaskTopoUpload(CNtaskTopoEntity hEntity, CNqueue hqueue);

// Group: Profiler Management
/**
 * @brief Starts profile collection.
 * Starts profile collection for the current process.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED
 */
CNresult cnProfilerStart(void);

// Group: Profiler Management
/**
 * @brief Stops profile collection.
 * Stops profile collection for the current process.
 *
 * @return
 * ::CN_SUCCESS,
 * ::CN_ERROR_DEINITIALIZED,
 * ::CN_ERROR_NOT_INITIALIZED
 */
CNresult cnProfilerStop(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif // CN_API_H_
