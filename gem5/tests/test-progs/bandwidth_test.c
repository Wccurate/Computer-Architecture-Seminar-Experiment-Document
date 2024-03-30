#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 为了避免交换，将数组大小设置为系统内存大小的一小部分
// 这里我们使用大约1/8的内存，大约64MB
#define SIZE (64 * 1024 * 1024 / sizeof(int))

int main() {
    // 分配内存
    int* array = (int*)malloc(SIZE * sizeof(int));
    if (array == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }

    clock_t start, end;
    double cpu_time_used;

    // 写操作测试
    start = clock();
    for (int i = 0; i < SIZE; i++) {
        array[i] = i;
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Writing to array took %f seconds.\n", cpu_time_used);

    // 读操作测试
    volatile int sink;
    start = clock();
    for (int i = 0; i < SIZE; i++) {
        sink = array[i];
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Reading from array took %f seconds.\n", cpu_time_used);

    // 清理资源
    free(array);
    return 0;
}

