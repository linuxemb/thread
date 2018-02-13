
#include <pthread.h>
#include <semaphore.h>
#include "doctor.h"
#include <strings.h>
#include <unistd.h>
typedef struct patient_config patient_config;
typedef struct patient patient;

struct patient_config
{
   char arrivalTime[9];
   char serviceTime[6];
   char payment[7];
   char type[6];
};

struct patient
{
   doctoroffice* doc;

   patient_config config;
   pthread_t thread;
   int id;
   int arrivalTime;
   sem_t leave_b_chair;
   sem_t receipt;
   sem_t finished;
   int stillWaiting;
//=========

#if 0
// state var for doctor  , init  to READY and NOT finished 
bool is_sleeping; // = false;
bool is_treating ; // = false ;
bool finished  ;//  = false ;// All patients taken care of (Changed in main)

int served; // = 0;
//// Semaphores for read/write access
sem_t rw_chairs   ;
sem_t rw_sleeping ; // protect access var "is_sleeping"
sem_t rw_treating  ; // protect access var "is_treating"

// Semaphore for state var

sem_t doctor_ready ; // Doctor is ready  
sem_t sleeping     ; // Doctor is sleeping
sem_t treating      ; // Doctor treating a patient

sem_t queue  ; //  Queue has free space

#endif







};

/* modify (and complete) the following functions */
void *patient_thread( void* arg );
int LoadCustomerData( char* patientDataFileName, patient patients[], int numCustomers );
void PrintCustomerData( patient* c );

