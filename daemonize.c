/*******************************************************************************
 * Daemonize
 *
 * This program starts the given command as a daemon and prints it's
 * pid to stdout
 ******************************************************************************/


/* crack */
/* sip */
#include <stdio.h>

#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

static const char *arguments[_POSIX_ARG_MAX]; // must be NULL terminated

int main(int argc, char **argv)
{
        pid_t pid;
        int child_ret;
        int pipefd[2];

        if(argc < 2) {
                fputs("No command specified...\n", stderr);
                return EXIT_FAILURE;
        }

        memcpy(arguments, &argv[1], argc*sizeof(char*));
        arguments[argc] = NULL;

        if(pipe(pipefd) < 0) {
                perror("pipe()");
                return EXIT_FAILURE;
        }

        if( (pid = fork()) < 0 ) {
                perror("fork()");
                return EXIT_FAILURE;
        }

        if(!pid) {
                /* the child */

                if(setsid() < 0) {
                        perror("setsid()");
                        return EXIT_FAILURE;
                }

                if( (pid = fork()) < 0 ) {
                        perror("child - fork()");
                        return EXIT_FAILURE;
                }

                if(pid > 0) {
                        /* has child */

                        write(pipefd[1], &pid, sizeof(pid_t));
                        close(pipefd[0]); close(pipefd[1]);

                        return EXIT_SUCCESS;
                }

                /* the daemon */
                for(int i = sysconf(_SC_OPEN_MAX); i >= 0; i--)
                        close(i);
                execvp(arguments[0], (char*const*) arguments);
                perror("execvp()");
                return EXIT_FAILURE;
        }else {
                /* wait for child, to ensure the pid can be read */
                waitpid(pid, &child_ret, 0);
                if(child_ret != EXIT_SUCCESS) return child_ret;

                read(pipefd[0], &pid, sizeof(pid_t));

                printf("%d\n", pid);
                close(pipefd[0]); close(pipefd[1]);
        }

        return EXIT_SUCCESS;
}
