#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "Players_data.h"
#include "Definitions.h"

//          POINTERS DECLARATION
MainPlayerList *mainPlayerList = NULL;
MainTeam *mainTeams = NULL;

void userMenuDisplay()
{
    int userChoice = 0;
    do
    {
        printf("==============================================================================\n\tICC ODI Player Performance Analyzer\n==============================================================================\n1. Add Player to Team\n2. Display Players of a Specific Team\n3. Display Teams by Average Batting Strike Rate\n4. Display Top K Players of a Specific Team by Role\n5. Display all Players of specific role Across All Teams by performance index\n6. Exit\n==============================================================================\nEnter your choice: ");
        if (scanf("%d", &userChoice) != 1 || userChoice > 6 || userChoice < 1)
        {
            userChoice = -1;
            printf("Invalid choice please choose a number from 1 to 6.\n");
            continue;
        }

        if (userChoice == 1)
        {
            addNewPlayer();
        }
        else if (userChoice == 2)
        {
            displayAllPlayersOfTeam();
        }
        else if (userChoice == 3)
        {
            displayTeamsByAvgStr();
        }
        else if (userChoice == 4)
        {
            displayTopKPlayers();
        }
        else if (userChoice == 5)
        {
            displayAllPlayers();
        }
    } while (userChoice != 6);
}

void deallocation()
{
    // first free the Doubly list of players
    MainPlayer *temp = mainPlayerList->head;
    while (temp != NULL)
    {
        MainPlayer *nextPlayer = temp->next;
        // freeing the strings
        free(temp->Name);
        free(temp->TeamName);
        free(temp->Role);
        free(temp);

        temp = nextPlayer;
    }
    free(mainPlayerList);

    // Free all the teams and their associated list of players in them
    for (int i = 0; i < INITIAL_TEAM_COUNT; i++)
    {
        PlayerInTeamNode *currNode = (mainTeams + i)->TeamListHead;
        while (currNode != NULL)
        {
            PlayerInTeamNode *nextNode = currNode->next;
            free(currNode);
            currNode = nextNode;
        }
        free((mainTeams + i)->Name);
    }
    free(mainTeams);
}

int main(int argc, char **argv)
{
    mainTeams = (MainTeam *)malloc(INITIAL_TEAM_COUNT * sizeof(MainTeam));
    if (mainTeams == NULL)
    {
        printf("Memory can't be allocated\n");
        exit(0);
    }
    mainPlayerList = (MainPlayerList *)malloc(sizeof(MainPlayerList));
    if (mainPlayerList == NULL)
    {
        printf("Memory can't be allocated\n");
        exit(0);
    }
    mainPlayerList->head = NULL;
    mainPlayerList->tail = NULL;
    initilizeTeams();
    initilizePlayers();

    userMenuDisplay();

    deallocation();
}