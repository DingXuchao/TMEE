#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include <main.h>
#include <funcOfThread.h>



struct stringS{
	char data;
	struct stringS* next;
	pthread_mutex_t mutex;
}


int main(int argc, char* argv[]){
	
		
	
	return NULL;
}

/* based on the input "N" to determine how many threads to create */
void createThreads(int N){}


