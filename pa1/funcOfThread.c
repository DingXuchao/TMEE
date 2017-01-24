#include <stdlib.h>
#include <unistd.h>
#include <time.h>

/* this file contains all functions required by threads*/

//func sleepThread is to let thread sleep between 100-500 ms 
float sleepThread(void){
	
	//generate a random number between 100 and 500
	srand(time(NULL));
	int i = rand()%401; //range is 0-400
	i = i + 100; //0-400 ==> 100-500

}




//lock is outside of the function write and read

//read string S and check if |S| < M * L; if true, return 1, else 0

read(char[] S){

return 1;
}

//write function is to let the current thread to append its alphabet to the tail of the string S
//the input of function write is current thread index, so that the func can append correct alphabet
void write(int i)
