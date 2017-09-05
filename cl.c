// shm_tc2.c - time client shared mem ver2 : use semaphores for locking
// program uses shared memory with key 99
// program uses semaphore set with key 9900

#include "stdio.h"
#include "sys/shm.h"
#include "time.h"
#include "sys/types.h"
#include "sys/ipc.h"
#include "sys/sem.h"
#include "stdlib.h"
#include <unistd.h>
#include "string.h"

#define TIME_MEM_KEY 99					      // kind of like a port #
#define TIME_SEM_KEY 9900					  // like a filename
#define SEG_SIZE ( (size_t)100 )              // size of segment
#define oops( m, x ) { perror(m); exit(x);}
union semun{ int val; struct semid_ds* buf; ushort* array; };

char* ctime();
void wait_and_lock( int );
void release_lock( int );

int main()
{       char line[128];
	int seg_id;
	char* mem_ptr;
        char *input;
	long now;
	int semset_id;							  // id for semaphore set
	int n=1;
	// create a shared memory segment
	seg_id = shmget( TIME_MEM_KEY, SEG_SIZE, 0777 );
	
	if( seg_id == -1 )
		oops( "shmget", 1 );
	
	// attach to it and get a pointer to where it attaches
       while(n){
        printf("> ");
	mem_ptr = shmat( seg_id, NULL, 0 );
         input=fgets(line,sizeof(line),stdin);
 	
	strcpy( mem_ptr, input );
	if ( mem_ptr == (void*) -1 )
		oops( "shmat", 2 );
	
	// Thing 1: connect to semaphore set 9900 with 2 semaphores
	semset_id = semget( TIME_SEM_KEY, 2, 0 );
	wait_and_lock( semset_id );
	sleep(1);
       
	
	
	release_lock( semset_id );
      }
	shmdt( mem_ptr );					      // detach, but not needed here
	
	return 0;
}

void wait_and_lock( int semset_id )
{
	union semun sem_info;					  // some properties
	struct sembuf actions[2];				  // action set, an array
	
	actions[0].sem_num = 1;					  // sem[1] is n_writers
	actions[0].sem_flg = SEM_UNDO;			  // auto cleanup
	actions[0].sem_op = 0;					  // wait for 0
	actions[1].sem_num = 0;					  // sem[0] is n_readers
	actions[1].sem_flg = SEM_UNDO;			  // auto cleanup
	actions[1].sem_op = 1;					  // incr n_readers
	
	if ( semop( semset_id, actions, 2 ) == -1 )
		oops( "semop: locking", 10 );
}

void release_lock( int semset_id )
{
	union semun sem_info;					  // some properties
	struct sembuf actions[1];				  // action set
	
	actions[0].sem_num = 0;					  // sem[0] is n_readers
	actions[0].sem_flg = SEM_UNDO;			  // auto cleanup
	actions[0].sem_op = -1;					  // decr reader country
	
	if ( semop( semset_id, actions, 1 ) == -1 )
		oops( "semop: unlocking", 10 );
}