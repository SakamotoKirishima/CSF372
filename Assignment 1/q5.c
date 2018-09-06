#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]){

	int pfd[2];		//Child to Parent

	//Create the pipe
	pipe(pfd);

	int pid = fork();
	if(pid==0){
		//Child Process

		close(pfd[0]);			//Close read of pipe
		dup2(pfd[1],1);			//Duplicate stdout to pipe write/

		char * args[] = {"grep","-n",argv[1],argv[2],NULL};			//Arguments for executing grep
		execvp(args[0],args);										//Execute grep
	}
	else{
		//Parent Process
		close(pfd[1]);			//Close write to pipe

		int flag = 0;			//Flag set to 0 when new line starts, and to 1 when encounters ':' (essentially cuts the line numbers out)
		char buff[1];			//Buffer to read next character
		while(read(pfd[0],buff,1) > 0){
			if(buff[0]==':') flag = 1;
			else if(buff[0]=='\n') flag = 0;

			if(flag==0) printf("%c",buff[0]);		//Print buffer if flag is 0
		}
		exit(0);
	}
}