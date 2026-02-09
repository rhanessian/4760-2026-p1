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

int counter = 0;
int total_launched = 0;
int total_fin = 0;

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
	fprintf(stderr, "Default proc is 9, default simul is 2, default iter is 3.\n");
}

void proc_exit(){
	pid_t pid;
	while ((pid = waitpid(-1, 0, WNOHANG)) > 0) {
    	printf("Current Simultaneous Processes: %d\n", counter);
    	counter--;
    	total_fin++;
    }
}

int main (int argc, char *argv[]){
	pid_t userpid = 0;
	int i, j;
	int status;
	
    char opt;
    options_t options;

    options.proc = 9;
    options.simul = 2;
    options.iter = 3;
	
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
		
	signal (SIGCHLD, proc_exit);
	while (total_launched < options.proc) {
		if (counter >= options.simul){
            waitpid(userpid,0,0);
            continue;
        }
		userpid = fork();
		if (userpid < 0){
			perror("Fork erro\nr");
			exit(EXIT_FAILURE);
		} else if (userpid == 0){
			printf("New child process launched.\n");
            char *newargv[3];
            char iterBuf[20];
            sprintf(iterBuf, "%d",options.iter);
            newargv[0] = "./user";
            newargv[1] = iterBuf;
            newargv[2] = NULL;
            execvp("./user",newargv);
            exit(EXIT_SUCCESS);
        } else {
        	counter++;
        	total_launched++;
        }   
	}
	
	while (counter > 0) {
		counter--;
		wait(NULL);
	}

	printf("Total number of processes finished: %d\n", total_fin);
	
	return 0;
}		
		