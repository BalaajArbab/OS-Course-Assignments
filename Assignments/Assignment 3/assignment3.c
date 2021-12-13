// Balaaj Arbab
// 251055771
// CS 3305A Assignment 3

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

int port[2];

void * sum(void * thread_id)
{
    int * thread = (int *) thread_id;
    char str[300];

    char x[100]; x[0] = '\0';
    char y[100]; y[0] = '\0';

    char k;

    read(port[0], &k, 1);

    while (k != ',')
    {
	char b[2]; b[0] = k; b[1] = '\0';

        strcat(x, b);

        read(port[0], &k, 1);

    }

    read(port[0], y, 99);

    sprintf(str, "thread (TID %d) reads X = %s and Y = %s from the pipe", *thread, x, y);
    puts(str);

    int X = atoi(x);
    int Y = atoi(y);

    int sum = X + Y;

    sprintf(str, "%d", sum);
    write(port[1], str, strlen(str));

    sprintf(str, "thread (TID %d) writes X + Y = %d to the pipe", *thread, sum);
    puts(str);

}

void * odd_even(void * thread_id) 
{
    int * thread = (int *) thread_id;
    char str[300];

    char sum[101]; sum[0] = '\0';

    read(port[0], sum, 100);

    sprintf(str, "thread (TID %d) reads X + Y = %s from the pipe", *thread, sum);
    puts(str);

    int summ = atoi(sum);

    if (summ % 2 == 0)
    {
	sprintf(str, "thread (TID %d) identifies X + Y = %s as an even number", *thread, sum);
	puts(str);

    }
    else 
    {
	sprintf(str, "thread (TID %d) identifies X + Y = %s as an odd number", *thread, sum);
	puts(str);

    }

    write(port[1], sum, strlen(sum));

    sprintf(str, "thread (TID %d) writes X + Y = %s to the pipe", *thread, sum);
    puts(str);

}

void * digit_count(void * thread_id)
{
    int * thread = (int *) thread_id;

    char str[300];

    char sum[101]; sum[0] = '\0';

    read(port[0], sum, 100);

    int len = strlen(sum);

    sprintf(str, "thread (TID %d) reads X + Y = %s from the pipe", *thread, sum);
    puts(str);

    sprintf(str, "thread (TID %d) identifies X + Y = %s as a %d digit number", *thread, sum, len);
    puts(str);

}


int main( int argc, char ** argv)
{
    char str[300];

    int lenX = strlen(argv[1]);
    int lenY = strlen(argv[2]);

    char X[lenX + 1];
    char Y[lenY + 1];

    strcpy(X, argv[1]);
    strcpy(Y, argv[2]);

    pid_t pid = getpid();
    
    int e = pipe(port);

    if (e == -1) 
    {
	    puts("Could not create pipe. Exiting.");
	    exit(e);

    }

    int thread_100 = 100;
    int thread_101 = 101;
    int thread_102 = 102;

    pthread_t t1;
    pthread_t t2;
    pthread_t t3;

    sprintf(str, "parent (PID %d) receives X = %s and Y = %s from the user", pid, X, Y);
    puts(str);

    write(port[1], X, lenX);
    write(port[1], ",", 1);
    write(port[1], Y, lenY);


    sprintf(str, "parent (PID %d) writes X = %s and Y = %s to the pipe", pid, X, Y);
    puts(str);

    pthread_create(&t1, NULL, sum, &thread_100);
    pthread_join(t1, NULL);

    pthread_create(&t2, NULL, odd_even, &thread_101);
    pthread_join(t2, NULL);

    pthread_create(&t3, NULL, digit_count, &thread_102);
    pthread_join(t3, NULL);



}
