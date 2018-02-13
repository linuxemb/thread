
#include <semaphore.h>
#include "fifo.h"
#include <time.h>
#include <stdio.h>

typedef struct doctoroffice doctoroffice;

struct doctoroffice
{
    sem_t max_capacity;
    sem_t sofa;
    sem_t chair;
    sem_t mutex1;
    sem_t mutex2;
    sem_t mutex3;
    sem_t cust_ready;

    //Added to ensure fair lineups while waiting in line
    int count;
    int sofacount;
    int doctorcount;


    fifo queue;

    //Added for easier event logging
    FILE *log;

int chairs    ;  //  = N ;// available chairs  


// state var for doctor  , init  to READY and NOT finished 
bool is_sleeping; // = false;
bool is_treating ; // = false ;
bool finished   ;// = false ;// All patients taken care of (Changed in main)

int served ; //= 0;
//// Semaphores for read/write access
sem_t rw_chairs   ;
sem_t rw_sleeping ; // protect access var "is_sleeping"
sem_t rw_treating  ; // protect access var "is_treating"

// Semaphore for state var

sem_t doctor_ready ; // Doctor is ready  
sem_t sleeping     ; // Doctor is sleeping
sem_t treating      ; // Doctor treating a patient

//sem_t queue  ; //  Queue has free space







};

/* modify (and complete) the following functions */
void *doctor_thread( void* arg );

