#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_SLEEP 5

//TODO Define global data structures to be used

/**
* This function is used to represent treating your patients
*/
void treat_patient(void) {
	//TODO define treat_patient for your use including any parameters needed
}


/**
 * This thread is responsible for getting patients from the waiting room 
 * to treat and sleep when there are no patients.
 */
void *doctor_thread(void *arg) {

	//TODO: Define set-up required
	
	while(!finished) {			//exit your doctor threads when no more patients coming (amount specified on cammond line)
		//TODO: Define doctor behaviour 
	}
	return NULL;
}

/**
 * This thread is responsible for acting as a patient, waking up doctors, waiting for doctors 
 * and be treated.
 */
void *patient_thread(void *arg) {

	//TODO: Define set-up required
	
	//TODO: Define Patient behaviour

	return NULL;
}

int main(int argc, char **argv) {

	//TODO: Define set-up required

	if(argc != 4){
		printf("Usage: DoctorsOffice <waitingSize> <patients> <doctors>\n");
		exit(0);
	}

	//TODO: store commandline options to be used

	//TODO: Start Doctor Threads
	
	//TODO: Start Patient Threads

	//TODO: Clean up

	return 0;	
}