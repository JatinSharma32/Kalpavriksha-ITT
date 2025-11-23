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

// Putting new element in the LRU Cache
void putCommand(int key, char *value)
{
    // find if this key already exist in LRU
    DLLQueNode *node = mapGet(key);

    if (node != NULL)
    {
        // if found then update its value in DLL.
        free(node->Value);
        node->Value = (char *)malloc(strlen(value) * sizeof(char));
        strcpy(node->Value, value);

        // Now this becomes MRU so move it to the DLL head
        moveToHead(node);
    }
    else
    {
        // if not foudn then create this node and put it in the dl and hash map both
        DLLQueNode *newNode = createDLLNode(key, value);

        addToHead(newNode);
        mapPut(key, newNode);
        // Increase the number of existing elements in cache
        cache->Size++;

        // if cache is full then remove the LRU node
        if (cache->Size > cache->Capacity)
        {
            DLLQueNode *lruNode = removeTail();
            mapRemove(lruNode->Key);
            free(lruNode->Value);
            free(lruNode);
            // reduce the size of cache
            cache->Size--;
        }
    }
}