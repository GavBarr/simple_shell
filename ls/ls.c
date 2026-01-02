#include <dirent.h>   // opendir, readdir, closedir
#include <sys/stat.h> // stat
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <pwd.h>


void builtin_ls(char *args[]){
//	if(args[0] == NULL){
		const char *cwd = getcwd(NULL,0);
		DIR *directory = opendir(cwd);
		char fullpath[PATH_MAX];
			
		if(directory != NULL){
			struct dirent *current_entry;
			while((current_entry = readdir(directory)) != NULL){
				if(current_entry->d_name[0] != '.'){
					if(args[0] != NULL){
						if(strcmp(args[0],"-l") == 0){
							struct stat buf;
							char output[PATH_MAX];
							int result = snprintf(output, sizeof(output),"%s/%s",cwd,current_entry->d_name);	
					
							int status;
							if((status = stat(output,&buf)) == 0){
								//Displaying Conversion from bytes to kB
								printf("perms: %1o",buf.st_mode & 0777);
								printf("%10ld ",buf.st_size);
								
								struct passwd *pw = getpwuid(buf.st_uid);

								printf("%-20s",pw->pw_name);
								
								time_t mod_time = buf.st_mtime;
								struct tm *timeinfo = localtime(&mod_time);
								char timestring[64];
								strftime(timestring,sizeof(timestring),"%Y-%m-%d %H:%M:%S", timeinfo);

								printf("%-20sd",timestring);
								printf("%-10s\n",current_entry->d_name);
							}

						}	
					
					}else{
						printf("%s\n",current_entry->d_name);
					}
				}	

			}
	
		}else{
		
			perror("opendir");

		}
	
		closedir(directory);
//	}

	return;

}
