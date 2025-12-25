#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
    int fd1[2], fd2[2];
    int arr[] = {9, 2, 7, 1, 5};
    int n = 5;
    pid_t pid;
    int i;

    if (pipe(fd1) == -1 || pipe(fd2) == -1)
    {
        return 1;
    }

    printf("Parent: Sending data: ");
    for (i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");

    pid = fork();

    if (pid == 0)
    {
        close(fd1[1]);
        close(fd2[0]);

        int childArr[5];
        read(fd1[0], childArr, sizeof(childArr));
        sort(childArr, n);
        write(fd2[1], childArr, sizeof(childArr));

        close(fd1[0]);
        close(fd2[1]);
        exit(0);
    }
    else
    {
        close(fd1[0]);
        close(fd2[1]);

        write(fd1[1], arr, sizeof(arr));
        wait(NULL);
        read(fd2[0], arr, sizeof(arr));

        printf("Parent: Received sorted data: ");
        for (i = 0; i < n; i++)
            printf("%d ", arr[i]);
        printf("\n");

        close(fd1[1]);
        close(fd2[0]);
    }

    return 0;
}