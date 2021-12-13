// CS3305A Assignment 1
// Student Name: Balaaj Arbab
// Student ID: 251055771

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char ** argv)
{
    pid_t pid;
    char str[200];

    pid = fork();

    // fork failed.
    if (pid == -1) exit(-1);

    // Parent
    if (pid > 0)
    {
        wait(NULL);
    }

    // Child_1
    if (pid == 0)
    {
	
        pid_t thispid = getpid();
	
        sprintf(str, "parent (PID %d) created child_1 (PID %d)", getppid(), thispid);
	puts(str);

	sprintf(str, "parent (PID %d) is waiting for child_1 (PID %d) to complete before creating child_2", getppid(), thispid);
	puts(str);

        char pidString[30];
       	sprintf(pidString, "%d", thispid);

        char * strr = " for child_1";

        strcat(pidString, strr); // Argument passed into execl

	sprintf(str, "child_1 (PID %d) is calling an external program %s and will be replaced", thispid, argv[1]);
	puts(str);

        execl(argv[1], argv[1], pidString, NULL);

    }

    pid = fork(); // Creates Child_2

    // Parent
    if (pid > 0)
    {
	wait(NULL);

	puts("child_1 and child_2 are completed and parent process is terminating...");
	exit(0);
    }

    // Child_2
    if (pid == 0)
    {

	sprintf(str, "parent (PID %d) created child_2 (PID %d)", getppid(), getpid());
	puts(str);

	pid = fork();

	// Child_2 
	if (pid > 0) 
	{
	    wait(NULL);
	}

	// Child_2.1
	if (pid == 0)
	{
	    sprintf(str, "child_2 (PID %d) created child_2.1 (PID %d)", getppid(), getpid());
	    puts(str);

	    char pidString[30];
	    sprintf(pidString, "%d", getpid());

	    char * strr = " for child_2.1";

	    strcat(pidString, strr);

	    sprintf(str, "child_2.1 (PID %d) is calling an external program %s and will be replaced", getpid(), argv[1]);
	    puts(str);

	    execl(argv[1], argv[1], pidString, NULL);

	}
	

    }

}

