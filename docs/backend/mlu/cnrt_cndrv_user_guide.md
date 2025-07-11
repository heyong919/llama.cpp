# 6.5. CNRT和CNDrv的使用

CNRT （Cambricon Runtime Library，寒武纪运行时库）和CNDrv（Cambricon Driver API， 寒武纪软件栈驱动接口）包含设备管理、内存管理、任务队列管理、设备端程序执行、通知管理等功能。 使用 Cambricon BANG C 编写的程序需要借助于 CNRT 或者 CNDrv 才能运行在 MLU 设备上。

## 6.5.1. CNRT 和 CNDrv 的区别与混合使用

CNRT 是 CNDrv 的上层，是对 CNDrv 的管理与封装，目的在于简化用户编程复杂度；CNRT 中没有 Context 与 Module 的概念，在 CNRT 中自行管理这两个模块，用户在使用 CNRT API 编程时无需关注。CNDrv 是驱动（Driver）的上层，是对驱动操作的抽象与封装。三者的具体的关系如 [图 6.2] [CNRT 、CNDrv 与 Driver 的关系] 所示：

[![](images/relationship_of_cnrt_and_cndrv.png)]

图 6.2 CNRT 、CNDrv 与 Driver 的关系

CNRT 和 CNDrv 非常相似，在很多情况下是可以互相替换的，并且也支持 CNRT 和 CNDrv API 的混合调用。

CNRT 简化了设备使用的流程，内部实现了 CNDrv 初始化和 Context、Kernel、Module 管理等功能（详情请参考 [第 9.4 章] [CNDrv API] ），但是其缺少一些对于设备的管理能力。

CNDrv 提供了更细粒度的资源操作能力，用户可以自由控制 Context 的切换以及 Kernel、Module 在设备上的加/卸载，从而避免 CNRT 在初始化阶段就将所有 Module 加载到设备上。但是对于 Kernel 的启动和参数配置等功能则需要用户调用一些接口完成。

在 CNRT 与 CNDrv 混合使用时，当一个Context已经被 CNDrv 接口创建并且被设为当前Context时，后续的 CNRT 接口则会使用这个Context而不是创建新的Context。

如果 CNRT 已经被初始化（CNRT 隐式初始化请参考 [第 6.5.2 章] [CNRT的初始化] ），那么可以通过调用 `cnCtxGetCurrent` 接口获取初始化时创建的 Shared Context，这个Context可以用于后续的 CNDrv 调用。

设备内存、Queue和Notifier均可以使用 CNRT 或 CNDrv 的接口进行申请、释放、创建、销毁。

```
CNaddr device_ptr;
CNqueue queue;
CNnotifier notifier;

cnMalloc(&device_ptr);
cnQueueCreate(&queue, 0);
cnNotifierCreate(&notifier, 0);

cnrtFree((void *)device_ptr);
cnrtQueueDestroy((cnrtQueue)queue);
cnrtNotifierDestroy((cnrtNotifier)notifier);

```

CNRT 和 CNDrv 的设备管理和版本管理部分的所有接口都可以互换使用。

## 6.5.2. CNRT的初始化

CNRT 没有显式的初始化函数，采用的是隐式初始化策略，用户在第一次调用 CNRT API 时就会完成对 CNRT 的初始化，具体流程如 [图 6.3] [CNRT 的初始化流程] 所示。

注解

* 当用户第一次调用的 CNRT API 不需要激活 Shared Context 时，CNRT 的初始化并不会激活 Shared Context，比如版本管理类 API；
* 当用户第一次调用的 CNRT API 需要激活 Shared Context 时，CNRT 就会激活 Shared Context并加载 Kernel，比如内存申请类 API。

CNRT 的初始化分为三个阶段：

1. 初始化运行环境：这个阶段 CNRT 不会申请设备资源；
2. 创建与激活 Shared Context；
3. 加载 Kernel Module：这个阶段会申请一部分的设备资源，用于存放 Kernel 运行相关数据和指令。

[![](images/cnrt_init.png)]

图 6.3 CNRT 的初始化流程

注意

由于在 CNRT 初始化流程中会创建并激活 Shared Context，同时会加载 Kernel Module 到设备侧。因此，对于任意一个会导致 CNRT 初始化的 API 来说，都有可能存在由于激活 Shared Context 或加载 Kernel Module 失败而返回错误码。

注意

当用户混合调用 CNRT API 和 CNDrv API 时需要注意：仅调用 `cnrtSetDevice()` 时，Shared Context 不会被激活，因此无法被 CNDrv API 使用。用户可以通过调用 CNRT 资源类的相关接口实现 Shared Context 的激活动作。

CNRT 初始化时，会为每个 MLU 设备创建一个 Context，该 Context 为此设备的 Shared Context，在所有主机线程之间共享。刚创建的 Shared Context 处于未激活的状态，在第一次调用 CNRT 申请资源类接口时，该 Shared Context 便会被初始化，并处于激活状态。基于 Shared Context，用户申请的资源可以在 CNRT 与 CNDrv 之间互通操作。

当用户在主机侧线程中调用 `cnrtDeviceReset()` 后，基于当前主机线程的 Shared Context 申请的设备资源会被销毁。此时，当用户将此设备作为当前设备且在任意一个主机线程中继续调用了 CNRT API，且该 API 需要处于激活状态的 Shared Context 时，该设备的 Shared Context 会重新被激活。

当用户需要在访问 Shared Context，而此时 Shared Context 处于未激活状态时，可以通过简单调用 CNRT 资源类的相关接口实现 Shared Context 的激活动作。譬如通过调用 `cnrtFree((void *)0)` 激活 Shared Context。

```
#include "cnrt.h"
#include "cn_api.h"

int main() {
   cnrtSetDevice(0);
   unsigned int ctx_flag = -1;
   int ctx_state = -1;
   cnSharedContextGetState(0, &ctx_flag, &ctx_state); // 此时 Shared context 状态为未激活
   cnrtFree((void *)0);
   cnSharedContextGetState(0, &ctx_flag, &ctx_state); // 此时 Shared context 状态为已激活

   return 0;
}

```

如上文所述，在 CNRT 初始化流程中还会进行 Kernel Module 加载，此行为会申请设备内存并将已经装载的 MLU 算子库进行解析，把 MLU 架构相关的指令和数据拷贝到设备内存上。
此时设备内存的申请和使用为CNRT与CNDrv库内部行为，其加载时机可以分为如下几类：

> * Kernel 链接到可执行程序时，其 Kernel Module 是在运行程序的第一个 CNRT 使用设备资源的接口进行加载。
> * Kernel 通过 `dlopen()` 方式加载时，其 Kernel Module 是在 `dlopen()` 后第一个 CNRT 使用设备资源的接口进行加载。
> * 若存在多个 `dlopen()`，则每次 `dlopen()` 后的第一个 CNRT 接口将加载前一次未被加载过的 Kernel Module。

可以使用如下方法进行统计分析 Kernel Module 使用的设备内存大小：

> * 程序没有 `dlopen()` 行为， 程序中第一个CNRT使用设备资源的接口前后，使用 CNDev 模块的进程内存统计功能进行分析。
> * 程序有 `dlopen()` 行为，在 `dlopen()` 后第一个 CNRT 使用设备资源的接口前后，使用 CNDev 模块的进程内存统计功能进行分析。

注意

* 使用 CNDev 进程设备内存统计功能的原因是 Kernel Module 资源是和 Context 绑定的，大部分情况下使用 CNRT 接口进行开发，进程内使用同一个Shared Context，也就是说共享一份设备侧 Kernel Module 数据。
* `cnrtSetDevice()` 仅仅会设置当前线程的 Context，其 Context 仍然为未激活状态；该接口之后不能直接调用 CNDrv API，否则会发生异常。

```
#include "cndev.h"
#include "cnrt.h"
#include <bang.h>

__mlu_global__ void empty(void) {
   printf("empty kernel\n");
}

void emptyKernel(cnrtDim3_t dim, cnrtFunctionType_t type,
               cnrtQueue_t queue) {
   empty<<<dim, type, queue>>>();
}

int main() {
   unsigned int i = 0;
   unsigned process_count = 1;
   cndevProcessInfo_t procInfo_pre;
   cndevProcessInfo_t *procInfo;
   cnrtQueue_t queue;
   cnrtDim3_t dim;
   dim.x = 1;
   dim.y = 1;
   dim.z = 1;
   cnrtFunctionType_t type = CNRT_FUNC_TYPE_BLOCK;

   /* Init cndev Lib */
   cndevCheckErrors(cndevInit(0));

   /* use CNRT api to run in card 0.
   * attention: this api will not open device. */
   CNRT_CHECK(cnrtSetDevice(0));

   /* This is the first cnrt api and will open device and load module */
   CNRT_CHECK(cnrtQueueCreate(&queue));

   emptyKernel(dim, type, queue);
   CNRT_CHECK(cnrtQueueSync(queue));

   /* First step is just for get process count in card 0 */
   procInfo_pre.version = CNDEV_VERSION_5;
   (void)cndevGetProcessInfo(&process_count, &procInfo_pre, 0);

   procInfo = (cndevProcessInfo_t *)malloc(process_count * sizeof(cndevProcessInfo_t));
   if (!procInfo) {
      printf("System has no enough memory!\n");
      exit(-1);
   }

   /* Second step is for get all process info in card 0 */
   procInfo->version = CNDEV_VERSION_5;
   cndevCheckErrors(cndevGetProcessInfo(&process_count, procInfo, 0));

   /* Here just printf current process used physical and virtual memory */
   for (i = 0; i < process_count; i++) {
      if (procInfo[i].pid == getpid()) {
         printf("Process %d pid %u, Use PhyMem: %luKiB, Use VirMem: %luKiB\n",
               i, procInfo[i].pid, procInfo[i].physicalMemoryUsed, procInfo[i].virtualMemoryUsed);

         free(procInfo);
         cndevRelease();
         exit(0);
      }
   }

   /* Something error */
   printf("Can not find current process info!\n");
   free(procInfo);
   cndevRelease();
   exit(-1);
}

```

## 6.5.3. CNDrv的初始化

与CNRT无显式初始化接口不同，在当前系统中初始化 CNDrv 运行环境，需要调用 `cnInit()` 进行设备初始化。该接口可以在进程开始调用，也可以在每个线程开始时调用，但必须在使用 CNDrv 接口前调用。

## 6.5.4. 设备管理接口

由于 CNRT 支持单机多卡运行，在执行设备端程序前需要指定一个 MLU 设备，因此 CNRT 设备管理接口的主要功能就是获取设备数量、指定执行设备、获取设备属性等。

表 6.2 CNRT 常用设备管理接口简介

| API 名称 | 功能简介 |
| --- | --- |
| cnrtGetDeviceCount() | 获取本机 MLU 设备数量 |
| cnrtSetDevice() | 指定即将使用的 MLU 设备 |
| cnrtGetDevice() | 获取当前使用的设备 |
| cnrtDeviceGetAttribute() | 获取设备属性信息 |
| cnrtDeviceReset() | 设备重置 |
| cnrtSyncDevice() | 设备同步 |

## 6.5.5. 设备内存管理

MLU 的编程模型假设系统由主机内存和设备内存组成，这两者是独立的内存系统。在设备上运行的 Kernel 程序使用的是设备内存，CNRT 提供了内存申请、释放以及内存数据在主机和设备之间的拷贝功能。设备内存的申请返回的是设备侧连续虚拟地址。这个虚拟地址可以作为指针在不同的 Kernel 函数中传递并由 MLU Core 直接访问。

注意

* MLU 加速卡平台虚拟地址位宽为64bit。
* CE(Cambricon Edge) 边缘计算平台虚拟地址位宽为64bit。

通常情况下，用户需要使用 `cnrtMalloc()` 申请设备内存并使用 `cnrtFree()` 对设备内存进行释放操作。

### 6.5.5.1. 设备内存拷贝

#### 6.5.5.1.1. 同步内存拷贝和异步内存拷贝介绍

内存拷贝包括同步拷贝和异步拷贝两种方式。

* 同步拷贝内存数据，支持主机端到设备端、设备端到主机端、设备端到设备端，以及主机端到主机端的数据拷贝，通常使用 `cnrtMemcpy()` 完成。
* 异步拷贝内存数据，适用于拷贝数据量较大的场景。支持主机端到设备端、设备端到主机端、设备端到设备端的数据拷贝， 通常使用 `cnrtMemcpyAsync()`。

注意

* 调用异步拷贝接口时，传入的设备内存和队列必须是在同一个Context下申请的，否则接口会报错。
* 调用异步拷贝接口时，用户需要保证在队列同步操作也就是拷贝完成前，设备地址和内存地址不会被释放，否则可能会导致不可预知的后果。

一般为了提高 MLU Core 核心利用率，用户至少需要创建两个线程，一个线程准备数据，另一个线程计算。使用异步拷贝，用户只需创建一个线程来完成数据准备和计算。 此外，数据拷贝和计算可以并行完成，不用等待前一份数据计算完成后，再进行下一份数据的拷贝，因此用户的线程不会再阻塞。

下面这个例子介绍通过拷贝接口完成数据从主机侧到设备侧的拷贝， 并交由 MLU Core 进行简单的标量加法操作。标量加法示例的完整代码参见安装路径：

```
samples/BANG/0_Concepts_and_Techniques/scalarAdd

```

```
#include "bang.h"

__mlu_global__ void mlu_add(float *x, float *y) {
  *y = *x + *y;
}

int main() {
   float *d0, *d1;
   size_t mem_size = sizeof(float);

   cnrtMalloc((void **)&d0, mem_size);
   cnrtMalloc((void **)&d1, mem_size);

   float h0 = 1.0, h1 = 2.0;
   cnrtMemcpy(d0, &h0, mem_size, cnrtMemcpyHostToDev);
   cnrtMemcpy(d1, &h1, mem_size, cnrtMemcpyHostToDev);

   cnrtDim3_t dim;
   dim.x = 1;
   dim.y = 1;
   dim.z = 1;

   cnrtFunctionType_t type = cnrtFuncTypeBlock;
   cnrtQueue_t queue;
   cnrtQueueCreate(&queue);

   mlu_add<<<dim, type, queue>>>(d0, d1);
   cnrtGetLastError();
   cnrtSyncDevice();

   cnrtFree(d0);
   cnrtFree(d1);
   cnrtQueueDestroy(queue);

   return 0;
}

```

#### 6.5.5.1.2. 设备内存多维同步拷贝介绍

对于不连续的内存拷贝，或数据格式化提取等操作，需要多维内存拷贝。例如规律的跳变搬运，可以使用二维内存拷贝实现。对于三维的数据拷贝或三维数据提取、转换等操作，
可以使用三维内存拷贝实现。

##### 6.5.5.1.2.1. 二维内存拷贝介绍

支持二维内存拷贝和二维内存转换。同步内存拷贝中，二维内存拷贝用 `cnrtMemcpy2D()` 完成。

从源地址 `src` 开始读取 `width` 个字节的一维数据，之后地址增加 `spitch` 个字节，进行下一个一维数据的读取，循环 `height` 次，
共读取 `width * height` 个字节。向目的地址 `dst` 写入 `width` 个字节的一维数据，之后地址增加 `dpitch` 个字节，进行下一个一维数据的写入，
循环 `height` 次，共写入 `width * height` 个字节到目的地址。 `dir` 是数据搬运的方向，可以为 `cnrtMemcpyDevToDev` 或 `cnrtMemcpyNoDirection`。
搬运成功返回 `cnrtSuccess`，否则返回相应的错误码。

```
cnrtRet_t cnrtMemcpy2D(void *dst, size_t dpitch, const void * src, size_t spitch, size_t width, size_t height, cnrtMemTransDir_t dir);

```

注意

* `dpitch` 和 `spitch` 不能小于 `width`。
* `width` 和 `height` 不能大于1MB。
* `dpitch` 和 `spitch` 不能大于4MB。
* 总的数据搬运量 `width * height` 不能大于16MB。

###### 6.5.5.1.2.1.1. 二维内存转换

支持一维和二维内存的互相转换，如一维内存转换为二维内存和二维内存转换为一维内存。
典型的使用场景如 RGB 三色图的不同色域的提取，可以使用二维内存转换为一维内存，逐一将源内存的 R 色域、 G 色域、 B 色域的数据提取到对应的一维内存中。
同理可以将处理后的 R 色域、 G 色域、 B 色域的数据，通过一维内存转换为二维内存的方式，合成 RGB 三色图。

二维内存转换为一维内存，是将源地址中的数据按照指定的 `width` 和 `spitch` 跳变地读取，再按照一维的方式存放到目的地址中。
其中 `width` 表示搬运一维数据的长度。 `spitch` 是搬运一次一维数据之后，下一次读取数据的地址相对于上一次读数据起始地址的偏移。

如 [图 6.4] [二维内存转一维内存] 所示，对源内存提取4行（ `height` ）8列（ `width` ）的数据，顺序地存储到目的内存中。

![](images/memcpy_2d_to_1d.png)

图 6.4 二维内存转一维内存

以下为二维内存转一维内存的示例代码，详细代码参见安装路径：

```
samples/cnrt/basic/6_memcpy2D3D

```

