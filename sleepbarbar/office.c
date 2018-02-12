#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#define N  4

int chairs       = N ;// available chairs
bool is_sleeping = false;
bool is_treating  = false ;
bool finished    = false ;// All patients taken care of (Changed in main)

int served = 0;
//// Semaphores for read/write access
sem_t rw_chairs   ;
sem_t rw_sleeping ;
sem_t rw_treating  ;

// Semaphore for state var

sem_t doctor_ready ; // Doctor ready  
sem_t sleeping     ; // Doctor is sleeping
sem_t treating      ; // Doctor treating a patient

void *doctor_thread(void *arg) {
    while (!finished) {
 //   while (served<10) {
        sem_wait(&rw_chairs);
        sem_wait(&rw_sleeping);
        //    // If no chairs are being used
        if (chairs == N) {
            is_sleeping = true;
            printf("Doctor is sleeping!\n");
            sem_post(&rw_chairs);     // Allow others to read/write
            sem_post(&rw_sleeping);


            sem_wait(&sleeping) ;  // Wait for patient to wake him up
            sem_post(&doctor_ready) ; // Allow patients into the chair
            printf("Doctor woke up!\n");
        }
        else 

        {
            sem_post(&rw_sleeping);
            chairs += 1;
            sem_post(&doctor_ready);
            sem_post(&rw_chairs);
            //  // If the doctor isn't done for the day, help the patient

   // if(served<10) {
            if(!finished) {
                printf("Helping a patient\n");
                //   // Wait a random amount of time
                sleep(rand() % 5);
                printf("Finished helping a patient\n");
                sem_post(&treating) ; 
            }
            else
                printf("Doctor is done for the day!\n");
        }
    }      
}
void *patient_thread(void *arg) {
    bool helped = false;
    if(served==6) 
       finished=true;
    while (!helped) {
        sem_wait(&rw_chairs);
        sem_wait(&rw_treating);
        if ((chairs == N)  && !is_treating) // If the doctor is free
        {
            sem_wait(&rw_sleeping);
            if (is_sleeping) 
            {
            sem_post(&sleeping); // Wake the doctor up
            is_sleeping = false;
            printf("patient has woken the doctor up\n");
            sem_post(&rw_sleeping);
            is_treating = true;
            sem_post(&rw_chairs);
            sem_post(&rw_treating);
            sem_wait(&doctor_ready) ;// Wait for the doctor to be ready
            sem_wait(&treating)     ; // Wait for him to finish &treating 
            printf("patient has been cared by doctor\n");
            helped = true;
            served ++;

            printf("Served patient = %d,\n", served);
            }
        }
        else if (chairs > 0) // has custom but doctor is busy

        {
            chairs -= 1;
            printf("patient has been cared by doctor\n");
            sem_post(&rw_chairs);
            sem_post(&rw_treating);
            sem_wait(&doctor_ready);
            sem_wait(&treating);
            printf("patient has been cared by doctor\n");
            served ++;

            printf("Served patient = %d,\n", served);
            sleep(5);
            helped = true;
           sem_post(&treating);
           sem_post(&doctor_ready);
  
        }
        else
        {
            sem_post(&rw_chairs);
            sem_post(&rw_treating);
            printf("All chairs taken, will return later\n");
            // Wait a random amount of time 
            sleep(3);
            printf("patient is leaving the doctor office\n");

        }
    }
}

int main()

{
    sem_init(&rw_chairs, 0, 1);

    sem_init( &rw_sleeping, 0, 1);
    sem_init( &rw_treating, 0, 1);

    sem_init( &doctor_ready, 0, 0);
    sem_init( &sleeping, 0, 0);
    sem_init( &treating, 0, 0);

    pthread_t tb[3], tc[5];
 for (int i=0; i<3; i++)

 {
     pthread_create(&tb[i], NULL, doctor_thread,NULL);
 
 }

 for (int j=0; j<5; j++)
 {
     pthread_create(&tc[j], NULL, patient_thread,NULL);
 }   

 printf("IN main\n");
  sleep(15);
 //sem_wait(&sleeping);
 if (served== 10)

 {
  printf("IN main ,,,,, set finished to true\n");
  finished = true;
 }
 for (int p=0; p<3;p++)
 {
  pthread_join(tb[p], NULL);
 }


 for (int q=0; q<3;q++)
 {
     pthread_join(tc[q], NULL);
 }
    sem_destroy(&rw_chairs);
    sem_destroy(&rw_sleeping);
    sem_destroy(&rw_treating);
    sem_destroy(&doctor_ready);
    sem_destroy(&sleeping);
    sem_destroy(&treating);
    pthread_exit(0);
}
