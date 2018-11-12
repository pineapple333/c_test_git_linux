#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//HOW TO COMPILE: gcc race.c -std=c99 -pthread -o main

// The sum is computed by the background threads
long long sum = 0;

//Thread function by the background sum of 0 to N
void* sum_runner(void* arg)
{
	long long *limit_ptr = (long long *) arg;
	long long limit = *limit_ptr;

	for (long long i = 0; i <= limit; i++){
		sum += 1;
	}

	pthread_exit(0);
}

void* sub_runner(void* arg)
{
	long long *limit_ptr = (long long *) arg;
	long long limit = *limit_ptr;

	for (long long i = 0; i <= limit; i++){
		sum -= 1;
	}

	pthread_exit(0);
}

int main(int argc, char **argv)
{
	// <number of thread> == number of pairs of threads
	if (argc != 3){
		printf("Usage: %s <number of threads> <number of operations>\n", argv[0]);
		exit(-1);
	}

	//number_args equils to the number of threads
	int num_args = atoi(argv[1]);

	//array for ids for summation threads
	pthread_t tids[num_args];
	//array for ids for substitution thread
	pthread_t tids_sub[num_args];
	//we have only one limit
	long long limit = atoll(argv[2]);

	for (int i=0;i < num_args; i++){
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&tids[i], &attr, sum_runner, &limit);

		pthread_attr_t attr_two;
		pthread_attr_init(&attr_two);
		pthread_create(&tids_sub[i], &attr_two, sub_runner, &limit);
	}

	//wait until the thread is done its work
	for (int i = 0; i < num_args; i++){
		pthread_join(tids[i], NULL);
		pthread_join(tids_sub[i], NULL);
	}

	printf("Sum is %lld\n", sum);
}

/*I changed that task to make it easier
the first argument is the number of pairs of threads
the second one is the limit to which the programm will add values
third : without short walues
*/

/* REPORT 
it returns 0 with 3 pairs of thread and limit equil to 100
but with 3 pairs of threads and limit quil to 100000000
it returns 3142923
threads: 6, limit 10000, result -1821
threads 6, limit 100000, result 14413 
*/
