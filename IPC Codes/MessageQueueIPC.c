#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

struct msg_buffer
{
    long msg_type;
    int arr[5];
};

void sort(int arr[], int n)
{
    int itr, innerItr, temp;
    for (itr = 0; itr < n - 1; itr++)
    {
        for (innerItr = 0; innerItr < n - itr - 1; innerItr++)
        {
            if (arr[innerItr] > arr[innerItr + 1])
            {
                temp = arr[innerItr];
                arr[innerItr] = arr[innerItr + 1];
                arr[innerItr + 1] = temp;
            }
        }
    }
}

int main()
{
    key_t key;
    int msgid;
    struct msg_buffer message;
    pid_t pid;
    int itr;

    key = ftok("progfile", 65);
    msgid = msgget(key, 0666 | IPC_CREAT);

    message.msg_type = 1;
    int inputData[] = {12, 5, 8, 1, 9};
    for (itr = 0; itr < 5; itr++)
        message.arr[itr] = inputData[itr];

    printf("Parent: Data sent to queue: ");
    for (itr = 0; itr < 5; itr++)
        printf("%d ", message.arr[itr]);
    printf("\n");

    msgsnd(msgid, &message, sizeof(message), 0);

    pid = fork();

    if (pid == 0)
    {
        msgrcv(msgid, &message, sizeof(message), 1, 0);
        sort(message.arr, 5);
        message.msg_type = 2;
        msgsnd(msgid, &message, sizeof(message), 0);
        exit(0);
    }
    else
    {
        wait(NULL);
        msgrcv(msgid, &message, sizeof(message), 2, 0);
        printf("Parent: Data received after sorting: ");
        for (itr = 0; itr < 5; itr++)
            printf("%d ", message.arr[itr]);
        printf("\n");
        msgctl(msgid, IPC_RMID, NULL);
    }

    return 0;
}