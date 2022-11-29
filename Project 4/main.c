#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <string.h>
#include <errno.h>

int main(int argc, char **argv) {
    sigset_t new_sig_set; // set up mask
    sigemptyset(&new_sig_set); // empty the mask
    sigaddset(&new_sig_set, SIGUSR1); // add SIGUSR1 to the mask
    sigaddset(&new_sig_set, SIGUSR2); // add SIGUSR2 to the mask

    int error;
    if((error = pthread_sigmask(SIG_BLOCK, &new_sig_set, NULL)) != 0) {
        fprintf(stderr, "pthread_sigmask: %s\n", strerror(error));
        return 1;
    }

    int pid = fork();

    if(pid == 0) {
        char * arguments[2];
        arguments[0] = "catcher";
        arguments[1] = NULL;


        //char* argument_list[] = {"catcher", NULL}; // NULL terminated array of char* strings

        if(execvp("./catcher", arguments) < 0) {
            perror("execvp");
            return 1;
        }
    }

    else if(pid > 0) {
        sleep(2);
        srand((unsigned)time(NULL));
        while(1) {
            printf("Generating Signal\n");
            fflush(stdout);
            int random = rand() % ((2+1)-1)+1;
            if(random == 1) {
                printf("Sending signal: SIGUSR1\n");
                fflush(stdout);
                if(kill(pid, SIGUSR1) < 0) {
                    perror("kill");
                }
            }
            else {
                printf("Sending signal: SIGUSR2\n");
                fflush(stdout);
                if(kill(pid, SIGUSR2) < 0) {
                    perror("kill");
                }
            }
            int sleepTime = (rand() % (100000 - 10000 + 1)) + 10000;
            // get rand between 0.01-0.1 secs
            usleep(sleepTime);
            // sleep for random time
        }
    }
    else {
        perror("fork");
    }
}
