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
    int arr[] = {50, 10, 30, 20, 40};
    int n = 5;
    pid_t pid;
    FILE *file;
    int itr;

    printf("Parent Process: Array before sorting: ");
    for (itr = 0; itr < n; itr++)
    {
        printf("%d ", arr[itr]);
    }
    printf("\n");

    file = fopen("data.txt", "w");
    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }
    for (itr = 0; itr < n; itr++)
    {
        fprintf(file, "%d ", arr[itr]);
    }
    fclose(file);

    pid = fork();

    if (pid == 0)
    {
        int childArr[5];
        file = fopen("data.txt", "r");
        for (itr = 0; itr < n; itr++)
        {
            fscanf(file, "%d", &childArr[itr]);
        }
        fclose(file);

        sort(childArr, n);

        file = fopen("data.txt", "w");
        for (itr = 0; itr < n; itr++)
        {
            fprintf(file, "%d ", childArr[itr]);
        }
        fclose(file);
        exit(0);
    }
    else
    {
        wait(NULL);
        file = fopen("data.txt", "r");
        printf("Parent Process: Array after sorting: ");
        for (itr = 0; itr < n; itr++)
        {
            fscanf(file, "%d", &arr[itr]);
            printf("%d ", arr[itr]);
        }
        printf("\n");
        fclose(file);
    }

    return 0;
}