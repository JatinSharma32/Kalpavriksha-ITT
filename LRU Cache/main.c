#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "StructsAndDefinations.h"

// Cache and hash declared
Cache *cache = NULL;
HashTable *hashTable = NULL;

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
            char *capacityStr = strtok(NULL, " \n");
            if (capacityStr == NULL)
            {
                printf("Invalid Capacity, should be positive.\n");
                free(userInput);
                userInput = NULL;
                continue;
            }
            int cap = atoi(capacityStr);
            if (cap < 1)
            {
                printf("Capacity should be more then 0.\n");
                continue;
            }
            createCache(cap);
        }

        else if (strcmp("put", command) == 0)
        {
            char *keyString = strtok(NULL, " ");
            char *valueString = strtok(NULL, " \n");
            if (keyString == NULL || valueString == NULL)
                printf("Invalid key and value.\n");
            else
            {
                int key = atoi(keyString);
                putCommand(key, valueString);
            }
        }
        else if (strcmp("get", command) == 0)
        {
            char *keyStr = strtok(NULL, " ");
            if (keyStr == NULL)
            {
                printf("Invalid key.\n");
                free(userInput);
                userInput = NULL;
                continue;
            }
            int key = atoi(keyStr);
            char *val = getCommand(key);
            if (val == NULL)
                printf("NULL\n", val);
            else
                printf("%s\n", val);
        }
        else if (strcmp("exit", command) == 0)
        {
            free(userInput);
            userInput = NULL;
            return;
        }
        else
        {
            printf("INVALID COMMAND\n");
        }
        free(userInput);
        userInput = NULL;
    } while (true);
}

void exitCache()
{
    if (cache != NULL)
    {
        DLLQueNode *curr = cache->qHead;
        while (curr != NULL)
        {
            DLLQueNode *next = curr->Next;
            if (curr->Value != NULL)
                free(curr->Value);
            free(curr);
            curr = next;
        }
        free(cache);
        cache = NULL;
    }

    if (hashTable != NULL)
    {
        for (int i = 0; i < hashTable->HashSize; i++)
        {
            HashMapNode *currHashNode = *(hashTable->HashTable + i);
            while (currHashNode != NULL)
            {
                HashMapNode *nextHashNode = currHashNode->Next;
                free(currHashNode);
                currHashNode = nextHashNode;
            }
        }
        free(hashTable->HashTable);
        free(hashTable);
        hashTable = NULL;
    }
}

int main()
{
    userCommand();
    exitCache();
    return 0;
}