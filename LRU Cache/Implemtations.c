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