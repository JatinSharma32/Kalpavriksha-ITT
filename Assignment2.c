#include <stdio.h>
#include <stdbool.h>
#include <string.h>

struct StudentData
{
    int ID;
    char Name[100];
    int Age;
};

// if Record already exists
bool alreadyExist(int id)
{
    FILE *fp = fopen("user.txt", "r");
    if (fp == NULL)
    {
        printf("File Creation Error\n");
        return true;
    }
    struct StudentData student;
    while (fscanf(fp, "%d, %[^,], %d\n", &student.ID, &student.Name, &student.Age) >= 1)
    {
        if (student.ID == id)
        {
            fclose(fp);
            return true;
        }
    }
    fclose(fp);
    return false;
}

void createRecord()
{
    FILE *fp = fopen("user.txt", "a");
    if (fp == NULL)
    {
        printf("File Creation Error\n");
        return;
    }
    struct StudentData student;
    printf("Enter the ID:");
    while (scanf("%d", &student.ID) != 1 || student.ID <= 0)
    {
        printf("Invalid input. Please enter a positive integer ID: ");
        while (getchar() != '\n')
            ;
    }
    printf("Enter the Name:");
    scanf(" %[^\n]", student.Name);
    while (strlen(student.Name) == 0)
    {
        printf("Name cannot be empty. Please enter a valid name: ");
        scanf(" %[^\n]", student.Name);
    }
    printf("Enter the Age:");
    while (scanf("%d", &student.Age) != 1 || student.Age <= 0 || student.Age > 150)
    {
        printf("Invalid input. Please enter a valid age (1-150): ");
        while (getchar() != '\n')
            ;
    }
    if (alreadyExist(student.ID))
    {
        printf("User ID already exist.\n");
        return;
    }
    fprintf(fp, "%d, %s, %d\n", student.ID, student.Name, student.Age);
    printf("Record added.\n");
    fclose(fp);
}

void readRecords()
{
    FILE *fp = fopen("user.txt", "r");
    if (fp == NULL)
    {
        printf("File not found.\n");
        return;
    }
    char line[200];
    printf("Records:\n");
    while (fgets(line, 199, fp))
    {
        printf("%s", line);
    }
    fclose(fp);
}

void updateRecord()
{
    FILE *fp = fopen("user.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (fp == NULL || temp == NULL)
    {
        printf("Error opening files.\n");
        if (fp)
            fclose(fp);
        if (temp)
            fclose(temp);
        return;
    }

    struct StudentData updatedInfo;
    struct StudentData student;
    printf("Enter the Student ID to be updated:");
    while (scanf("%d", &updatedInfo.ID) != 1 || updatedInfo.ID <= 0)
    {
        printf("Invalid input. Please enter a positive integer ID: ");
        while (getchar() != '\n')
            ;
    }

    printf("Enter the Name:");
    scanf(" %[^\n]", &updatedInfo.Name);
    while (strlen(updatedInfo.Name) == 0)
    {
        printf("Name cannot be empty. Please enter a valid name: ");
        scanf(" %[^\n]", updatedInfo.Name);
    }
    printf("Enter the new Age:");
    while (scanf("%d", &updatedInfo.Age) != 1 || updatedInfo.Age <= 0 || updatedInfo.Age > 150)
    {
        printf("Invalid input. Please enter a valid age (1-150): ");
        while (getchar() != '\n')
            ;
    }

    bool found = false;
    while (fscanf(fp, "%d, %[^,], %d\n", &student.ID, &student.Name, &student.Age) >= 1)
    {
        if (student.ID == updatedInfo.ID)
        {
            found = true;
            fprintf(temp, "%d, %s, %d\n", updatedInfo.ID, updatedInfo.Name, updatedInfo.Age);
            continue;
        }
        fprintf(temp, "%d, %s, %d\n", student.ID, student.Name, student.Age);
    }
    fclose(fp);
    fclose(temp);

    if (found)
    {
        if (remove("user.txt") != 0)
        {
            printf("Removing user file failed.");
            return;
        }
        if (rename("temp.txt", "user.txt") != 0)
        {
            printf("Renaming file failed.");
            return;
        }
        printf("Record updated.\n");
        return;
    }
    printf("Record not found.\n");
    remove("temp.txt");
}

void deleteRecord()
{
    FILE *fp = fopen("user.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (fp == NULL || temp == NULL)
    {
        printf("Error opening files.\n");
        if (fp)
            fclose(fp);
        if (temp)
            fclose(temp);
        return;
    }

    int id;
    struct StudentData student;
    printf("Enter the Student ID to be deleted:");
    while (scanf("%d", &id) != 1 || id <= 0)
    {
        printf("Invalid input. Please enter a positive integer ID: ");
        while (getchar() != '\n')
            ;
    }

    bool found = false;
    while (fscanf(fp, "%d, %[^,], %d\n", &student.ID, &student.Name, &student.Age) >= 1)
    {
        if (student.ID == id)
        {
            found = true;
            continue;
        }
        fprintf(temp, "%d, %s, %d\n", student.ID, student.Name, student.Age);
    }
    fclose(fp);
    fclose(temp);
    if (found)
    {
        if (remove("user.txt") != 0)
        {
            printf("Removing user file failed.");
            return;
        }
        if (rename("temp.txt", "user.txt") != 0)
        {
            printf("Renaming file failed.");
            return;
        }
        printf("Record deleted.\n");
        return;
    }
    printf("Record not found.\n");
    remove("temp.txt");
}

int main(int argc, char **argv)
{
    int option = 0;
    do
    {
        printf("Create -> 1\nRead -> 2\nUpdate -> 3\nDelete -> 4\nExit -> 5\nEnter your Choice: ");
        while (scanf("%d", &option) != 1 || option < 1 || option > 5)
        {
            printf("Invalid choice. Please enter a number between 1-5: ");
            while (getchar() != '\n')
                ;
        }
        if (option == 1)
            createRecord();
        else if (option == 2)
            readRecords();
        else if (option == 3)
            updateRecord();
        else if (option == 4)
            deleteRecord();
    } while (option != 5);
}
