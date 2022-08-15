

//part b
// sends sem_post to thread factory to unblock threads
#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include<semaphore.h>
#include<errno.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<fcntl.h>
#include<pthread.h>

#define true 1
#define false 0
#define SUCCESS 0



//semaphore
sem_t *pSem_t;
//num of threads to wake counter
int wake_num=-1;
int main(void) {

	pSem_t =sem_open("pSem_t",0);
	printf("thread waker's pid:%d",getpid());

	//do while till 0 is entered
	do{
		printf("\nHow many threads do you want to wake,ENTER 0 to exit\n");
		scanf("%d",&wake_num);
		for(int i=0; i<wake_num; i++){
			sem_post(pSem_t);
		}
	}while(wake_num!=0);
	//check semaphor's been closed and destroyed
	 if(sem_close(pSem_t) ==-1) perror("sem_close returned -1");
	 if(sem_destroy(pSem_t ==-1)) perror("sem_destroy unsuccessful");
	return EXIT_SUCCESS;
}