```
int memcpy_2d_to_1d_example(void)
{
  uint8_t *cnAddrSrc;
  uint8_t *cnAddrDst;
  uint64_t size = 0;

  /* 初始化二维拷贝参数 */
  u64 dpitch = 0x8;  /* 和 width 相等, 表示将数据顺序地写入目的地址 */
  u64 spitch = 0x10; /* 读取并搬运 width 个字节的数据之后，下一次读数据的地址相对于上一次读数据起始地址的偏移 */
  u64 width = 0x8;   /* 二维搬运的宽度（一维数据的长度） */
  u64 height = 0x4;  /* 二维搬运的高度（搬运一维数据的次数） */

  /* 分配源内存和目的内存 */
  size = spitch * height;
  ASSERT_EQ(cnrtSuccess, cnrtMalloc((void **)&cnAddrSrc, size));
  ASSERT_EQ(cnrtSuccess, cnrtMalloc((void **)&cnAddrDst, size));

  /* 预处理源数据和目的数据 */
  ......

  ASSERT_EQ(cnrtSuccess,
    cnrtMemcpy2D(cnAddrDst, dpitch, cnAddrSrc, spitch,
    width, height, cnrtMemcpyDevToDev));

  /* 释放资源 */
  ......
}

```

一维内存转换为二维内存，是将源地址中的数据按照一维的方式连续读取，再按照指定的 `width` 和 `dpitch` 转换为二维数据，并存放到目的地址中。

如 [图 6.5] [一维内存转二维内存] 所示，顺序读取源内存共32B（ `width * height` ）的数据，转换为4行8列
的二维数据，并存储到目的内存中。

![](images/memcpy_1d_to_2d.png)

图 6.5 一维内存转二维内存

```
int memcpy_1d_to_2d_example(void)
{
  uint8_t *cnAddrSrc;
  uint8_t *cnAddrDst;
  uint64_t size = 0;

  /* 初始化二维拷贝参数 */
  uint64_t dpitch = 0x10; /* 写入 width 个字节之后，下一次写数据的地址相对于上一次写数据起始地址的偏移 */
  uint64_t spitch = 0x8;  /* 和 width 相等, 表示从源地址顺序读取数据 */
  uint64_t width = 0x8;   /* 二维搬运的宽度 */
  uint64_t height = 0x4;  /* 二维搬运的高度 */

  /* 分配源内存和目的内存 */
  size = dpitch * height;
  ASSERT_EQ(cnrtSuccess, cnrtMalloc((void **)&cnAddrSrc, size));
  ASSERT_EQ(cnrtSuccess, cnrtMalloc((void **)&cnAddrDst, size));

  /* 预处理源数据和目的数据 */
  ......

  ASSERT_EQ(cnrtSuccess,
    cnrtMemcpy2D(cnAddrDst, dpitch, cnAddrSrc, spitch,
    width, height, cnrtMemcpyDevToDev));

  /* 释放资源 */
  ......
}

```

###### 6.5.5.1.2.1.2. 二维内存拷贝

二维内存到二维内存的搬运，典型的应用场景如图层的叠加，可以将处理好的图层叠加到目的图片上，实现图像的快速处理。
可以将预处理好的 R 色域图层，按照相同的规则（spitch=dpitch）读取和写入到对应的目的图层上。

如 [图 6.6] [二维内存拷贝] 所示：

![](images/memcpy_2d_to_2d.png)

图 6.6 二维内存拷贝

```
int memcpy_2d_to_2d_example(void)
{
  uint8_t *cnAddrSrc;
  uint8_t *cnAddrDst;
  uint64_t size = 0;

  /* 初始化二维拷贝参数 */
  u64 dpitch = 0x10; /* 搬运 width 字节后, 下一次写数据的地址相对于上一次写数据起始地址的偏移 */
  u64 spitch = 0x10; /* 搬运 width 字节后, 下一次读数据的地址相对于上一次读数据起始地址的偏移 */
  u64 width = 0x8;   /* 二维搬运的宽度 */
  u64 height = 0x4;  /* 二维搬运的高度 */

  /* 分配源内存和目的内存 */
  size = spitch * height;
  ASSERT_EQ(cnrtSuccess, cnrtMalloc((void **)&cnAddrSrc, size));
  ASSERT_EQ(cnrtSuccess, cnrtMalloc((void **)&cnAddrDst, size));

  /* 预处理源数据和目的数据 */
  ......

  ASSERT_EQ(cnrtSuccess,
    cnrtMemcpy2D(cnAddrDst, dpitch, cnAddrSrc, spitch,
    width, height, cnrtMemcpyDevToDev));

  /* 释放资源 */
  ......
}

```

##### 6.5.5.1.2.2. 三维内存拷贝介绍

支持三维内存拷贝和三维内存转换。同步内存拷贝中，三维内存拷贝用 `cnrtMemcpy3D()` 完成。

从源地址的起始位置 `p->srcPtr.ptr` 或 `p->src` 开始读取 `p->extent.width` 个字节的一维数据，之后地址增加 `p->srcPtr.pitch` 个字节，
进行下一个一维数据的读取，循环 `p->extent.height` 次，完成一次二维数据的读取。之后从起始地址增加 `srcPtr.ysize * srcPtr.pitch` 个字节，
进行下一个二维数据的读取，循环 `p->extent.depth` 次，共读取 `p->extent.width * p->extent.height * p->extent.depth` 个字节。

向目的地址的起始位置 `p->dstPtr.ptr` 或 `p->dst` 写入 `p->extent.width` 个字节的一维数据，之后地址增加 `p->dstPtr.pitch` 个字节，
进行下一个一维数据的写入，循环 `p->extent.height` 次，完成一次二维数据的写入。之后从起始地址增加 `dstPtr.ysize * dstPtr.pitch` 个字节，
进行下一个二维数据的写入，循环 `p->extent.depth` 次，共写入 `p->extent.width * p->extent.height * p->extent.depth` 个字节。

`dir` 是数据搬运的方向，可以为 `cnrtMemcpyDevToDev` 或 `cnrtMemcpyNoDirection`。
搬运成功返回 `cnrtSuccess`，否则返回相应的错误码。

```
cnrtRet_t cnrtMemcpy3D(const cnrtMemcpy3dParam_t *p);

```

注意

* `p->dstPtr.pitch` 和 `p->srcPtr.pitch` 不能小于 `p->extent.width`。
* `p->dstPtr.ysize` 和 `p->srcPtr.ysize` 不能小于 `p->extent.height`。
* `p->extent.width` 和 `p->extent.height` 不能大于1MB。
* `p->dstPtr.pitch` 和 `p->srcPtr.pitch` 不能大于4MB。
* `p->dstPtr.xsize` 和 `p->srcPtr.xsize` 为保留参数，一般与 `p->extent.width` 相等。
* 总的数据搬运量 `p->extent.width * p->extent.height * p->extent.depth` 不能大于16MB。

###### 6.5.5.1.2.2.1. 三维内存转换

支持一维和三维内存的互相转换。例如将三维立体图中的一个子集提取到一维内存，或将处理后的一维数据，恢复到三维立体图中。

三维内存转换为一维内存，首先从源内存的起始地址读取 `width` 个字节的一维数据，顺序写入目的内存。然后地址增加 `srcPtr.pitch` 个字节，进行下一个
一维数据的读取，并顺序写入目的内存，循环 `height` 次，完成一次二维内存的搬运。然后从起始地址增加 `srcPtr.ysize * srcPtr.pitch` 个字节，
开始下一个二维内存的搬运，循环 `depth` 次，从而完成三维内存的拷贝。

其中 `width` 表示搬运一维数据的长度。 `srcPtr.pitch` 是搬运一次一维数据之后，下一次读取数据的地址相对于上一个一维数据起始地址的偏移。
`srcPtr.ysize * srcPtr.pitch` 是搬运完一次二维数据之后，下一次读取数据的地址相对于上一个二维数据起始地址的偏移。

如 [图 6.7] [三维内存转一维内存] 所示，对源内存提取4\*4\*4（ `height * width * depth` ）的立方体数据，
顺序地存储到目的内存中。

![](images/memcpy_3d_to_1d.png)

图 6.7 三维内存转一维内存

```
int memcpy_3d_to_1d_example(void)
{
  uint8_t *cnAddrSrc;
  uint8_t *cnAddrDst;
  cnrtMemcpy3dParam_t p = {0};
  uint64_t size = 0;

  /* 初始化三维拷贝参数 */
  p.dstPtr.pitch = 0x4; /* 和 width 相等, 表示将数据顺序地写入目的地址 */
  p.dstPtr.xsize = 0x4; /* 目的内存的 x 方向长度，一般与 width 相等 */
  p.dstPtr.ysize = 0x4; /* 目的内存的 y 方向长度, 与 height 相等且 ysize * pitch 等于 height * width， 表示将数据顺序地写入目的地址*/

  p.extent.depth = 0x4;  /* 三维搬运的深度 */
  p.extent.height = 0x4; /* 三维搬运的高度 */
  p.extent.width = 0x4;  /* 三维搬运的宽度 */

  p.srcPtr.pitch = 0x8; /* 搬运 width 字节后, 下一次读取数据的地址相对于上一次读数据起始地址的偏移 */
  p.srcPtr.xsize = 0x4; /* 源内存的 x 方向长度，一般与 width 相等 */
  p.srcPtr.ysize = 0x8; /* 源内存的 y 方向长度, ysize * pitch 是搬运完一次二维数据之后，下一次读取数据的地址相对于上一个二维数据起始地址的偏移 */

  /* 分配源内存和目的内存 */
  size = p.srcPtr.pitch * p.srcPtr.ysize * p.extent.depth;
  ASSERT_EQ(cnrtSuccess, cnrtMalloc((void **)&cnAddrSrc, size));
  ASSERT_EQ(cnrtSuccess, cnrtMalloc((void **)&cnAddrDst, size));

  /* 预处理源数据和目的数据 */
  ......

  p.srcPtr.ptr = (void *)cnAddrSrc;
  p.dstPtr.ptr = (void *)cnAddrDst;

  ASSERT_EQ(cnrtSuccess, cnrtMemcpy3D(&p));

  /* 释放资源 */
  ......
}

```

一维内存转换为三维内存，是将源地址中的数据按照一维的方式连续读取，再按照三维拷贝参数转换为三维数据，并存放到目的地址中。

如 [图 6.8] [一维内存转三维内存] 所示，顺序读取源内存共64B（ `width * height * depth` ）的数据，转换为4\*4\*4
的三维数据，并存储到目的内存中。

![](images/memcpy_1d_to_3d.png)

图 6.8 一维内存转三维内存

```
int memcpy_1d_to_3d_example(void)
{
  uint8_t *cnAddrSrc;
  uint8_t *cnAddrDst;
  cnrtMemcpy3dParam_t p = {0};
  uint64_t size = 0;

  /* 初始化三维拷贝参数 */
  p.dstPtr.pitch = 0x8; /* 搬运 width 字节后, 下一次写数据的地址相对于上一次写数据的地址的偏移 */
  p.dstPtr.xsize = 0x4; /* 目的内存的 x 方向长度，一般与 width 相等 */
  p.dstPtr.ysize = 0x8; /* 目的内存的 y 方向长度, ysize * pitch 是搬运完一次二维数据之后，下一次写数据的地址相对于上一个二维数据起始地址的偏移 */

  p.extent.depth = 0x4;  /* 三维搬运的深度 */
  p.extent.height = 0x4; /* 三维搬运的高度 */
  p.extent.width = 0x4;  /* 三维搬运的宽度 */

  p.srcPtr.pitch = 0x4; /* 和 width 相等, 表示从源地址顺序读取数据 */
  p.srcPtr.xsize = 0x4; /* 源内存的 x 方向长度，一般与 width 相等 */
  p.srcPtr.ysize = 0x4; /* 源内存的 y 方向长度, 与 height 相等且 ysize * pitch 等于 height * width， 表示从源地址顺序读取数据*/

  /* 分配源内存和目的内存 */
  size = p.dstPtr.pitch * p.dstPtr.ysize * p.extent.depth;
  ASSERT_EQ(cnrtSuccess, cnrtMalloc((void **)&cnAddrSrc, size));
  ASSERT_EQ(cnrtSuccess, cnrtMalloc((void **)&cnAddrDst, size));

  /* 预处理源数据和目的数据 */
  ......

  p.srcPtr.ptr = (void *)cnAddrSrc;
  p.dstPtr.ptr = (void *)cnAddrDst;

  ASSERT_EQ(cnrtSuccess, cnrtMemcpy3D(&p));

  /* 释放资源 */
  ......
}

```

###### 6.5.5.1.2.2.2. 三维内存拷贝

使用三维拷贝，可以在8\*8\*8的立方体中，提取出一个4\*4\*4的子立方体。
首先对源内存读取 4 字节数据，写入对应的目的内存。然后跳 8 字节到第二个位置继续搬运下一个 4 字节数据。循环 4 次，完成立方体第一个面的搬运。
然后跳 8 \* 8 字节开始搬运立方体的下一个面，循环 4 次完成子立方体的提取。

如 [图 6.9] [三维内存拷贝] 所示：

![](images/memcpy_3d_to_3d.png)

图 6.9 三维内存拷贝

```
int memcpy_3d_to_3d_example(void)
{
  uint8_t *cnAddrSrc;
  uint8_t *cnAddrDst;
  cnrtMemcpy3dParam_t p = {0};
  uint64_t size = 0;

  /* 初始化三维拷贝参数 */
  p.dstPtr.pitch = 0x8; /* 目的立方体的宽度为 8 */
  p.dstPtr.xsize = 0x4; /* 目的立方体的 x 方向长度，一般与 width 相等 */
  p.dstPtr.ysize = 0x8; /* 目的立方体的 y 方向长度 */

  p.extent.depth = 0x4;  /* 子立方体的深度为4 */
  p.extent.height = 0x4; /* 子立方体的高度为4 */
  p.extent.width = 0x4;  /* 子立方体的宽度为4 */

  p.srcPtr.pitch = 0x8; /* 源立方体的宽度为8 */
  p.srcPtr.xsize = 0x4; /* 源立方体的 x 方向长度，一般与 width 相等 */
  p.srcPtr.ysize = 0x8; /* 源立方体的 y 方向长度 */

  /* 分配源内存和目的内存 */
  size = p.dstPtr.pitch * p.dstPtr.ysize * p.extent.depth;
  ASSERT_EQ(cnrtSuccess, cnrtMalloc((void **)&cnAddrSrc, size));
  ASSERT_EQ(cnrtSuccess, cnrtMalloc((void **)&cnAddrDst, size));

  /* 预处理源数据和目的数据 */
  ......

  p.srcPtr.ptr = (void *)cnAddrSrc;
  p.dstPtr.ptr = (void *)cnAddrDst;

  ASSERT_EQ(cnrtSuccess, cnrtMemcpy3D(&p));

  /* 释放资源 */
  ......
}

```

### 6.5.5.2. 云侧MLU平台设备内存操作介绍

#### 6.5.5.2.1. MLU加速卡平台设备NUMA内存管理

NUMA（Non-Uniform Memory Access，非一致性内存访问）架构可以理解为每个处理器都有本地内存模块，能够直接进行访问。如果数据驻留在远端内存，访问速度就会慢一些，总体上来说，距离处理器越远的内存访问成本越高。在 NUMA 模式中，每个节点有各自的本地内存模块，因此内存访问能够避免与共享内存总线相关的吞吐限制和争用问题；同时，处理器之间通过共享总线（或其他互连形式）访问其他处理器的内存模块。

对于加速卡形态产品，由于芯片架构不同，部分产品设备内存是基于 NUMA 架构进行管理的。 对于通过 NUMA 架构进行管理的， 可通过 CNDrv 提供的 `cnDeviceGetAttribute()` 接口获取设备 NUMA 信息，与此同时， CNDrv 提供了一组带 NUMA node 的申请设备内存的接口。

下面这个例子介绍通过 CNDrv 提供的 `cnMallocNode()` 接口指定 NUMA node 申请设备内存并下发 Kernel 完成计算的实例。

```
#include "bang.h"

__mlu_global__ void mlu_add(int *x, int *y， uint32_t size) {
   for (uint32_t i = 0; i < size; ++i) {
      y[i] = x[i] + y[i];
   }
}

int main() {
   /* CNRT API must be called first when CNDrv and CNRT are mixed. */
   unsigned int dev_num = 0;
   cnrtGetDeviceCount(&dev_num);
   if (dev_num < 1) {
      printf("There is no device in current system.\n");
      return -1;
   }

   int node_count = 0;
   CNdev device;
   cnDeviceGet(&device, 0);

   /* get NUMA node attribute */
   cnDeviceGetAttribute(&node_count, CN_DEVICE_ATTRIBUTE_GLOBAL_MEMORY_NODE_COUNT, device);

   int *d0, *d1;
   size_t mem_size = 1024 * sizeof(int);

   /* CNresult cnMallocNode(CNaddr *pmluAddr, cn_uint64_t bytes, int node) */
   /* The NUMA node is in the range [0, node_count - 1]. */
   /* Take '2' as 'node' in this sample and then alloc device memory by specified NUMA node */
   cnMallocNode((void **)&d0, mem_size, 2);
   cnMallocNode((void **)&d1, mem_size, 2);

   int h0[1024], h1[1024];
   for (int i = 0; i < 1024; i++) {
      h0[i] = 1;
      h1[i] = 2;
   }

   cnrtMemcpy(d0, h0, mem_size, cnrtMemcpyHostToDev);
   cnrtMemcpy(d1, h1, mem_size, cnrtMemcpyHostToDev);

   cnrtDim3_t dim;
   dim.x = 1;
   dim.y = 1;
   dim.z = 1;

   cnrtFunctionType_t type = cnrtFuncTypeBlock;
   cnrtQueue_t queue;
   cnrtQueueCreate(&queue);

   mlu_add<<<dim, type, queue>>>(d0, d1, mem_size / sizeof(int));
   cnrtGetLastError();
   cnrtSyncDevice();

   cnrtFree(d0);
   cnrtFree(d1);
   cnrtQueueDestroy(queue);

   return 0;
}

```

