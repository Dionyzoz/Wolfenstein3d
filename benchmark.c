#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <math.h>

int main()
{
    clock_t start_t, end_t;
    double total_t;
    start_t = clock();
    for (int i = 1; i < 1000000000; i++)
    {
        sqrt(i * i);
    }
    end_t = clock();
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    printf("sqrt took %f\n", total_t);
    start_t = clock();
    for (int i = 1; i < 1000000000; i++)
    {
        cos(i) * i;
    }
    end_t = clock();
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    printf("cos took %f\n", total_t);
}