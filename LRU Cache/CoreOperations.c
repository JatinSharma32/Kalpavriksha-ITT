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
        putInHashMap(key, newNode);
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

char *getCommand(int key)
{
    // find if this key already exist in LRU
    DLLQueNode *node = mapGet(key);

    // if doesn't exist then return nothing
    if (node == NULL)
        return NULL;

    // Make this node MRU
    moveToHead(node);
    return node->Value;
}