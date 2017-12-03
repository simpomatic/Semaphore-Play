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
#include "/home/luis/source/repos/Semaphore-Play/semaphore.h"
using namespace std;

const int MAXCHAR = 10;
const int BUFFSIZE = 4;
pid_t* childProcesses = new pid_t[4];
enum {GET_U, GET_V}; // set up names of my 2 semaphores

void generateNumber_proc(SEMAPHORE &sem, char *shmBUF, int id){
    cout << childProcesses[id] << endl;
    ushort* semvals = sem.get_values();
    for(int i = 0; i < 2; i++) {
        cout << semvals[i] << endl;
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
            if(childProcesses[k] = fork()) {
		        generateNumber_proc(sem, shmBUF, k);
            }
            //Keeps the number child processes at bay
            else {
                break;
            }
        }
    }
    return 0;
}