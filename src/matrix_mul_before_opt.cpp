#include <CL/sycl.hpp>  // 引入SYCL头文件，用于异构计算。
#include <iostream>     // 引入iostream，用于输入/输出操作。

constexpr size_t N = 1024;  // 定义矩阵的大小。

int main() {
  // 初始化NxN大小的矩阵A、B和C。矩阵A和B分别填充2.0f和3.0f，矩阵C初始化为零。
  std::vector<float> matrixA(N * N, 2.0f);
  std::vector<float> matrixB(N * N, 3.0f);
  std::vector<float> matrixC(N * N, 0.0f);

  try {
    sycl::queue myQueue;  // 创建SYCL队列，用于提交任务。
    sycl::range<2> size(N, N);  // 定义矩阵的2D范围。

    // 为矩阵A、B和C创建SYCL缓冲区。这些缓冲区允许数据在主机和设备之间传输。
    sycl::buffer<float, 2> bufferA(matrixA.data(), size);
    sycl::buffer<float, 2> bufferB(matrixB.data(), size);
    sycl::buffer<float, 2> bufferC(matrixC.data(), size);

    // 向队列提交一个命令组。该命令组描述了要完成的工作，即矩阵乘法。
    myQueue.submit([&](sycl::handler& cgh) {
      // 为SYCL缓冲区创建访问器，并设置适当的访问模式。
      auto accA = bufferA.get_access<sycl::access::mode::read>(cgh);
      auto accB = bufferB.get_access<sycl::access::mode::read>(cgh);
      auto accC = bufferC.get_access<sycl::access::mode::write>(cgh);

      // 定义一个矩阵乘法的并行操作。
      cgh.parallel_for<class MatrixMultiply>(size, [=](sycl::id<2> idx) {
        float sum = 0.0f;
        // 对结果矩阵中的每个元素执行点积运算。
        for (int k = 0; k < N; ++k) {
          sum += accA[idx[0]][k] * accB[k][idx[1]];
        }
        accC[idx] = sum;  // 在矩阵C中存储计算得到的值。
      });
    });

    myQueue.wait();  // 等待队列中的所有任务完成。
  } catch (sycl::exception& e) {
    // 捕获并打印任何SYCL异常。
    std::cerr << "An exception occurred: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