### 6.5.5.3. CE 边缘计算平台设备内存操作介绍

#### 6.5.5.3.1. CE 边缘计算平台内存模型基础与缓存一致性

对于 CE 边缘计算产品，物理内存是一块连续地址空间，但是在系统资源分配上将其分为两部分：

> * 操作系统可见内存，即 OS 内存（Operating System Memory）；
> * 操作系统不可见内存，即设备内存（Device Memory)，由 CNRT 进行管理。

内存模型的基本概念如下图所示：

[![](images/cndrv_memory_arch.png)]

图 6.10 CE边缘计算内存架构图

注解

* JPU（JPEG Processing Unit，图片处理单元）；
* VPU（Video Processing Unit，视频处理单元）。

从图中可以看出，CPU 对内存的访问是经过缓存的，而设备（JPU/VPU/MLU Core等）则没有缓存作为缓冲。这种访问方式与 DMA 内存的访问方式较为相似，因此本文选择了 DMA 内存模型作为基础模型来介绍内存框架。

#### 6.5.5.3.2. 内存模型与缓存一致性

CPU 进行内存写入时有两种方式：

* 透写（Write Through）：数据同时更新到缓存和内存中，该方式更加简单、可靠，主要用于无需频繁写入缓存的场景。
* 回写（Write Back）：数据仅在缓存中更新，只有当缓存行（Cache Line）准备好被替换后，数据才会更新到内存中。

CPU 读取内存的情况与写入类似。

设备通过 SoC 的内部总线访问物理内存时不需要经过 CPU，也就没有 CPU 的缓存缓冲功能。

考虑如下情况：

1. MLU Core 将计算结果数据写入到物理内存中，但是此时缓存中对应该物理内存的缓存行保存了之前的数据，并且状态为有效，因此缓存的硬件无法感知当前物理内存中的数据已经被 MLU Core 改写（红叉路径）；
2. CPU 读取 MLU Core 写入的物理内存，但是由于缓存的存在，CPU 获取到的是缓存存储的旧数据；
3. CPU 解析 MLU Core 输出，由于数据不正确而解析失败。

该访问流程如下图所示：

[![](images/cndrv_memory_err_sample.png)]

图 6.11 缓存一致性示意图

#### 6.5.5.3.3. 缓存一致性维护

为了保证 CPU 和设备之间内存访问的数据一致性，需要遵循一定的访问规则，如下图所示。

[![](images/cndrv_memory_access.png)]

图 6.12 缓存一致性维护示意图

提示

为便于描述， `cpu_sync()` 和 `dev_sync()` 在本节只作为抽象接口来描述缓存的同步操作，并非实际接口名称。CNRT 提供了 CPU 访问后与设备进行内存数据同步的接口，以及在设备访问后与 CPU 同步内存数据的接口，具体接口使用描述请参见 [第 6.5.5.3.6 章节] [CE 平台缓存操作]。

如 [图6.12] [缓存一致性维护示意图] 所示，访问过程时的数据一致性保证分为 CPU 访问和设备访问两个部分：

* CPU 对 DMA 内存进行读/写：在操作 API 启动设备前，调用 `cpu_sync()` 同步接口保证 CPU 的缓存内容与 DMA 内存一致；
* 设备对DMA内存进行读/写：在API返回后，调用 `dev_sync()` 同步接口保证设备内存操作已经与缓存一致。

同步接口 `cpu_sync()` 和 `dev_sync()` 操作的最小单位是一个缓存行（Cahce Line，CE平台上是64字节）。当 CPU 和设备操作同一块缓存行对齐的物理内存时，需要维护缓存一致性。如果 CPU 或者设备访问缓存行的前半部分，设备或者 CPU 访问缓存行的后半部分，两个地址不重叠，也需要维护缓存一致性。有些变量是原子类型以及包含原子变量的结构体，例如互斥锁，信号量，读写锁等，这些变量的内存是缓存属性的，它们访问的都是 CPU Cache，不能使用具有缓存属性的 DMA 内存域地址。其他类型的变量也不能使用具有缓存属性的 DMA 内存域地址。

无法保证缓存一致性的错误做法:

> * 错误做法一: 将具有缓存属性的 DMA 内存域地址 `cache_dma_addr` 强转成原子变量的地址。
>
>   ```
>   atomic<int> *p_atomic_val = (atomic<int> *)cache_dma_addr;
>
>   ```
> * 错误做法二: 将具有缓存属性的 DMA 内存域地址 `cache_dma_addr` 强转成整型变量的地址。
>
>   ```
>   unsigned int *p_u32_val = (unsigned int *)cache_dma_addr;
>
>   ```
> * 错误做法三：将具有缓存属性的 DMA 内存域地址 `cache_dma_addr` 强转成包含原子变量的互斥锁地址。
>
>   ```
>   pthread_mutex_t *p_mutex_lock = (pthread_mutex_t *)cache_dma_addr;
>
>   ```

#### 6.5.5.3.4. CE 平台设备内存申请与释放

本小节介绍CE平台设备内存申请与释放，包括普通设备内存申请与扩展设备内存申请。

##### 6.5.5.3.4.1. 设备内存申请与释放的通用方法

**申请设备内存**

该接口为端侧、云侧统一形式的设备内存申请接口，用于申请指定 `bytes` 大小的MLU设备内存。申请成功返回 `cnrtSuccess`，否则返回相应的错误码。

```
cnrtRet_t cnrtMalloc(void **pPtr, size_t bytes);

```

**释放指定的内存空间**

释放 `ptr` 参数指向的MLU端的内存空间。释放内存空间时，用户无需指定设备。释放成功返回 `cnrtSuccess`，否则返回相应的错误码。

```
cnrtRet_t cnrtFree(void *ptr);

```

注意

* 建议解映射后再调用 `cnrtFree`，释放已经映射过的设备内存；虽然 `cnrtFree` 不会限制用户释放未进行解映射的内存，但在释放前进行解映射是很好的编程习惯。
* `cnrtFree` 不能释放从系统申请到的内存，也不能释放映射后的主机侧内存。

#### 6.5.5.3.5. CE 平台内存映射与解映射

##### 6.5.5.3.5.1. 建立指定范围的设备侧内存到无缓存属性的用户态主机侧内存的映射

将输入参数 `dev_ptr` 指向的设备侧的内存空间，按照输入 `size` 的大小与 `pHostPtr` 对应的无缓存属性的内存进行映射。

```
cnrtRet_t cnrtMmap(void *ptr, void **pHostPtr, size_t size);

```

注意

请避免使用 `cnrtMmap` 对某一块固定的设备内存进行反复映射。
由于 `cnrtMmap` 接口多次调用的耗时与调用次数成正比，因此，为降低耗时，请减少对同一块设备内存的映射次数。

```
/* 错误的使用方式：对同一块设备内存反复映射 */
void *dev_ptr;
size_t s = 0x100000;
cnrtMalloc(&dev_ptr, s);
void *host_ptr[MAP_REPEAT];
for (i = 0;i < MAP_REPEAT; i++) {
  cnrtMmap(dev_ptr, &host_ptr[i], s);
  access_host_ptr(host_ptr[i]);

  ......

}

/* 正确的使用方式：每次映射一部分，访问读取时只使用映射过的部分*/
void *dev_ptr;
size_t s = 0x100000;
int div = 16;
cnrtMalloc(&dev_ptr, s);
void *host_ptr[MAP_DEVIDE * div];
for (i = 0; i < MAP_DEVIDE * div; i++) {
  cnrtMmap(dev_ptr + i * (s/div), &host_ptr[i], s/div);
  access_host_ptr(host_ptr[i]);

  ......

}

```

##### 6.5.5.3.5.2. 建立指定范围的设备侧内存到具有缓存属性的用户态的主机侧地址的映射

将输入参数 `dev_ptr` 指向的MLU端的内存空间，按照输入 `size` 的大小与 `pHostPtr` 对应的具有缓存属性的内存进行映射。

```
cnrtRet_t cnrtMmapCached(void *ptr, void **pHostPtr, size_t size);

```

注意

* 具有缓存属性的内存在使用的时候，要注意缓存的一致性。当读取的数据出现概率性的不一致时，可能是缓存一致性维护的问题，此时建议换为无缓存属性的内存进行比对测试。
* 不建议对同一块地址多次映射，多次映射会增加驱动合法性检查的时间，此外系统有映射次数限制，次数太多会失败；每次映射会消耗系统内存，如果多次映射超级大的内存，会导致系统内存溢出。
* 如果需要多次建立映射，建议减少映射次数，尽量避免对同一段设备侧内存做多次映射。
* `cnrtMmap` 和 `cnrtMmapCached` 是独立接口，它们之间没有联系，建议先使用 `cnrtMmap` 接口把功能调对，之后用 `cnrtMmapCached` 接口调性能。因为相比具有缓存属性的内存，无缓存属性的内存虽然不需要维护缓存一致性，但是会有较差的性能。
* 当用户调用 `cnrtMmap` 和 `cnrtMmapCached` 接口传入的 `size` 超过申请时的 `size` 时，接口会返回错误。

##### 6.5.5.3.5.3. 指定大小的内存解映射

输入 `hostPtr` 参数指向需要解映射的内存虚拟地址，按照输入的 `size` 取消 `cnrtMmap` 或 `cnrtMmapCached` 建立的内存虚拟地址映射。

```
cnrtRet_t cnrtMunmap(void *hostPtr, size_t size);

```

注意

* `cnrtMmap` 必须使用 `cnrtMunmap` 进行对应的解映射；
* 无缓存属性的内存不需要关注缓存的一致性。但相比具有缓存属性的内存，无缓存属性的内存使用时会有较差的性能。

##### 6.5.5.3.5.4. CE 平台升级指导

1. `cnrtMap` 建议由 `cnrtMmapCached` 替换，旧接口 `cnrtMap` 后的地址默认为缓存的，新接口 `cnrtMmapCached` 可以实现这个功能。替换风险：相比旧的接口会有一定程度性能下降。
2. `cnrtUnmap` 建议由 `cnrtMunmap` 替换。该新接口需要与 `cnrtMmap` 或 `cnrtMmapCached` 配合使用。

#### 6.5.5.3.6. CE 平台缓存操作

CNRT提供了在CE 边缘平台对内存进行缓存操作的接口。

##### 6.5.5.3.6.1. 缓存的刷新或使无效

**缓存操作接口**

根据输入的 `opr` 对 `hostPtr` 指向的缓存地址按照输入的 `size` 大小进行刷新或使其无效的操作。

```
cnrtRet_t cnrtMCacheOperation(void *ptr, void *hostPtr, size_t size, cnrtCacheOps_t opr);

```

##### 6.5.5.3.6.2. CE 平台的升级指导

旧接口 `cnrtCacheOperation` 以及 `cnrtCacheOperationRange` 可由 `cnrtMcacheOperation` 替代。 `cnrtMcacheOperation` 除了支持缓存的刷新（flush）操作之外，还支持使缓存失效（invalidate）的操作。此外，用户调用 `cnrtMmap` 或 `cnrtMmapCached` 获取的地址只能通过 `cnrtMCacheOperation` 进行相应的操作。

注意

* 用户要注意维护缓存一致性。当缓存由CPU写入后作为 MLU Core 的输入，此时如果用户忘记调用 `cnrtMCacheOperation` 对这块内存进行刷新，就会导致 MLU Core 没有读取到正确数值。比较典型的现象是，MLU Core 计算时而正确时而错误，但是出错的时候，CPU读回的输入数据却一直是正确的。
* 由于没有维护缓存一致性，还会导致读取不到正确的数据。例如当CPU在其他设备写入DDR数据前读取了地址内数据，导致其他CPU线程获取不到正确的计算结果。

提示

* 调用 `cnrtMCacheOperation` 接口传入的 `size` 需要小于或等于实际的内存大小，否则接口会直接返回合法性检查错误，此时不会进行缓存相关操作。
* 调用 `cnrtMcacheOperation` 接口传入的 `ptr` 和 `hostPtr` 要一一匹配，否则接口会直接返回合法性检查错误。
* 用户通过旧接口 `cnrtMap` 获取的内存不能使用 `cnrtMCacheOperation` 做缓存操作。`cnrtMCacheOperation` 需要传入的地址是通过 `cnrtMmapCached` 获取的具有缓存属性的内存。

#### 6.5.5.3.7. CE 平台缓存内存使用建议

相比具有缓存属性的内存，无缓存属性的内存读写性能会差一些。CPU 对内存写操作做了优化，并且由于缓存属性的内存多了 `cnrtMcacheOperation` 刷新内存操作，导致缓存属性内存的写操作不一定比无缓存属性内存的写操作性能高。CNToolkit Sample提供了 `cacheWritePerformance` 例子，通过给定不同的拷贝大小，可以找到缓存属性和非缓存属性带宽的大概交点，根据交点处的拷贝大小指导用户选择内存属性类型。例如多次运行测例，如果发现当数据量小于32KB时，即使性能有波动，但总体上，缓存内存的写性能低于无缓存内存的写性能；当数据量大于32KB时，缓存内存的写性能永远高于无缓存内存的写性能。那么当 CPU 只有写操作，没有读操作，并且数据量小于32KB时，可以选择用无缓存属性的内存，否则使用缓存属性的内存。

#### 6.5.5.3.8. CE 平台内存信息查询

##### 6.5.5.3.8.1. 内存属性查询

**获取内存地址的属性**

```
cnrtRet_t cnrtPointerGetAttributes(cnrtPointerAttributes_t *attr, const void *ptr);

```

`cnrtPointerGetAttributes` 接口可以一次性获取输入地址的所有信息，便于用户查询以及进行相关判断。 `deviceBaseAddr` 属性可以获取设备地址的基地址，`size` 可以返回申请内存的大小，这两个属性可用于替代旧接口 `cnrtFindDevAddrByMappedAddr` 与 `cnrtGetMemorySize` 的功能； `devicePointer` 属性可以获取映射前的设备地址，可以用于替代旧接口 `cnrtFindDevAddrWithOffsetByMappedAddr`。

##### 6.5.5.3.8.2. 使用与升级实例

1. `cnrtFindDevAddrByMappedAddr` 当前仅仅作为兼容性接口，用户可调用 `cnrtPointerGetAttributes` 并读取 `attr` 中的 `deviceBaseAddr` 将其替代。

   ```
   void _REPLACE_cnrtFindDevAddrByMappedAddr(void *host_ptr, void **dev_ptr) {
     cnrtPointerAttributes_t attr;
     cnrtPointerGetAttributes(&attr, host_ptr);
     *dev_ptr = attr.deviceBaseAddr;
   }

   ```
2. `cnrtFindDevAddrWithOffsetByMappedAddr` 当前仅仅作为兼容性接口，用户可调用 `cnrtPointerGetAttributes` 并读取 `attr` 中的 `devicePointer` 将其替代。

   ```
   /* 获取一块映射后的主机内存 */
   void _REPLACE_cnrtFindDevAddrWithOffsetByMappedAddr(void *host_ptr, void **dev_ptr) {
     cnrtPointerAttributes_t attr;
     cnrtPointerGetAttributes(&attr, host_ptr);
     *dev_ptr = attr.devicePointer;
   }

   ```
3. `cnrtGetMemorySize` 的功能可调用 `cnrtPointerGetAttributes` 接口后通过查询属性中的 `deviceBaseAddr` 和 `size` 来替代。

   ```
   /* 根据输入的设备地址查询申请的起始地址和大小 */
   void _REPLACE_cnrtGetMemorySize(void **devBasePtr, void *devPtr, size_t *size) {
     cnrtPointerAttributes_t attr;
     cnrtPointerGetAttributes(&attr, host_ptr);
     *devBasePtr = attr.deviceBaseAddr;
     *size = attr.deviceAddrSize;
   }

   ```
4. 判断输入的设备地址是否合法。

   ```
   /* 若合法返回0，不合法或其他错误返回-1 ，不是设备地址返回1 */
   int judge_device_addr_sanity(void *ptr) {
     cnrtPointerAttributes_t attr;
     if (cnrtPointerGetAttributes(&attr, ptr)) {
       printf("unknown error.\n");
       return -1;
     }
     if (cnrtMemTypeUnregistered == attr.type) {
       return -1;
     } else if (cnrtMemTypeHost == attr.type) {
       return 1;
     } else if (cnrtMemTypeDevice == attr.type) {
       return 0;
     } else {
       printf("unknown error.\n");
       return -1;
     }
   }

   ```
5. 判断某个地址是否是具有缓存属性的地址。

   ```
   /*若是返回0，不是则返回1，其他错误返回-1*/
   int judge_addr_is_mapped(void *ptr) {
     cnrtPointerAttributes_t attr;
     if (cnrtPointerGetAttributes(&attr, ptr)) {
       printf("unknown error.\n");
       return -1;
     }
     if (cnrtUvaCached == attr.cacheMode) {
       return 0;
     } else if (cnrtUvaUnCached == attr.cacheMode) {
       return 1;
     } else {
       printf("unknown error.\n");
       return -1;
     }
   }

   ```

#### 6.5.5.3.9. CE 平台升级指导汇总

表 6.3 新旧接口对比汇总

