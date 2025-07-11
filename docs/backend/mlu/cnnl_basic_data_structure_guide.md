# 6. CNNL 基础数据结构使用指南

## 6.1. 张量

张量，具有统一数据类型的多维数组。

张量描述符，描述所指向的张量具体信息的载体，包含了张量数据的数据布局、数据类型、维度个数、维度大小、步长大小等信息。通过张量描述符的表征，每一块张量数据才得以具有实际的语义信息，进而可以用来表示图像、语音或视频等各领域的数据。

张量描述符的信息如下：

- 数据布局：表示没有维度语义的array，或者具有维度语义的布局，如NCHW，NHWC，HWCN，NDHWC，NCDHW。Cambricon CNNL使用 `cnnlTensorLayout_t` 来描述数据布局。
- 数据类型：表示张量中数据的类型，如half，float，int8，int16等。Cambricon CNNL使用 `cnnlDataType_t` 来描述数据类型。
- 维度个数：针对数据布局为array的张量，表示张量的维度，Cambricon CNNL支持1-8维。
- 维度大小：表示每个维度的大小。
- 步长大小：表示在同一个维度中访问两个相邻元素时需要走过的步数。举例：我们定义一个\[3,4\]的二维矩阵matrix，默认情况下其具有隐含的步长信息数组\[4,1\]，隐含着用户访问矩阵同维度内相邻元素需要间隔的元素个数信息，步长数组\[4,1\]的1表示访问第二个维度相邻元素时（例如matrix\[2\]\[1\]和matrix\[2\]\[2\]之间），内存上的间隔是0，也就是步长为1；访问第一个维度相邻元素时（例如matrix\[1\]\[1\]和matrix\[2\]\[1\]之间），内存上的间隔是3，也就是步长为4。用户可以在Cambricon CNNL提供的张量描述符的相关接口中显示地、灵活地配置步长数组，来对真实张量数据进行灵活地操作。

每一个Cambricon CNNL算子，维度大小一般是可变的，用户可以在调用对应接口之前通过 `cnnlSetTensorDescriptor()` 或 `cnnlSetTensorDescriptorEx()` 来设置张量信息。其中调用接口 `cnnlSetTensorDescriptor()` 时会自动设置张量的步长大小。如果要设置特殊的步长大小，可以调用 `cnnlSetTensorDescriptorEx()` 接口。另外，为了减少多次接口调用的时间开销，我们为用户提供了group接口 `cnnlCreateGroupTensorDescriptors` ， `cnnlSetGroupTensorDescriptors` 和 `cnnlDestroyGroupTensorDescriptors` ，用户可以在一次接口调用中完成一组张量描述符的创建、设置与销毁。详情查看《Cambricon CNNL Developer Guide》。

CNNL函数库目前与张量描述符相关的接口有如下几个类别：

- 普通张量描述符相关接口：对张量描述符的普通操作，例如创建、设置、获取、销毁，重置以及设置片上数据类型、量化参数等信息。用户可以从接口名字上显而易见地了解其含义，这里不在赘述。
- Group张量描述符相关接口：对一组张量描述符的普通操作，例如一组张量描述符的创建、设置和销毁。该接口旨在为同时大量创建、设置和销毁一组张量描述的使用场景提供简单易用且高性能的接口。通常来说，该接口的性能收益来自两个部分：一、一次接口调用代替了原先的多次接口调用，减少了接口调用的开销。二、该接口内剔除了一些繁琐的检查操作，例如剔除了检查这一组描述符指针中是否有NULL，剔除了张量设置时候的最大维度限制检查。用户应当小心使用这一组接口，我们建议用户在保证了功能开发正确后，进行极致地性能调优的时候可以考虑使用该组接口。
- Seq张量描述符相关接口：对序列张量描述符的一系列操作，例如创建、设置、获取和销毁，以及为描述符设置量化参数Position和Scale。序列张量是一种特殊的张量，经常用于自然语言处理领域。除去普通张量所包含的信息以外，它还包含了序列的个数、每个序列的长度，以及序列的填补区域（当实际序列长度小于最大序列长度时）需要写入的值等特殊信息。
- Set张量描述符相关接口：对集合张量描述符的一系列操作，例如创建、设置和销毁，以及为集合中的一个张量描述符进行一系列的初始化、设置相关参数等操作。Set接口具有特有描述符 `cnnlTensorSetDescriptor_t` 。目前我们建议用户仅在调用RNN相关算子，例如 `cnnlRNNForwardInference` ， `cnnlInitRNNExtraInput` 时使用该接口。

各个接口的分类以及简介，可以参照下表：

