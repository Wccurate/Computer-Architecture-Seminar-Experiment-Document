#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 定义一个大型数组的大小
#define ARRAY_SIZE (1024 * 1024 * 64) // 64MB的数组

// 函数用于计算数组所有元素的和
double calculateSum(double *array, int size) {
    double sum = 0.0;
    for (int i = 0; i < size; ++i) {
        sum += array[i];
    }
    return sum;
}

int main() {
    // 分配大型数组
    double *array = (double *)malloc(ARRAY_SIZE * sizeof(double));
    if (array == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }

    // 用随机数初始化数组
    srand(time(NULL));
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        array[i] = (double)rand() / RAND_MAX * 100;
    }

    // 计算开始时间
    clock_t start = clock();

    // 执行计算
    double sum = calculateSum(array, ARRAY_SIZE);

    // 计算结束时间
    clock_t end = clock();

    // 打印结果
    printf("Array sum: %f\n", sum);
    printf("Time taken: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    // 释放数组
    free(array);

    return 0;
}
