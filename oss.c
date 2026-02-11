//Rebecca Hanessian
//CS4760
//Project 1: Skeleton Multiple Processes

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

volatile sig_atomic_t counter = 0;
volatile sig_atomic_t total_fin = 0;
int total_launched = 0;

typedef struct {
    int proc;
    int simul;
    int iter;
} options_t;

void print_usage (const char* argmt){
	fprintf(stderr, "Usage: %s [-h] [-n proc] [-s simul] [-t iter]\n", argmt);
	fprintf(stderr, "	proc is the number of user processes to launch\n");
	fprintf(stderr, "	simul is the number of processes that can run simultaneously\n");
	fprintf(stderr, "	iter is the number to be passed to the user processes\n");
	fprintf(stderr, "Default proc is 9, default simul is 3, default iter is 5.\n");
}

void proc_exit(int signum){
	pid_t pid;
	while ((pid = waitpid(-1, NULL, WNOHANG)) > 0) {
    	counter--;
    	total_fin++;
    }
}

int main (int argc, char *argv[]){
	pid_t userpid;
    char opt;
    options_t options;

    options.proc = 9;
    options.simul = 3;
    options.iter = 5;
	
	opterr = 0;

	while ((opt = getopt (argc, argv, "hn:s:t:")) != -1)
		switch (opt) {
            case 'h':
                print_usage (argv[0]);
                return (EXIT_SUCCESS);
			case 'n':
				options.proc = atoi(optarg);
				break;
			case 's':
				options.simul = atoi(optarg);
				break;
			case 't':
				options.iter = atoi(optarg);
				break;
			default:
				printf ("Invalid option %c\n", opt);
				print_usage (argv[0]);
				return (EXIT_FAILURE);		
		}
		
	struct sigaction sigact;
    sigact.sa_handler = proc_exit;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = SA_RESTART;
		
	if (sigaction(SIGCHLD, &sigact, NULL) == -1) {
		perror("Sigaction error\n");
		exit(EXIT_FAILURE);
	}
	
	sigset_t mask, oldmask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGCHLD);
	
	while (total_launched < options.proc) {
		if (counter >= options.simul) {
            sigprocmask(SIG_BLOCK, &mask, &oldmask);
            while (counter >= options.simul) {
                sigsuspend(&oldmask);
            }
            sigprocmask(SIG_UNBLOCK, &mask, NULL);
        }
        
		userpid = fork();
		
		if (userpid == 0){
			printf("New child process launched.\n");
            char iterBuf[20];
            snprintf(iterBuf, sizeof(iterBuf), "%d", options.iter);
           	char *newargv[] = {"./user", iterBuf, NULL};
            execvp(newargv[0],newargv);
            perror("Execvp error\n");
            _exit(EXIT_FAILURE);
        } else if (userpid > 0){
			counter++;
        	total_launched++;
		} else {
			perror("Fork error\n");
			exit(EXIT_FAILURE);
		}  
	}
	
	sigprocmask(SIG_BLOCK, &mask, &oldmask);
	
	while (total_fin < options.proc) {
		sigsuspend(&oldmask);
	}
	
    sigprocmask(SIG_UNBLOCK, &mask, NULL);


	printf("Total number of processes finished: %d\n", total_fin);
	
	return 0;
}		
		