<table><caption><span>表 6.1 </span><span>张量描述符相关接口功能</span> <a href="http://127.0.0.1:8000/5_concept/#id2">¶</a></caption> <colgroup><col> <col> <col></colgroup><thead><tr><th><p>接口名字</p></th><th><p>接口功能</p></th><th><p>注意事项</p></th></tr></thead><tbody><tr><td colspan="3"><p>普通张量描述符相关接口</p></td></tr><tr><td><p>cnnlCreateTensorDescriptor</p></td><td><p>创建一个张量描述符</p></td><td><p>无</p></td></tr><tr><td><p>cnnlSetTensorDescriptor</p></td><td><p>为一个张量描述符设置信息</p></td><td><p>Position、Scale、Offset等不变</p></td></tr><tr><td><p>cnnlGetTensorDescriptor</p></td><td><p>获取一个张量描述符信息</p></td><td><p>无</p></td></tr><tr><td><p>cnnlDestroyTensorDescriptor</p></td><td><p>销毁一个张量描述符</p></td><td><p>无</p></td></tr><tr><td><p>cnnlResetTensorDescriptor</p></td><td><p>重置一个张量描述符的所有信息</p></td><td><p>Position、Scale、Offset等也会 被重置</p></td></tr><tr><td><p>cnnlSetTensorDescriptorEx</p></td><td><p>为一个张量描述符设置信息</p></td><td><p>可以设置 Stride 信息</p></td></tr><tr><td><p>cnnlGetTensorDescriptorEx</p></td><td><p>获取一个张量描述符的信息</p></td><td><p>可以获取 Stride 信息</p></td></tr><tr><td><p>cnnlSetTensorDescriptorOnchipDataType</p></td><td><p>为一个张量描述符设置片上数据类 型信息</p></td><td><p>无</p></td></tr><tr><td><p>cnnlGetTensorDescriptorOnchipDataType</p></td><td><p>获取一个张量描述符的片上数据类 型信息</p></td><td><p>无</p></td></tr><tr><td><p>cnnlSetTensorDescriptorPosition</p></td><td><p>为一个张量描述符设置量化参数 Position</p></td><td><p>无</p></td></tr><tr><td><p>cnnlGetTensorDescriptorPosition</p></td><td><p>获取一个张量描述符的量化参数 Position</p></td><td><p>无</p></td></tr><tr><td><p>cnnlSetTensorDescriptorPositionAndScale</p></td><td><p>为一个张量描述符设置量化参数 Position 与 Scale</p></td><td><p>无</p></td></tr><tr><td><p>cnnlGetTensorDescriptorPositionAndScale</p></td><td><p>获取一个张量描述符的量化参数 Position 与 Scale</p></td><td><p>无</p></td></tr><tr><td><p>cnnlSetTensorDescriptorPositionScaleAndOffset</p></td><td><p>为一个张量描述符设置量化参数 Position，Scale 与 Offset</p></td><td><p>无</p></td></tr><tr><td><p>cnnlGetTensorDescriptorPositionScaleAndOffset</p></td><td><p>获取一个张量描述符的量化参数 Position，Scale 与 Offset</p></td><td><p>无</p></td></tr><tr><td colspan="3"><p>Group 张量描述符相关接口</p></td></tr><tr><td><p>cnnlCreateGroupTensorDescriptors</p></td><td><p>创建一组张量描述符</p></td><td><p>无</p></td></tr><tr><td><p>cnnlSetGroupTensorDescriptors</p></td><td><p>为一组张量描述符设置信息</p></td><td><p>无</p></td></tr><tr><td><p>cnnlDestroyGroupTensorDescriptors</p></td><td><p>销毁一组张量描述符</p></td><td><p>无</p></td></tr><tr><td colspan="3"><p>Seq 张量描述符相关接口</p></td></tr><tr><td><p>cnnlCreateSeqDataDescriptor</p></td><td><p>创建一个序列数据描述符</p></td><td><p>无</p></td></tr><tr><td><p>cnnlSetSeqDataDescriptor</p></td><td><p>为一个序列数据描述符设置信息</p></td><td><p>无</p></td></tr><tr><td><p>cnnlGetSeqDataDescriptor</p></td><td><p>获取一个序列数据描述符信息</p></td><td><p>无</p></td></tr><tr><td><p>cnnlDestroySeqDataDescriptor</p></td><td><p>销毁一个序列数据描述符</p></td><td><p>无</p></td></tr><tr><td><p>cnnlSetSeqDataDescriptorPositionAndScale</p></td><td><p>为一个序列数据描述符设置量化参 Position 以及 Scale</p></td><td><p>无</p></td></tr><tr><td><p>cnnlSetSeqDataDescriptorOnchipDataType</p></td><td><p>为序列数据描述符设置片上数据类 型</p></td><td><p>无</p></td></tr><tr><td><p>cnnlGetSeqDataDescriptorOnchipDataType</p></td><td><p>获取序列数据描述符的片上数据类 型</p></td><td><p>无</p></td></tr><tr><td colspan="3"><p>Set 张量描述符相关接口</p></td></tr><tr><td><p>cnnlCreateTensorSetDescriptor</p></td><td><p>创建一个张量描述符集合的描述符</p></td><td><p>无</p></td></tr><tr><td><p>cnnlGetTensorSetDescriptor</p></td><td><p>获取一个张量描述符集合的描述符</p></td><td><p>无</p></td></tr><tr><td><p>cnnlDestroyTensorSetDescriptor</p></td><td><p>销毁一个张量描述符集合的描述符</p></td><td><p>无</p></td></tr><tr><td><p>cnnlInitTensorSetMemberDescriptor</p></td><td><p>为集合描述符中的一个张量成员描 述符设置信息</p></td><td><p>无</p></td></tr><tr><td><p>cnnlInitTensorSetMemberDescriptorPositionAndScale</p></td><td><p>为集合描述符中的一个张量成员描 述符设置信息</p></td><td><p>无</p></td></tr><tr><td><p>cnnlGetTensorSetDescriptorSize</p></td><td><p>获取一个集合描述符中所有的张量 描述符所描述的张量的尺寸总和</p></td><td><p>无</p></td></tr><tr><td><p>cnnlGetTensorAndDataFromTensorSet</p></td><td><p>获取集合描述符中的一个张量成员 描述符以及该描述符所绑定张量数 据的硬件地址</p></td><td><p>无</p></td></tr></tbody></table>