| 旧版本API名称 | 新版本替代的API名称 |
| --- | --- |
| cnrtCacheOperation(void \*host\_ptr,  cnrtCacheOps\_t opr) | cnrtMcacheOperation(void \*ptr,  void \*hostPtr, size\_t size, cnrtCacheOps\_t ops) |
| cnrtCacheOperationRange(void \*host\_ptr,  size\_t size, cnrtCacheOps\_t opr) | cnrtMcacheOperation(void \*ptr,  void \*hostPtr, size\_t size, cnrtCacheOps\_t ops) |
| cnrtFindDevAddrByMappedAddr  (void \*mapped\_host\_ptr, void \*\*dev\_ptr) | cnrtPointerGetAttributes  (cnrtPointerAttributes\_t \*attr, const void \*ptr) |
| cnrtFindDevAddrWithOffsetByMappedAddr  (void \*mapped\_host\_ptr, void \*\*dev\_ptr) | cnrtPointerGetAttributes  (cnrtPointerAttributes\_t \*attr, const void \*ptr) |
| cnrtGetMemInfo(size\_t \*free, size\_t \*total,  cnrtChannelType\_t channel) | cnrtMemGetInfo  (size\_t \*free, size\_t \*total) |
| cnrtGetMemorySize(void \*\*devBasePtr,  void \*devPtr, size\_t \*bytes) | cnrtPointerGetAttributes  (cnrtPointerAttributes\_t \*attr, const void \*ptr) |
| cnrtMap  (void \*\*host\_ptr, void \*dev\_ptr) | cnrtMmapCached  (void \*ptr, void \*\*pHostPtr, size\_t size) |
| cnrtMapRange(void \*\*host\_ptr,  void \*dev\_ptr, size\_t size) | cnrtMmapCached  (void \*ptr, void \*\*pHostPtr, size\_t size) |
| cnrtUnmap(void \*host\_ptr) | cnrtMunmap(void \*hostPtr, size\_t size) |

## 6.5.6. 设备 L2 Cache 管理

当前版本 CNDrv 不开放 L2 Cache 管理功能，只允许内部基础软件平台使用此资源。

## 6.5.7. 主机侧页锁定机制内存管理

CNRT提供了使用页锁定机制（Page-Locked）的主机内存管理接口：

* `cnrtHostMalloc()` 和 `cnrtFreeHost()` ：申请和释放页面锁定主机内存；
* `cnrtAcquireMemHandle()`， `cnrtMapMemHandle()` ，`cnrtUnMapMemHandle()` ：页锁定内存的进程间共享需要使用设备内存的进程间共享接口来完成。

注解

页锁定机制内存管理不同于通过 `malloc()` 接口申请的普通可分页主机内存。

使用页锁定主机内存有以下几点好处：

* 任务并行：在页锁定主机内存和设备内存之间进行异步数据拷贝可以和计算任务执行同时进行；
* 带宽优化：由于页锁定主机内存和设备内存之间拷贝，不需要对主机内存进行锁页操作，其带宽要优于普通可分页主机内存和设备内存之间拷贝的带宽。

注意

* 页锁定内存是一种稀缺的内存资源。相较于普通可分页内存，页锁定内存更容易申请失败。
* 由于通过占用大量的主机物理内存来实现页锁定，所以占用较多的页锁定内存会降低操作系统的整体性能。
* 页锁定内存在分配时默认是可缓存的，目前没有提供接口修改缓存属性。
* 使用页锁定的主机内存与设备交互时，可以获得最好的性能。
* 用户调用 `MemcpyAsync_V2` 接口传入非页锁定的主机内存进行异步数据拷贝时，异步接口会变为同步行为。

```
#define LOOP_COUNTS (1000)

static double __get_bandwidth(size_t size, float us)
{
    return (((size * LOOP_COUNTS) / us) * 1000000) / (1UL << 20);
}

int main(void) {
    size_t maxSz = 1UL << 28;
    cnrtNotifier_t notifier_s, notifier_e;
    cnrtQueue_t queue;
    float ms;

    char *hostMem = NULL;
    char *hostPinnedMem = NULL;
    void *devMem = NULL;

    cnrtNotifierCreate(&notifier_s);
    cnrtNotifierCreate(&notifier_e);

    cnrtQueueCreate(&queue);

    hostMem = (char *)malloc(maxSz);
    cnrtHostMalloc((void **)&hostPinnedMem, maxSz);
    cnrtMalloc(&devMem, maxSz);

    memset(hostPinnedMem, 0x2c, maxSz);
    memset(hostMem, 0x3c, maxSz);

    cnrtPlaceNotifier(notifier_s, queue);
    for (int i = 0; i < LOOP_COUNTS; i++) {
        cnrtMemcpyAsync(devMem, hostMem, maxSz, queue, cnrtMemcpyHostToDev);
    }
    cnrtPlaceNotifier(notifier_e, queue);

    cnrtQueueSync(queue);
    cnrtNotifierElapsedTime(notifier_s, notifier_e, &ms);
    printf("Pageable Host Memory bandwidth: %lfMB/s\n", __get_bandwidth(maxSz, ms * 1000));

    cnrtPlaceNotifier(notifier_s, queue);
    for (int i = 0; i < LOOP_COUNTS; i++) {
        cnrtMemcpyAsync(devMem, hostPinnedMem, maxSz, queue, cnrtMemcpyHostToDev);
    }
    cnrtPlaceNotifier(notifier_e, queue);

    cnrtQueueSync(queue);
    cnrtNotifierElapsedTime(notifier_s, notifier_e, &ms);
    printf("Page-locked Host Memory bandwidth: %lfMB/s\n", __get_bandwidth(maxSz, ms * 1000));
    return 0;
}

```

## 6.5.8. 虚拟内存管理

CNDrv 提供了直接管理统一设备虚拟地址空间的虚拟内存管理接口。用户使用 `cnrtMalloc()` 申请设备内存会得到一个指向设备内存的内存地址。返回的地址可以与其他
的 CNRT 和 CNDrv 的接口一起使用，也可以传递到 Kernel 中进行访问。不过分配的内存无法根据用户的内存需求调整大小。为了增加分配的大小，用户必须显式分配更大的
设备内存空间，并将初始分配的设备内存的数据拷贝到新的设备内存空间内，并释放掉初始分配的设备内存空间。然后继续使用新的设备内存指针执行后续任务。这通常会导致应
用程序的性能下降以及设备内存的峰值内存利用率更高。本质上，用户有一个类似 `malloc()` 的接口来分配设备内存，但没有相应的 `realloc()` 机制。虚拟内存管理
接口将地址和内存的概念解耦，允许应用程序分别进行管理。此外，应用程序在合适的时候可以在虚拟地址范围映射和解映射设备内存。

CNDrv 虚拟内存管理接口向用户提供了细粒度控制，用以管理应用程序中的设备内存。它允许用户态：

* 将分配在不同设备上的物理内存放入连续的虚拟地址范围内。
* 在支持这个特性的设备上可以选择不同的设备内存类型。

为了分配内存，虚拟内存管理编程模型实现了以下功能：

* 分配设备物理内存。
* 预留虚拟地址区间。
* 将分配的设备物理内存映射到虚拟地址区间内。
* 控制映射区间的访问权限。

### 6.5.8.1. 确认设备是否支持虚拟地址管理特性

在使用虚拟内存管理的接口之前，应用程序需要确认想要使用的设备支持虚拟内存管理特性。下面的代码实例展示了如何确认设备是否支持虚拟内存管理特性：

```
int deviceSupportVMM;
CNresult result = cnDeviceGetAttribute(&deviceSupportVMM, CN_DEVICE_ATTRIBUTE_VIRTUAL_ADDRESS_MANAGEMENT_SUPPORTED, CNdev);
if (deviceSupportVMM != 0) {
   // `CNdev` supports Virtual Memory Management
}

```

### 6.5.8.2. 申请物理内存

通过虚拟内存管理的接口申请内存的第一步是申请一块物理内存。申请物理内存时需要调用 `cnMemCreate()` 接口。通过这个接口申请的内存没有任何主机侧或者是设备侧的映射。
接口参数中的 `CNmemAllocationProp` 描述了要分配的物理内存的属性，例如：分配的位置、分配的内存是否要共享给其他进程，或者要分配的内存的物理属性。用户必须确保
分配的大小是按照合适的粒度进行对齐的。可以通过 `cnMemGetAllocationGranularity()` 查询支持的分配粒度的信息。下面的代码展示了如何使用 `cnMemCreate()` 申请物理内存：

```
CNmemGenericAllocationHandle allocPhysicalMemory(int devOrdinal, size_t size)
{
   CNmemGenericAllocationHandle handle;
   CNmemAllocationProp prop = {};
   size_t granularity = 0, alignedSize = 0;

   prop.type = CN_MEM_ALLOCATION_TYPE_DEFAULT;
   prop.location.type = CN_MEM_LOCATION_TYPE_DEVICE;
   prop.location.id = devOrdinal;

   cnMemGetAllocationGranularity(&granularity, &prop, CN_MEM_ALLOC_GRANULARITY_MINIMUM);
   alignedSize = ROUND_UP(size, granularity);
   cnMemCreate(&handle, alignedSize, &prop, 0);

   return handle;
}

```

不同于 `cnrtMalloc()` 类的分配接口返回一个指向设备内存的指针，可以直接通过设备上执行的 Kernel 进行访问，`cnMemCreate()` 分配的内存是通过接口参数
中 `CNmemGenericAllocationHandle` 的引用返回的。分配得到的内存除了使用 `cnMemGetAllocationPropertiesFromHandle` 查询物理内存属性以外，不
能用于任何操作。为了让申请的内存可以正常访问，应用程序需要将内存映射到 `cnMemAddressReserve()` 预留的虚拟地址区间，并为其配置适当的访问权限。应用
程序必须使用 `cnMemRelease()` 接口释放分配的内存。

注意

调用 `cnMemRelease()` 时如果物理内存正在被占用（例如：调用 `cnMemMap()` 建立虚拟地址与物理内存的映射或调用 `cnMemRetainAllocationHandle()`
获取虚拟地址映射的物理内存），那么物理内存不会被真正回收。只有当对于物理内存的引用都已经释放时，物理内存资源才会被真正的回收。

#### 6.5.8.2.1. 物理内存类型

应用程序通过 `cnMemCreate()` 的时候可以使用 `CNmemAllocationProp::allocFlags` 指定物理内存的类型要求，用以选择特定的内存功能。应用程序在配置内存属性时
需要确保分配内存的设备支持指定的内存类型。

##### 6.5.8.2.1.1. 可压缩内存

可压缩内存可以节省 DRAM 带宽、L2 读取带宽和 L2 容量，具体取决于正在操作的数据。应用程序想要在支持计算数据压缩的设备上分配可压缩的内存时，可以通过将
`CNmemAllocationProp::allocFlags::compressionType` 配置为 `CN_MEM_ALLOCATION_COMP_GENERIC` 来实现。在申请可压缩内存之前用户必须通过
`CN_DEVICE_ATTRIBUTE_GENERIC_COMPRESSION_SUPPORTED` 确认设备是否支持计算数据压缩特性。下面的代码实例展示了如何确认设备是否支持计算数据压缩特性：

```
int deviceSupportCompress = 0;
CNresult result = cnDeviceGetAttribute(&deviceSupportCompress, CN_DEVICE_ATTRIBUTE_GENERIC_COMPRESSION_SUPPORTED, CNdev);
if (deviceSupportCompress != 0) {
   // `CNdev` supports alloc compressible memory with cnMemCreate
}

```

在支持计算数据压缩的设备上，用户需要在分配时增加以下的代码，以表示需要申请可压缩内存：

```
prop.allocFlags.compressionType = CN_MEM_ALLOCATION_COMP_GENERIC;

```

由于硬件资源有限等各种原因，分配得到的内存可能没有压缩属性。用户需要使用 `cnMemGetAllocationPropertiesFromHandle()` 获取分配到的物理内存的内存属性，
确认是否支持压缩属性。

```
CNmemAllocationProp allocProp = {};
cnMemGetAllocationPropertiesFromHandle(&allocProp, handle);

if (allocProp.allocFlags.compressionType == CN_MEM_ALLOCATION_COMP_GENERIC) {
   // Obtained compressible memory allocation
}

```

### 6.5.8.3. 预留虚拟地址区间

在使用虚拟内存管理接口申请内存的时候，地址和内存的概念不同。应用程序必须预留一段虚拟地址区间，以容纳由 `cnMemCreate()` 申请的物理内存。
预留的虚拟地址区间的大小必须保证至少与用户计划放入其中的所有物理内存的大小的总和一样大。预留的虚拟地址大小也需要保证按照适当的粒度进行对齐，
这个对齐粒度与申请物理内存时要求的对齐粒度是一样的。 应用程序可以通过将适当的参数传递给 `cnMemAddressReserve()` 来预留虚拟地址范围，获得的
虚拟地址范围不会有任何与之关联的设备或主机物理内存。用户必须调用 `cnMemAddressFree()` 来释放预留的虚拟地址范围。

注意

调用 `cnMemAddressFree()` 释放虚拟地址区间之前需要保证释放的虚拟地址区间内均没有映射物理内存。

下面的代码片段说明了接口的使用方法：

```
CNaddr cnAddr;
CNresult result = cnMemAddressReserve(&cnAddr, size, 0, 0, 0); // alignment = 0 for default alignment;

// ......

// make sure cnAddr is not mapped any device memory handle.
cnMemAddressFree(cnAddr, size);

```

### 6.5.8.4. 支持虚拟地址重名

虚拟内存管理接口提供了将不同的虚拟地址段映射到同一段物理内存的能力，即虚拟地址重名。在虚拟地址重名的场景下通过不同的虚拟地址读写设备内存的操作
无法保证数据的一致性，在这种场景下需要应用程序自己利用同步机制（例如：Queue 或者 Notifier）保证不同的虚拟地址对于设备内存的读写操作是有序的。

下面的代码示例的行为是一种未定义的行为，无法保证输出结果是唯一的。这里假设地址 `cnAddrA` 与 `cnAddrB` 映射的是同一块设备物理内存：

```
#include "bang.h"

__mlu_global__ void kernel(char *cnAddrA, char *cnAddrB) {
   *cnAddrA = 0x1c;
   __bang_printf("%x\n", *cnAddrB); // Undefined behavior! *cnAddrB can take on either the previous value or some value in-between.
}

```

下面的代码示例的行为是已定义的，可以确保最终的结果是确定的。

```
#include "bang.h"

__mlu_global__ void KernelA(char *cnAddrA) {
   *cnAddrA = 0x1c;
}

__mlu_global__ void KernelB(char *cnAddrB) {
   __bang_printf("%x\n", *cnAddrB); // *cnAddrB == *cnAddrA == 0x1c.
}

kernelA<<<1, 1, 1, queueA>>>(cnAddrA);
cnrtPlaceNotifier(notifier, queueA);
cnrtQueueWaitNotifier(notifier, queueB, 0);
kernelB<<<1, 1, 1, queueB>>>(cnAddrB);

```

### 6.5.8.5. 建立内存映射

为了让之前分配出来的物理内存以及预留的虚拟地址可用，用户必须调用 `cnMemMap()` 将 `cnMemAddressReserve()` 返回的虚拟地址以及
`cnMemCreate()` 返回的物理内存进行绑定。在需要解除物理内存和虚拟地址的映射关系时，用户必须调用 `cnMemUnmap()` 对地址进行解映射。
下面的代码说明了接口的使用方法：

```
// `cnAddr`: address in the address range previously reserved by cnMemAddressReserve
// `handle`: CNmemGenericAllocationHandle obtained by a previously call to cnMemCreate
CNresult result = cnMemMap(cnAddr, size, 0, handle, 0);

```

注意

* 物理内存和虚拟地址之间可以进行多次映射和解映射操作。
* 不允许在已经映射的虚拟地址区间上重复建立映射。
* 建立映射时传入的 `size` 需要保证与传入 `handle` 的大小保持一致。
* `cnMemMap()` 的参数 `offset` 暂时只支持传入0，单次映射长度只能是传入 `handle` 的大小。

**高效建立映射建议**

在实际使用过程中，为了减少设备物理内存分配过程中的内存碎片化问题，建议应用程序在调用 `cnMemCreate()` 申请物理内存时需要保证传入的 `size` 按照推荐粒度
对齐。用户可以通过调用 `cnMemGetAllocationGranularity()` 并传入 `CN_MEM_ALLOC_GRANULARITY_RECOMMENDED` 获取推荐的对齐粒度。

在预留虚拟内存的过程中，建议用户按照当前应用程序最大使用的设备内存的大小进行预留，如果不确定最大使用内存的时候可以通过 `cnrtMemGetInfo()` 获取当前设备的
最大可用内存，并将返回值按照推荐的对齐粒度对齐之后作为预留虚拟地址的大小。

```
CNmemGenericAllocationHandle handle;
CNmemAllocationProp prop = {};
size_t granularity = 0, alignedSize = 0, resvSize = 0;

cnMemGetAllocationGranularity(&granularity, &prop, CN_MEM_ALLOC_GRANULARITY_RECOMMENDED);
prop.type = CN_MEM_ALLOCATION_TYPE_DEFAULT;
prop.location.type = CN_MEM_LOCATION_TYPE_DEVICE;
prop.location.id = devOrdinal;
alignedSize = ROUND_UP(size, granularity);
cnMemCreate(&handle, alignedSize, &prop, 0);

cnrtMemGetInfo(&resvSize, NULL);
resvSize = ROUND_UP(resvSize, granularity);
cnMemAddressReserve(&cnAddr, resvSize, 0, 0, 0); // alignment = 0 for default alignment;

```

