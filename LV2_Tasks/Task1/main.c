#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define SIZE 10000
#define CHUNK_SIZE 100

int main(void)
{
	double* matrix = malloc(SIZE * SIZE * sizeof(double));
	if (matrix == NULL)
	{
		perror("Cannot allocate matrix. Exiting...");
		exit(-1);
	}

	double time = omp_get_wtime();

	int i;
	#pragma omp parallel for schedule(dynamic, CHUNK_SIZE)
	for (i = 0; i < SIZE; i++)
	{
		for (int j = i * SIZE; j < i * SIZE + SIZE; j++)
			matrix[j] = i;
	}

	time = omp_get_wtime() - time;
	printf("%f", time);

	free(matrix);
	return 0;
}