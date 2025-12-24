#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

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
    key_t key = ftok("shmfile", 65);
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);
    int *arr = (int *)shmat(shmid, (void *)0, 0);
    int i;
    pid_t pid;

    int input[] = {100, 20, 60, 40, 80};
    for (i = 0; i < 5; i++)
        arr[i] = input[i];

    printf("Parent: Writing to shared memory: ");
    for (i = 0; i < 5; i++)
        printf("%d ", arr[i]);
    printf("\n");

    pid = fork();

    if (pid == 0)
    {
        int *childArr = (int *)shmat(shmid, (void *)0, 0);
        sort(childArr, 5);
        shmdt(childArr);
        exit(0);
    }
    else
    {
        wait(NULL);
        printf("Parent: Reading from shared memory: ");
        for (i = 0; i < 5; i++)
            printf("%d ", arr[i]);
        printf("\n");
        shmdt(arr);
        shmctl(shmid, IPC_RMID, NULL);
    }

    return 0;
}