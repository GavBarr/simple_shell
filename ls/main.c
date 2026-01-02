#include "ls.h"
#include <stdio.h>


int main(void){
	char *argv[]={"-l"};
	builtin_ls(argv);
	return 0;
}
