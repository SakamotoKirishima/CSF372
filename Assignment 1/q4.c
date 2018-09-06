#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

int main(){

	//Input variables
	int n,k,r;
	printf("Enter n: ");
	scanf("%d",&n);
	printf("Enter k: ");
	scanf("%d",&k);

	char K[10];					//Store k as string to pass to exec 	//TODO: Figure out why the array size below 10 gives error
	sprintf(K,"%d",k+1);

	printf("Enter r: ");
	scanf("%d",&r);

	//Pipe that passes pid of process to kill to child process
	int pidPipe[2];
	pipe(pidPipe);

	int pid = fork();

	if(pid==0){
		//Child Process
		close(pidPipe[1]);		//Close write to pipe

		while(1){
			int a = r;			//Stores iterations remaining before getting input to kill
			while(a>0){
				//Separator for each iteration
				printf("-------------------------------------------------------------------------------------------------------------------------------------\n");
				a--;
				int child = fork();
				if(child==0){
					//Child process (prints data)
					int headPipe[2];		//Passes data from ps to head
					pipe(headPipe);
					int head = fork();

					if(head==0){
						//Child process (prints top k lines) 		//This is the head process being called
						close(headPipe[1]);
						dup2(headPipe[0],0);						//Stdin from pipe (coming from ps stdout)

						
						char * args[] = {"head","-n",K};			
						execvp(args[0],args);	
					}
					else{
						//Parent process (runs ps command and gets data)		//This is the ps process being called
						close(headPipe[0]);
						dup2(headPipe[1],1);									//Stdout to pipe
						char * args[] = {"ps","ax","o","\045cpu,command","--sort","-\045cpu",NULL};
						execvp(args[0],args);
					}

				}
				else{
					//Parent process 
					sleep(n);
				}
			}
			int id;
			char message[] = "\nEnter pid of process to kill (or -1 to ignore): ";
			write(1,message,sizeof(message));
			read(pidPipe[0],&id,sizeof(id));			//Read pid from pipe
			if(id!=-1) kill(id,SIGINT);					//Kill process unless -1
		}
	}
	else{
		//Parent Process
		close(pidPipe[0]);
		int id;
		while(1){									//Loop read pid to kill until Ctrl+C
			scanf("%d",&id);
			write(pidPipe[1],&id,sizeof(id));		//Write PID to pipe
		}
	}
}