相关接口详情，参看《Cambricon CNNL Developer Guide》。

对于具有维度语义的张量，其形状的对应含义如下：

| N (batch size) | C (channel) | H (height) | W (width) | D (depth) |
| --- | --- | --- | --- | --- |
| batch size | 特征图数目 | 图片高度 | 图片宽度 | 图片深度 |

NCHW，NHWC，HWCN等维度布局方式为4-D张量的数据布局方式，NDHWC与NCDHW为5-D数据布局方式。 用户可以通过指定支持张量的数据类型来进行不同精度的运算。使用 `cnnlDataType_t` 来定义数据类型。数据类型的含义如下表所示：

| 数据类型 | 含义 |
| --- | --- |
| CNNL\_DATA\_INVALID | 无效数据类型 |
| CNNL\_DATA\_HALF | HALF数据类型 |
| CNNL\_DATA\_FLOAT | FLOAT数据类型 |
| CNNL\_DATA\_INT8 | INT8数据类型 |
| CNNL\_DATA\_INT16 | INT16数据类型 |
| CNNL\_DATA\_INT31 | INT31数据类型 |
| CNNL\_DATA\_INT32 | INT32数据类型 |
| CNNL\_DATA\_UINT8 | UINT8数据类型 |
| CNNL\_DATA\_BOOL | BOOL数据类型 |


## 6.2. 句柄

MLU设备资源不能被直接使用，Cambricon CNNL算子计算时通过句柄（handle）来维护MLU设备信息和队列信息等Cambricon CNNL 库运行环境的上下文。因此，需要在使用 Cambricon CNNL 库时，创建句柄并将句柄绑定到使用的MLU设备和队列上。

用户需要先调用 `cnnlCreate()` 接口在初始化 Cambricon CNNL时创建一个句柄。创建的句柄随后会作为参数传递给算子相关接口完成数据计算。当Cambricon CNNL库使用结束时，用户需要调用 `cnnlDestroy()` 接口释放与 Cambricon CNNL 库有关的资源。

在同一线程中， `cnnlCreate()` 和 `cnnlDestroy()` 接口之间的调用只会使用与句柄绑定的MLU设备。如果想要使用多个MLU设备，用户可以创建多个句柄来完成。通过使用句柄用户可以灵活地在多线程、多设备、多队列等多种场景下，完成相应的计算任务。例如，用户可以在多个线程中调用 `cnrtSetDevice()` 接口来设置使用不同的MLU设备，同时调用 `cnnlCreate()` 接口创建多个句柄绑定到不同的MLU设备上，再调用 Cambricon CNNL算子计算接口传入句柄， 从而实现在不同线程中，算子的计算任务运行在不同的设备上。

## 6.2.1. 绑定句柄和MLU设备

