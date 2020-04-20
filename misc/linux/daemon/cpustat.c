#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>

#define SHMKEY 0x850729
#define SHMSZ  64

int main()
{
	int shmid;
	float *pshm;
/*
   wait daemon for 5 seconds ?
	sleep(5);
*/	
	if((shmid=shmget(SHMKEY,SHMSZ,0660))<0)
	{
		perror("shmget");
		exit(-1);
	}
	if((pshm=(float *)shmat(shmid,0,0))<(float *)0)
	{
		perror("shmat");
		exit(-1);
	}

	printf("Current CPU Usage: %.2f%%\n",100.0*(*pshm));

	if((shmdt(pshm))<0)
	{
		perror("shmdt");
		exit(-1);
	}

	return 0;
}
