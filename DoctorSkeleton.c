#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#define N 10

#define MAX_SLEEP 5
//TODO Define global data structures to be used

unsigned int total_patient =10;
unsigned int num_doctor =3;
unsigned int queue_size = 5; 
unsigned int v[N];
unsigned treat_time;
int status; 
bool finished;
unsigned num_patient_done;
pthread_mutex_t lock;

pthread_cond_t cond_doctor, cond_patient;

/**
 * This function is used to represent treating your patients
 take varible time passed by patent thread*/
void treat_patient(void) {
    //TODO define treat_patient for your use including any parameters needed
    // start timer_treat 
    sleep(treat_time);
    pthread_cond_broadcast (&cond_doctor) ; // wake up waiting patient

}

void take_nap(void) {
    printf("takenap..\n");
    sleep(5);
    pthread_cond_wait (&cond_patient, NULL);
    pthread_cond_signal ( &cond_doctor); 
}
/**
 * This thread is responsible for getting patients from the waiting room 
 * to treat and sleep when there are no patients.
 */
void *doctor_thread(void *arg) {

    printf("doctor_thread..\n");
    printf("in function \n", __func__); 

    //TODO: Define set-up required 
    // if patient_queue is not empty---> create a tread_patient thread start timer 
    //  else --->to sleep mode

    pthread_t tid_sleep, tid_work;  
    for (int i=0; i< num_doctor ; i++)
    {
        if (qempty())

        {
            pthread_create (&tid_sleep, &sleep , NULL, NULL);
        }
        else
        {
            treat_time = dequeue(v);
            pthread_create( &tid_work, &treat_patient, NULL, &treat_time); // pass patient_treat time to work thread 
            total_patient = total_patient - num_doctor;
        }
    }


    // Verify if fished all patients
    if  ( !total_patient) {
        finished = true;
    }

    while(!finished) {			//exit your doctor threads when no more patients coming (amount specified on cammond line)
        //TODO: Define doctor behaviour


    }
    return NULL;
}

void create_patient()
{
    printf("in function \n", __func__); 
        sleep(rand()% 5) ;  
        // create patient  init the treat time to be rand betwwen 4-8
        int treat_time = 4 + rand() % 4;

        enqueue(v, treat_time);

        total_patient --;
        printf("total patient \n",total_patient );
}

/**
 * This thread is responsible for acting as a patient, waking up doctors, waiting for doctors 
 * and be treated.
 */
void *patient_thread(void *arg) {
    printf("in patient_thread \n");

    //TODO: Define set-up required
    //each 1-5 s produce one patient

    while(total_patient)  {

        create_patient();
        // start wait for doctor 
        pthread_cond_wait( &cond_doctor, &lock);
        // now we have doctor

        dequeue(v);
        // wake up the sleep thread
        pthread_cond_signal(&cond_patient);

    }

        return NULL;
}

int main(int argc, char **argv) {
    printf ("start...\n");
    struct timespec t_patient, t_treat;

    bool finished = false;
    //TODO: Define set-up required
    pthread_t thread_p,thread_d;

    // each 3-5s produce a patient push it to queue_p;
    //


    if(argc != 4){
        printf("Usage: DoctorsOffice <waitingSize> <patients> <doctors>\n");
        exit(0);
    }


    //TODO: store commandline options to be used



    //TODO: Start Doctor Threads
    pthread_create(&thread_d, NULL, (void*) doctor_thread,&v );    //
    //

    //TODO: Start Patient Threads

    pthread_create(&thread_p, NULL, (void*) patient_thread,&v);    //
    //TODO: Clean up
    //


    pthread_join(&thread_d,NULL);
    pthread_join(&thread_p, NULL);
    pthread_exit(0);
    //     return 0;	
}
