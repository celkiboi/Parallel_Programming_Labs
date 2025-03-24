#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <stdint.h>

#define SIZE 2048

int main(void)
{
	int* matrix_A = malloc(SIZE * SIZE * sizeof(int));
	if (matrix_A == NULL)
		return -1;
	int* matrix_B = malloc(SIZE * SIZE * sizeof(int));
	if (matrix_B == NULL)
		return -1;
	int* matrix_C = malloc(SIZE * SIZE * sizeof(int));
	if (matrix_C == NULL)
		return -1;
	int i, j, k;

	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			matrix_A[i * SIZE + j] = i + j;
			matrix_B[i * SIZE + j] = i + j;
			matrix_C[i * SIZE + j] = 0;
		}
	}

	double time = omp_get_wtime();

	for (i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			for (k = 0; k < SIZE; k++)
			{
				matrix_C[i * SIZE + j] += matrix_A[i * SIZE + k] * matrix_B[k * SIZE + k];
			}
		}
	}

	printf("Serial: %f\n", omp_get_wtime() - time);
	int64_t checksum = 0;
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
			checksum += matrix_C[i * SIZE + j];
	}
	printf("Checksum: %lld\n", checksum);

	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
			matrix_C[i * SIZE + j] = 0;
	}

	time = omp_get_wtime();
	#pragma parallel for collapse(2) shared(matrix_A) shared(matrix_B) shared(matrix_C)
	for (i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			for (k = 0; k < SIZE; k++)
			{
				matrix_C[i * SIZE + j] += matrix_A[i * SIZE + k] * matrix_B[k * SIZE + k];
			}
		}
	}
	printf("parallel: %f\n", omp_get_wtime() - time);
	checksum = 0;
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
			checksum += matrix_C[i * SIZE + j];
	}
	printf("Checksum: %lld\n", checksum);

	free(matrix_A);
	free(matrix_B);
	free(matrix_C);
	return 0;
}