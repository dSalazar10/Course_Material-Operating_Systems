// shm_ts2.c -- time server shared mem ver2 : use semaphores for locking
// program uses shared memory with key 99
// program uses semaphore set with key 9900

#include "stdio.h"
#include "stdlib.h"
#include "sys/shm.h"
#include "time.h"
#include "sys/types.h"
#include "sys/sem.h"
#include "signal.h"
#include "string.h"

#define TIME_MEM_KEY 99						  // like a filename
#define TIME_SEM_KEY 9900
#define SEG_SIZE ( (size_t)100 )			  // size of segment
#define oops( m, x ) { perror(m); exit(x); }

// semaphore union
union semun { int val; struct semid_ds* buf; ushort* array; };

int seg_id, semset_id;					      // global for cleanup()

void cleanup( int );

char* ctime();
void cleanup( int );
void set_sem_value( int, int, int );
void wait_and_lock( int );
void release_lock( int );

int main()
{
  signal( SIGINT, cleanup );                  // ctrl-C calls cleanup()
    
	char* mem_ptr;
	time_t now;
	int n;
	
	// create a shared memory segment
	seg_id = shmget( TIME_MEM_KEY, SEG_SIZE, IPC_CREAT | 0777 );
	
	if ( seg_id == -1 )
		oops( "shmget", 1 );
	
	// attach to it and get a pointer to where it attaches
	mem_ptr = shmat( seg_id, NULL, 0 );
	
	if ( mem_ptr == ( void* ) -1 )
		oops( "shmat", 2 );
	
	// Thing 1: create a semset: key 9900, 2 semaphores, and mode rw-rw-rw
	semset_id = semget( TIME_SEM_KEY, 2, ( 0666 | IPC_CREAT | IPC_EXCL ) );
	
	if ( semset_id == -1 )
		oops( "semget", 3 );
	
	// Thing 2: Set both semaphores to 0:
	set_sem_value( semset_id, 0, 0 );		  // set counters
	set_sem_value( semset_id, 1, 0 );		  // both to zero
	
	// run for a minute
	for ( n = 0; n < 60; n++ )
	{
		time( &now );						  // get the time
		printf( "\tshm_ts2 waiting for lock\n" );
		
    // Thing 3:  Wait until no readers, then increment num_writers
		wait_and_lock( semset_id );			  // lock memory
		printf( "\tshm_ts2 update memory\n" );
		strcpy( mem_ptr, ctime( &now ) );	  // write to mem
		sleep(5);                             // wait 5 seconds
    
		// Thing 4: Decrement num_writers
		release_lock( semset_id );
		printf( "\tshm_ts2 released lock\n" );
		sleep(1);							  // wait a second
	}
	
	// Thing 5:  delete the semaphore
	cleanup(0);
	
	return 0;
}

// Thing 5: Delete the semaphore
void cleanup( int n )
{
	shmctl( seg_id, IPC_RMID, NULL );		  // remove shared memory
	semctl( semset_id, 0, IPC_RMID, NULL );	  // remove semaphore set
}

// Thing 2:  initialize a semaphore
void set_sem_value( int semset_id, int semnum, int val )
{
	union semun initval;
	
	initval.val = val;
	
	if ( semctl( semset_id, semnum, SETVAL, initval ) == -1 )
		oops( "semctl", 4 );
}

// Thing 3:  build and execute a 2-element action set:
//     wait for 0 on n_readers AND increment n_writers
void wait_and_lock( int semset_id )
{
	struct sembuf actions[2];				  // action set, an array
	
	actions[0].sem_num = 0;					  // sem[0] is n_readers
	actions[0].sem_flg = SEM_UNDO;			  // auto cleanup
	actions[0].sem_op = 0;				      // wait til no readers
	actions[1].sem_num = 1;                   // sem[1] is n_writers
	actions[1].sem_flg = SEM_UNDO;		      // auto cleanup
	actions[1].sem_op = 1;					  // increment number of writers
	
	if ( semop( semset_id, actions, 2 ) == -1 )
		oops("semop: locking", 10 );
}

// Thing 4:  build and execute a 1-element action set: decrement num_writers
void release_lock( int semset_id )
{
	struct sembuf actions[1];				  // action set, an array
	actions[0].sem_num = 1;					  // sem[0] is n_writers
	actions[0].sem_flg = SEM_UNDO;			  // auto cleanup
	actions[0].sem_op = -1;					  // decrement number of writer count
	
	if ( semop( semset_id, actions, 1 ) == -1 )
		oops( "semop: unlocking", 10 );
}