### 6.5.8.6. 设置内存访问权限

虚拟内存管理接口支持应用程序控制对于一段虚拟地址区间的访问权限。上面使用 `cnMemMap()` 将物理内存和虚拟地址建立映射之后依旧无法让
地址可以被访问。用户必须调用 `cnMemSetAccess()` 指定这段虚拟地址与物理内存映射的访问权限。下面的代码展示了接口的使用方式：

```
CNmemAccessDesc desc = {};

/* set [cnAddr, cnAddr + eachHandleSize) as readwrite */
desc.accessFlags = CN_MEM_ACCESS_FLAGS_PROT_READWRITE;
desc.location.type = CN_MEM_LOCATION_TYPE_DEVICE;
desc.location.id = devOrdinal;
cnMemSetAccess(cnAddr, eachHandleSize, &desc, 1);

/* set [cnAddr + eachHandleSize, cnAddr + eachHandleSize * 2) as readonly */
desc.accessFlags = CN_MEM_ACCESS_FLAGS_PROT_READONLY;
desc.location.type = CN_MEM_LOCATION_TYPE_DEVICE;
desc.location.id = devOrdinal;
cnMemSetAccess(cnAddr + eachHandleSize, eachHandleSize, &desc, 1);

```

注意

* 调用 `cnMemSetAccess()` 时传入的 `CNmemAccessDesc::location::id` 必须保证与申请物理内存时指定的设备保持一致。
* 目前 `cnMemSetAccess()` 传入参数中的 `count` 只能为 1。

## 6.5.9. 线性设备内存加速

用户使用 `cnrtMalloc()` 申请设备内存会得到一个指向设备内存的虚拟内存地址。在传递到 Kernel 中进行访问的过程中，会先通过硬件的页表翻译模块将虚拟地址
翻译为物理地址之后，硬件再通过实际的物理地址访问设备内存。为了避免每次进行页表翻译造成访存性能的下降，硬件提供了一块缓存空间用于暂存虚拟地址到物理地址的
映射关系，从而加速了页表翻译的这个过程。随着设备内存容量的增大，在离散访问的场景下，暂存虚拟地址到物理地址映射的缓存空间无法覆盖所有的设备内存空间进而
导致离散访问的性能相比连续访问存在明显的性能差异。

为了解决上述问题，寒武纪提供了线性设备内存的加速机制，此机制在用户调用 `cnrtMalloc()` 按照一定的约束申请设备内存时会返回线性设备内存的虚拟地址。硬件在
通过线性设备内存虚拟地址访问设备内存的过程中不需要进行页表翻译。从而有效的避免上述问题带来的性能损失。CNDrv 提供了线性设备内存加速相关的属性查询字段，用
以指导应用程序利用线性设备内存的特性来加速访存性能。

使用线性设备内存加速特性有以下几点好处：

* 离散优化：没有页表翻译的过程，也就不需要考虑页表映射关系的缓存替换问题。离散访问的性能会更加稳定。
* 跨设备数据拷贝优化：优化了在不同设备间进行异步数据拷贝的任务执行时间。

注意

只有 CNRT 的 `cnrtMalloc` 以及 CNDrv 的 `cnMalloc` 和 `cnZmalloc` 可以申请线性设备内存。

### 6.5.9.1. 确认设备是否支持线性设备内存加速特性

在使用线性设备内存加速的特性之前，应用程序需要确认想要使用的设备是否支持线性设备内存加速的特性。下面的代码示例展示了如何确认设备是否支持线性设备内存加速特性：

```
int deviceSupportLinear;
CNresult result = cnDeviceGetAttribute(&deviceSupportLinear, CN_DEVICE_ATTRIBUTE_LINEAR_MAPPING_SUPPORTED, CNdev);
if (deviceSupportLinear != 0) {
   // `CNdev` supports linear mapping feature
}

```

### 6.5.9.2. 申请线性设备内存

在确认当前设备支持线性设备内存加速特性之后，应用程序调用 `cnrtMalloc` 即可有机会申请到线性设备内存。需要注意的是 `cnrtMalloc` 返回的地址不能保证一定是
线性设备内存，应用程序可以调用相关接口查询返回的地址是否是线性设备内存地址。下面的代码示例展示了如何申请线性设备内存以及确认设备虚拟地址是否是线性设备内存虚拟地址：

```
int isLinear = 0;
void *devPtr;
size_t size = 1UL << 30;

cnrtMalloc(&devPtr, size);
cnGetMemAttribute((void *)&isLinear, CN_MEM_ATTRIBUTE_ISLINEAR, devPtr);
if (isLinear) {
   // `devPtr` is a linear device memory pointer;
} else {
   // `devPtr` is a normal device memory pointer;
}

```

### 6.5.9.3. 高效使用线性设备内存加速特性

在实际使用过程中，为了能够更稳定的申请和使用线性设备内存，建议应用程序在调用 `cnrtMalloc` 申请设备内存的时候尽量一次将本次业务所需的所有设备内存都申请到。避免
多次调用 `cnrtMalloc` 引入设备内存碎片，导致线性设备内存分配失败。下面的代码展示了申请到最大的线性设备内存的方法：

```
unsigned long granularity = 0, maxFreeSize = 0;
CNresult result;
int isLinear = 0;
void *devPtr = NULL;

cnDeviceGetAttribute(&granularity, CN_DEVICE_ATTRIBUTE_LINEAR_RECOMMEND_GRANULARITY, CNdev);

cnrtMemGetInfo(&maxFreeSize, NULL);

maxFreeSize = (maxFreeSize > granularity) ? ROUND_DOWN(maxFreeSize, granularity) : rounddown_pow_of_two(maxFreeSize);

while (maxFreeSize > 0) {
   result = cnrtMalloc(&devPtr, maxFreeSize);
   if (!result) {
      cnGetMemAttribute((void *)&isLinear, CN_MEM_ATTRIBUTE_ISLINEAR, devPtr);
      if (isLinear)
         break;
      else
         cnrtFree(devPtr);
   }

   if (maxFreeSize < granularity) {
      maxFreeSize = rounddown_pow_of_two(maxFreeSize - 1);
   } else {
      maxFreeSize -= granularity;
   }
}

printf("allocated maximum linear memory is %#llx, size is :%#lx\n", (unsigned long)devPtr, maxFreeSize);

```

如果应用程序无法做到一次性申请到所有的设备内存，为了尽可能使用线性设备内存加速特性，需要保证在所有申请内存时传入的 size 尽可能按照以下规则进行对齐：

* 当传入的 size 小于 `CN_DEVICE_ATTRIBUTE_LINEAR_RECOMMEND_GRANULARITY`，需要保证 size 按照二次幂向下对齐。
* 当传入的 size 大于 `CN_DEVICE_ATTRIBUTE_LINEAR_RECOMMEND_GRANULARITY`，需要保证 size 按照 `CN_DEVICE_ATTRIBUTE_LINEAR_RECOMMEND_GRANULARITY` 向下对齐。

**线性设备内存申请实例说明**

以上述示例代码为例，假设当前通过 `cnrtMemGetInfo` 获取到的 `maxFreeSize` 是 0x13B2500000. 经过与 `granularity` 向下对齐之后得到的新的 `maxFreeSize`
是 0x1380000000 (当前 `CN_DEVICE_ATTRIBUTE_LINEAR_RECOMMEND_GRANULARITY` 返回的对齐粒度是 1GB。对齐粒度后续可能会发生变化，强烈建议用户在开发过程中
重新读取 `CN_DEVICE_ATTRIBUTE_LINEAR_RECOMMEND_GRANULARITY` 获取当下的对齐粒度)。因此在这次申请时有 805MB (0x13B2500000 - 0x1380000000 = 0x32500000)
无法作为线性设备内存被申请走。如果应用程序可以申请多次设备内存，按照上述规则对剩余的 805MB 按照二次幂向下对齐拆分为多笔内存申请请求再次申请剩余的线性设备内存。
不过由于设备内存缓存加速机制的存在当剩余内存小于 32MB 的时候就无法再次申请设备内存。即 805MB 中只能分多次申请到总计 800MB 的线性内存，剩余 5MB 无法分配和使用。

### 6.5.9.4. 使用线性设备内存的风险

在申请和使用线性设备内存的过程中，同样存在如下的风险和弊端：

* 上述建议是需要所有设备使用者都遵循的条件下才可达到，因此在多用户情况下如何保证原则被遵守是需要开发者仔细审视的。这对程序部署者有着较高的使用挑战。
* 线性设备内存资源比较紧张，容易受到其他设备内存申请请求的影响。按照上述的使用建议只是提高了申请到线性设备内存的可能性，依旧存在申请不到线性设备内存的可能。因此这对主动使用线性内存的开发者在编程上的技巧要求会更高。
* 由于是否成功申请到线性内存易受到申请顺序和规模影响，因此容易出现由内存申请行为导致的硬件性能数据变化，并且问题定位存在一定难度。这对性能分析、性能优化过程可能会带来比较大的困扰。
* 在访问线性设备内存时，硬件不再对非法访问做检查，因此由程序的越界访问、内存踩踏行为导致的问题定位会比较困难。这对程序调试，尤其是多用户并行调试可能会带来比较大的困扰。
* 运行阶段如果出现内存 ECC 错误，在没有及时复位的时候。驱动会将出错的物理页暂时下线，这会加剧物理内存碎片化。影响线性内存分配的成功率。

基于上述线性设备内存的风险，如果用户想要在开发阶段临时关闭线性设备内存的话，可以参考《寒武纪驱动安装包使用手册》的“线性设备内存配置参数”章节，通过配置驱动加载参数
来临时关闭线性设备内存的功能。

## 6.5.10. 异步并行执行

寒武纪底层软件支持以下操作作为相互独立的任务并行执行:

* 主机上的计算任务；
* 设备上的计算任务；
* 主机到设备的数据拷贝；
* 设备到主机的数据拷贝；
* 设备之间的数据拷贝。

### 6.5.10.1. 主机和设备之间的并发执行

CNRT提供的异步接口支持异步执行设备任务。调用这些异步接口，设备上执行的任务会加入到任务队列，待设备资源满足需求再执行。主机线程可以在执行这些异步任务同时处理其他任务。下述操作相对于主机是可以异步执行：

* Kernel任务的启动和执行。
* 单一设备内部内存数据拷贝。
* 设备和主机间通过调用Async后缀的接口进行内存数据拷贝。
* 通过调用Async后缀的接口进行Memory set。

### 6.5.10.2. Kernel并发执行

如果Kernel任务所需要的计算资源满足要求，多个计算任务可以并发执行，根据任务规模和具体硬件计算能力限制可以并发执行的Kernel任务数量可以通过接口 `cnGetCtxMaxParallelUnionTask` （CNToolkit >= 2.5或libcndrv >= 1.5） 获取。

### 6.5.10.3. 内存拷贝和 Kernel 执行并行

数据在设备和主机之间拷贝和Kernel的执行可以并行进行。设备内内存拷贝任务和Kernel执行也可以并行。例如分别向不同的 CNRT Queue 中下发拷贝和计算任务，由于两个Queue中的任务是可以并行执行的，Kernel计算任务从Queue中取出交由MLU处理，拷贝任务从Queue中取出交由DMA处理。但是如果计算和拷贝任务在同一个Queue中，由于Queue任务是FIFO（First In First Out，先进先出）执行，所以此时两个任务无法并行。

### 6.5.10.4. 内存拷贝任务之间并行

多个设备和主机之间数据拷贝任务可以并行执行。例如分别给设备A和设备B下发主机侧到设备侧的拷贝任务，对于这两个任务来说是分别使用两个设备的DMA来进行。

## 6.5.11. Queue

Queue是用来管理并行操作的一种方式。在同一个Queue中所下发的操作顺序是依次按照下发顺序执行的，在不同Queue中的执行操作则是乱序的。Queue所支持的操作不仅包括Kernel（在设备上的执行程序），还包括Notifier、内存拷贝、设备与主机的数据传输等一系列操作，所有需要按序执行的操作都可以下发至同一个Queue，Queue在当前操作满足执行条件后会依次按照进入顺序执行操作；而不同Queue的操作则不保证顺序，这些操作的顺序是无法预测的。Queue的查询操作可以查看当前Queue的任务执行是否完成，而同步操作在执行成功的时候则可以确保所有的任务执行完成。

### 6.5.11.1. 创建和销毁

通过 `cnrtQueueCreate()` 接口可以创建Queue对象，该对象可以容纳需要进入该Queue的所有操作。需要进入Queue的操作需要在接口中把创建的Queue对象作为参数。创建Queue的样例代码如下面代码所示，该样例代码创建了两个Queue：

```
cnrtQueue_t Queue[2];
for (int i = 0; i < 2; i++) {
    if (cnrtQueueCreate(&Queue[i]) != cnrtSuccess) {
        printf("Create Queue Failed\n");
    }
}

```

创建的Queue结构体可以作为所有需要Queue的接口参数，下面的代码展示了将Kernel以及设备与主机数据传输操作放入Queue的方法：

```
float *hostAddr;
void *inputAddr;
void *outputAddr;
cnrtHostMalloc(&hostAddr, size * 2);
cnrtMalloc(&inputAddr, size * 2);
cnrtMalloc(&outputAddr, size * 2);

for （int i = 0; i < 2; i++） {
    cnrtMemcpyAsync(inputAddr + i * size, hostAddr + i * size,
                    size, Queue[i], cnrtMemcpyHostToDev);
    Kernel<<<1, 1, 1, Queue[i]>>>(OutputAddr + i * size, inputAddr + i * size, size);
    cnrtMemcpyAsync(hostAddr + i * size, OutputAddr + i * size,
                    size, Queue[i], cnrtMemcpyDevToHost);
}

```

上面的样例代码将输入数据拷贝到设备的拷贝操作放入Queue[i]中，并紧接着在同一个Queue中放入了Kernel操作，再放入了将输出数据传输到主机的拷贝操作，放入同一个Queue的这三个操作是依次执行的。而创建的两个Queue均放入了三个同样的操作，两个Queue中的操作则可以并行执行。两个不同的Queue的拷贝操作与Kernel执行操作，可以并行执行相互覆盖其中的执行延迟。

Queue对象可以通过 `cnrtQueueDestroy()` 接口进行销毁，销毁后该Queue对象不可以再用作接口参数进行接收执行操作。

```
for (int i = 0; i < 2; i++) {
   if (cnrtQueueDestroy(Queue[i])) {
       printf("Destroy Queue Failed\n");
   }
}

```

注解

如果当前Queue中有操作未执行或正在执行，调用上面的接口后会立刻返回成功并且销毁还未执行的操作或打断正在执行的操作。

### 6.5.11.2. 默认 Queue

对于Kernel的执行操作，其中的Queue参数可以不传，或者传入Queue参数为0时，代表着当前使用默认Queue。对于所有需要传入Queue参数的接口，当传入Queue的参数为0时，均认为当前使用默认Queue进行操作。
默认Queue与普通的Queue功能完全相同，也是对所有操作的并行执行的管理，并且对于每一个设备，每一个主机侧线程都有一个自己的默认Queue。

注意

* 用户不能通过接口 `cnrtQueueDestroy()` 进行销毁；
* 默认Queue对象的创建与销毁由库自行管理，用户无需管理。

### 6.5.11.3. 显式同步

Queue的同步操作是用来等待当前Queue中的所有任务均执行完成的接口，当前有多种同步的操作。

* `cnrtSyncDevice()` 等待所有设备上的所有Queue执行完成。
* `cnrtQueueSync()` 该接口会有一个Queue对象参数，接口会等待指定Queue的所有操作均执行完成。当Queue中的操作发生异常后，该Queue会立即返回并返回失败；并且此时Queue不再能够下发任务。
* `cnrtQueueQuery()` 该接口会有一个Queue对象参数，接口会查询指定Queue的操作执行状态；当指定Queue中的所有任务均执行完成，则会返回 `cnrtSuccess` ；否则返回 `cnrtErrorNotReady`。
* `cnrtQueueWaitNotifier()` 该接口会有一个Queue参数以及Notifier参数。关于Notifier说明请参见 [第 6.5.12 章节] [Notifier]。所有在指定Queue中并且在该接口之后下发的操作，均需要等待对应的Notifier执行完成。

注意

`cnrtQueueSync()` 以及 `cnrtQueueQuery()` 接口中的Queue参数同样支持传入0值，此时则查询默认Queue的状态。若Queue内部从未有过任何操作，同步接口也会返回成功。

### 6.5.11.4. 同步行为设置

对于 `cnrtSyncDevice()` 以及 `cnrtQueueSync()` 同步操作接口来说，由于可能存在有多个主机侧线程调用同时同步的场景，此时等待的操作同步完成的方式则对性能以及CPU的占用率有影响。因此，对于同步操作提供了几种同步的方式，供不同的场景进行选择。

#### 6.5.11.4.1. 同步行为的选择方法

可选择的同步行为如下：

```
typedef enum {
    cnrtDeviceScheduleSpin = 0,
    /*!< CPU actively spins when waiting for the Device execution result. */
    cnrtDeviceScheduleBlock = 1,
    /*!< CPU thread is blocked on a synchronization primitive when waiting for the Device execution results. */
    cnrtDeviceScheduleYield = 2,
    /*!< CPU thread yields when waiting for the Device execution results. */
    cnrtDeviceFlagsMaxNum
    /*!< The last one. */
} cnrtDeviceFlags_t;

```

