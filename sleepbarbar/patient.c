#include <semaphore.h>
#include "fifo.h"
#include <stdbool.h>
#include "patient.h"
/*This thread represent behavior of patients */
#define N 4
void *patient_thread(void *arg) {
    
    patient * p = (patient*) arg;
// init



    sem_init(&p->doc->rw_chairs, 0, 1);

    sem_init( &p->doc->rw_sleeping, 0, 1);
    sem_init( &p->doc->rw_treating, 0, 1);

    sem_init( &p->doc->doctor_ready, 0, 0);
    sem_init( &p->doc->sleeping, 0, 0);
    sem_init( &p->doc->treating, 0, 0);

    sem_init (&p->doc->queue,0,0);

    bool helped = false;
    while (!helped) {
        sem_wait(&p->doc-> rw_chairs);
        sem_wait(&p->doc-> rw_treating);
        if ((p->doc->chairs == N)  && !p->doc->is_treating) // If the doctor is free
        {
            sem_wait(&p->doc->rw_sleeping);
            if (p->doc->is_sleeping) 
            {
            sem_post(&p->doc->sleeping); // Wake the doctor up
           p-> doc-> is_sleeping = false;
            printf("patient has woken the doctor up\n");
            sem_post(&p->doc->rw_sleeping);
            p->doc->is_treating = true;
            sem_post(&p->doc->rw_chairs);
            sem_post(&p->doc->rw_treating);
            sem_wait(&p->doc->doctor_ready) ;// Wait for the doctor to be ready
            sem_wait(&p->doc->treating)     ; // Wait for him to finish &treating 
            printf("patient has been cared by doctor\n");
           helped = true;
            p->doc->served ++;

        printf("Served patient = %d,\n", p->doc->served);
            }
        }
        else if (p->doc->chairs > 0) // has custom but doctor is busy

        {
           p->doc-> chairs -= 1;   // one more chair been free
            // NEED enqueue another patient thread by main thread
           // sem_post(&queue);
            
           
            printf("patient has been cared by doctor\n");
            sem_post(&p->doc->rw_chairs);
            sem_post(&p->doc->rw_treating);
            sem_wait(&p->doc->doctor_ready);
            sem_wait(&p->doc->treating);
            printf("patient has been cared by doctor\n");
            p->doc->served ++;

            printf("Served patient = %d,\n", p->doc->served);
        int treat_time = 4 + rand() % 4;
            sleep(treat_time);
        helped = true;
           sem_post(&p->doc->treating);
           sem_post(&p->doc->doctor_ready);
  
        }
        else
        {
            sem_post(&p->doc->rw_chairs);
            sem_post(&p->doc->rw_treating);
            printf("All chairs taken, will return later\n");
            // Wait a random amount of time 
            sleep(3);
            printf("patient is leaving the doctor office\n");

        }
    }
}



