#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "StructsAndDefinations.h"

// Hash table initilization
void createHashTable(int capacity)
{
    hashTable = (HashTable *)malloc(sizeof(HashTable));
    if (hashTable == NULL)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }

    // Making its size prime for less collisions
    hashTable->HashSize = capacity * 3 + 31;
    hashTable->HashTable = (HashMapNode **)calloc(hashTable->HashSize, sizeof(HashMapNode *));
    if (hashTable->HashTable == NULL)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }
}