用户在调用同步接口之前，通过 `cnrtSetDeviceFlag()` 接口设置同步操作行为，将对应的同步操作行为枚举 `cnrtDeviceFlags_t` 设置为对应的同步操作行为。

同步操作行为的区别如下：

* `cnrtDeviceScheduleSpin` ： 调用同步接口的线程，会一直循环查询等待操作完成。此行为性能最优，但在多线程同时调用同步操作的场景下，可能会导致CPU占用率较高。一般多用于对性能要求较高，但对CPU占用率不敏感的场景。
* `cnrtDeviceScheduleBlock` ：调用同步接口的线程，会被阻塞在等待接口中，执行完成后才会继续执行。此行为可以减少多线程调用同步操作时CPU占用率，但性能会略微下降。一般多用于对性能不敏感，但要求CPU占用率较低的场景。
* `cnrtDeviceScheduleYield` ：调用同步接口的线程，会被阻塞在等待接口中，并主动让出阻塞线程供其他执行线程进行执行。此行为主动让出当前线程，因此当前CPU负载较高时其性能会较低，而负载较低时性能较高；仅有同步操作线程时，其行为等同于 `cnrtDeviceScheduleSpin`。

注意

对于端侧平台，当前默认方式为 `cnrtDeviceScheduleBlock`，其主要目的是为了最大化利用端侧 CPU 资源；而对于云侧平台，默认为 `cnrtDeviceScheduleSpin` 方式，主要是为了最大化性能。

### 6.5.11.5. 不同Queue操作的重叠行为

由于同一个Queue操作顺序执行，不同Queue则乱序执行，因此，对于不同Queue的大量操作，会出现重叠行为（Overlap Behavior）。

对于下面的用例代码：

```
for （int i = 0; i < 2; i++） {
    cnrtMemcpyAsync(inputAddr + i * size, hostAddr + i * size,
                    size, Queue[i], cnrtMemcpyHostToDev);
    Kernel<<<1, 1, 1, Queue[i]>>>(OutputAddr + i * size, inputAddr + i * size, size);
    cnrtMemcpyAsync(hostAddr + i * size, OutputAddr + i * size,
                    size, Queue[i], cnrtMemcpyDevToHost);
}

```

由于主机与设备的数据拷贝操作可以与Kernel并行执行，因此对于Queue[1]上的主机到设备的数据拷贝操作就与Queue[0]上的Kernel重叠；同样而言，Queue[1]上的Kernel Launch则与Queue[0]上的设备到主机数据拷贝操作重叠。该重叠行为可以掩盖掉一部分执行时间，从而提高代码性能。

### 6.5.11.6. Queue的优先级

具有优先级的Queue可以用调用 `cnrtQueueCreateWithPriority()` 来创建。优先级的范围是 [最高优先级，最低优先级]，最高优先级和最低优先级的数值可以通过 `cnrtDeviceGetQueuePriorityRange()` 来获取。在运行时，高优先级的Queue中的待处理任务的执行将优先于低优先级的Queue中的待处理任务。

下面的用例代码展示了如何获取优先级范围并创建最大优先级队列和最小优先级队列：

```
int priority_highest, priority_lowest;
cnrtDeviceGetQueuePriorityRange(&priority_lowest, &priority_highest);
cnrtQueue_t queue_highest, queue_lowest;
cnrtQueueCreateWithPriority(&queue_lowest, 0, priority_lowest);
cnrtQueueCreateWithPriority(&queue_highest, 0, priority_highest);

```

## 6.5.12. Notifier

Notifier可以帮助用户对Queue中的任务执行进行管理与检查。Notifier可以记录当前Queue还未被执行的任务，或是作为一个等待任务被放置到队列中执行。相比计算任务Notifier不执行实际的硬件操作。当Notifier记录了某个Queue未被执行的任务状态后，可以在另一个Queue中等待该Queue的未被执行的任务完成。Notifier会在记录的Queue任务都完成后记录一个时间，可以通过两个Notifier来实现对计算任务耗时的统计。

表 6.4 CNRT 常用Notifier管理接口

| API 名称 | 功能简介 |
| --- | --- |
| `cnrtNotifierCreate()` | 创建一个Notifier。 |
| `cnrtNotifierDestroy()` | 删除一个Notifier。 |
| `cnrtPlaceNotifier()` | 使Notifier记录一个任务队列中还未完成的任务。 |
| `cnrtQueueWaitNotifier()` | 任务队列等待Notifier记录的任务完成。 |
| `cnrtWaitNotifier()` | 等待Notifier记录的任务队列任务完成。 |
| `cnrtQueryNotifier()` | 查询Notifier记录的任务队列任务是否完成。 |
| `cnrtNotifierElapsedTime()` | 计算两个Notifier之间所经过的时间（单位：毫秒）。 |
| `cnrtIpcGetNotifierHandle()` | 获取Notifier的IPC句柄。 |
| `cnrtIpcOpenNotifierHandle()` | 从一个IPC句柄创建一个新的Notifier。 |

### 6.5.12.1. 创建和销毁Notifier

通过 `cnrtNotifierCreate()` 接口与 `cnrtNotifierCreateWithFlags()` 接口可以创建Notifier对象，在使用到需要Notifier对象的接口时，把创建的Notifier对象作为参数。

通过 `cnrtNotifierDestroy()` 接口可以将Notifier对象进行销毁。

创建和销毁Notifier示例代码示例如下。

```
cnrtNotifier_t notifier_start;
cnrtNotifier_t notifier_end;

/* Create notifier*/
ERROR_CHECK(cnrtNotifierCreate(&notifier_start));
/* Create notifier*/
ERROR_CHECK(cnrtNotifierCreate(&notifier_end));

/* Destroy notifier*/
ERROR_CHECK(cnrtNotifierDestroy(notifier_start));
/* Destroy notifier*/
ERROR_CHECK(cnrtNotifierDestroy(notifier_end));

/* Create notifier with flags */
ERROR_CHECK(cnrtNotifierCreateWithFlags(&notifier_start, CNRT_NOTIFIER_DISABLE_TIMING_ALL));
/* Create notifier with flags same as cnrtNotifierCreate */
ERROR_CHECK(cnrtNotifierCreateWithFlags(&notifier_end, 0));

```

注意

如果在销毁Notifier对象时，Notifier仍然有记录的Queue任务还未完成，或是在Queue中有还未完成的 `cnrtQueueWaitNotifier()` 任务，这些任务都会被正常的结束。

### 6.5.12.2. 记录用户队列任务

使用 `cnrtPlaceNotifier()` 接口，可以在Notifier对象中，记录Queue中此时刻还未完成的任务。通过 `cnrtQueryNotifier()` 与 `cnrtWaitNotifier()` 接口，可以查询或等待这些被记录的任务完成。通过 `cnrtQueueWaitNotifier()` 接口，可以在另一个Queue上等待这些任务完成。当Notifier对象创建时没有使用 `CNRT_NOTIFIER_DISABLE_TIMING_SW` 或 `CNRT_NOTIFIER_DISABLE_TIMING_ALL`，在被记录到的这些任务完成时，Notifier对象将记录此时的时间。

注意

* `cnrtPlaceNotifier()` 接口传入的Queue与Notifier对象，必须创建于同一个Context。
* 当多次调用 `cnrtPlaceNotifier()` 接口时，Notifier对象只会记录最后一次时的Queue任务状态。但不会影响已经调用接口完成的 `cnrtQueueWaitNotifier()` 所记录的任务。
* Notifier对象只能记录Queue的任务完成与否，但不能记录任务执行是否成功。如果 `cnrtPlaceNotifier()` 接口记录的Queue任务发生了异常，`cnrtQueryNotifier()` 与 `cnrtWaitNotifier()` 接口仍然会返回成功。在另一个Queue中的 `cnrtQueueWaitNotifier()` 任务仍然会正常完成。

### 6.5.12.3. 用户等待Notifier任务完成

当使用 `cnrtPlaceNotifier()` 接口，在Notifier中记录了某个Queue的任务后，用户可以使用接口 `cnrtQueryNotifier()` 与 `cnrtWaitNotifier()` 检查或等待任务是否完成。 `cnrtWaitNotifier()` 接口会阻塞用户线程，直到被记录的Queue任务都完成。

```
cnrtMemcpyAsync(host_mem1, dev_mem1, size, queue_1, cnrtMemcpyDevToHost);
cnrtPlaceNotifier(notifier_1, queue_1);

/* Wait until queue task finish */
cnrtWaitNotifier(notifier_0);

```

### 6.5.12.4. 多队列间同步

当使用 `cnrtPlaceNotifier()` 接口，在Notifier中记录了某个Queue的任务后，可以使用接口 `cnrtQueueWaitNotifier()` 在其他Queue中等待这些任务都完成，以实现多队列间的同步功能。

用Notifier实现多队列间同步的代码示例如下：

```
// set: dev_mem0, dev_mem1, host_mem0, host_mem1, size...
cnrtNotifier_t notifier_0;
cnrtNotifier_t notifier_1;
cnrtNotifierCreate(&notifier_0);
cnrtNotifierCreate(&notifier_1);

cnrtQueue_t queue_0;
cnrtQueue_t queue_1;
cnrtQueueCreate(&queue_0);
cnrtQueueCreate(&queue_1);

cnrtMemcpyAsync(dev_mem0, host_mem0, size, queue_0, cnrtMemcpyHostToDev);
cnrtPlaceNotifier(notifier_0, queue_0);
cnrtMemcpyAsync(host_mem1, dev_mem1, size, queue_1, cnrtMemcpyDevToHost);
cnrtPlaceNotifier(notifier_1, queue_1);

// queue_1 wait queue_0, queue_0 wait queue_1
cnrtQueueWaitNotifier(notifier_0, queue_1, 0);
cnrtQueueWaitNotifier(notifier_1, queue_0, 0);

cnrtQueueSync(queue_0);
cnrtQueueSync(queue_1);

```

### 6.5.12.5. 统计时间

通过调用 `cnrtNotifierElapsedTime()` 接口，用户可以获得传入的两个Notifier对象分别记录的Queue任务完成的时间差。

使用Notifier统计时间的代码示例如下。

```
void cnrtNotifierElapsedKernel(void) {
    cnrtNotifier_t start, end[5];
    cnrtQueue_t queue;

    ERROR_CHECK(cnrtNotifierCreate(&start));
    for (int i = 0; i < 5; i++) {
        ERROR_CHECK(cnrtNotifierCreate(&end[i]));
    }

    ERROR_CHECK(cnrtQueueCreate(&queue));

    ERROR_CHECK(cnrtPlaceNotifier(start, queue));
    for (int i = 0; i < 5; i++) {
        simpleKernelAdd(queue);
        ERROR_CHECK(cnrtPlaceNotifier(end[i], queue));
    }

    ERROR_CHECK(cnrtQueueSync(queue), cnrtSuccess);
    float ms[5];
    for (int i = 0; i < 5; i++) {
        ERROR_CHECK(cnrtNotifierElapsedTime(start, end[i], &ms[i]));
        printf("basic test elapsed time is %f\n", ms[i]);
    }

    ERROR_CHECK(cnrtNotifierDestroy(start));
    for (int i = 0; i < 5; i++) {
        ERROR_CHECK(cnrtNotifierDestroy(end[i]));
    }

    ERROR_CHECK(cnrtQueueDestroy(queue));
    ERROR_CHECK(cnrtDeviceReset());
}

```

注解

* 使用 `CNRT_NOTIFIER_DISABLE_TIMING_ALL` 创建的Notifier对象，不允许用户使用时间获取接口，也不会统计时间。
* 用户需要自行维护 `start` 与 `end` 的Notifier对象的先后顺序，否则得到的时间值可能为负。

### 6.5.12.6. IPCNotifier接口

通过IPCNotifier接口，用户可以将一个Notifier对象的信息通过 `cnrtIpcNotifierHandle` 在多个用户进程间共享。一个进程可以等待另一个进程记录的Queue任务完成。

使用IPCNotifier功能的代码示例如下。

```
struct ipc_sample_s {
  cnrtIpcMemHandle m_handle;
  cnrtIpcNotifierHandle n_handle;
  int start;
};
void IpcNotifierSample(void) {
  cnrtQueue_t queue;
  cnrtNotifier_t notifier;
  void *dev_mem;
  char *host_mem;
  int mem_size = 0x100;
  struct ipc_sample_s *info = (struct ipc_sample_s *)mmap(NULL, sizeof(struct ipc_sample_s),
                               PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0);
  info->start = 0;
  pid_t pid = fork();
  if (0 == pid) {
    host_mem = calloc(1, mem_size);
    CNRT_CHECK(cnrtSetDevice(0));
    CNRT_CHECK(cnrtQueueCreate(&queue));
    while(0 == info->start) { sched_yield(); }
    CNRT_CHECK(cnrtMapMemHandle(&dev_mem, info->m_handle, 0))
    CNRT_CHECK(cnrtIpcOpenNotifierHandle(&notifier, info->n_handle));
    CNRT_CHECK(cnrtQueueWaitNotifier(notifier, queue, 0));
    CNRT_CHECK(cnrtMemcpyAsync(dev_mem, (void *)host_mem,
                    mem_size, queue, cnrtMemcpyDevToHost));
    CNRT_CHECK(cnrtQueueSync(queue));

    // check host_mem value

    CNRT_CHECK(cnrtNotifierDestroy(notifier));
    CNRT_CHECK(cnrtQueueDestroy(queue));
    CNRT_CHECK(cnrtUnMapMemHandle(dev_mem));
    free(host_mem);
    exit(EXIT_SUCCESS);
  } else {
    int status;
    host_mem = calloc(1, mem_size);
    memset(host_mem, 0xAB, mem_size);
    CNRT_CHECK(cnrtSetDevice(0));
    CNRT_CHECK(cnrtQueueCreate(&queue));
    CNRT_CHECK(cnrtNotifierCreateWithFlags(&notifier,
               CNRT_NOTIFIER_DISABLE_TIMING_ALL | CNRT_NOTIFIER_INTERPROCESS));
    CNRT_CHECK(cnrtMalloc(&dev_mem, mem_size));
    CNRT_CHECK(cnrtAcquireMemHandle(&info->m_handle, dev_mem));
    CNRT_CHECK(cnrtIpcGetNotifierHandle(&info->n_handle, notifier));
    CNRT_CHECK(cnrtMemcpyAsync(dev_mem, (void *)host_mem,
                    mem_size, queue, cnrtMemcpyHostToDev));
    CNRT_CHECK(cnrtPlaceNotifier(notifier, queue));
    __sync_add_and_fetch(&info->start, 1);

    /* Wait child finish */
    waitpid(pid, &status, 0);

    CNRT_CHECK(cnrtQueueSync(queue));
    CNRT_CHECK(cnrtNotifierDestroy(notifier));
    CNRT_CHECK(cnrtQueueDestroy(queue));
    CNRT_CHECK(cnrtFree(dev_mem));
    free(host_mem);

    exit(EXIT_SUCCESS);
  }
}

```

注解

* 通过检查 `cnrtAttrIPCNotifierSupported` 的值，可以确定当前设备环境是否支持使用 `cnrtIpcGetNotifierHandle()` 接口。
* 如果要使用 `cnrtIpcGetNotifierHandle()` 接口得到Notifier的 `cnrtIpcNotifierHandle` 对象，在创建Notifier对象时，需要同时传入 `CNRT_NOTIFIER_DISABLE_TIMING_ALL` 与 `CNRT_NOTIFIER_INTERPROCESS`。
* 通过 `cnrtIpcOpenNotifierHandle()` 接口得到的Notifier对象，需要使用 `cnrtNotifierDestroy()` 接口进行销毁。
* 通过 `cnrtIpcOpenNotifierHandle()` 接口得到的Notifier对象，不能再次调用 `cnrtIpcGetNotifierHandle()` 接口。
* 通过 `cnrtIpcGetNotifierHandle()` 得到的 `cnrtIpcNotifierHandle` 对象，不能在当前进程中使用。
* 通过 `cnrtIpcGetNotifierHandle()` 得到的 `cnrtIpcNotifierHandle` 对象，在所使用的Notifier对象被释放后，将变为无效对象。

## 6.5.13. 错误检查

CNRT 提供了一系列的错误管理接口，帮助用户更方便地调试 CNRT 程序。

表 6.5 CNRT 常用错误管理接口

| API 名称 | 功能简介 |
| --- | --- |
| `cnrtGetErrorName()` | 获取错误码表示的错误名。 |
| `cnrtGetErrorStr()` | 获取错误码表示的错误信息。 |
| `cnrtGetLastError()` | 获取最近一次调用 CNRT 接口的错误码，错误码返回后会重置为 `cnrtSuccess`。 |
| `cnrtPeekAtLastError()` | 获取最近一次调用 CNRT 接口的错误码, 错误码返回后不会重置。 |

## 6.5.14. TaskTopo

TaskTopo 提供了将异步任务打包下发的编程模型。
一个 TaskTopo 是一系列异步任务流的描述，基于 TaskTopo 编程模型可以实现任务流一次定义多次运行。

将任务流的定义和执行分开，带来了如下好处：

1. 在实例化阶段可以做更多准备工作，降低任务下发时候的 CPU 负载。
2. 将整个任务流呈现给 CNRT，有利于底层做更多优化。

表 6.6 TaskTopo 相关术语解释