绑定句柄和MLU设备是在句柄创建时完成。Cambricon CNNL 将句柄创建时当前线程使用的设备绑定到句柄中。如果用户没有设置当前线程使用的MLU设备，则默认绑定MLU设备0。

创建句柄和绑定MLU设备的步骤如下：

1. 调用 `cnrtGetDevice()` 获取指定MLU设备对应的设备号。
2. 调用 `cnrtSetDevice()` 设置当前线程使用的MLU设备。
3. 调用 `cnnlCreate()` 创建句柄，将创建的句柄与当前线程所使用的设备绑定。

如果需要切换算子运行的MLU设备，可以重复调用以上步骤来重新设置设备并且创建一个新的句柄。相关CNRT接口说明，参考《寒武纪CNRT开发者手册》。

## 6.2.2. 绑定句柄和队列

Cambricon CNNL 中所有算子计算任务都是通过队列来完成。因此，句柄除了在创建时会绑定设备外，还需要绑定一个队列，使Cambricon CNNL 算子的计算任务能够在指定队列上执行。一旦队列与句柄绑定，Cambricon CNNL 算子的计算任务都是基于句柄中绑定的队列来下发。用户可以通过 `cnnlSetQueue()` 来绑定队列和句柄，通过 `cnnlGetQueue()` 来获取绑定到句柄上的队列。有关队列详情，参考《寒武纪CNRT用户手册》。

执行下面步骤将句柄与队列绑定：

1. 调用 `cnrtQueueCreate()` 创建一个队列。
2. 调用 `cnnlCreate()` 创建一个句柄。
3. 将队列绑定到句柄。调用 `cnnlSetQueue()` 将创建好的队列绑定到已有的句柄中。此接口可用来切换句柄中绑定的队列。
4. 使用队列下发任务。 调用 `cnnlXXX()` ，将句柄作为参数传入该接口，其中XXX为算子名，如 `cnnlConvolutionForward` 。Cambricon CNNL 在任务下发时，会将任务下发至句柄中绑定的队列上。任务下发完成后接口便会异步返回。
5. 调用 `cnnlGetQueue()` 获取句柄中已经绑定好的队列。
6. 调用 `cnrtQueueSync()` 同步队列。该接口会阻塞队列直到队列中所有任务均完成。
7. 调用 `cnrtQueueDestroy()` 销毁队列所占的资源。

此外，用户需要在 Cambricon CNNL 程序运行最后调用 `cnnlDestroy()` 接口释放 Cambricon CNNL 运行上下文资源。

## 6.2.3. 句柄使用示例

下面以卷积算子为例说明句柄的使用。示例中忽略卷积运算中张量描述符及存放输入输出数据内存的创建和释放过程，仅展示与句柄相关的设备和队列的操作。

```
int dev;
cnrtGetDevice(&dev); // 获取设备0对应的设备号。
cnrtSetDevice(dev); // 设置当前线程下需要使用的设备(设备0)。

cnnlHandle_t handle;
cnnlCreate(&handle); // 创建句柄, 此时创建的 handle 会与设备0绑定。

cnrtQueue_t q0;
cnrtQueueCreate(&q0); // 创建队列。
cnnlSetQueue(handle, q0); // 将队列绑定到 handle 中, 此接口也可用来更改句柄中的队列。

cnnlConvolutionForward(handle, conv_desc, algo, x_desc, data_x, w_desc, data_w, desc_bias, data_bias, workspace, workspace_size, y_desc, data_y)); // 传入已设置好的 handle，此时卷积计算任务会在设备0上的 queue 0上运行。

cnrtQueue_t q1;
cnnlGetQueue(handle, &q1); // 如果上下文没有保存队列信息，可以通过 cnnlGetQueue 从 handle 中获取队列，此时获取的 q1 与 q0 相等。
cnrtQueueSync(q1); // cnrtQueueSync(q0)

cnnlDestroy(handle); // 释放 Cambricon CNNL 运行上下文资源。
cnrtQueueDestroy(q0); // 释放队列资源。
cnrtQueueDestroy(q1); // 释放队列资源。
```

## 6.3. 工作空间

Cambricon CNNL 库中部分算子除了输入和输出数据的存放需要内存空间外，还需要额外的内存空间用于算子计算的优化。这部分额外的内存空间被称为工作空间 （workspace）。

需要workspace的算子在调用算子接口前，要申请workspace。Cambricon CNNL 为每个需要使用 workspace 的算子提供了一个获取 workspace 大小的接口，用户通过调用该接口获取算子需要使用的 workspace 大小（单位为字节）。接口名为 `cnnlGetXXXWorkspaceSize()` ，其中 `XXX` 为算子名。例如，卷积前向算子（ConvolutionForward）获取 workspace 大小的接口为 `cnnlGetConvolutionForwardWorkspaceSize()` 。用户根据获取的 workspace 大小调用 `cnrtMalloc()` 来申请 workspace，并在算子计算时，如 `cnnlConvolutionForward()` 将申请的 workspace 地址和 workspace 大小传给算子计算接口的 `workspace` 和 `workspace_size` 参数。最后，用户需要调用 `cnrtFree()` 释放 workspace 空间。

