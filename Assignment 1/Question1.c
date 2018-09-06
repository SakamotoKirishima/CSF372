#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<limits.h>
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
		int check= execvp(tokens[0], tokens);
		if (check==-1) perror("Error in the child process\n");


	}
	else if (process_id<0) perror("Error forking\n");
	else{
		signal(SIGINT, SIGINTHandler);
		while ((wpid= wait(&status))>0);
	} 
	return 1;
}
char *get_full_path(char **tokens, int ignore_index){
	char *directory;
	int len_max=1;
	directory= malloc(len_max*sizeof(char));
	int i=0;
	int j;
	char *token= tokens[i++];
	for(j=0;j<ignore_index;j++) token= tokens[i++];
	token= tokens[i++];
	while (token!=NULL){
		directory= realloc(directory, (len_max+strlen(token))*sizeof(char));
		len_max+=strlen(token);
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
	char *fullpathname= get_full_path(tokens,0);
	if (fullpathname==NULL) return 1;
	if (strcmp(fullpathname,"cwd")==0) return 1;
	int check= chdir(fullpathname);
	if (check==-1) fprintf(stderr, "%s\n", "Unable to change directory" );
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
	printf("\n");
	return 1;
}
char *cwd(){
	long size;
	char *buf, *ptr;
	size = pathconf(".",_PC_PATH_MAX);
	if ((buf=malloc((size_t)size))!=NULL) ptr= getcwd(buf, (size_t)size);
	return ptr;
}
int ls_command(char **tokens){
	char *fullpathname= get_full_path(tokens,0);
	if (fullpathname==NULL) return 1;
	if (strcmp(fullpathname,"cwd")==0) fullpathname= cwd();
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
	int i=0;
	if (tokens[1]==NULL) {
		fprintf(stderr, "%s\n", "Arguments not recognized" );
		return 1;
	}
	if (tokens[2]==NULL){
		fprintf(stderr, "%s\n", "File not recognized" );
		return 1;
	}
	char *fullpathname= get_full_path(tokens,1);
	char *option= tokens[1];
	char *check= "check";
	int check1= option[0]=='-';
	if (fullpathname==NULL){
		return 1;

	}
	for(i=1; i<strlen(option) && check!=NULL;i++){
		if (option[i]!= 'l'&& option[i]!='w'&& option[i]!= 'c'&& option) {
			check=NULL;
			break;
		}
	}
	if ((strlen(option)>4)||check==NULL|| check1==0){
		fprintf(stderr, "%s\n", "Arguments not recognized" );
		return 1;
	}
	FILE *fileptr = fopen(fullpathname,"r");
	if (fileptr==NULL){
		fprintf(stderr, "%s\n", "Coldn't open file");
		return 1;
	}
	char c;
	int flag=0;
	check= strchr(option,'l');
	if (check != NULL){
		
		int no_of_lines=0;
		for (c= getc(fileptr); c!= EOF;c=getc(fileptr)){
			if (c=='\n') no_of_lines++;
		}
		printf("%d ",no_of_lines);
		flag=1;
	}
	check= strchr(option,'w');
	fseek(fileptr,0,0);
	if (check!=NULL){
		int no_of_words=0;
		for(c=getc(fileptr); c!=EOF;c=getc(fileptr)){
			if (c=='\n'|| c==' '|| c=='.' || c==','|| c=='?'|| c== '!'){
				no_of_words++;
			} 
		}
		printf("%d ", no_of_words );
		flag=1;
	}
	check= strchr(option,'c');
	fseek(fileptr,0,0);
	if (check!=NULL){
		int no_of_bytes;
		for(no_of_bytes=0; getc(fileptr)!=EOF;++no_of_bytes);
		printf("%d", no_of_bytes );
		flag =1;
	}
	if (flag==0) {
		printf("%s\n", "Arguments not recognized" );
		return 1;
	}
	fclose(fileptr);
	printf("\n");
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
	while (cont){
		char *ptr= cwd();
		printf("%s", ptr );
		printf("%s", ">>>" );
		char *readline;
		char **tokens=NULL;
		readline= read_line();
		tokens= get_tokens(readline);
		cont= find_command(tokens);
		free(readline);
		free(tokens);

	}
	return 0;

}

