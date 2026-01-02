#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>

#define RESET   "\x1b[0m"
#define MAGENTA "\x1b[35m"
#define BOLD    "\x1b[1m"
#define BLUE    "\033[34m"
#define UP_ARROW    "\x1b[A"
#define DOWN_ARROW    "\x1b[B"
#define LEFT_ARROW    "\x1b[D"
#define RIGHT_ARROW    "\x1b[C"


char *get_input();
void clear_terminal_screen();
void display_tokens(char *tokens[]);
void display_line(void);
void execute_args(char *token[]);
void tokenize_input(char *input, char *tokens[], int max_tokens);

int main(){
    clear_terminal_screen();
    while(1){
        display_line();
        char *input = get_input();
        
        if(input == NULL){
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        char *tokens[64];  // Array to hold up to 64 token pointers
        tokenize_input(input, tokens, 64);
        //display_tokens(tokens);
	execute_args(tokens);
        
        // Free tokens after use
        for(int i = 0; tokens[i] != NULL; i++){
            free(tokens[i]);
        }
        free(input);
    }
    return 0;
}

void display_tokens(char *tokens[]){
	for (int i = 0; tokens[i] != NULL; i++){
		printf("%s\n",tokens[i]);
	}
}

void execute_args(char *tokens[]){
    if(tokens[0] == NULL) return;  // Empty command
    
    if(strcmp(tokens[0], "cd") == 0){
        if(tokens[1] != NULL){	      
		
		if(strcmp(tokens[1],"~") == 0){
			char *home_dir = "/home";	
			if(chdir(home_dir) != 0){
				perror("cd");
				
		}
		}else if(chdir(tokens[1]) != 0){
                	perror("cd");
            	}
       	}else{
	
            fprintf(stderr, "cd: missing argument\n");
	}
	return;
    }

    if(strcmp(tokens[0],"exit") == 0){
	    exit(0);
    }
    
    pid_t pid = fork();
    if(pid == 0){
        execvp(tokens[0], tokens);
        perror("execvp");
        exit(1);
    } else {
        wait(NULL);
    }
}

void tokenize_input(char *input, char *tokens[], int max_tokens){
    int token_count = 0;
    int i = 0;
    
    while(input[i] != '\0' && token_count < max_tokens - 1){
        // Skip spaces
        while(input[i] == ' ' || input[i] == '\t'){
            i++;
        }
        if(input[i] == '\0') break;
        
        // Mark start of token
        int start = i;
        
        // Find end of token
        while(input[i] != '\0' && input[i] != ' ' && input[i] != '\t'){
            i++;
        }
        
        // Calculate length and allocate memory for this token
        int length = i - start;
        tokens[token_count] = malloc(length + 1);
        strncpy(tokens[token_count], &input[start], length);
        tokens[token_count][length] = '\0';
        
        token_count++;
    }
    tokens[token_count] = NULL;  // Null-terminate the array
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
	char *current_dir = getcwd(NULL, 0);

	if (current_dir != NULL){
		printf(BOLD MAGENTA "gavin@GOOB" RESET);
		printf(BOLD ":");
		printf(BOLD BLUE "%s$ " RESET, current_dir);
		free(current_dir);
	}else{
		perror("getcwd");
	}

}

