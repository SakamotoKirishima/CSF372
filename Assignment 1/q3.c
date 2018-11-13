#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/prctl.h>
#include <signal.h>

int main(){
	int pfd[2];		//Stores pipe descriptors
	pipe(pfd);
	int pid = fork();
	if(pid==0){
		//Child Process

		close(pfd[1]);	//Close write

		int in;		//Buffer for storing data read from pipe
		int s = 0;		//Stores sum of all prime numbers processed till now
		while(1){
			read(pfd[0],&in,sizeof(int));		//Read 1 byte from pipe to buffer
			int i;					//Iterator
			int isPrime = 1;		//Prime flag

			//Check if prime (rudimentary method)
			for(i=2;i<in;i++){
				if(in%i==0){
					isPrime = 0;
					break;
				}
			}

			if(isPrime==1) s+=in;	//If prime, add to 's'
			printf("%s\n",isPrime==1?"It is prime":"It is composite");
			printf("s = %d\n\n",s);
			sleep(in%3);
		}
		exit(0);

	}
	else{
		//Parent process
		//Close read
		close(pfd[0]);		//Close read

		int array[10] = {11,12,13,14,15,16,17,18,19,20};		//Random array
		int chosen[10] = {0};	//Array to check if the number has been chosen before or not

		srand(time(NULL));	//Different random seeder, so that each run produces different results

		int a=10;	//Count of numbers never chosen
		while(a!=0){
			int r1 = rand()%10;
			int x = array[r1];
			int r2 = rand()%10;
			int y = array[r2];
			if(x!=y){
				chosen[r1] = 1;
				chosen[r2] = 1;
				printf("x = %d and y = %d\n",x,y);
				write(pfd[1],&y,sizeof(int));			//Send data to child via pipe
				sleep(x/3);
			}


			//Check how many numbers are never chosen
			a=0;
			int i;
			for(i=0;i<10;i++){
				if(chosen[i]==0) a++;
			}
		}

		kill(pid, SIGINT);	//Kill child when done with all numbers
		exit(0);
	}
}
