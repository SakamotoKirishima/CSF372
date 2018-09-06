#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<limits.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>
int n=5;
char *commands[]= {"cd", "echo", "ls", "wc","quit"};
void SIGINTHandler(int signal){
	printf("%s\n", "Only use quit to terminate the process" );
	return;
}
int start_process(char **tokens){
	pid_t process_id= fork();
	pid_t wpid;
	int status=0;
	if (process_id==0) {
		signal(SIGINT, SIGINTHandler);
		//printf("%s\n", "Child process");
		int check= execvp(tokens[0], tokens);
		if (check==-1) perror("Error in the child process\n");


	}
	else if (process_id<0) perror("Error forking\n");
	else{
		signal(SIGINT, SIGINTHandler);
		while ((wpid= wait(&status))>0);
		//printf("%s\n", "Parent process");
	} 
	return 1;
}
char *get_full_path(char **tokens){
	char *directory;
	int len_max=1;
	directory= malloc(len_max*sizeof(char));
	int i=0;
	char *token= tokens[i++];
	token= tokens[i++];
	while (token!=NULL){
		directory= realloc(directory, (len_max+strlen(token))*sizeof(char));
		len_max+=strlen(token);
		//printf("%s\n", token );
		strcat(directory,token);
		token= tokens[i++];
	}
	directory[len_max]='\0';
	if (strlen(directory)==0) return "cwd";
	char actualpath[PATH_MAX+1];
	char *fullpathname= realpath(directory, actualpath);
	if (fullpathname==NULL) {
		fprintf(stderr, "%s\n", "Unable to find directory" );
		return NULL;
	}
}
int cd_command(char **tokens){
	char *fullpathname= get_full_path(tokens);
	if (strcmp(fullpathname,"cwd")==0) return 1;
	if (fullpathname==NULL) return 1;
	//printf("%s\n", fullpathname);
	//printf("%s\n", directory );
	int check= chdir(fullpathname);
	if (check==-1) fprintf(stderr, "%s\n", "Unable to change directory" );
	//int check= chdir(directory);
	//if (check) printf("%s\n", "FAIL" );
	//else printf("%s\n", "FAIL" );
	return 1;

}
int echo_command(char **tokens){
	int i=0;
	char *token= tokens[i++];
	token= tokens[i++];
	while (token!=NULL){
		printf("%s ", token);
		token= tokens[i++];
	}
	return 1;
}
char *cwd(){
	long size;
	char *buf, *ptr;
	size = pathconf(".",_PC_PATH_MAX);
	if ((buf=malloc((size_t)size))!=NULL) ptr= getcwd(buf, (size_t)size);
	return ptr;
}
int is_regular_file( char *path){
	struct stat path_status;
	stat(path, &path_status);
	return S_ISREG(path_status.st_mode);
}
int ls_command(char **tokens){
	char *fullpathname= get_full_path(tokens);
	if (fullpathname==NULL) return 1;
	if (strcmp(fullpathname,"cwd")==0) fullpathname= cwd();
	//printf("%s\n", fullpathname );
	//printf("%d\n", is_regular_file(fullpathname) );
	DIR *d;
	struct dirent *dir;
	d= opendir(fullpathname);
	if (d){
		while((dir= readdir(d))!=NULL){
			printf("%s\n", dir->d_name );
		}
		closedir(d);
	}
	else fprintf(stderr, "%s\n", "Couldn't open as a directory" );
	return 1;

}
int wc_command(char **tokens){
	printf("%s\n", "wc_command" );
	return 1;
}
int quit_command(){
	return 0;
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
		if (tokens[0]==NULL) return 1;
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
			return 1;
		}
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
	signal(SIGINT, SIGINTHandler);
	int cont=1;
	char *ptr= cwd();
	printf("%s", ptr );
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
	/*long size1;
	char *buf1, *ptr1;
	size1 = pathconf(".",_PC_PATH_MAX);
	if ((buf=malloc((size_t)size1))!=NULL) ptr1= getcwd(buf1, (size_t)size1);
	printf("%s", ptr1 );*/
	printf("cont=%d\n", cont);
	free(readline);
	free(tokens);
	return 0;

}

