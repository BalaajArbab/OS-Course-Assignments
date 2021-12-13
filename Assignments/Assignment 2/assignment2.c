// Balaaj Arbab
// 251055771
// CS 3305A Assignment 2

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char ** argv)
{
    int lenX = strlen(argv[1]) + 1;
    int lenY = strlen(argv[2]) + 1;
    int lenZ = strlen(argv[3]) + 1;
    int total = lenX + lenY + lenZ;
    int totalWithPad = total + 150;

    char X[lenX], Y[lenY], Z[lenZ], str[totalWithPad];

    int port[2];

    int e = pipe(port);

    if (e == -1)
    {
        puts("Could not create pipe. Exiting.");
        exit(-1);
    }

    pid_t pid = fork();

    if (pid > 0) // Parent
    {
        sprintf(str, "Parent (PID %d) created a child (PID %d)", getpid(), pid);
        puts(str);

        strcpy(X, argv[1]);

        sprintf(str, "Parent (PID %d) receives X = \"%s\" from the user", getpid(), X);
        puts(str);

        sprintf(str, "Parent (PID %d) writes X = \"%s\" to the pipe", getpid(), X);
        puts(str);
        write(port[1], X, lenX);

        wait();

        char Z_Prime[total];

        read(port[0], Z_Prime, total);

        sprintf(str, "Parent (PID %d) reads concatenated result from the pipe (Z' = \"%s\")", getpid(), Z_Prime);
        puts(str);

    }

    if (pid == 0) // Child
    {
        read(port[0], X, lenX);

        strcpy(Y, argv[2]);
        strcpy(Z, argv[3]);

        sprintf(str, "Child (PID %d) receives Y = \"%s\" and Z = \"%s\" from the user", getpid(), Y, Z);
        puts(str);

        char Y_Prime[total]; Y_Prime[0] = ' '; Y_Prime[1] = '\0';
        strcat(Y, " ");
        strcat(Y_Prime, Y);
        strcat(Y_Prime, Z);

        sprintf(str, "Child (PID %d) concatenates Y and Z to generate Y' = \"%s\"", getpid(), Y_Prime);
        puts(str);

        sprintf(str, "Child (PID %d) reads X from the pipe \"%s\"", getpid(), X);
        puts(str);

        char Z_Prime[total]; Z_Prime[0] = '\0';
        strcat(Z_Prime, X);
        strcat(Z_Prime, Y_Prime);

        sprintf(str, "Child (PID %d) concatenates X and Y' to generate Z' = \"%s\"", getpid(), Z_Prime);
        puts(str);

        sprintf(str, "Child (PID %d) writes Z' into the pipe", getpid());
        puts(str);

        write(port[1], Z_Prime, strlen(Z_Prime) + 1);


    }

}