参考《Cambricon CNNL Developer Guide》中各算子的相关章节了解各算子是否需要额外申请 workspace。如果算子接口中包含 `workspace` 和 `workspace_size` 参数，则说明该算子需要申请 workspace，否则该算子不需要申请 workspace。

如果网络中有多个算子需要 workspace，用户可以通过调用 `cnnlGetXXXWorkSpaceSize()` 分别获取各算子所需的 workspace 大小，然后只申请这些算子所需最大workspace大小，而无需为每个算子都申请 workspace。在这种情况下，workspace\_size 并不是确切的 workspace 指针指向的内存块的大小，而是指算子需要的工作空间的大小。该方法可以充分复用MLU设备上的内存空间，并且减少多个算子因多次申请和释放 workspace 所带来的时间开销。但是为了确保 workspace 读写安全性，该方法只限于所有算子严格按照顺序执行，并且这些算子的执行在同一个队列内，而不是在多个队列中并行运行。顺序执行可以确保在同一时间仅有一个任务访问这一块 workspace，而多队列并行运行时可能会造成多个并行任务同时访问这一块 workspace，从而造成多个任务互相踩踏内存，并且CNRT会返回 "mlu unfinished error"或算子计算结果出错。

## 6.3.1. 使用方法

执行下面步骤，为一个算子申请 workspace：

1. 调用 `cnnlCreateTensorDescriptor()` 和 `cnnlSetTensorDescriptor()` 创建 tensor descriptor，并设置算子所需参数。
2. 调用 `cnnlGetXXXWorkspaceSize()` 获取到该算子所需要的 workspace 大小，其中 `XXX` 需替换为算子名。
3. 根据获取的workspace 大小设置 `size` ，调用 `cnrtMalloc()` 申请 workspace 空间。
4. 调用 `cnnlXXX()` ，将 tensor descriptor、输入、输出和 workspace 等参数传入该接口来执行算子的计算任务，其中 `XXX` 需替换为算子名。
5. 调用 `cnrtMemcpy()` 将 MLU 输出结果从设备端拷贝回主机端。
6. 调用 `cnnlDestroyTensorDescriptor()` 释放 tensor descriptor。
7. 调用 `cnrtFree()` 释放 workspace 空间。

相关CNRT接口详情，请查看《CNRT开发者手册》。

## 6.3.2. 示例

以卷积ConvolutionForward算子为例展示 workspace 的使用，示例中忽略卷积张量描述符、数据内存申请和释放以及同步队列中执行任务的过程。

```
size_t workspace_size;
cnnlGetConvolutionForwardWorkspaceSize(handle, x_desc, w_desc, y_desc, conv_desc, algo, &workspace_size); // 获取ConvolutionForward算子所需workspace大小。

void *workspace = NULL;
cnrtMalloc(&workspace, workspace_size); // 为workspace分配内存。
cnnlConvolutionForward(handle, conv_desc, algo, x_desc, data_x, w_desc, data_w, desc_bias, data_bias, workspace, workspace_size, y_desc, data_y)); // 完成ConvolutionForward计算任务，其中workspace为workspace地址，workspace_size为workspace大小。

cnrtFree(workspace); // 释放workspace内存资源。
```

## 6.4. 额外输入

Cambricon CNNL 库中的部分算子除了算子语义上的输入和输出外，还需要一个额外的输入内存空间（extraInput）用于算子的性能优化。extraInput 和正常的输入具有相同的特点。用户在使用带有 extraInput 参数的算子接口时，需要将MLU 设备端的 extraInput 指针传入算子接口中。Cambricon CNNL 会提供相关的接口和操作机制来对 extraInput 指向的内存进行赋值，用户只需要调用相关接口完成extraInput内存的数据赋值即可。

对于使用到 extraInput 的算子，需要在 CPU端和 MLU设备端分别申请一块 extraInput 内存空间。通过 `cnnlGetXXXExtraInputSize()` 接口获得，其中 `XXX` 为算子名。此外，还需通过 `cnnlInitXXXExtraInput` 接口对 CPU 端申请的 extraInput 内存进行数据初始化。然而 `cnnlInitXXXExtraInput` 接口的最终目的是将初始化的数据拷贝到 MLU设备端，供算子计算时使用这些数据进行计算优化。所以在完成 CPU端extraInput 内存的初始化之后，还需要调用 `cnrtMemcpy` 或其他相关的拷贝接口，将 CPU 侧 extraInput 内存上的数据拷贝到 MLU 侧的 extraInput 内存上，然后将 extraInput 的 MLU 指针传给算子接口。

