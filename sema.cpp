#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include "semaphore.h"

const int MAXCHAR = 10;
const int BUFFSIZE = 3;
enum {GET_U, GET_V}; // set up names of my 2 semaphores

int main() {
    int shmid;
	char *shmBUF;

	SEMAPHORE sem(2);
	sem.V(GET_U);
	sem.V(GET_V);
	shmid = shmget(IPC_PRIVATE, BUFFSIZE*sizeof(char), PERMS);
	shmBUF = (char *)shmat(shmid, 0, SHM_RND);
    
    sem.remove();
    sem.~SEMAPHORE();
    return 0;
}