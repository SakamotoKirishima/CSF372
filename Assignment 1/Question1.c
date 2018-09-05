#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<unistd.h>
#include<sys/wait.h>
int n=5;
char *commands[]= {"cd", "echo", "ls", "wc","quit"};
int start_process(char **tokens){
	pid_t process_id= fork();
	pid_t wpid;
	int status=0;
	if (process_id==0) {
		signal(SIGINT, SIG_IGN);
		//printf("%s\n", "Child process");
		int check= execvp(tokens[0], tokens);
		if (check==-1) perror("Error in the child process\n");


	}
	else if (process_id<0) perror("Error forking\n");
	else{
		signal(SIGINT, SIG_IGN);
		while ((wpid= wait(&status))>0);
		//printf("%s\n", "Parent process");
	} 
	return 1;
}
int cd_command(char **tokens){
	printf("%s\n", "cd_command" );
	return 1;

}
int echo_command(char **tokens){
	printf("%s\n", "echo_command" );
	return 1;
}
int ls_command(char **tokens){
	printf("%s\n", "ls_command" );
	return 1;

}
int wc_command(char **tokens){
	printf("%s\n", "wc_command" );
	return 1;
}
int quit_command(){
	printf("%s\n", "quit_command" );
	return 1;
}
int execute_command(char **tokens, int command_index){
	switch (command_index){
		case 0: return cd_command(tokens);
		case 1: return echo_command(tokens);
		case 2: return ls_command(tokens);
		case 3: return wc_command(tokens);
		case 4: return quit_command();
	}
	return 0;
}
int find_command(char **tokens){
		if (tokens[0]==NULL) return 0;
		int i,check;
		check=0;
		for (int i = 0; i < n; i++)
		{
			if (strcmp(tokens[0],commands[i])==0){
				check=1;
				return execute_command(tokens,i);
			} 

		}
		if (!check) {
			fprintf(stderr, "%s\n", "Could not find command");
			return 0;
		}
		printf("%s\n", "END" );
		return start_process(tokens);
}
void get_token_error(char **tokens){
	if (!tokens){
		fprintf(stderr, "%s\n", "Get tokens error");
		exit(EXIT_FAILURE);
	}
}
char **get_tokens(char *line){
	char **tokens=NULL;
	char *token;
	unsigned int len_max=1;
	unsigned int len;
	unsigned int i=0;
	len=len_max;
	char *delimiters="\t\v\r\n ";
	tokens = malloc(len_max * sizeof(char*));
	get_token_error(tokens);
	token= strtok(line,delimiters);
	while(token !=NULL){
		//printf("%s\n", token);
		tokens[i++]= token;
		if (i>=len){
			len= i+len_max;
			tokens= realloc(tokens, len * sizeof(char*));
			get_token_error(tokens);

		}
		token= strtok(NULL, delimiters);
	}
	tokens[i]= NULL;
	return tokens;

}
char *read_line(){
	char *line=NULL;
	char ch;
	unsigned int len_max=1;
	unsigned int len;
	unsigned int i=0;
	line= malloc(len_max * sizeof(char));
	len= len_max;
	while ((ch= getchar())!= '\n' && ch != EOF){
		line[i++]=(char) ch;
		if (i>=len){
			len= i+len_max;
			line = realloc(line,len * sizeof(char));
			if (!line){
				fprintf(stderr, "Read line error\n");
				exit(EXIT_FAILURE);

			}
		}
	}
	line[i]='\0';
	return realloc(line, sizeof(char)*len);
}
int main(){
	signal(SIGABRT, SIG_IGN);
	signal(SIGINT, SIG_IGN);
	int cont;
	printf("%s", ">>>" );
	char *readline;
	char **tokens=NULL;
	readline= read_line();
	//printf("%s\n", readline);
	tokens= get_tokens(readline);
	/*char *token;
	int i=0;
	token= tokens[i++];
	while (token != NULL){
		printf("%s\n",token );
		token= tokens[i++];
	}*/
	cont= find_command(tokens);
	printf("cont=%d\n", cont);
	free(readline);
	free(tokens);
	return 0;

}

