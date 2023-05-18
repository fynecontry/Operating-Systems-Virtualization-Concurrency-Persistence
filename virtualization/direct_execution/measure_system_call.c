#include <assert.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

const int SYSCALL_COUNT = 100;

int main(int argc, char *argv[])
{
    struct timeval t1, t2;
    char *buffer = malloc(sizeof(char) * 2);
    assert(buffer);
    gettimeofday(&t1, NULL);
    for (int i = 0; i < SYSCALL_COUNT; i++)
    {
        if (read(STDOUT_FILENO, buffer, 0) == -1)
        {
            printf("Read Failed\n");
            exit(1);
        }
    }
    gettimeofday(&t2, NULL);
    printf("Seconds elapsed since calls: %ld\n", t2.tv_sec - t1.tv_sec);
    double elapsed_time_microseconds = (t2.tv_sec - t1.tv_sec) * 1000000 + (t2.tv_usec - t1.tv_usec);
    printf("Microseconds elapsed since calls: %.2f\n", elapsed_time_microseconds);
    printf("Average microsecond per call: %.2f\n", elapsed_time_microseconds / SYSCALL_COUNT);
    return 0;
}