#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "StructsAndDefinations.h"

// A hash function to reduce the number of collisions, took help from Knuth’s Multiplicative Hash.
int hashFunction(int key, int hashSize)
{
    long long hash = key;
    hash = hash * 2654435761;
    int index = (int)(hash % hashSize);
    if (index < 0)
    {
        index = index * -1;
    }
    return index;
}

void userCommand()
{
    do
    {
        char *userInput = (char *)malloc(1001 * sizeof(char));
        if (userInput == NULL)
        {
            printf("Memory allocation failed\n");
            exit(1);
        }

        fgets(userInput, 1000, stdin);
        userInput[strcspn(userInput, "\n")] = 0;

        char *command = strtok(userInput, " ");

        if (command == NULL)
        {
            free(userInput);
            userInput = NULL;
            continue;
        }
        else if (strcmp("createCache", command) == 0)
        {
            char *capacityStr = strtok(NULL, " ");
            if (capacityStr == NULL)
            {
                printf("Invalid Capacity.\n");
                free(userInput);
                userInput = NULL;
                continue;
            }
            int cap = atoi(capacityStr);
            if (cap < 1)
            {
                printf("Capacity should be more then 0.\n");
            }
            else
            {
                createCache(cap);
                createHashTable(cap);
            }
        }

        else if (strcmp("put", command) == 0)
        {
            // If node already exist then make it MRU.
        }
        else if (strcmp("get", command) == 0)
            ;
        else if (strcmp("exit", command) == 0)
            ;
        else
        {
            printf("INVALID COMMAND\n");
        }
        free(userInput);
        userInput = NULL;
    } while (true);
}
