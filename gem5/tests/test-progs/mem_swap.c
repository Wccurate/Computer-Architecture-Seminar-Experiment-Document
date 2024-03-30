#include <stdio.h>
#include <stdlib.h>

#define SIZE 80000000 // Adjust this to be larger than 512MB to force swapping

int main() {
    long long *largeArray = malloc(SIZE * sizeof(long long));
    if (largeArray == NULL) {
        printf("Failed to allocate memory.\n");
        return 1;
    }

    // Access every element to force it into memory, potentially causing swapping
    for (long long i = 0; i < SIZE; i++) {
        largeArray[i] = i;
    }

    printf("Large dataset manipulation completed.\n");
    free(largeArray);
    return 0;
}
