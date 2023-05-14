#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    printf("Main process: (%d) begins\n", (int) getpid());
    fflush(stdout);
    pid_t rc = fork();
    if (rc == -1) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0) {
        printf("Forked Child process: (%d)\n", (int) getpid());
        fflush(stdout);
        extern char **environ;
        char *args[] = {"/bin/ls", NULL};
        // Spawn /bin/ls using various exec() calls
        
        // execl("/bin/ls", args[0], NULL);
        // execlp("/bin/ls", args[0], NULL);
        // execv("/bin/ls", args);
        // execvp(args[0], args);
        execvpe("/bin/ls", args, environ);    
        // execle("/bin/ls", args[0], NULL, environ);

        fprintf(stderr, "exec failed\n");
        exit(1);
    } else {
        // parent goes down this path (original process)
        pid_t wc = wait(NULL);
        printf("Main process: %d resumes after child process (%d) finishes.\n", (int) getpid(), wc);
    }
    return 0;
}