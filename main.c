// Grupo: Aran Leite de Gusmão (Lembrar de passar o número inicial como argumento)
#include <omp.h>
#include <stdalign.h>
#include <stdio.h>
#include <stdlib.h>

#define ARR_SIZE 1024
#define BLOCK_SIZE 64
#define THREADS_NUM 4

int main(int argc, char* argv[]) {
    if (argc != 2) {
        return 1;
    }

    alignas(64) int inputArray[ARR_SIZE], outputArray[ARR_SIZE];
    int initialNumber = atoi(argv[1]);

    if (!initialNumber || initialNumber <= 0) {
        return 1;
    }

    int outputSum = 0;

    int totalBlocks = ARR_SIZE * sizeof(int);
    int cacheBlocks = totalBlocks / BLOCK_SIZE;
    int logicalSize = BLOCK_SIZE / sizeof(int);

    omp_set_num_threads(THREADS_NUM);

#pragma omp parallel
    {
        int threadsNumber = omp_get_num_threads(), id = omp_get_thread_num();
        // Dividir os blocos igualmente para as threads e multiplicar pelo número dela (offset)
        int maxBlock = cacheBlocks / threadsNumber * (id + 1);

        // Para cada bloco na sua região
        for (int block = 0; block < maxBlock; block++) {
            // Percorra todo o bloco
            for (int offset = 0; offset < logicalSize; offset++) {
                int index = block * logicalSize + offset;

                inputArray[index] = initialNumber + index;

                int result = inputArray[index] + 1;
                outputArray[index] = result;

#pragma omp atomic
                outputSum += result;
            }
        }
    }

    printf("O resultado do somatório dos elementos do array de saída é: %d\n", outputSum);

    return 0;
}
