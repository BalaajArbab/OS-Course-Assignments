// Balaaj Arbab
// 251055771
// CS 3305A Assignment 5

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

struct Client
{
    int cNo;

    char actions[100];
    int actionCount;
    int accountNumber[100];
    int amount[100];

} typedef Client;

int parseALine(int * bankAccount, Client * client, int fd, int * accountCount, int * clientCount);
void parseAccount(int * bankAccount, int fd, int * accountCount);
void parseClient (Client * client, int fd, int * clientCount);

int bankAccounts[50];

pthread_mutex_t lock[50];

void * accessAccount(void * clientStruct)
{
    Client client = *((Client *) clientStruct);

    for (int i = 0; i < client.actionCount; i++)
    {
        int account = client.accountNumber[i];
        char action = client.actions[i];
        int amount = client.amount[i];

        pthread_mutex_lock(&lock[account]); // Critical Section IN

        if (action == 'w')
        {
            if (bankAccounts[account] >= amount) bankAccounts[account] -= amount;
            // printf("Client %d withdrawing %d from account: %d\n", client.cNo, amount, account);
        }

        if (action == 'd')
        {
            bankAccounts[account] += amount;
            // printf("Client %d depositing %d to account: %d\n", client.cNo, amount, account);
        }

        pthread_mutex_unlock(&lock[account]); // Critical Section OUT
    }
}

int main()
{

    int fd = open("assignment_5_input.txt", NULL);

    if (fd == -1)
    {
        puts("Error opening file. Exiting.");
        exit(-1);
    }

    Client clients[100];

    int accountCount = 0;
    int clientCount = 0;

    while (true)
    {

        if (parseALine(&bankAccounts[accountCount + 1], &clients[clientCount + 1], fd, &accountCount, &clientCount) == 0) break;

    }

    for (int i = 1; i <= accountCount; i++) printf("Bank Account %d: %d\n", i, bankAccounts[i]);

    /*
    printf("c%d %c a%d %d\n", clients[1].cNo, clients[1].actions[0], clients[1].accountNumber[0], clients[1].amount[0]);
    printf("c%d %c a%d %d %c a%d %d\n", clients[2].cNo, clients[2].actions[0], clients[2].accountNumber[0], clients[2].amount[0], clients[2].actions[1], clients[2].accountNumber[1], clients[2].amount[1]);
    printf("c%d %c a%d %d %c a%d %d %c a%d %d\n", clients[3].cNo, clients[3].actions[0], clients[3].accountNumber[0], clients[3].amount[0], clients[3].actions[1], clients[3].accountNumber[1], clients[3].amount[1], clients[3].actions[2], clients[3].accountNumber[2], clients[3].amount[2]);
    printf("c%d %c a%d %d %c a%d %d\n", clients[4].cNo, clients[4].actions[0], clients[4].accountNumber[0], clients[4].amount[0], clients[4].actions[1], clients[4].accountNumber[1], clients[4].amount[1]);
    printf("AccountCount: %d, ClientCount: %d\n", accountCount, clientCount);
    */

    // for (int i = 1; i <= clientCount; i++) printf("Account %d action count: %d\n", i, clients[i].actionCount);

    for (int i = 1 ; i <= accountCount; i++)
    {

        pthread_mutex_init(&lock[i], NULL);
    }

    pthread_t threads[clientCount];

    for (int i = 0; i < clientCount; i++)
    {
        pthread_create(&threads[i], NULL, accessAccount, &clients[i + 1]);
    }

    for (int i = 0; i < clientCount; i++)
    {
        pthread_join(threads[i], NULL);
    }

    for (int i = 1; i <= accountCount; i++)
    {
        pthread_mutex_destroy(&lock[i]);
    }

    puts("\n\nAfter Transactions:");

    for (int i = 1; i <= accountCount; i++) printf("a%d b %d\n", i, bankAccounts[i]);

    return 1;

}

int parseALine(int * bankAccount, Client * client, int fd, int * accountCount, int * clientCount)
{
    char firstLetter;

    int endOfFile = read(fd, &firstLetter, 1);

    if (endOfFile == 0) return 0;

    if (firstLetter == 'a') parseAccount(bankAccount, fd, accountCount);

    if (firstLetter == 'c') parseClient(client, fd, clientCount);

}

void parseAccount(int * bankAccount, int fd, int * accountCount)
{
    char str[100]; str[0] = '\0';

    char toAdd;

    int spaces = 0;

    while(spaces < 2)
    {
        read(fd, &toAdd, 1);

        if (toAdd == ' ') spaces++;
    }

    while(true)
    {
        read(fd, &toAdd, 1);

        if (toAdd == '\n') break;

        char toAddStr[2]; toAddStr[0] = toAdd; toAddStr[1] = '\0';
        strcat(str, toAddStr);
    }

    *bankAccount = atoi(str);
    (*accountCount)++;

}

void parseClient (Client * client, int fd, int * clientCount)
{
    (*clientCount)++;

    (*client).cNo = *clientCount;

    char chr = 'z';


    while (chr != ' ') read(fd, &chr, 1);

    int actionCount = 0;

    while (true)
    {
        char str[100]; str[0] = '\0';

        read(fd, &chr, 1); // Action to perform

        (*client).actions[actionCount] = chr;

        read(fd, &chr, 1);
        read(fd, &chr, 1);

        while (true) // Parsing account number to perform action on
        {
            read(fd, &chr, 1);

            if (chr == ' ') break;

            char toAddStr[2]; toAddStr[0] = chr; toAddStr[1] = '\0';

            strcat(str, toAddStr);

        }

        (*client).accountNumber[actionCount] = atoi(str);
        str[0] = '\0';



        while (true) // Amount to perform action with
        {
            if (read(fd, &chr, 1) == 0) chr = '\n';

            if (chr == ' ' || chr == '\n') break;

            char toAddStr[2]; toAddStr[0] = chr; toAddStr[1] = '\0';

            strcat(str, toAddStr);
        }

        (*client).amount[actionCount] = atoi(str);

        actionCount++;
        client->actionCount = actionCount;

        if (chr == '\n') break;

    }





}
