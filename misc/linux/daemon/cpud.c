#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory.h>
#include <signal.h>
#include <sys/resource.h>
#include <sys/shm.h>

#define SHMKEY 0x850729
#define SHMSZ  64

void sigfunc(int signo)
{
	int smid;
	if((smid=shmget(SHMKEY,SHMSZ,0660))<0)
		exit(-1);
	if(shmctl(smid,IPC_RMID,0)<0)
		exit(-1);
	exit(0);
}

int main()
{
	int i,fd0,fd1,fd2;
	pid_t pid,sid;
	struct rlimit  rfd;

	if(getrlimit(RLIMIT_NOFILE,&rfd)<0)
		exit(-1);

	if((pid=fork())<0)
		exit(-1);
	else if(pid!=0)
		exit(0);

	if((sid=setsid())<0)
		exit(-1);

	if(chdir("/")<0)
		exit(-1);

	umask(0);
	
	signal(SIGTERM, sigfunc);

	if(rfd.rlim_max==RLIM_INFINITY)
		rfd.rlim_max=1024;
	for(i=0;i<rfd.rlim_max;i++)
		close(i);
	fd0=open("/dev/null",O_RDWR);
	fd1=dup(0);
	fd2=dup(0);

	double usrA,niceA,sysA,idleA,waitA,irqA,sirqA;
	double usrB,niceB,sysB,idleB,waitB,irqB,sirqB;
	float cpuUsage;
	FILE * fp;
	char buf[512];
	char statBuf[7][64];
	memset(buf,0,sizeof(char)*512);
	memset(statBuf,0,sizeof(char)*7*64);
	
	int shmid;
	float *pshm;
	if((shmid=shmget(SHMKEY,SHMSZ,IPC_CREAT|IPC_EXCL|0660))<0)
		exit(-1);

	while(1)
	{

		if((fp=fopen("/proc/stat","r"))==NULL)	
			exit(-1);
		if((fgets(buf,sizeof(buf),fp))==NULL)
			exit(-1);
		if((sscanf(buf,"%*s %s %s %s %s %s %s %s", statBuf[0], statBuf[1], statBuf[2], statBuf[3], statBuf[4], statBuf[5], statBuf[6]))<7)
			exit(-1);
		
		usrA=atol(statBuf[0]);
		niceA=atol(statBuf[1]);
		sysA=atol(statBuf[2]);
		idleA=atol(statBuf[3]);
		waitA=atol(statBuf[4]);
		irqA=atol(statBuf[5]);
		sirqA=atol(statBuf[6]);
		
		memset(buf,0,sizeof(char)*512);
		memset(statBuf,0,sizeof(char)*7*64);		
		fclose(fp);
		
		sleep(3);		
		
		if((fp=fopen("/proc/stat","r"))==NULL)
			exit(-1);
		if((fgets(buf,sizeof(buf),fp))==NULL)
			exit(-1);
		if((sscanf(buf,"%*s %s %s %s %s %s %s %s", statBuf[0], statBuf[1], statBuf[2], statBuf[3], statBuf[4], statBuf[5], statBuf[6]))<7)
			exit(-1);
		
		usrB=atol(statBuf[0]);
		niceB=atol(statBuf[1]);
		sysB=atol(statBuf[2]);
		idleB=atol(statBuf[3]);
		waitB=atol(statBuf[4]);
		irqB=atol(statBuf[5]);
		sirqB=atol(statBuf[6]);

		memset(buf,0,sizeof(char)*512);
		memset(statBuf,0,sizeof(char)*7*64);				
		fclose(fp);
		
		cpuUsage=1.0-(idleB-idleA)/((usrB+niceB+sysB+idleB+waitB+irqB+sirqB)-(usrA+niceA+sysA+idleA+waitA+irqA+sirqA));
		if((pshm=(float *)shmat(shmid,0,0))<0)
			exit(-1);		
		*pshm=cpuUsage;
		if((shmdt(pshm))<0)
			exit(-1);
		/*	
		sleep(30);
		*/
	}	
}

