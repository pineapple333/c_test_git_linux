#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

long double sum = 0;

// building: gcc -Wall -g -std=c99 -Werror -pthread second.c -o secondm

//alt shift r : rename all in eclipse


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct thread_struct {
	float a;
	float b;
};

void* counting_thread(void* arg)
{
	struct thread_struct *arg_struct = (struct thread_struct*) arg;
	float answer = 0;
	printf("In thread: a: %f, b: %f\n", arg_struct->a, arg_struct->b);
	for (float i = arg_struct->a; i < arg_struct->b; i+=0.01){

		answer += 4 - i*i;

	}
	pthread_mutex_lock (&mutex);

	sum += answer;

	pthread_mutex_unlock (&mutex);

	pthread_exit(0);
}

int main (int argc, char **argv)
{
	//parameters a, b, threads
	if (argc != 4){
		printf("Usage: %s <a> <b> <number of threads>\n", argv[0]);
		exit(-1);
	}

	//number_args equils to the number of threads
	int num_args = atoi(argv[3]);

	//Check
	for(int i = 1; i <= 3; i++){
		printf("%d", atoi(argv[i]));
	}
	printf("\n");

	//array for ids for summation threads
	pthread_t tids[num_args];

	//limits of the integral
	int a = atoi(argv[1]);
	int b = atoi(argv[2]);
	long double h = (b - a)/num_args;
	printf("h: %Lf\n", h);

	//the array of structures
	struct thread_struct args[num_args];

	for (int i = 0; i < num_args; i++){
		args[i].a = a + i*h;
		args[i].b = args[i].a + h;

		printf("Check\n");

		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&tids[i], &attr, counting_thread, &args[i]);

	}

	for (int i = 0; i < num_args; i++){
		pthread_join(tids[i], NULL);
	}

	printf("The sum is %Lf\n", sum);

	return (0);
}
