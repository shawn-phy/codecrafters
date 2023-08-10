#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>


void mk_dir(char *temp_dir_path, char *command_path){
	for (int i = strlen(temp_dir_path) + 1; command_path[i] != '\0'; i++){
		if (command_path[i] == '/'){
			command_path[i] = '\0';
			if (mkdir(command_path, 0777) < 0){
			    printf("cannot make directory %s\n", command_path);
			    perror("mkdir");
			}
			command_path[i] = '/';
		}
	}
}
void copy_files(char *from_path, char *to_path){
  	int  from;
  	int  to;
  	char buffer[4096];
  	int  ret;
  	from = open(from_path, O_RDONLY);
  	if (from < 0){
  	    perror("error open from path");
  		exit(1);
  	}
  	to = open(to_path, O_WRONLY | O_CREAT | O_EXCL, 0777);
  	if (to < 0){
  	    perror("error open to path");
  		exit(1);
  	}
  	while ((ret = read(from, buffer, 4096)) > 0){
  		if (ret < 0 || write(to, buffer, ret) < 0){
  		    perror("error write");
  	        exit(1);
  		}
  	}
  	close(from);
  	close(to);
}
void createDirContainer(char *command, char *temp_dir_path){
      char command_path[4096];
      char *n = mkdtemp(temp_dir_path);
      if(n == NULL) {
          printf("Cannot create temp directory.\n");
          exit(1);
      }
      sprintf(command_path, "%s%s", temp_dir_path, command);
      mk_dir(temp_dir_path, command_path);
      copy_files(command, command_path);
}
// Usage: your_docker.sh run <image> <command> <arg1> <arg2> ...
int main(int argc, char *argv[])
{
	// Disable output buffering
	setbuf(stdout, NULL);

	int StdOutErrToParent[2];
	pipe(StdOutErrToParent);

	char *command = argv[3];
	int child_pid = fork();

	if (child_pid == -1)
	{
		printf("Error forking!");
		return 1;
	}

	if (child_pid == 0)
	{
		// Replace current program with calling program

		dup2(fileno(stdout), StdOutErrToParent[1]);
		dup2(fileno(stderr), StdOutErrToParent[1]);
		close(StdOutErrToParent[0]);
		execv(command, &argv[3]);
		if (execv(command, &argv[3]) == -1)
		{
			printf("Error loading child process");
			return 1;
		}
	}
	else
	{

		dup2(StdOutErrToParent[0], fileno(stdout));
		close(StdOutErrToParent[1]);
				//catch the child exit status on completion
		int status;
		if (waitpid(child_pid, &status, 0) == -1) {
			printf("Error in process wait state");
			return 1;
		}
		if (WEXITSTATUS(status))
		{
			printf("Exit code: %d\n", WEXITSTATUS(status));
        	return WEXITSTATUS(status);       
		}
	}

	return 0;
}
