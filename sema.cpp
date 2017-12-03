#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h> 
#include "semaphore.h"
using namespace std;

const int MAXCHAR = 10;
const int BUFFSIZE = 4;
pid_t* childProcesses = new pid_t[4];
enum {GET_U, GET_V}; // set up names of my 2 semaphores

inline void delay( unsigned long ms ) {
    usleep( ms * 1000 );
}

void generateNumber_proc(SEMAPHORE &sem, char *shmBUF, int id){
    ushort* semvals;
    while(true) {
        srand(time(NULL));
        semvals = sem.getValues();
        if(semvals[0] > 0) {
            sem.P(GET_U);
            int random = 1001;
            while((random%827395609 != 0) && random > 1000) {
                random = rand();
            }
            cout << "[" << id << "]: Produced the number " << random << " for int U" << endl;
            delay(1000);
            sem.V(GET_U);
            delay(4000);
        }
        else if(semvals[1] > 0) {
            sem.P(GET_V);
            int random = 1001;
            while((random%962094883 != 0) && random > 1000) {
                random = rand();
            }
            cout << "[" << id << "]: Produced the number " << random << " for int V" << endl;
            delay(1000);
            sem.V(GET_V);
            delay(4000);
        }
    }
}

void parent_cleanup (SEMAPHORE &sem, int shmid) {
    string input;
	int status;			/* child status */
    //Will continue until the terminate command is set in place 
    while(input != "!wq"){
        cin >> input;
        if(input == "!wq"){
            for(int k=0; k<4; k++) {
                kill(childProcesses[k], SIGKILL);
            }
        }
    }
	shmctl(shmid, IPC_RMID, NULL);	/* cleaning up */
	sem.remove();
    sem.~SEMAPHORE();
} // parent_cleanup

int main() {
    int shmid;
	char *shmBUF;
    long childPID;

	SEMAPHORE sem(2);
	shmid = shmget(IPC_PRIVATE, BUFFSIZE*sizeof(char), PERMS);
	shmBUF = (char *)shmat(shmid, 0, SHM_RND);

    //Parent clean up
    if(fork()){
        parent_cleanup(sem, shmid);
    }
    //Child that will essentially fork four more children to generate numbers
    else {
        for(int k=0; k<4; k++) {
            if(childProcesses[k] = fork()) {}
            //Keeps the number child processes at bay
            else {
                generateNumber_proc(sem, shmBUF, k);
                break;
            }
        }
    }
    return 0;
}