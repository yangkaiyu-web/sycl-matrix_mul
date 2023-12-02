#include <CL/sycl.hpp>
#include <iostream>
#include <vector>

constexpr size_t N = 1024;       // 矩阵大小
constexpr size_t blockSize = 16; // 线程块的大小

using namespace cl::sycl;

int main()
{
    std::vector<float> matrixA(N * N, 2.0f); // 初始化矩阵A
    std::vector<float> matrixB(N * N, 3.0f); // 初始化矩阵B
    std::vector<float> matrixC(N * N, 0.0f); // 初始化矩阵C

    try
    {
        queue myQueue; // 创建SYCL队列

        buffer<float, 2> bufferA(matrixA.data(), range<2>(N, N));
        buffer<float, 2> bufferB(matrixB.data(), range<2>(N, N));
        buffer<float, 2> bufferC(matrixC.data(), range<2>(N, N));

        myQueue.submit([&](handler &cgh)
                       {
            auto accA = bufferA.get_access<access::mode::read>(cgh);
            auto accB = bufferB.get_access<access::mode::read>(cgh);
            auto accC = bufferC.get_access<access::mode::write>(cgh);

            local_accessor<float, 2> localA(range<2>(blockSize, blockSize), cgh),
                                    localB(range<2>(blockSize, blockSize), cgh);

            cgh.parallel_for<class matrixMul>(nd_range<2>(range<2>(N, N), range<2>(blockSize, blockSize)), [=](nd_item<2> item) {
                size_t globalRow = item.get_global_id(0);
                size_t globalCol = item.get_global_id(1);
                size_t localRow = item.get_local_id(0);
                size_t localCol = item.get_local_id(1);

                float sum = 0.0f;
                for (int k = 0; k < (int)(N / blockSize); ++k) {
                    // 加载数据到本地内存
                    localA[localRow][localCol] = accA[globalRow][k * blockSize + localCol];
                    localB[localRow][localCol] = accB[k * blockSize + localRow][globalCol];
                    item.barrier(access::fence_space::local_space);

                    // 计算子块的乘积
                    for (int n = 0; n < blockSize; ++n)
                        sum += localA[localRow][n] * localB[n][localCol];
                    item.barrier(access::fence_space::local_space);
                }
                accC[globalRow][globalCol] = sum;
            }); });

        myQueue.wait_and_throw(); // 等待队列完成所有任务并处理异常

        // 读取结果
        auto accC = bufferC.get_host_access();
        for (size_t i = 0; i < N; i++)
        {
            for (size_t j = 0; j < N; j++)
            {
                std::cout << accC[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
    catch (sycl::exception const &e)
    {
        std::cerr << "SYCL exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
