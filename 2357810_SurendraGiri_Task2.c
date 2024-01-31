// Assignment Task 2
// Name : Surendra Giri
// Student Id : 2357810

#include <stdio.h>  //this library is used for the input & output operations 
#include <pthread.h> //this library allows used for the multithreading 
#include <stdlib.h> //this library is used for the dynamic  memory allocation 
#include <math.h>  //this library is used to do different mathmatical operations like pow,sqrt,cbrt etc
#include <unistd.h> //this library is used for the sleep functions


//creating the global Variables
double initial_value_of_pi = 0.0;
double final_value_of_pi = 0.0;  //actual value of PI
pthread_mutex_t mutex;  //creating the mutex variable for the synchronize thread operations

//typedef function is used for creating user-defined datatype
// here "helperVar" is user-defined datatype for the stucture that contains start,end and sum
typedef struct {
    int start;
    int end;
    double sum;
} helperVar;

//creating void pointer function for the calculating the initial value of PI
void *PI_calculations(void *p){
	helperVar *help = (helperVar *)p;
	double sum = 0.0;
	
	// locking the mutex which helps in preventing from thread races
	pthread_mutex_lock(&mutex);
	int i=0;
	//starting the loop from the starting point to the ending point 
	// in every iterations , it performm leibniz series 
	for (i=help->start;i<help->end;i++){
		double upperPart = pow(-1,i);
		double lowerPart = ((2*i)+1);
		
		//updating the initial value of PI
		initial_value_of_pi = initial_value_of_pi + (upperPart/lowerPart);	
	}
	//unlocking the  Mutex
	pthread_mutex_unlock(&mutex);
	//exit through the thread
	pthread_exit(0);
}

//creating the main function
int main(){
	int Number_of_threads, Number_of_iterations;
	
	//taking user input for the Number of threads
	printf("Enter the Number of Thread: ");
	scanf("%d",&Number_of_threads);
	
	//taking user input for the iterations
	printf("Enter the Number of iterations: ");
	scanf("%d",&Number_of_iterations);
	
	//creating the slices variable which is resposible for dividing the number of iteration among the threads
	int slices[Number_of_threads];
	
	//creating an array of threads
	pthread_t thread[Number_of_threads];
	
	//creating an array of arguments of each thread
	
	helperVar arguments[Number_of_threads];  //creating the Arguments array to pass arguments to each thread
	
	int iterations_Per_Threads = Number_of_iterations/Number_of_threads;
	int extra_iterations = Number_of_iterations%Number_of_threads;
	
	//initializing the mutex for the synchronized operation of the threads
	pthread_mutex_init(&mutex, NULL);
	
	
	int i=0;
	for (i=0;i<Number_of_threads;i++){
		slices[i] = iterations_Per_Threads;
	}
	for (i=0;i<extra_iterations;i++){
		slices[i] +=1;
	}
	
	// Now calculating the start and end for each thread
	for (i=0;i<Number_of_threads;i++){
		if(i==0){
			arguments[i].start = 0;
		}else{
			arguments[i].start = arguments[i-1].end +1;
		}
		arguments[i].end = arguments[i].start + slices[i]-1;
	}
	
	//creating a threads
	for (i=0;i<Number_of_threads;i++){
		pthread_create(&thread[i],NULL,PI_calculations,&arguments[i]);
	}
	
	//using join method
	for (i=0;i<Number_of_threads;i++){
		pthread_join(thread[i],NULL);
	}
	
	//destroying the Mutex
	pthread_mutex_destroy(&mutex);
	
	//calculating the actual value of PI
	final_value_of_pi = initial_value_of_pi * 4;
	
	//displaying the actual value of PI
	printf("The value of PI is: %lf",final_value_of_pi);
	return 0;
}
