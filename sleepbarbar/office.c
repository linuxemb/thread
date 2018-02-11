#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#define N  4

int chairs       = N ;// available chairs
bool is_sleeping = false;
bool is_cutting  = false ;
bool finished    = false ;// All customers taken care of (Changed in main)

//// Semaphores for read/write access
sem_t rw_chairs   ;
sem_t rw_sleeping ;
sem_t rw_cutting  ;

// Semaphore for state var

sem_t barber_ready ; // Barber ready to cut hair
sem_t sleeping     ; // Barber is sleeping
sem_t cutting      ; // Barber cutting a customer's hair

Barber() {
    while (!finished) {
        sem_wait(&rw_chairs);
        sem_wait(&rw_sleeping);
        //    // If no chairs are being used
        if (chairs == N) {
            is_sleeping = true;
            printf("Barber is sleeping!\n");
            sem_post(&rw_chairs);     // Allow others to read/write
            sem_post(&rw_sleeping);


            sem_wait(&sleeping)      ;  // Wait for Customer to wake him up
            sem_post(&barber_ready) ; // Allow customers into the chair
            printf("Barber woke up!\n");
        }
        else 

        {
            sem_post(&rw_sleeping);
            chairs += 1;
            sem_post(&barber_ready);
            sem_post(&rw_chairs);
            //  // If the barber isn't done for the day, help the customer

            if(!finished) {
                printf("Helping a customer\n");
                //   // Wait a random amount of time
                sleep(rand() % 5);
                printf("Finished helping a customer\n");
                sem_post(&cutting) ;// Let the customer leave after the hair cut
            }
            else
                printf("Barber is done for the day!\n");
        }
    }      
}
Customer() {
    bool helped = false;
    while (!helped) {
        sem_wait(&rw_chairs);
        sem_wait(&rw_cutting);
        if ((chairs == N)  && !is_cutting) // If the barber is free
        {
            sem_wait(&rw_sleeping);
            if (is_sleeping) 
            {
            sem_post(&sleeping); // Wake the barber up
            is_sleeping = false;
            printf("Customer has woken the barber up\n");
            sem_post(&rw_sleeping);
            is_cutting = true;
            sem_post(&rw_chairs);
            sem_post(&rw_cutting);
            sem_wait(&barber_ready) ;// Wait for the barber to be ready
            sem_wait(&cutting)     ; // Wait for him to finish &cutting hair
            printf("Customer has had his hair cut\n");
            helped = true;
            }
        }
        else if (chairs > 0) // has custom but barber is busy

        {
            chairs -= 1;
            printf("Customer is sem_waiting in a chair\n");
            sem_post(&rw_chairs);
            sem_post(&rw_cutting);
            sem_wait(&barber_ready);
            sem_wait(&cutting);
            printf("Customer has had his hair cut\n"); // been wakeup after random 5 s 
            sleep(5);
            helped = true;
          // -------------------?? after helped RW , need post
           sem_post(&cutting);
           sem_post(&barber_ready);
  
        }
        else
        {
            sem_post(&rw_chairs);
            sem_post(&rw_cutting);
            printf("All chairs taken, will return later\n");
            // Wait a random amount of time 
            sleep(3);
            printf("Customer is leaving the barbershop\n");

        }
    }
}

int main()

{
    sem_init(&rw_chairs, 0, 1);

    sem_init( &rw_sleeping, 0, 1);
    sem_init( &rw_cutting, 0, 1);

    sem_init( &barber_ready, 0, 0);
    sem_init( &sleeping, 0, 0);
    sem_init( &cutting, 0, 0);


    pthread_t tb[3], tc[5];
 for (int i=0; i<3; i++)

 {
     pthread_create(&tb[i], NULL, Barber,NULL);
 
 }

 for (int j=0; j<10; j++)
 {
     pthread_create(&tc[j], NULL, Customer,NULL);
 }   
  sleep(3);
     pthread_join(tb, NULL);
    pthread_join(tc, NULL);

    sem_destroy(&rw_chairs);
    sem_destroy(&rw_sleeping);
    sem_destroy(&rw_cutting);
    sem_destroy(&barber_ready);
    pthread_exit(0);
    //sem_destroy(&rm_barbar_ready);
    //sem_destroy(&rm_barbar_ready);
}

