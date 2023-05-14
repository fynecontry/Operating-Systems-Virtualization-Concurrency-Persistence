#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    pid_t cpid = fork();
    if (cpid < 0)
    {
        // Failed to create child process
        fprintf(stderr, "Fork failed Error: %s.\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    else if (cpid == 0) {
        // child: close standard output file descriptor
        close(STDOUT_FILENO);
        printf("Child process, trying to print to stdout...\n");
    }
    else {
        // pid_t wc = wait(NULL);
        printf("Hi this is parent process!\n.");
    }
    return 0;
}
