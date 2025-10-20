#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void swap(int *num1, int *num2)
{
    // Using rotation by swapping value as I found swapping by pointers/addresses is impossible in a contiguous memory of N*N matrix.
    int temp = *num1;
    *num1 = *num2;
    *num2 = temp;
}
void fillMatrix(int N, int matrixPtr[N][N])
{
    for (int row = 0; row < N; row++)
    {
        for (int col = 0; col < N; col++)
        {
            *(*(matrixPtr) + (N * row + col)) = rand() % 256;
        }
    }
}
int printMatrix(int N, int matrixPtr[N][N])
{
    for (int row = 0; row < N; row++)
    {
        for (int col = 0; col < N; col++)
            printf("%d\t", *(*(matrixPtr) + (N * row + col)));
        printf("\n");
    }
    printf("\n");
}

void rotateMatrix(int N, int matrixPtr[N][N])
{

    for (int row = 0; row < N; row++)
    {
        for (int col = row + 1; col < N; col++)
        {
            swap(*(matrixPtr) + (N * row + col), *(matrixPtr) + (N * col + row));
        }
    }
    for (int row = 0; row < N; row++)
    {
        for (int col = 0; col < N / 2; col++)
        {
            swap((*(matrixPtr) + (N * row + col)), (*(matrixPtr) + (N * row + (N - 1 - col))));
        }
    }
}

void matrixSmoothing(int N, int matrixPtr[N][N])
{
    if (N == 1)
        return;
    int tempArray[N];
    int previous;
    for (int currRow = 0; currRow < N; currRow++)
    {
        previous = -1;
        for (int currCol = 0; currCol < N; currCol++)
        {
            int neighborSum = *(*(matrixPtr) + (N * currRow + currCol)), noOfNeighbor = 1;

            // First take the sum of upper row's 3 elements
            for (int rowIterator = -1; rowIterator <= 1; rowIterator++)
            {
                if ((currRow - 1) < 0)
                    continue;
                if ((N * (currRow - 1) + (currCol + rowIterator)) < (N * (currRow - 1)) || (N * (currRow - 1) + (currCol + rowIterator)) > (N * (currRow - 1) + (N - 1)))
                    continue;
                neighborSum += *(tempArray + (currCol + rowIterator));
                noOfNeighbor++;
            }

            // Then take the sum of down row's 3 elements
            for (int rowIterator = -1; rowIterator <= 1; rowIterator++)
            {
                if ((currRow + 1) >= N)
                    continue;
                if ((N * (currRow + 1) + (currCol + rowIterator)) < (N * (currRow + 1)) || (N * (currRow + 1) + (currCol + rowIterator)) > (N * (currRow + 1) + (N - 1)))
                    continue;
                neighborSum += *(*(matrixPtr) + (N * (currRow + 1) + (currCol + rowIterator)));
                noOfNeighbor++;
            }
            // Then take current elements left element
            if ((N * currRow + (currCol - 1)) >= (N * currRow))
            {
                neighborSum += previous;
                noOfNeighbor++;
            }
            // Then take current elements right element
            if ((N * currRow + (currCol + 1)) <= (N * currRow + (N - 1)))
            {
                neighborSum += *(*(matrixPtr) + (N * currRow + (currCol + 1)));
                noOfNeighbor++;
            }
            int averageValue = neighborSum / noOfNeighbor;
            // Update tempArray's currRow-1, currCol-1 with previous
            if (currCol - 1 >= 0)
            {
                *(tempArray + (currCol - 1)) = previous;
            }
            // Update previous with current value
            previous = *(*(matrixPtr) + (N * currRow + currCol));

            // Update current value with average
            *(*(matrixPtr) + (N * currRow + currCol)) = averageValue;
        }
        // Update last value of upepr row tempArray with previous
        *(tempArray + (N - 1)) = previous;
    }
}

int main(int argc, char **argv)
{
    srand(time(NULL));
    int N;
    printf("Enter matrix size (2-10): ");
    if (scanf("%d", &N) != 1 || N < 2 || N > 10)
    {
        printf("Invalid Input, matrix should be in range [2,10].");
        exit(1);
    }
    int matrix[N][N];

    fillMatrix(N, matrix);
    printf("Original Randomly Generated Matrix:\n");
    printMatrix(N, matrix);

    rotateMatrix(N, matrix);
    printf("Matrix after 90 Degree Clockwise Rotation:\n");
    printMatrix(N, matrix);

    matrixSmoothing(N, matrix);
    printf("Matrix after Applying 3x3 Smoothing Filter:\n");
    printMatrix(N, matrix);
}