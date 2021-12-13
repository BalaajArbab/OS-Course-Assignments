// Balaaj Arbab
// 251055771
// CS 3305A Assignment 4

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>

struct File_Info
{
    char name[128];
    int arrivalTime;
    int burstTime;

    int remainingBurstTime;
    int waitingTime;
    int turnAroundTime;

} typedef File_Info;

char parseACharacter(int fd)
{
    char chr;

    int endOfFile = read(fd, &chr, 1);

    if (endOfFile == 0) return '\n';

    return chr;
}

File_Info parseAProcess(int fd, int * quantum, int * processCount)
{
    char str[300]; str[0] = '\0';

    char toAdd;

    while (true)
    {
        toAdd = parseACharacter(fd);

        if (toAdd == ' ' || toAdd == '\n') break;

       	char toAddStr[2];
        toAddStr[0] = toAdd;
        toAddStr[1] = '\0';

        strcat(str, toAddStr);
    }

    if (toAdd == '\n') // End of Line
    {
        *quantum = atoi(str);
        File_Info endOfLine;
        strcpy(endOfLine.name, "quantum value reached");
        return endOfLine;
    }

    File_Info toReturn;

    strcpy(toReturn.name, str);

    str[0] = '\0';

    while (true)
    {
        toAdd = parseACharacter(fd);

        if (toAdd == ' ') break;

        char toAddStr[2];
        toAddStr[0] = toAdd;
        toAddStr[1] = '\0';

        strcat(str, toAddStr);
    }
    toReturn.arrivalTime = atoi(str);

    str[0] = '\0';

    while (true)
    {
        toAdd = parseACharacter(fd);

        if (toAdd == ' ') break;

        char toAddStr[2];
        toAddStr[0] = toAdd;
        toAddStr[1] = '\0';

        strcat(str, toAddStr);
    }
    toReturn.burstTime = atoi(str);

    (*processCount)++;
    return toReturn;

}

void processFiles(File_Info files[], int quantum, int processCount)
{
    bool filesProcessed[processCount];

    for (int i = 0; i < processCount; i++) filesProcessed[i] = false;

    int numberOfFilesProcessed = 0;
    int totalTimePassed = 0;

    bool processHasArrived[processCount];
    processHasArrived[0] = true;
    for (int i = 1; i < processCount; i++) processHasArrived[i] = false;


    for (int i = 0; i < processCount; i++) // copy remaining burst time section
    {
        files[i].remainingBurstTime = files[i].burstTime;
        files[i].waitingTime = 0;
        files[i].turnAroundTime = 0;
    }

    while (true)
    {

        for (int i = 0; i < processCount; i++) // Current process being processed
        {

            if (filesProcessed[i] != true && processHasArrived[i] == true) // Whether current process has completed and has arrived
            {
                for (int j = 1; j <= quantum; j++) // Time quantum ticks for current process if it has not completed
                {
                    for (int k = 0; k < processCount; k++) // Updating waiting time for all other incomplete processes
                    {
                        if (filesProcessed[k] != true && processHasArrived[k] && i != k) // Incomplete process check
                        {
                            files[k].waitingTime++;
                        }

                    }

                    totalTimePassed++;

                    files[i].remainingBurstTime--; // Remaining counter for current process

                    if (files[i].remainingBurstTime == 0) // Completion check for current process
                    {
                        filesProcessed[i] = true;
                        numberOfFilesProcessed++;

                        break;
                    }

                    for (int l = 0; l <= processCount; l++) // Updates arrival status of processes after each tick, based on total time passed
                {
                    if (files[l].arrivalTime <= totalTimePassed) processHasArrived[l] = true;

                }

                }

            }

        }

        if (numberOfFilesProcessed == processCount) break; // Check for completion of all processes
    }


    for (int i = 0; i < processCount; i++)
    {
        files[i].turnAroundTime = files[i].burstTime + files[i].waitingTime;
    }

    int totalTurnaround = 0;
    int totalWaiting = 0;

    for (int i = 0; i < processCount; i++)
    {
        printf("Name: %s, Arrival Time: %d, Burst Time: %d, Waiting Time: %d, Turnaround Time: %d\n", files[i].name, files[i].arrivalTime, files[i].burstTime, files[i].waitingTime, files[i].turnAroundTime);

        totalTurnaround += files[i].turnAroundTime;
        totalWaiting += files[i].waitingTime;
    }

    printf("Total Turnaround Time: %d\nAverage Waiting Time: %.2f\nAverage Turnaround Time: %.2f\n\n", totalTurnaround, (float)totalWaiting / processCount, (float)totalTurnaround / processCount);
}

int main()
{

    int fd = open("rr_input.txt", NULL);

    if (fd == -1)
    {
        puts("Error opening file. Exiting.");
        exit(-1);
    }

    File_Info files[100];

    while(true)
    {
        int i = 0;
	int processCount = 0;
	int quantum;

        while(true)
        {
            files[i] = parseAProcess(fd, &quantum, &processCount);

            if (strcmp(files[i].name, "quantum value reached") == 0) break;

	    i++;
        }

        if(processCount == 0) break; // End of file

        processFiles(files, quantum, processCount);

    }

}


