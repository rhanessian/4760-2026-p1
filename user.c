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

int main (int argc, const char *argv[]){
	int iterations = atoi(argv[1]);
	int j;
	int pid = getpid();
	int ppid = getppid();
	for (j = 0; j < iterations; j++){
		fprintf(stderr, "USER PID: %d PPID: %d Iteration: %d before sleeping\n", pid, ppid, j+1);
		sleep(1);
		fprintf(stderr, "USER PID: %d PPID: %d Iteration: %d after sleeping\n", pid, ppid, j+1);
	}
	return 0;
}