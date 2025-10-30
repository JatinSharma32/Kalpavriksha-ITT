#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void swap(int *num1, int *num2)
{
    // Using rotation by swapping value as I found swapping by pointers/addresses is impossible in a contiguous memory of matrixSize*matrixSize matrix.
    int temp = *num1;
    *num1 = *num2;
    *num2 = temp;
}
void fillMatrix(int matrixSize, int matrixPtr[matrixSize][matrixSize])
{
    for (int row = 0; row < matrixSize; row++)
    {
        for (int col = 0; col < matrixSize; col++)
        {
            *(*(matrixPtr) + (matrixSize * row + col)) = rand() % 256;
        }
    }
}
int printMatrix(int matrixSize, int matrixPtr[matrixSize][matrixSize])
{
    for (int row = 0; row < matrixSize; row++)
    {
        for (int col = 0; col < matrixSize; col++)
            printf("%d\t", *(*(matrixPtr) + (matrixSize * row + col)));
        printf("\n");
    }
    printf("\n");
}

void rotateMatrix(int matrixSize, int matrixPtr[matrixSize][matrixSize])
{

    for (int row = 0; row < matrixSize; row++)
    {
        for (int col = row + 1; col < matrixSize; col++)
        {
            swap(*(matrixPtr) + (matrixSize * row + col), *(matrixPtr) + (matrixSize * col + row));
        }
    }
    for (int row = 0; row < matrixSize; row++)
    {
        for (int col = 0; col < matrixSize / 2; col++)
        {
            swap((*(matrixPtr) + (matrixSize * row + col)), (*(matrixPtr) + (matrixSize * row + (matrixSize - 1 - col))));
        }
    }
}

void matrixSmoothing(int matrixSize, int matrixPtr[matrixSize][matrixSize])
{
    if (matrixSize == 1)
        return;
    int tempArray[matrixSize];
    int previous;
    for (int currRow = 0; currRow < matrixSize; currRow++)
    {
        previous = -1;
        for (int currCol = 0; currCol < matrixSize; currCol++)
        {
            int neighborSum = *(*(matrixPtr) + (matrixSize * currRow + currCol)), noOfNeighbor = 1;

            // First take the sum of upper row's 3 elements
            for (int rowIterator = -1; rowIterator <= 1; rowIterator++)
            {
                if ((currRow - 1) < 0)
                    continue;
                if ((matrixSize * (currRow - 1) + (currCol + rowIterator)) < (matrixSize * (currRow - 1)) || (matrixSize * (currRow - 1) + (currCol + rowIterator)) > (matrixSize * (currRow - 1) + (matrixSize - 1)))
                    continue;
                neighborSum += *(tempArray + (currCol + rowIterator));
                noOfNeighbor++;
            }

            // Then take the sum of down row's 3 elements
            for (int rowIterator = -1; rowIterator <= 1; rowIterator++)
            {
                if ((currRow + 1) >= matrixSize)
                    continue;
                if ((matrixSize * (currRow + 1) + (currCol + rowIterator)) < (matrixSize * (currRow + 1)) || (matrixSize * (currRow + 1) + (currCol + rowIterator)) > (matrixSize * (currRow + 1) + (matrixSize - 1)))
                    continue;
                neighborSum += *(*(matrixPtr) + (matrixSize * (currRow + 1) + (currCol + rowIterator)));
                noOfNeighbor++;
            }
            // Then take current elements left element
            if ((matrixSize * currRow + (currCol - 1)) >= (matrixSize * currRow))
            {
                neighborSum += previous;
                noOfNeighbor++;
            }
            // Then take current elements right element
            if ((matrixSize * currRow + (currCol + 1)) <= (matrixSize * currRow + (matrixSize - 1)))
            {
                neighborSum += *(*(matrixPtr) + (matrixSize * currRow + (currCol + 1)));
                noOfNeighbor++;
            }
            int averageValue = neighborSum / noOfNeighbor;
            // Update tempArray's currRow-1, currCol-1 with previous
            if (currCol - 1 >= 0)
            {
                *(tempArray + (currCol - 1)) = previous;
            }
            // Update previous with current value
            previous = *(*(matrixPtr) + (matrixSize * currRow + currCol));

            // Update current value with average
            *(*(matrixPtr) + (matrixSize * currRow + currCol)) = averageValue;
        }
        // Update last value of upepr row tempArray with previous
        *(tempArray + (matrixSize - 1)) = previous;
    }
}

int main(int argc, char **argv)
{
    srand(time(NULL));
    int matrixSize;
    printf("Enter matrix size (2-10): ");
    if (scanf("%d", &matrixSize) != 1 || matrixSize < 2 || matrixSize > 10)
    {
        printf("Invalid Input, matrix should be in range [2,10].");
        exit(1);
    }
    int matrix[matrixSize][matrixSize];

    fillMatrix(matrixSize, matrix);
    printf("Original Randomly Generated Matrix:\n");
    printMatrix(matrixSize, matrix);

    rotateMatrix(matrixSize, matrix);
    printf("Matrix after 90 Degree Clockwise Rotation:\n");
    printMatrix(matrixSize, matrix);

    matrixSmoothing(matrixSize, matrix);
    printf("Matrix after Applying 3x3 Smoothing Filter:\n");
    printMatrix(matrixSize, matrix);
}