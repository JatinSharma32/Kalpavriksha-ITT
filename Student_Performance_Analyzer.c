#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

// Constants
#define NAME_SIZE 100

// Data Structures
struct Student_Data
{
    int RollNo;
    char Name[NAME_SIZE];
    int Marks[3];
};

int totalMarks(int Marks[])
{
    return Marks[0] + Marks[1] + Marks[2];
}

float averageMarks(int total)
{
    return 1.0 * total / 3;
}

char gradeOfStudent(float average)
{
    if (average >= 85)
        return 'A';
    else if (average >= 70)
        return 'B';
    else if (average >= 50)
        return 'C';
    else if (average >= 35)
        return 'D';
    else
        return 'F';
}

void printingOutput(struct Student_Data studentDetails)
{
    printf("Roll: %d\n", studentDetails.RollNo);
    printf("Name: %s\n", studentDetails.Name);
    int total = totalMarks(studentDetails.Marks);
    printf("Total: %d\n", total);
    float average = averageMarks(total);
    printf("Average: %2f\n", average);
    char grade = gradeOfStudent(average);
    printf("Grade: %c\n", grade);
    if (grade == 'F')
        return;
    printf("Performance: ");
    for (int noOfStars = 0; noOfStars < ('E' - grade) + 1; noOfStars++)
    {
        printf("*");
    }
    printf("\n");
}

void printingRollNumbers(int rollNo)
{
    if (rollNo == 0)
        return;
    printingRollNumbers(rollNo - 1);
    printf("%d ", rollNo);
}

int main(int argc, char *argv[])
{
    int noOfStudents;
    if (scanf("%d", &noOfStudents) != 1 || noOfStudents <= 0 || noOfStudents > 100)
    {
        printf("Invalid Number of Students (should be in [1,100] range).");
        exit(1);
    }

    // Taking Student data as Input
    struct Student_Data studentDataArray[noOfStudents];
    for (int student = 0; student < noOfStudents; student++)
    {
        struct Student_Data studentDetails;
        if (scanf("%d", &studentDetails.RollNo) != 1 || studentDetails.RollNo < 1 || studentDetails.RollNo > 100)
        {
            printf("Invalid Roll Number (should be in [1,100] range).");
            exit(1);
        }
        if (scanf(" %99[^0-9]", studentDetails.Name) != 1 || strlen(studentDetails.Name) == 0)
        {
            printf("Invalid Name.");
            exit(1);
        }
        if (scanf("%d", &studentDetails.Marks[0]) != 1 || studentDetails.Marks[0] < 0 || studentDetails.Marks[0] > 100)
        {
            printf("Invalid Mark (should be in [0,100] range).");
            exit(1);
        }
        if (scanf("%d", &studentDetails.Marks[1]) != 1 || studentDetails.Marks[1] < 0 || studentDetails.Marks[1] > 100)
        {
            printf("Invalid Mark (should be in [0,100] range).");
            exit(1);
        }
        if (scanf("%d", &studentDetails.Marks[2]) != 1 || studentDetails.Marks[2] < 0 || studentDetails.Marks[2] > 100)
        {
            printf("Invalid Mark (should be in [0,100] range).");
            exit(1);
        }
        studentDataArray[student] = studentDetails;
    }

    // Printing Student data one by one.
    for (int student = 0; student < noOfStudents; student++)
    {
        printf("\n");
        printingOutput(studentDataArray[student]);
    }

    // Printing the Roll Numbers using Recursion.
    printf("\nList of Roll Numbers (via recursion): ");
    printingRollNumbers(noOfStudents);
    printf("\n");
    return 0;
}