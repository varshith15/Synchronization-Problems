#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void *runner(void* param);
sem_t *chop;				//chopstick semaphore
int k;						//n value
time_t curr,p;


int main(int argc,char* argv[]){
	curr = time(NULL);
	int n;
	n = atoi(argv[2]);
	k = n;
	chop = (sem_t*)malloc(sizeof(sem_t)*n);
	for(int i=0;i<n;i++){
		sem_init(&chop[i],0,1);		// initializing the semeaphores
	}
	pthread_attr_t attr;
  	pthread_attr_init(&attr);
	pthread_t tid[n];
	int a[n];
	for(int i=0;i<n;i++){
		a[i]=i;
		pthread_create(&tid[i], &attr,runner,(void*)&a[i]);	//threads creating
	}
	for(int i=0;i<n;i++){
		pthread_join(tid[i], NULL);		//thread joinig
	}
	printf("process is done\n");
}

void *runner(void *param){
	int tid = *((int *)param);
	char s[1000] = "dp_out_thread_";
	char s1[1000] = "_DL.txt";
	char s2[1000];
	sprintf(s2,"%d",tid);
	strcat(s,s2);
	strcat(s,s1);
	FILE *fptr = fopen(s,"w"); 	//file to print the states
	for(int i=0;i<100;i++){
		//thiniking state
		p = time(NULL);
		fprintf(fptr,"Philosopher %d entering thiniking state, time: %s, round: %d\n",tid,ctime(&p),i);
		usleep((rand()%50+50)*1000);
		//hungry state
		p = time(NULL);
		fprintf(fptr,"Philosopher %d entering hungry state, time: %s, round: %d\n",tid,ctime(&p),i);
		sem_wait(&chop[tid]);
		sem_wait(&chop[(tid+1)%k]);
		//printf("thread:%d, turn: %d\n",tid,i);
		//eating state
		p = time(NULL);
		fprintf(fptr,"Philosopher %d entering eating state, time: %s, round: %d\n",tid,ctime(&p),i);
		usleep((rand()%100+100)*1000);
		sem_post(&chop[tid]);
		sem_post(&chop[(tid+1)%k]);
	}
	//leaving state
	p = time(NULL);
	fprintf(fptr,"Philosopher %d entering leaving state, time: %s\n",tid,ctime(&p));
}
