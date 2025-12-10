#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "Implementations.h"

Queue *ReadyQueue = NULL;
Queue *WaitingQueue = NULL;
Queue *TerminatedQueue = NULL;
int SystemClock = 0;

int main(int argc, char **argv)
{
}