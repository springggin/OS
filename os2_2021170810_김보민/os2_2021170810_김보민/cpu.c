#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/syscall.h>
#include <sched.h>
#include <stdint.h>
#include <string.h>

#define ROW (100)
#define COL ROW

long long total_time;
int pid;
int count;

//extra task
void sighandler_child(int signo) {
	printf("DONE!! Process %02d : %d  %lldms \n", pid, count, total_time/1000000 );
	exit(0);
}
void sighandler_parent(int signo){
    pid_t terminatedChild = waitpid(pid, NULL, 0);
    exit(0);
}



int calc() {
	int i,j,k;
	int matrixA[ROW][COL];
	int matrixB[ROW][COL];
	int matrixC[ROW][COL];

		for (i = 0; i < ROW; i++) {
			for (j = 0; j < COL; j++) {
				for (k = 0; k < COL; k++) {
					matrixC[i][j] += matrixA[i][j] * matrixB[i][j];
				}
			}
		}
}

struct sched_attr {
   uint32_t size;              /* Size of this structure */
   uint32_t sched_policy;      /* Policy (SCHED_*) */
   uint64_t sched_flags;       /* Flags */
   int32_t  sched_nice;        /* Nice value (SCHED_OTHER,
                                  SCHED_BATCH) */
   uint32_t sched_priority;    /* Static priority (SCHED_FIFO,
                                  SCHED_RR) */
   /* Remaining fields are for SCHED_DEADLINE */
   uint64_t sched_runtime;
   uint64_t sched_deadline;
   uint64_t sched_period;
};


static int sched_setattr(pid_t pid, const struct sched_attr *attr, unsigned int flags)
{
    return syscall(SYS_sched_setattr, pid, attr, flags);
}



int main(int argc, char* argv[]) {
	int i;
	int num_of_processes, time_to_execute;
	pid_t pids[100];

	// input argument error 
	if (argc != 3){
		fprintf(stderr, "Usage: %s <num of processes> <time to execute>\n", argv[0]);
		exit(1);
	}

	num_of_processes = atoi(argv[1]);
	time_to_execute = atoi(argv[2]);


	struct timespec begin, end;
	int timeslice;
	struct sched_attr attr;
	memset(&attr, 0, sizeof(attr));
	attr.size = sizeof(struct sched_attr);

	attr.sched_priority = 10;     // set process's priority to 10.
	attr.sched_policy = SCHED_RR; // set scheduler to RoundRobin.
	int result = sched_setattr(getpid(), &attr, 0);
	if (result == -1) printf("Error calling sched_setattr.\n");
	

	signal(SIGINT, sighandler_parent);

	for (i = 0; i < num_of_processes; i++) {
		printf("Creating Process: #%d\n", i);
		pid=i;
		pids[i] = fork(); //creating process
		if (pids[i] < -1) return -1;
		else if (pids[i] == 0) {
			clock_gettime(CLOCK_MONOTONIC, &begin);//시작시각
			pid=i;
			timeslice=1;

			while (1) {
				calc();
				signal(SIGINT, sighandler_child);
				clock_gettime(CLOCK_MONOTONIC, &end);//종료시각
				count++;

				total_time = ((end.tv_sec - begin.tv_sec) * 1000000000 + (end.tv_nsec - begin.tv_nsec)); //ns
				if (total_time/100000000 == timeslice){
					printf("PROCESS #%02d count = %04d %dms\n", pid, count, 100);
					timeslice++;
				}
				//총시간이 time_to_execute 초과할 시
				if (total_time > (time_to_execute*1000000000LL)){
					printf("Done!! PROCESS #%02d : %06d %lldms\n", pid, count, total_time/1000000);  
					break;
				}	
			}
			exit(0);
		}

	}

	//wait until child ends
	for(i = 0; i < num_of_processes; i++) {
      		pid_t terminatedChild = waitpid(pids[i], NULL, 0);
        
   	 }

	return 0;
}

