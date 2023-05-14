#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

// Function prototypes
void check_write_operation(int fd, int rc, char *buffer);

int main(int argc, char *argv[]) {

    printf("Main process: (%d)\n", (int) getpid());
    fflush(stdout);
    int fd = open("/tmp/q2.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    assert(fd >= 0);
    // fork child process 
    pid_t rc = fork();
    if (rc == -1) {
        // Failed to create child process
        fprintf(stderr, "Fork failed.\n");
        exit(1);
    }
    else if (rc == 0) {
        char *buffer = malloc(sizeof(char) * 50);
        sprintf(buffer, "Hi, child process: (%d) wrote this.\n", (int) getpid());
        int rc = write(fd, buffer, strlen(buffer));
        /* synchronization mechanism to prevent concurrent writes 
        if (fsync(fd) == -1)
        {
            fprintf(stderr, "Flush to disk failed.\n");
            close(fd);
            exit(1);
        }
        */
        check_write_operation(fd, rc, buffer);
        free(buffer);
    }
    else {
        char *buffer = malloc(sizeof(char) * 50);
        sprintf(buffer, "Hi, parent process: (%d) wrote this.\n", (int) getpid());
        int rc = write(fd, buffer, strlen(buffer));
        /* synchronization mechanism to prevent concurrent writes
        if (fsync(fd) == -1)
        {
            fprintf(stderr, "Flush to disk failed.\n");
            close(fd);
            exit(1);
        }
        */
        check_write_operation(fd, rc, buffer);
        free(buffer);
    }
    // Close access to file
    close(fd);
    return 0;
}

// Checks if write was successful
void check_write_operation(int fd, int rc, char *buffer)
{
    if (rc != strlen(buffer)) {
        fprintf(stderr, "Write failed.\n");
        free(buffer);
        close(fd);
        exit(1);
    }
}