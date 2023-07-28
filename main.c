#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

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