该内存的生命周期和算子的普通输入保持一致。在每次算子接口调用前，都需要执行 MLU设备端的 extraInput 的内存申请。另外每次算子接口调用前，CPU端都需要执行 extraInput 内存申请、初始化以及向MLU 设备端的拷贝动作，因为该内存大小和实际数据与算子参数如 `tensor_descriptor` 相关，并且用户需要确保 CPU 的内存不要存在多线程踩踏行为。

extraInput 与 workspace 不同之处在于，用户无需关心 workspace 中的内存数据，也无需对该内存数据进行任何操作，只需要在 MLU 端申请合适大小的 workspace 内存并传给算子。而 extraInput 作为一个额外的 “输入”， 需要用户 分配和初始化CPU 端的内存以及从 CPU 拷贝数据到 MLU 端。

参考《Cambricon CNNL Developer Guide》中各算子的相关章节了解各算子是否需要额外申请 extraInput。如果算子接口中包含 `extraInput` 和 `extraInput_size` 参数，则说明该算子需要申请 extraInput，否则该算子不需要申请 extraInput。

## 6.4.1. 使用方法

执行下面步骤完成带有 extraInput 参数的算子计算：

1. 调用 `cnnlCreateTensorDescriptor()` 和 `cnnlSetTensorDescriptor()` 创建 tensor descriptor，并设置算子所需参数。
2. 调用 `cnnlGetXXXExtraInputSize()` 获取到该算子所需要的 extraInput 大小 `extraInput_size` ，其中 `XXX` 为算子名。这里获取到的 `extraInput_size` 是主机端和 MLU 端的内存大小。
3. 根据获取的 `extraInput_size` ，调用接口在 CPU 端申请 extraInput 内存 `extraInput_cpu` ，例如 `malloc` 。在 MLU 端调用接口申请 MLU 侧的 extraInput 内存 `extraInput_mlu` ，例如 `cnrtMalloc` 。
4. 调用 `cnnlInitXXXExtraInput` 接口，在 CPU 端初始化 `extraInput_cpu` 内存。
5. 调用如 `cnrtMemcpy` 或其他内存拷贝接口，将 CPU 端已经初始化完的内存拷贝到 MLU 端。也就是将 `extraInput_cpu` 中的数据拷贝到 `extraInput_mlu` 。
6. 调用 `cnnlXXX()` ，将tensor descriptor、输入、输出和 extraInput\_MLU 等参数传入该接口来执行算子的计算任务，其中 `XXX` 为算子名。
7. 调用 `cnnlDestroyTensorDescriptor()` 释放tensor descriptor。
8. 释放其余申请的相关资源。

如果使用CNRT相关接口分配内存、初始化内存和拷贝内存数据，请参考《寒武纪CNRT开发者手册》。

## 6.4.2. 示例

以 Reorg 算子为例展示 extraInput 的使用，示例中忽略Reorg tensor descriptor、数据内存申请和释放以及同步队列中执行任务的过程。

```
size_t extraInput_size;
cnnlGetReorgExtraInputSize(handle, reorg_desc, x_desc, y_desc, &extraInput_size); // 获取 Reorg 算子所需的 extraInput 大小。

void *extraInput_cpu = NULL;
void *extraInput_mlu = NULL;
extraInput_cpu = malloc(extraInput_size); // 为 extraInput 申请 CPU 内存
cnrtMalloc(&extraInput_mlu, extraInput_size); // 为 extraInput 申请 MLU 内存

cnnlInitReorgExtraInput(handle, reorg_desc, x_desc, y_desc, extraInput_cpu); // 在 CPU 端初始化 extraInput 内存
cnrtMemcpy(extraInput_MLU, extraInput_cpu, extraInput_size, CNRT_MEM_TRANS_DIR_HOST2DEV); // 将 CPU 端已初始化的内存数据拷贝至 MLU 端
cnnlReorg_v2(handle, reorg_desc, x_desc, x, extraInput_mlu, extraInput_size, y_desc, y, workspace, workspace_size);  // 调用算子接口，传入 MLU 端 extraInput 指针及其他相关参数

// 释放相关资源
free(extraInput_cpu);
cnrtFree(extraInput_mlu);
```

## 6.5. 算子描述符