| 术语 | 说明 |
| --- | --- |
| TaskTopo | 任务图。描述任务流的数据结构，包含了任务节点和任务节点的依赖关系。 |
| TaskTopoEntity | 任务图实体，简称实体图。特定时刻任务图的快照，并按照执行逻辑组织数据结构。 |
| TaskTopoNode | 任务节点。描述具体的异步任务，包括了任务的类型和参数信息。节点间的有向边描述了节点间的依赖关系。 |

使用 TaskTopo 下发任务，如 [图 6.13] [TaskTopo 的编程模型] 所示，包括三个阶段：

* 构建阶段：描述图中任务节点的操作和任务之间的依赖关系。该阶段生成任务图描述符。
* 实例化阶段：创建任务图的快照，对该任务图的可执行性进行基本的验证，并进行大部分初始化操作以
  降低启动开销。该阶段生成实体图描述符。
* 执行阶段：实体图可以像其他异步任务一样被下发到 Queue 上运行，也可以在任意 Queue 上重复下发。

![digraph TaskTopoWorkFlow {
	graph [fontname="Noto Sans CJK SC"; rankdir=TB; margin=0.01]
	node [fontname="Noto Sans CJK SC";fontsize="10"]
	edge [fontname="Noto Sans CJK SC";fontsize="10"]

	"build_tasktopo" [shape=plaintext;label=""]

	node [shape="ellipse"; style="filled"; colorscheme=blues9]
	"TaskTopo" [label="任务图\nTaskTopo";color=2]
	"TaskTopoEntity" [label="实体图\nTaskTopoEntity";color=3]
	"AsyncTaskRun" [shape=round; label="异步任务执行"; color=9; fontcolor=white]

	edge [dir=forward;minlen=1]
	"build_tasktopo" -> "TaskTopo" [label=" 构建\l 修改"]
	"TaskTopo" -> "TaskTopoEntity" [label=" 实例化\l cnrtTaskTopoInstantiate"]
	"TaskTopoEntity" -> "AsyncTaskRun" [label=" 任务下发\l cnrtTaskTopoEntityInvoke"]
}](images/graphviz-dcaed8413caab449b1626ec68d37a7fd309d7275.png)
![](images/graphviz-dcaed8413caab449b1626ec68d37a7fd309d7275.png)

图 6.13 TaskTopo 的编程模型

```
// 任务图编程实例
cnrtTaskTopo_t topo;
cnrtTaskTopoEntity_t entity;

// 创建任务图
cnrtTaskTopoCreate(&topo, 0);

// 构图
// ...

// 实例化
cnrtTaskTopoInstantiate(&entity, topo, NULL, NULL, 0);

// 原始任务图可销毁，不影响实体图运行
cnrtTaskTopoDestroy(topo);

// 实体图可下发到不同的 Queue 中执行，或者在同一个 Queue 中多次下发
cnrtTaskTopoEntityInvoke(entity, queue0);
cnrtTaskTopoEntityInvoke(entity, queue0);
cnrtTaskTopoEntityInvoke(entity, queue1);

```

### 6.5.14.1. 任务图的结构

任务图是任务流的描述，使用节点与节点之间的有向边表示异步任务和异步任务的依赖关系。

任务图支持以下类型的任务节点：

表 6.7 任务图中支持的节点类型

| 节点类型 | 说明 |
| --- | --- |
| KernelNode | Kernel 任务节点。 |
| HostFuncNode | CPU 函数任务节点。 |
| AsyncMemcpyNode | 异步内存拷贝节点。目前仅支持 1D 拷贝。 |
| AsyncMemsetNode | 异步内存初始化节点。目前仅支持 1D 初始化。 |
| EmptyNode | 空节点。不执行任何操作，可以作为任务的聚合点。 |
| ChildNode | 子图节点。将任务图作为一个任务节点嵌套到另一个任务图中。 |

注意

创建或修改 AsyncMemcpyNode 异步拷贝节点时，所使用到的 Host 内存地址必须是页锁定的内存(请参考 [主机侧页锁定机制内存管理] )。否则可能会导致 TaskTopo 的运行性能受损，或执行出错。

![digraph child_task_topo_example {
	graph [rankdir=TB;compound=true;dpi=60]
	node [shape=circle; width=0.6]
	A;
	B [color=blue];
	C;W;X;Y;Z

	subgraph cluster_graph1 {
		color="black"
		label="TaskTopo A"
		A;B;C
	}

	subgraph cluster_graph_child_B {
		label="TaskTopo B"
		color=blue;
		W;X;Y;Z;
	}

	edge [arrowhead=vee]
	A -> B -> C
	W -> {X, Y} -> Z

	edge [arrowhead=normal;style=dashed]
	B:e -> X [lhead=cluster_graph_child_B; label="Node[B] is TaskTopo B"]
}](images/graphviz-a4571198d9d561130e0bc1c99a076df2325ad821.png)
![](images/graphviz-a4571198d9d561130e0bc1c99a076df2325ad821.png)

图 6.14 子任务图示例

注解

[图 6.14] [子任务图示例] 中，TaskTopo B 是一个独立的任务图，也是任务图 A 中的一个子图节点。

### 6.5.14.2. 使用 TaskTopo 接口构造任务图

CNRT 提供了一组接口，支持创建和修改任务图。主要包括以下接口：

表 6.8 TaskTopo 主要接口

| 接口 | 说明 |
| --- | --- |
| `cnrtTaskTopoCreate` | 创建任务图。 |
| `cnrtTaskTopoDestroy` | 销毁任务图（包括其中节点和边）。 |
| `cnrtTaskTopoAdd<TYPE>Node` | 添加一个任务节点（ `<TYPE>` 为节点任务类型）。 参数包括该节点依赖的前置任务节点，以及该节点的参数（如果有参数）。 |
| `cnrtTaskTopoDestroyNode` | 销毁任务图中具体节点以及节点的边。 |
| `cnrtTaskTopo<TYPE>Node(Get|Set)Params` | 获取/修改节点的参数。 |
| `cnrtTaskTopoNodeGetDependencies` | 获取依赖该任务节点的任务节点（后继任务节点）。 |
| `cnrtTaskTopoNodeGetDependentNodes` | 获取该任务节点依赖的任务（前序任务节点）。 |
| `cnrtTaskTopoRemoveDependencies` | 删除两个节点之间的依赖边。 |
| `cnrtTaskTopoNodeGetType` | 获取任务节点类型。 |
| `cnrtTaskTopoGetEdges` | 获取任务图的所有依赖边。 |
| `cnrtTaskTopoGetNodes` | 获取任务图的所有节点。 |
| `cnrtTaskTopoGetRootNodes` | 获取任务图的所有根（入度为0）节点。 |
| `cnrtTaskTopoClone` | 复制任务图，生成当前任务图的快照。生成的任务图和原始任务图生命周期独立。 |
| `cnrtTaskTopoNodeFindInClone` | 根据原始任务图中的节点，获取复制图中对应的节点。 |

使用 TaskTopo 接口构建任务图的示例如下。

![digraph create_task_topo_api_example {
	graph [rankdir=TB;compound=true;margin=0.01;dpi=50]
	edge [color=navy]
	node [shape=circle; width=0.6; fontcolor=navy; color=navy; fontsize=20]

	A -> {B, C} -> D
	{C, D} -> E
}](images/graphviz-8d79a3b5f25c611fcb26cd9cd20316d0fd691ae7.png)
![](images/graphviz-8d79a3b5f25c611fcb26cd9cd20316d0fd691ae7.png)

图 6.15 使用 TaskTopo 接口创建任务图

```
cnrtTaskTopo_t topo;
cnrtTaskTopoNode_t a, b, c, d, e;

// 创建一个空的任务图
cnrtTaskTopoCreate(&topo, 0);

// 添加任务节点，但是不建立任务间依赖关系
cnrtTaskTopoAddKernelNode(&a, topo, NULL, 0, &nodeParams);
cnrtTaskTopoAddKernelNode(&b, topo, NULL, 0, &nodeParams);
cnrtTaskTopoAddKernelNode(&c, topo, NULL, 0, &nodeParams);
cnrtTaskTopoAddKernelNode(&d, topo, NULL, 0, &nodeParams);

// 建立任务节点间的依赖关系
cnrtTaskTopoAddDependencies(topo, &a, &b, 1); // A->B
cnrtTaskTopoAddDependencies(topo, &a, &c, 1); // A->C
// 同时建立多组依赖关系
cnrtTaskTopoNode_t nodeFrom[] = {b, c};
cnrtTaskTopoNode_t nodeTo[] = {d, d};
cnrtTaskTopoAddDependencies(topo, nodeFrom, nodeTo); // B->D;C->D

// 添加任务节点同时建立依赖关系
cnrtTaskTopoNode_t nodeDeps[] = {c, d};
cnrtTaskTopoAddKernelNode(&e, topo, nodeDeps, 2, &nodeParams); // C->E;D->E

```

注意

`cnrtTaskTopo_t` 的访问修改不是线程安全的，如果用户需要在多个线程同时访问或修改
`cnrtTaskTopo_t` 需要加锁保护。

### 6.5.14.3. 使用 QueueCapture 接口构造任务图

将一段异步任务下发的代码插入在 `cnrtQueueBeginCapture()` 和 `cnrtQueueEndCapture()` 之间，
即可完成任务图的构造。这套机制可以在不对已有代码做任何修改的前提下创建任务图。示例如下：

```
// 设置一个 Queue 为捕获模式
cnrtQueueBeginCapture(queue, cnrtQueueCaptrueModeGlobal);

// 向这个 Queue 中下发异步任务
kernel_A<<<..., queue>>>(...);
kernel_B<<<..., queue>>>(...);
anotherFuncCall(queue, ...);
libraryCall(queue, ...);
cnrtInvokeHostFunc(queue, ...);

// 结束捕获，并得到任务图
cnrtQueueEndCapture(queue, &taskTopoOut);

```

调用 `cnrtQueueBeginCapture()` 可以将一个 Queue 设置为捕获模式。

下发到捕获模式的 Queue 中的异步任务不会直接下发执行，而是被加入到一个内部的任务图
成为一个任务节点。
任务节点间的依赖关系由任务在 Queue 中的下发顺序决定，后下发的任务依赖前序被捕获的任务。
后续任务依赖的任务节点，称为捕获 Queue 中后续被捕获任务的 **前序依赖节点**。

提示

* 前序依赖节点可能有多个，参考 [第 6.5.14.3.1 节] [多 Queue 间任务借助 Notifier 建立依赖]。
* 前序依赖可被修改，修改方式参考 [表 6.9][QueueCapture 主要接口] 中 `cnrtQueueUpdateCaptureDependencies()`。

调用 `cnrtQueueEndCapture()` 时候会结束该 Queue 的捕获模式，同时返回任务图。
后续该任务图和捕获用的 Queue 没有任何绑定关系，实例化后可以下发到任意 Queue 上。

注意

* 处于捕获模式的 Queue 不可进行 Sync 和 Query 操作，否则接口会报错
  `cnrtErrorQueueCaptureUnsupported`，同时捕获也会失败。
* Queue 捕获失败到结束捕获前：

  + 下发到该 Queue 的异步任务都会返回 `cnrtErrorQueueCaptureInvalidated`。
  + 结束捕获也会报错 `cnrtErrorQueueCaptureInvalidated`，但是可以正常结束该 Queue 的捕获模式。
* Queue 捕获不可重入，对捕获状态的 Queue 开始捕获操作会报错 `cnrtErrorIllegalState`。
* 捕获模式下所调用的 MemcpyAsync 异步任务，所使用到的 Host 内存地址必须是页锁定的(请参考 [主机侧页锁定机制内存管理] )。否则可能会导致后续生成的 TaskTopo 的运行性能受损，或执行出错。

注解

* 目前不支持对 Default Queue 进行捕获操作。
* 捕获模式下的内部任务图和其中的节点不可销毁。结束捕获后返回该任务图就无此限制。

表 6.9 QueueCapture 主要接口

| 接口名 | 说明 |
| --- | --- |
| `cnrtQueueBeginCapture` | 设置 Queue 为捕获模式。 |
| `cnrtQueueEndCapture` | 结束 Queue 的捕获模式，并返回捕获生成的 TaskTopo。 |
| `cnrtQueueIsCapturing` | 获取 Queue 是否处于捕获状态。 |
| `cnrtQueueGetCaptureInfo` | 获取 Queue 捕获信息。 |
| `cnrtQueueUpdateCaptureDependencies` | 修改 Queue 后续捕获任务的前序依赖。 |

Queue 捕获支持三种启动模式，参见 [表 6.10] [QueueCapture 模式和说明]。

表 6.10 QueueCapture 模式和说明

| 模式 | 说明 |
| --- | --- |
| `cnrtQueueCaptureModeGlobal` | 全局捕获模式。  开始捕获和结束捕获必须在同一个线程。 当前进程任意一个 Queue 进入全局捕获模式，会有如下约束：   * 任意 Queue 的 query 和 sync 操作会失败，并导致当前捕获状态异常。 * 任意 TaskTopo 实例化会失败，并导致当前捕获状态异常。 * 同步内存分配会失败，并导致当前捕获状态异常。 |
| `cnrtQueueCaptureModeThreadLocal` | 线程局部捕获模式（暂不支持）。  开始捕获和结束捕获必须在同一个线程。 任意线程下以 ThreadLocal 模式开始 Queue 捕获，该线程下的约束同全局模式。 其他线程不受影响。 |
| `cnrtQueueCaptureModeRelaxed` | 无约束捕获模式。 |

#### 6.5.14.3.1. 多 Queue 间任务借助 Notifier 建立依赖

Queue 捕获支持借助 `cnrtPlaceNotifier()` 和 `cnrtQueueWaitNotifier()` 接口建立多 Queue 间
任务的依赖关系，将不同 Queue 中任务合并到一个任务图中。

当调用 `cnrtQueueBeginCapture()` 设置 Queue1 为捕获模式，下发到 Queue1 中的任务会被加入到
TaskTopo 中。
期间调用 `cnrtPlaceNotifier()` 接口，使用 Notifier 记录一个捕获状态的 Queue1 时，
它会记录当前捕获 Queue1 的前序依赖节点集合，称此时 Notifier 为 **捕获 Notifier**。

当另一个非捕获模式的 Queue2 调用 `cnrtQueueWaitNotifier()` 等待这个捕获 Notifier 时候， Queue2 也会被
设置为捕获模式。此时 Queue2 的前置依赖节点集合会复制捕获 Notifier 的前置依赖节点集合。
在 Queue2 结束捕获模式前，下发到 Queue2 中的异步任务也会被加入到 TaskTopo 中。
为了便于说明，称呼 Queue1 为原始捕获 Queue，Queue2 为分支捕获 Queue。只有发起 `cnrtQueueBeginCapture()`
的 Queue 才可以调用 `cnrtQueueEndCapture()` 结束捕获状态，即只有原始捕获 Queue 可以结束捕获状态。

使用 Notifier 记录一个分支捕获 Queue2，之后原始捕获 Queue1 等待该 Notifier ，两个 Queue 将会聚合。
两个 Queue 上的任务前置依赖集合会合并到 Queue1。

当所有分支捕获 Queue 都聚合回原始捕获 Queue，原始捕获 Queue 才可以正常调用接口结束捕获状态，否则
整个捕获操作都会失败，无法返回 TaskTopo。

使用 QueueCapture 逻辑生成 [图 6.15][使用 TaskTopo 接口创建任务图] 的参考代码如下所示。

```
// 设置 Queue1 为捕获模式
cnrtQueueBeginCapture(&queue1, cnrtQueueCaptureModeGlobal);

// 往 Queue1 中 TaskTopo 添加任务节点 A
// 当前 Queue1 的前置依赖节点为 {A}
kernel_A<<<..., queue1>>>();

// Queue1 分叉到 Queue2
// Queue2 的前置依赖节点复制自 Queue1，为 {A}
cnrtPlaceNotifierWithFlags(notifier, queue1, CNRT_NOTIFIER_PLACE_DEFAULT);
cnrtQueueWaitNotifier(notifier, queue2, CNRT_NOTIFIER_WAIT_DEFAULT);

// 下发任务 B 到 Queue1，建立边 A->B
// Queue1 的前置依赖节点为 {B}
kernel_B<<<..., queue1>>>();
// 下发任务 C 到 Queue1，建立边 A->C
// Queue2 的前置依赖节点为 {C}
kernel_C<<<..., queue2>>>();

// 合并 Queue2 的到 Queue1
// 当前 Queue1 的前置依赖节点为 {B, C}
cnrtPlaceNotifierWithFlags(notifier, queue2, CNRT_NOTIFIER_PLACE_DEFAULT);
cnrtQueueWaitNotifier(notifier, queue1, CNRT_NOTIFIER_WAIT_DEFAULT);

// 下发任务到 Queue1，建立边 B->D 和 C->D
// Queue1 的前置依赖节点为 {D}
kernel_D<<<..., queue1>>>();

// 再次合并 Queue2 到当前的 Queue1
// 当前 Queue1 的前置依赖节点为 {C, D}
cnrtQueueWaitNotifier(notifier, queue1, CNRT_NOTIFIER_WAIT_DEFAULT);

// 下发任务 E 到 Queue1，建立依赖边 C->E 和 D->E
kernel_E<<<..., queue1>>>();

// 结束捕获
cnrtQueueEndCapture(queue1, &taskTopoOut);

```

注意

