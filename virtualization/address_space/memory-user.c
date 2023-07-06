#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>

static volatile sig_atomic_t got_SIGALRM = 0;

void timer_handler(int signum)
{
    if (signum == SIGALRM)
        got_SIGALRM = 1;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <size-in-MB> [seconds]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Allocate memory for array on the heap
    float arr_size = strtof(argv[1], NULL) * 1024 * 1024;     /*bytes to MB*/
    float *arr = (float*) malloc(arr_size);
    
    if (arr == NULL)
    {
        fprintf(stderr, "Failed to allocate memory.\n");
        exit(EXIT_FAILURE);
    }

    // Set up signal handler
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = timer_handler;

    if (sigaction(SIGALRM, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    // Set up timer
    struct itimerval timer;

    // Activate timer if timer arg. is passed in CMD
    if (argc == 3)
    {
        timer.it_value.tv_sec = atoi(argv[2]);
        if (setitimer(ITIMER_REAL, &timer, NULL) == -1)
        {
            perror("setitimer");
            exit(EXIT_FAILURE);
        }    
    }

    // Constantly stream through the array, display each entries address space
    int arr_length = (int) arr_size / sizeof(float);
    printf("%d\n", arr_length);
    int index = 0;
    while (!got_SIGALRM)
    {
        printf("Address of array[%d]: %p\n", index, &arr[index]);      
        index = (index + 1) % arr_length;
    }

    printf("Timer elpased!\nPerforming cleanup...\n");
    free(arr);
    exit(EXIT_SUCCESS);
}