#if 0
void *Customer( void* arg )
{
   customer* c = (customer*)arg;
   c->stillWaiting = 1;
   c->arrivalTime = GetSimTimeStampSec(c->bs->simStartTime, c->bs->realStartTime);

   char *stamp = malloc(sizeof(char)*15);

    TimeStampSec2Str(stamp, GetSimTimeStampSec(c->bs->simStartTime, c->bs->realStartTime));

    sem_init(&c->leave_b_chair, 0, 0);
    sem_init(&c->finished, 0, 0);
    sem_init(&c->receipt, 0, 0);

    int temp = 0;
    sem_getvalue(&c->bs->max_capacity, &temp);

    //Wait for there to be room
   sem_wait(&c->bs->max_capacity);

   //Kill self if there's no more time
    if(GetSimTimeStampSec(c->bs->simStartTime, c->bs->realStartTime) > (3600*17)){
            TimeStampSec2Str(stamp, GetSimTimeStampSec(c->bs->simStartTime, c->bs->realStartTime));
            printf("<%s>Customer %d is turned away.\n", stamp, c->id);
            fprintf(c->bs->log,"<%s>Customer %d is turned away.\n", stamp, c->id);
           pthread_exit(NULL);
       }


   int curCount = -1;

   TimeStampSec2Str(stamp, GetSimTimeStampSec(c->bs->simStartTime, c->bs->realStartTime));
   printf("<%s>Customer %d arrives to the barber shop\n", stamp, c->id);
   fprintf(c->bs->log,"<%s>Customer %d arrives to the barber shop\n", stamp, c->id);


    //Enter the waiting room
    //Make sure no one else can get in while we can
    //And only go in if we are the next in line
    do{
        curCount = c->bs->count;
    }while(curCount != c->id);

    sem_wait(&c->bs->mutex1);
    c->bs->count++;
    sem_post(&c->bs->mutex1);

    curCount = -1;

    c->stillWaiting = 0;
    TimeStampSec2Str(stamp, GetSimTimeStampSec(c->bs->simStartTime, c->bs->realStartTime));
     printf("<%s>Customer %d enters the waiting room\n", stamp, c->id);
     fprintf(c->bs->log,"<%s>Customer %d enters the waiting room\n", stamp, c->id);
     fflush(stdout);

    //Wait for there to be room on the sofa and
    //for it to be our turn
    do{
        curCount = c->bs->sofacount;
    }while(curCount != c->id);
    sem_wait(&c->bs->sofa);
    c->bs->sofacount++;

    curCount = -1;

    TimeStampSec2Str(stamp, GetSimTimeStampSec(c->bs->simStartTime, c->bs->realStartTime));
    printf("<%s>Customer %d sits on sofa\n", stamp, c->id);
    fprintf(c->bs->log,"<%s>Customer %d sits on sofa\n", stamp, c->id);
    fflush(stdout);

    //Once we're on the sofa, wait for the barber chair to open up
    //and for it to be our turn
    do{
        curCount = c->bs->barbcount;
    }while(curCount != c->id);
    sem_wait(&c->bs->barber_chair);
    c->bs->barbcount++;
    sem_post(&c->bs->sofa);

    TimeStampSec2Str(stamp, GetSimTimeStampSec(c->bs->simStartTime, c->bs->realStartTime));
    printf("<%s>Customer %d goes on barber chair\n", stamp, c->id);
    fprintf(c->bs->log,"<%s>Customer %d goes on barber chair\n", stamp, c->id);

    //Wait in barber chair and add to the queue
    sem_wait(&c->bs->mutex2);
    Enqueue(&c->bs->queue1, c->id);
    sem_post(&c->bs->cust_ready);
    sem_post(&c->bs->mutex2);

    //Finish haircut
    sem_wait(&c->finished);
    sem_post(&c->leave_b_chair);

    TimeStampSec2Str(stamp, GetSimTimeStampSec(c->bs->simStartTime, c->bs->realStartTime));
    printf("<%s>Customer %d leaves barber chair\n", stamp, c->id);
    fprintf(c->bs->log,"<%s>Customer %d leaves barber chair\n", stamp, c->id);

    //Wait to pay
    sem_wait(&c->bs->mutex3);
    Enqueue(&c->bs->queue2, c->id);
    sem_post(&c->bs->payment);
    sem_post(&c->bs->mutex3);

    //Get receipt
    sem_wait(&c->receipt);

    TimeStampSec2Str(stamp, GetSimTimeStampSec(c->bs->simStartTime, c->bs->realStartTime));
     printf("<%s>Customer %d receives receipt\n", stamp, c->id);
     fprintf(c->bs->log,"<%s>Customer %d receives receipt\n", stamp, c->id);
     fflush(stdout);

    //Leave
    sem_post(&c->bs->max_capacity);
    TimeStampSec2Str(stamp, GetSimTimeStampSec(c->bs->simStartTime, c->bs->realStartTime));
     printf("<%s>Customer %d leaves the barber shop.\n", stamp, c->id);
    fprintf(c->bs->log,"<%s>Customer %d leaves the barber shop.\n", stamp, c->id);
     fflush(stdout);


    c->arrivalTime = GetSimTimeStampSec(c->bs->simStartTime, c->bs->realStartTime) - c->arrivalTime;

   pthread_exit(NULL);
}

/* Load customer data file and store contents in an array of customer structs. */
/* 'LoadCustomerData' returns -1 if there are complications during the load; returns 1 on success. */
int LoadCustomerData( char* customerDataFileName, customer customers[], int numCustomers )
{
	FILE *customerData;
	customerData = fopen(customerDataFileName, "rt");
	int i = 0;
	char * line = malloc(sizeof(char)*80);
    int count;
	while(fgets(line, 80, customerData) != NULL && i<= numCustomers)
	{
	    i++;
	    count++;
		char *tokens;
		tokens = strtok(line, " ");
        char *a = malloc(sizeof(char)*25);
        strcpy(a, tokens);
        tokens = strtok(NULL, " ");
        char *b = malloc(sizeof(char)*25);
        strcpy(b, tokens);
        tokens = strtok(NULL, " ");
        char *c = malloc(sizeof(char) * 25);
        strcpy(c, tokens);
        tokens = strtok(NULL, " ");
       char *d = malloc(sizeof(char) * 25);
        strcpy(d, tokens);
		struct customer_config newCust;
		strncpy(newCust.arrivalTime, a, 9);
		strncpy(newCust.serviceTime, b, 6);
		strncpy(newCust.payment, c, 7);
		strncpy(newCust.type, d, 6);
		customers[i].config = newCust;

		i++;

	}

   return 1;
}

/* Print customer data. */

#endif




