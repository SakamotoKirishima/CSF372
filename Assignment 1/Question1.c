#include <stdio.h>
#include <stdlib.h>
char *read_line(){
	char *line=NULL;
	char ch;
	unsigned int len_max=1;
	unsigned int len;
	unsigned int i=0;
	line= malloc(len_max);
	len= len_max;
	while ((ch= getchar())!= '\n' && ch != EOF){
		line[i++]=(char) ch;
		if (i==len){
			len= i+len_max;
			line = realloc(line,len);
			if (!line){
				fprintf(stderr, "ERROR\n");
				exit(EXIT_FAILURE);

			}
		}
	}
	line[i]='\0';
	return realloc(line, sizeof(char)*len);
}
int main(){
	printf("%s", ">>>" );
	char *readline;
	readline= read_line();
	printf("%s\n", readline);
	free(readline);
	return 0;

}