某些算子具备一些固有属性，如Convolution算子在计算时需要的pad、stride等信息。Cambricon CNNL使用算子描述符（Descriptor）来描述算子的固有属性，并提供相关接口创建、设置和销毁算子描述符，从而简化算子计算接口中参数的数量。对于使用了算子描述符的算子，描述符中包含的参数需要通过设置描述符传入算子计算接口，而不是通过张量来设置。用户需要在调用算子计算接口前完成算子描述符的设置。

需要使用算子描述符的算子如下：

- Activation
- Attention
- Convolution
- Customized Active
- Grep
- Gru
- OpTensor
- Pooling
- Reduce
- Reorg
- Transpose
- Trigon

## 6.5.1. 使用方法

算子描述符被存放在该算子的结构体中，通常命名为 `cnnlXXXDescriptor_t` ，其中 `XXX` 为算子名。例如Gru算子的算子描述符信息被存放在 `cnnlGruDescriptor_t` 结构体中。

注意

Pooling算子根据计算维度，需要调用不同的接口来设置算子描述符。如果是2维Pooling计算，调用 `cnnlSetPooling2dDescriptor()` 接口， 如果是3维Pooling计算，调用 `cnnlSetPoolingNdDescriptor()` 接口。但是，创建和销毁算子描述符与维度无关，均调用 `cnnlCreatePoolingDescriptor()` 和 `cnnlDestroyPoolingDescriptor()` 接口。

执行下面步骤设置算子描述符：

1. 调用 `cnnlCreateXXXDescriptor()` 接口创建一个描述符。如调用 `cnnlCreateGruDescriptor()` 接口创建Gru算子的算子描述符。
2. 调用 `cnnlSetXXXDescriptor()` 接口为算子描述符中每一个算子的固有属性赋值，如调用 `cnnlSetGruDescriptor()` 为Gru的 `algo` 、 `is_bidirectional` 、 `num_layer` 赋值。
3. 调用算子计算接口 `cnnlXXX()` 时，如 `cnnlGru` ，将算子描述符作为参数传递给接口。
4. 调用 `cnnlDestroyXXXDescriptor()` 接口，如 `cnnlDestroyGruDescriptor()` 销毁该描述符。

对于不使用算子描述符的算子，按照 `cnnl.h` 中暴露的接口进行调用即可。


# 7. 编程模型

## 7.1. 编程模型介绍

寒武纪Cambricon CNNL采用异构编程模型，实现了不同架构和指令集的混合编程。

## 7.1.1. 异构编程模型

异构计算系统通常由通用处理器和协处理器组成，其中通用处理器作为控制设备，通常称为Host端（主机端），负责调度。协处理器作为辅助计算设备，即MLU端（设备端），负责专有领域的大规模并行计算。Host端和MLU端协同完成计算任务。

寒武纪Cambricon CNNL异构编程模型是CPU和MLU的协作编程模型。Host端负责调用CNRT接口用来初始化设备、管理设备内存、准备Cambricon CNNL的参数、调用Cambricon CNNL接口以及释放设备资源。MLU端作为协处理器，帮助Host端CPU完成人工智能任务，并达到低能耗、高可用的效果。Cambricon CNNL每个算子由Host端CPU发射，在MLU端异步执行。

## 7.1.3. Cambricon CNNL是如何工作的

对于一个网络模型，用户通过调用常用框架，如PyTorch、TensorFlow等或自定义框架，解析模型并生成Cambricon CNNL算子计算所需要的输入参数和数据。根据输入参数和数据，主机端调用Cambricon CNNL算子接口，完成算子的高性能计算。

由于Cambricon CNNL算子的计算是在MLU设备端完成，Cambricon CNNL通过句柄与计算时使用的MLU设备和队列绑定。MLU设备端的设备初始化、计算时输入输出数据的内存分配、队列的创建等都是通过主机端调用CNRT接口完成。

MLU设备端完成计算后，计算结果通过调用CNRT接口返回到主机端CPU来进行下一步任务。

## 7.1.4. Cambricon CNNL头文件

Cambricon CNNL头文件包含了支持的Cambricon CNNL算子接口和Cambricon CNNL数据类型声明。当主机端调用Cambricon CNNL算子所在的文件时，用户需要包含Cambricon CNNL头文件 `cnnl.h` 。


## 7.2. 编程指南