* 分支捕获 Queue 调用 `cnrtQueueEndCapture()` 会报错： `cnrtErrorQueueCaptureUnmatched`。
* 当原始捕获 Queue 结束捕获时仍有分支 Queue 未聚合会报错： `cnrtErrorQueueCaptureUnjoined`。
* 当两个无关的捕获序列尝试聚合时候会报错： `cnrtErrorQueueCaptureIsolation`。

### 6.5.14.4. 实体图执行与更新

如前文所述，任务图的执行需要调用 `cnrtTaskTopoInstantiate()` 实例化生成实体图才可下发。
实体图是任务图中任务流的一个快照，包括了任务信息及任务间的依赖关系，为了任务更快下发，
按照执行视角重新组织了数据结构，提前完成了必须的资源分配和初始化。

实体图不可修改图的拓扑结构，如果任务流结构有修改（比如：增加删除了节点；改变了依赖关系等），那么必须重新实例化生成新的实体图。如果仅仅是任务节点上参数有修改，可以直接修改实体图。

任务图进行实例化时候需要额外做一些不适合在构图阶段做的检查：

* 图是否有环，只有有向无环图才能表示一个有效的任务流。
* 任务节点的参数检查。
* 当前平台是否满足任务节点运行的检查（比如部分平台和驱动版本不支持 HostFunc 功能）。

生成的 TaskTopoEntity 数据结构和原始的 TaskTopo 生命周期无绑定关系，两者都可以独立的修改或者销毁。

#### 6.5.14.4.1. 实体图参数更新

CNRT 提供了两种方式更新实体图的参数：整图更新或者具体节点更新。

整图更新调用 `cnrtTaskTopoEntityUpdate()` 输入修改过参数的任务图更新对应实体图的参数。
要求任务图和实体图的任务流一样，否则更新失败。

```
// 整图更新示例
cnrtTaskTopo_t taskTopo = NULL;
cnrtTaskTopoEntity_t taskTopoEntity = NULL;
cnrtTaskTopoEntityUpdateResult updateResult;

for (size_t i = 0; i < 100; ++i) {
  // 捕获一段任务流
  cnrtQueueBeginCapture(queue, cnrtQueueCaptrueModeGlobal);
  // ...
  kernel_A<<<..., queue>>>();
  // ...
  libraryCall(queue);
  functionCall(queue);
  // ...
  cnrtQueueEndCapture(queue, &taskTopo);
  assert(taskTopoOut != NULL);

  // 如果 taskTopoEntity 已经实例化过，尝试更新它。更新的开销小于实例化
  if (taskTopoEntity != NULL) {
    cnrtTaskTopoEntityUpdate(taskTopoEntity, taskTopo, NULL, &updateResult);
  }

  // 如果 taskTopoEntity 没有实例化过，或者之前更新失败了尝试初始化它
  if (taskTopoEntity == NULL || updateResult != cnrtTaskTopoEntityUpdateSuccess) {
    if (taskTopoEntity != NULL) cnrtTaskTopoEntityDestroy(taskTopoEntity);
    cnrtTaskTopoInstantiate(&taskTopoEntity, taskTopo, NULL, NULL, 0);
    assert(taskTopoEntity != NULL);
  }

  cnrtTaskTopoDestroy(taskTopo);
  // 下发任务
  cnrtTaskTopoEntityInvoke(taskTopoEntity, queue);
  cnrtQueueSync(queue);
}

```

具体节点更新调用 `cnrtTaskTopoEntity<TYPE>NodeSetParams`，使用原始 TaskTopo 中任务节点更新 `TaskTopoEntity` 中对应的节点。

```
// 更新节点参数示例
cnrtTaskTopo_t taskTopo;
cnrtTaskTopoEntity_t taskTopoEntity;
cnrtTaskTopoNode_t node_a;

cnrtTaskTopoCreate(&taskTopo, 0);
cnrtKernelNodeParams_t nodeParams = {
  .func = (void *)kernel_A,
  .dim = {1, 1, 1},
  .type = cnrtFuncTypeBlock,
  .kernelParams = NULL,
  .extra = NULL;
};
cnrtTaskTopoAddKernelNode(&node_a, taskTopo, NULL, 0, &nodeParams);

// ...

cnrtTaskTopoInstantiate(&taskTopoEntity, taskTopo, NULL, NULL, 0);
cnrtTaskTopoEntityInvoke(taskTopoEntity, queue);

// 使用 taskTopo 中 node_a 更新 taskTopoEntity 中对应的节点
nodeParams.dim.x = 4;
cnrtTaskTopoEntityKernelNodeSetParams(taskTopoEntity, node, &nodeParams);
cnrtTaskTopoEntityInvoke(taskTopoEntity, queue);

cnrtQueueSync(queue);

```

#### 6.5.14.4.2. 实体图执行

实体图可以在任意 Queue 上被重复执行，以分摊构造任务图的开销。
一个实体图被下发到 Queue 中，会和 Queue 中其他任务保序，但是任务图实体内部无依赖关系的任务节点是可以并行执行的。

任务图中任务节点间的依赖关系不可被打乱， `cnrtTaskTopoEntity_t` 无法和自己并发执行。当一个实体图同时在多个
Queue 中下发时，实体图会被串行下发。

## 6.5.15. 队列有序内存分配器

队列有序内存分配器（Queue Order Memory Allocator）提供了内存异步申请和释放的编程模型，这使得内存申请释放任务可以在Queue上异步执行，和其他Queue上任务按照下发先后顺序执行。

队列有序内存分配器好处如下：

* 提高内存使用效率：内存分配器分配内存时，会根据异步申请释放内存任务的顺序，尽量重用内存。
* 支持用户控制分配器的内存缓存行为。当用户接受一定的内存占用时，可以设置内存释放阈值。此时分配器会缓存内存，这将大大减少内存申请释放造成的操作系统调用开销。
* 支持在进程间安全的共享内存。

此外，对一些应用程序来说，队列有序内存分配器可以提供高性能内存管理能力；对于已经实现了内存分配器的应用程序和库，队列有序内存分配器可以使多个库共享由驱动程序管理的公共内存池，从而提高内存利用率。

### 6.5.15.1. `cnrtMemAllocAsync` 和 `cnrtMemFreeAsync` 接口介绍

`cnrtMemAllocAsync` 和 `cnrtMemFreeAsync` 的参数 `Queue`，决定了分配的内存何时可以开始使用，以及何时停止使用。
`cnrtMemAllocAsync` 虽然是一个异步任务，但是调用接口时，接口同步的返回内存指针。
最简单的使用模式是分配、使用内存、释放都在同一个Queue中。

```
void *ptr;
size_t size = 512;
cnrtMemAllocAsync(&ptr, size, cnrtQueuePerThread);
// The kernel performs the assignment using the allocation
kernel<<<...,cnrtQueuePerThread>>>(ptr, ...);
cnrtMemFreeAsync(ptr, cnrtQueuePerThread);

```

当在分配Queue以外的Queue使用分配的内存时，用户必须保证访问操作在分配操作之后发生。同时，用户必须保证释放操作在分配操作和访问操作之后发生。在分配操作之前和释放操作之后对分配内存的任何使访问都会导致未定义的行为。
可以使用 Notifier 或者同步Queue保证不同Queue上分配、访问、释放顺序。

```
cnrtMemAllocAsync(&ptr, size, queue0);
cnrtPlaceNotifier(notifier, queue0);
// queue1 must wait for the allocation to be ready before accessing
cnrtQueueWaitNotifier(notifier0, queue1, CNRT_NOTIFIER_WAIT_DEFAULT);
kernel<<<...,queue1>>>(ptr, ...);
cnrtPlaceNotifier(notifier1, queue1);
// queue2 must wait for queue1 to finish accessing the allocation before freeing the allocation
cnrtQueueWaitNotifier(notifier1, queue2, CNRT_NOTIFIER_WAIT_DEFAULT);
cnrtMemFreeAsync(ptr, queue2);

```

通过 `cnrtMalloc()` 申请的内存可以通过 `cnrtMemFreeAsync()` 释放，用户依旧需要保证内存访问要在内存释放操作前完成。

```
cnrtMalloc(&ptr, size);
kernel<<<...,queue>>>(ptr, ...);
cnrtMemFreeAsync(ptr, queue);

```

可以使用 `cnrtFree()` 同步地释放 `cnrtMemAllocAsync()` 申请的内存。
使用 `cnrtQueueQuery()`， `cnrtQueueSync()`， `cnrtQueryNotifier()`， `cnrtWaitNotifier()` 或 `cnrtSyncDevice()` 保证异步操作已经完成。

```
cnrtMemAllocAsync(&ptr, size, queue);
kernel<<<...,queue>>>(ptr, ...);
// synchronize is needed to avoid prematurely freeing the memory
cnrtQueueSync(queue);
cnrtFree(ptr);

```

### 6.5.15.2. 内存池和 `cnrtMemPool_t` 结构体介绍

内存池封装了虚拟地址和物理内存资源，根据池的属性分配和管理这些资源。

可以调用 `cnrtDeviceGetDefaultMemPool()` 获取设备的默认内存池。设备对默认内存池申请出的内存始终可以读写。设备默认内存池不支持IPC。

注解

* 设备对默认内存池申请出的内存始终可以读写。
* 设备默认内存池不支持IPC。
* 设备默认内存池不需要用户显式创建，在调用相关接口时驱动隐式创建。

可以调用 `cnrtMemPoolCreate()` 创建一个显示池。显示池可以支持设置最大内存池大小、指定从某一个设备申请内存、IPC能力等。

```
cnrtMemPoolProps poolProps = {};

poolProps.type = cnrtMemAllocationTypeDefault;
poolProps.requestedHandleTypes = cnrtMemHandleTypeNone;
poolProps.location.type = cnrtMemLocationTypeDevice;
poolProps.location.id = 0;
poolProps.maxSize = 1 << 30;
cnrtMemPoolCreate(&memPool, &poolProps);

```

### 6.5.15.3. 内存池的缓存行为

默认情况下，分配器会尽量减少物理内存的占用。但为了尽量减少分配释放物理内存的操作系统调用开销，内存池支持缓存部分内存。应用程序可以给内存池设置内存缓存阈值 `cnrtMemPoolAttrReleaseThreshold`。

```
uint32_t value = UINT64_MAX;
cnrtMemPoolSetAttribute(mempool, cnrtMemPoolAttrReuseFollowNotifierDependencies, &value);

```

将 `cnrtMemPoolAttrReleaseThreshold` 设置成 `UINT64_MAX` 可以使内存池缓存的内存只增不减。同时此类用户有显式释放内存池缓存的需求，可以使用 `cnrtMemPoolTrimTo` 接口实现。用户设置参数 `minByteToKeep` 可以保留在后续执行过程中预期需要的内存量。

```
uint32_t value = UINT64_MAX;
cnrtMemPoolSetAttribute(mempool, cnrtMemPoolAttrReuseFollowNotifierDependencies, &value);

// application phase needing a lot of memory from the queue ordered allocator
for (i = 0; i < 10; i++) {
 for (j = 0; j < 10; j++) {
   cnrtMemAllocAsync(&ptrs[j], size[j], queue);
 }
 kernel<<<...,queue>>>(ptrs, ...);
 for (j = 0; j < 10; j++) {
   cnrtMemFreeAsync(ptrs[j], queue);
 }
}

// Process requires less memory for the next phase.
// Synchronize the process to inform the trimming operation that the allocations are no longer in use.
cnrtQueueSync(queue);
cnrtMemPoolTrimTo(mempool, 0);

// Other process/allocation mechanism can now use the physical memory released by the trimming operation.

```

### 6.5.15.4. 内存池的内存重用策略

分配器在处理内存分配请求时，会首先尝试重用 `cnrtMemFreeAsync()` 释放的内存，然后再尝试从操作系统分配更多内存。例如，在Queue中释放的内存可以被同一Queue中后续申请操作分配重用；当Queue同步时，先前在该Queue中释放的内存可以在任何Queue中重新分配。

分配器有几个可配置内存池属性决定分配器的分配策略 `cnrtMemPoolAttrReuseFollowNotifierDependencies`、 `cnrtMemPoolAttrReuseAllowOpportunistic`、 `cnrtMemPoolAttrReuseAllowInternalDependencies`。

#### 6.5.15.4.1. `cnrtMemPoolAttrReuseFollowNotifierDependencies`

在向操作系统申请更多的设备侧内存前，分配器检查由 Notifier 引入的Queue之间的依赖关系，尝试重用分配另一个Queue释放的内存。

```
cnrtMemAllocAsync(&ptr, size, originalQueue);
kernel<<<..., originalQueue>>>(ptr, ...);
cnrtMemFreeAsync(ptr, originalQueue);
cnrtPlaceNotifier(notifier, originalQueue);

// Waiting on the notifier that captures the free memory in another queue allows the allocator to reuse the memory to satisfy a new allocation request in the other queue when cnrtMemPoolAttrReuseFollowNotifierDependencies is enabled.
cnrtQueueWaitNotifier(notifier, newQueue, CNRT_NOTIFIER_WAIT_DEFAULT);
cnrtMemAllocAsync(&ptr, size, newQueue);

```

#### 6.5.15.4.2. `cnrtMemPoolAttrReuseAllowOpportunistic`

根据 `cnrtMemPoolAttrReuseAllowOpportunistic` 内存重用策略，分配器检查已经被调用了异步释放的内存，在Queue上是否执行完成。分配器会重用分配Queue上已经经执行完成了的异步释放的内存。

```
cnrtMemAllocAsync(&ptr, size, originalQueue);
kernel<<<..., originalQueue>>>(ptr, ...);
cnrtMemFreeAsync(ptr, originalQueue);

// After a period of time, the kernel has completed its execution.
wait(10);

// When cnrtMemPoolAttrReuseAllowOpportunistic is enabled this allocation request can be fulfilled with the prior allocation based on the progress of originalQueue.
cnrtMemAllocAsync(&ptr_reuse, size, otherQueue);

```

#### 6.5.15.4.3. `cnrtMemPoolAttrReuseAllowInternalDependencies`

分配器可以通过添加依赖到另一个Queue，来复用其内存。如果找到这样的内存，分配器会给申请Queue和被复用Queue加上隐式依赖。

```
cnrtMemAllocAsync(&ptr, size, originalQueue);
kernel<<<..., originalQueue>>>(ptr, ...);
cnrtMemFreeAsync(ptr, originalQueue);

// When cnrtMemPoolAttrReuseAllowInternalDependencies is enabled, the driver can effectively execute a cnrtQueueWaitNotifier within the allocating queue. This ensure that any subsequent tasks in other Queue will be scheduled to run only after the tasks in the original queue finished and allowed to access the original allocation.
cnrtMemAllocAsync(&ptr_reuse, size, otherQueue);

```

## 6.5.16. FAQ

1、Queue中的任务如何执行？

在一个Queue中，操作是按顺序串行执行的，不同Queue中的操作则可以并发执行；同一队列中所有任务都遵循FIFO原则。

2、创建Queue后，是否可以不销毁？

调用 `cnrtQueueCreate()` 创建的队列，用户需要调用 `cnrtQueueDestroy()` 销毁，Queue的生命周期由开发人员维护。目前退出 CNRT 或 CNDrv 运行环境时, 会自动释放用户已申请的资源。但是不调用 `cnrtQueueDestroy()` 销毁Queue的举动会有隐患。

3、通过 `dlopen()` 打开一个动态库后，使用 `<<<...>>>` 调用该动态库Kernel时，需要注意什么？

CNRT支持通过 `dlopen()` 动态加载kernel，需要注意的是，当指定的Kernel运行完毕，在进程结束前，请使用 `dlclose()` 关闭之前打开的动态库，否则，在某些环境下（比如 Ubuntu 16.04）进程退出时会出现未知错误。

4、在CNRT的运行环境中，使用 `fork()` 函数时有哪些注意事项？

用户必须在调用任一 CNRT 或 CNDrv 接口之前调用 `fork()` 函数，否则会出现API报错或其他未知错误。

5、当前进程第一次 `cnrtSetDevice()` 后，使用CNDrv接口需要注意什么？

用户在当前进程调用 `cnrtSetDevice()` 后，会创建基于当前设备的 Shared Context，但此时 Shared Context 仍处于未激活状态。通过 `cnSharedContextGetState()` 可以查询到此时 Shared Context 未激活从而无法使用，若此时调用 `cnMalloc()` 等资源类相关接口，会返回 `CN_CONTEXT_ERROR_INVALID` 错误码。若此时需要使用激活的 Shared Context，用户可以通过调用 `cnrtFree((void *)0)` 来触发 Shared Context 激活的操作，这个调用不会引入错误信息打印。 需要注意的是调用后用户可能会发现当前设备内存被占用了一部分，这是因为激活 Shared Context 的操作会触发加载 Kernel Module 的动作，即预先申请一部分资源用于存放Kernel运行的相关数据和指令。

6、当第一次调用 `cnrtQueueCreate()` 时，发现有设备内存被占用是怎么回事？

CNRT的初始化是隐式初始化，当用户第一次调用的 CNRT API（比如 `cnrtMalloc()` 或 `cnrtQueueCreate()` 等） 需要激活 Shared Context 时，CNRT 就会激活 Shared Context 并加载 Kernel Module。加载 Kernel Module 阶段会申请一部分设备资源用于存放 Kernel 运行的相关数据和指令。因此当用户在当前进程中第一次调用的 CNRT 接口是 `cnrtQueueCreate()` 这一类的接口时，有可能会看到设备内存被占用。
