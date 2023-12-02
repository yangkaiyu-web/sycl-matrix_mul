# 并⾏矩阵乘法SYCL实现

### 问题描述

编写⼀个基于oneAPI的C++/SYCL程序来执行矩阵乘法操作。需要考虑大尺寸矩阵的乘法操作以及不同线程之间的数据依赖关系。通常在实现矩阵乘法时，可以使用块矩阵乘法以及共享内存来提高计算效率。

### 项目介绍和分析

本项目利用基于SYCL的编程模型在GPU上实现矩阵乘法的计算，步骤如下：

1. 分配内存：在主机端分配内存空间用于存储输⼊矩阵和输出矩阵，同时在GPU端分配内存空间用于存储相应的输入和输出数据。

2. 数据传输：将输入矩阵数据从主机端内存传输到GPU端内存中。

3. 核函数调用：在SYCL中，矩阵乘法的计算通常会在GPU上使用核函数来实现并行计算。核函数会分配线程块和线程来处理不同的数据块。

4. 并行计算：在核函数中，每个线程负责计算输出矩阵的⼀个单独的元素。为了最大限度地利用GPU的并行计算能力，通常会使用⼆维线程块和线程网格的方式来处理矩阵的乘法计算。

5. 数据传输：计算完成后，将输出矩阵数据从GPU端内存传输回主机端内存中，以便进⼀步处理或分析。

在并行计算矩阵乘法时，可以利用线程块和线程的层次结构来优化计算。通过合理划分矩阵数据并利用共享内存来减少全局内存访问的次数，可以⼤幅提高计算效率。此外，还可以利用GPU上的多个计算单元并执行行矩阵乘法，进⼀步提高计算速度。

sycl源代码在src目录下，其中`src/matrix_mul_before_opt.cpp`源文件为使用Intel devcloud平台GPU硬件加速但未优化的代码，`src/matrix_mul_cpu.cpp`为仅使用Intel devcloud平台CPU的代码，`src/matrix_mul.cpp`为使用Intel devcloud平台GPU且优化后的代码。

### 构建项目时采用的技术栈及主要实现方案

![image-20231202225124127](/Users/kaiyu/Library/Application Support/typora-user-images/image-20231202225124127.png)

上图中列出了Intel OneAPI对异构编程提供的开发工具，我在开发中使用了SYCL语言在VScode和Intel Devcloud平台上进行开发，使用了OpenMP和gdb-oneapi debugger工具。

### 运行

在项目目录下执行`qsub build.sh`，然后执行`qsub run.sh`，即可看到类似如下输出：

![image-20231202222902551](/Users/kaiyu/Library/Application Support/typora-user-images/image-20231202222902551.png)

### 结果

在sycl编程环境下，使用GPU进行加速可以比仅使用CPU快0.49125倍，而同样使用GPU和sycl并行编程条件下，使用local memory和worker group进行优化后可以比未优化快14.0748倍。

### [代码分析](https://github.com/yangkaiyu-web/sycl-matrix_mul/blob/main/src/%E4%BB%A3%E7%A0%81%E5%88%86%E6%9E%90.md)

见src目录下。

### 收获

我通过使用SYCL深入理解了异构计算的概念，这是现代计算中越来越重要的领域；异构计算涉及使用多种类型的处理器（如CPU、GPU、FPGA等）来处理计算任务，能够大幅提高性能和效率；同时提升了并行编程的技能，特别是在如何有效地将计算任务分配到多个处理器上；学习并实践了如何使用SYCL库进行编程，这是一个开放标准，用于简化CPU、GPU等异构设备上的C++并行编程；学会了利用设备的本地缓存优化，加速计算。总的来说，Intel的OneAPI提供了非常好的统一编程环境，值得进一步深入学习。