Cambricon CNNL编程中，每个算子都需要绑定句柄。句柄主要用于保存当前运行环境的上下文、计算设备信息和队列信息。不同的算子可以处在不同的队列中，各个队列可以并行执行，而队列内部顺序执行。更多详情，查看 [句柄](http://127.0.0.1:8000/5_concept/handle.html#id1) 。

部分Cambricon CNNL算子计算时需要额外内存空间workspace用于算子优化。Cambricon CNNL内部不会申请workspace，用户需要调用workspace相关接口申请计算所需要的workspace，并将workspace大小传入算子接口。详情请查看 [工作空间](http://127.0.0.1:8000/5_concept/workspace.html#workspace) 。

此外，由于CPU和MLU是异步执行任务，用户需要调用 `cnrtQueueSync()` 接口同步队列中执行的任务。

## 7.2.1. 单算子编程指南

如果运行单个Cambricon CNNL算子，执行下面操作：

1. 调用 `cnrtGetDevice()` 获取设备对应的设备号。
2. 调用 `cnrtSetDevice()` 绑定当前线程所使用的设备。
3. 调用 `cnnlCreate()` 创建一个Cambricon CNNL句柄。句柄将与当前线程所使用的设备绑定。
4. 调用 `cnrtQueueCreate()` 创建一个计算队列。
5. 调用 `cnnlSetQueue()` 将队列和Cambricon CNNL句柄绑定。
6. 如果接口需要设置 `cnnlTensorDescriptor_t` ，调用 `cnnlCreateTensorDescriptor()` 创建tensor descriptor，并调用 `cnnlSetTensorDescriptor()` 设置算子输入和输出描述信息。具体信息包括数据类型、形状信息、维度顺序等。
7. 如果接口需要设置算子描述符 `cnnlXXXDescriptor_t` ，调用 `cnnlCreateXXXDescriptor()` 创建算子描述符，并调用 `cnnlSetXXXDescriptor()` 为该算子添加描述。 `XXX` 需要替换为算子名称。
8. 如果接口需要申请额外workspace，即需要设置 `workspace` 和 `workspace_size` ，调用 `cnnlXXXGetWorkspaceSize()` 推导该算子需要的最小的临时空间大小。 `XXX` 需要替换为算子名称。
9. 调用 `cnrtMalloc()` 开辟算子需要的输入、输出和临时空间。
10. 调用 `cnrtMemcpy()` 将输入数据拷贝到设备端。拷贝到设备端的数据必须与之前设置的tensor descriptor信息保持一致。
11. 调用算子的API接口 `cnnlXXX()` ，传入Cambricon CNNL句柄信息。 `XXX` 需要替换为算子名称。
12. 调用 `cnrtQueueSync()` 同步CPU和MLU端。
13. 调用 `cnrtMemcpy()` 将输出从设备端拷贝回主机端。
14. 调用 `cnrtFree()` 释放设备端的空间。
15. 如果执行了第7步，调用 `cnnlDestroyTensorDescriptor()` 释放描述信息。
16. 如果执行了第8步，调用 `cnnlDestroyXXXDescriptor()` 释放资源。 `XXX` 需要替换为算子名称。
17. 调用 `cnrtQueueDestroy()` 释放队列信息。
18. 调用 `cnnlDestroy()` 释放Cambricon CNNL句柄。

有关接口详情，请查看《Cambricon CNNL Developer Guide》。

## 7.2.2. 多算子搭建网络

使用Cambricon CNNL搭建一个多算子的网络，可以考虑如下方法，达到空间复用的效果。操作步骤如下：

1. 执行 的1-6步完成初始化等操作。
2. 为网络的每一层准备张量描述符 `cnnlTensorDescriptor_t` ，调用 `cnnlCreateTensorDescriptor()` 创建tensor descriptor，并调用 `cnnlSetTensorDescriptor()` 设置算子输入和输出描述信息。具体信息包括数据类型、形状信息、维度顺序等。
3. 为网络里面含有算子描述符的层创建算子 `cnnlXXXDescriptor_t` ，调用 `cnnlCreateXXXDescriptor()` 创建算子描述符，并调用 `cnnlSetXXXDescriptor()` 为该算子添加描述。其中 `XXX` 需要替换为算子名称。
4. 为网络里面所有需要的workspace的层设置 `workspace` 和 `workspace_size` ，调用 `cnnlXXXGetWorkspaceSize()` 推导该算子需要的最小的临时空间大小，其中 `XXX` 需要替换为算子名称。
5. 调用 `cnrtMemcpy()` 将输入层的数据拷贝到设备端。
6. 按照网络层的顺序调用 `cnnlXXX()` ，每一层绑定上面申请的句柄，保证他们处于同一计算队列中。 `XXX` 需要替换为算子名称。
7. 调用 `cnrtQueueSync()` 同步CPU端和设备端。
8. 拷回数据、释放句柄和设备端空间。执行 的14-19步。

有关接口详情，请查看《Cambricon CNNL Developer Guide》。