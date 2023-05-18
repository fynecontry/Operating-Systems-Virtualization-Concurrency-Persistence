#define _GNU_SOURCE
#include <errno.h>
#include <sched.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

const int SWITCH_COUNT = 100;

int main()
{
    int pipe1fd[2];          /* pipe1fd (parent -> child) */
    int pipe2fd[2];          /* pipe2fd (parent <- child) */
    char buffer[2];

    // Create pipes
    if (pipe(pipe1fd) == -1 || pipe(pipe2fd) == -1)
    {
        // Failed to creat pipe
        fprintf(stderr, "Pipe failed\n");
        exit(EXIT_FAILURE);
    }

    // Fork process
    pid_t p2 = fork();
    if (p2 == -1)
    {
        // Failed to create child process
        fprintf(stderr, "Fork failed\n");
        exit(EXIT_FAILURE);
    }
    else if (p2 == 0)
    {
        close(pipe1fd[STDOUT_FILENO]);      /* Close unused write end of pipe1fd */
        close(pipe2fd[STDIN_FILENO]);       /* Close unused read end of pipe2fd */

        // Set CPU affinity to CPU 0
        cpu_set_t set;
        CPU_ZERO(&set);
        CPU_SET(0, &set);
        if (sched_setaffinity(0, sizeof(set), &set) == -1)
        {
            fprintf(stderr, "Failed to sets CPU affinity mask\n");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < SWITCH_COUNT; i++)
        {
            read(pipe1fd[STDIN_FILENO], buffer, 2);
            write(pipe2fd[STDOUT_FILENO], "p2", 2);
        }

        close(pipe2fd[STDOUT_FILENO]);       /* Close write end when done */
        exit(EXIT_SUCCESS);
    }
    else
    {
        close(pipe1fd[STDIN_FILENO]);           /* Close unused read end of pipe1fd */
        close(pipe2fd[STDOUT_FILENO]);          /* Close unused write end of pipe2fd */

        // Set CPU affinity to CPU 0
        cpu_set_t set;
        CPU_ZERO(&set);
        CPU_SET(0, &set);
        if (sched_setaffinity(0, sizeof(set), &set) == -1)
        {
            fprintf(stderr, "Failed to sets CPU affinity mask\n");
            exit(EXIT_FAILURE);
        }
        
        struct timeval t1, t2;
        gettimeofday(&t1, NULL);

        for (int i = 0; i < SWITCH_COUNT; i++)
        {
            write(pipe1fd[STDOUT_FILENO], buffer, 2);
            read(pipe2fd[STDIN_FILENO], "p1", 2);
        }


        gettimeofday(&t2, NULL);
        printf("Seconds elapsed since calls: %ld\n", t2.tv_sec - t1.tv_sec);
        double elapsed_time_microseconds = (t2.tv_sec - t1.tv_sec) * 1000000 + (t2.tv_usec - t1.tv_usec);
        printf("Microseconds elapsed since context switching: %.2f\n", elapsed_time_microseconds);
        printf("Average microsecond per switch: %.2f\n", elapsed_time_microseconds / SWITCH_COUNT);

        close(pipe1fd[STDOUT_FILENO]);
        close(pipe2fd[STDIN_FILENO]);

        exit(EXIT_SUCCESS);
    }
}