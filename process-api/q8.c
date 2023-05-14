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
    int pipefd[2];

    if (pipe(pipefd) == -1)
    {
        // Failed to creat pipe
        fprintf(stderr, "Pipe failed Error: %s.\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    pid_t cpid1 = fork();
    if (cpid1 < 0)
    {
        // Failed to create child process
        fprintf(stderr, "Child 1 Fork failed Error: %s.\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    else if (cpid1 == 0) {
        close(pipefd[0]);           // Close pipe's read end
        dup2(pipefd[1], 1);         // Duplicate pipe write
        close(pipefd[1]);           // Close one of write end of pipe
        char *myargs[] = {"cat", "testfile.txt", NULL};
        execvp(myargs[0], myargs);
        fprintf(stderr, "Execvp failed Error: %s.\n", strerror(errno));
        _exit(1);
    }
    
    else
    {
        //pid_t rc = wait(NULL);
        close(pipefd[1]);       // Close pipe's write end
        dup2(pipefd[0], 0);     // Duplicate pipe's read end
        close(pipefd[0]);       // Close one of read end of pipe
        char *myargs[] = {"sort", NULL};
        execvp(myargs[0], myargs);
        fprintf(stderr, "Execvp failed Error: %s.\n", strerror(errno));
        exit(1);
    }
}