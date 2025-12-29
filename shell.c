#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>

#define RESET   "\x1b[0m"
#define MAGENTA "\x1b[35m"
#define BOLD    "\x1b[1m"



char *get_input();
void clear_terminal_screen();
void display_line(void);
void tokenize_input(char *input, char token[],int token_size);

int main(){
	clear_terminal_screen();
	while(1){
	display_line();
	char *input = get_input();
	char *tokens[2];
		if(input==NULL){
			break;
		}else{
		
			input[strcspn(input, "\n")] ='\0';
		}
		
		printf("Input: %s \n",input);		
		
		char token[32];
		tokenize_input(input, token, 32);
		//create new process for the command being entered
		pid_t pid = fork();
		if(pid == 0){
			char *argv[] = {token,NULL};
			execvp(argv[0],argv);
			exit(1);
		}else{
			wait(NULL);
		}	
	}
		
	return 0;
}

void tokenize_input(char *input, char token[], int token_size){
	
		int pos = 0;
		int i = 0;

		//token[0] = '\0';
		while(input[i] != '\0'){
			while(input[i]==' '){
				i++;
			}

			if(input[i] == '\0') break;
			pos=0; //reset for each token
			while(input[i] != '\0' && input[i] != ' '){

				if (pos < token_size -1){
					token[pos] = input[i];
					pos++;
					i++;
				}else{
					break;
				}
			}
	
			token[pos] = '\0';
			//printf("Token: '%s'\n", token);
		}
		
	//	return token;
}


char *get_input(){
	
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;

	nread = getline(&line,&len,stdin);
	if(nread == -1){
		free(line);
		return NULL;
	}
	
	return line;
}

void clear_terminal_screen(){
	
	pid_t pid = fork();
	
	if (pid == 0){
		char *args[] = {"clear",NULL};
		execvp(args[0],args);
	}else{
		wait(NULL);
	}
}


void display_line(void){
	printf(BOLD MAGENTA "GOOB$ " RESET);
	

}

