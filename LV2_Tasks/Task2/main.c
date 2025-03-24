#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define SIZE 100000000
#define RANDOM_SEED 123456789

int main(void)
{
	srand(RANDOM_SEED);

	int* data = malloc(SIZE * sizeof(int));
	if (data == NULL)
	{
		perror("Cannot allocate array for data. Exiting...");
		exit(-1);
	}
	for (int i = 0; i < SIZE; i++)
		data[i] = rand() % 256;

	int counts[256] = { 0 };
	int i;
	double time = omp_get_wtime();
	for (i = 0; i < SIZE; i++)
		counts[data[i]]++;
	printf("Serial: %f\n", omp_get_wtime() - time);

	int parallel_counts[256] = { 0 };
	time = omp_get_wtime();
	#pragma omp parallel for shared(parallel_counts) shared(data)
	for (i = 0; i < SIZE; i++)
		#pragma omp atomic
		{
			parallel_counts[data[i]]++;
		}
	printf("Parallel: %f\n", omp_get_wtime() - time);

	free(data);
	return 0;
}