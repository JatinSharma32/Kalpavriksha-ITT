#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "Players_data.h"
#include "Definitions.h"

int findTeamNo(const char *teamName)
{
    for (int teamNo = 0; teamNo < INITIAL_TEAM_COUNT; teamNo++)
    {
        if (strcmp(teamName, (mainTeams + teamNo)->Name) == 0)
        {
            return teamNo;
        }
    }
    return -1;
}

void fixTeamOrder(int updatedIndex)
{
    // fixing the order by performing left or rigth swap like insertion sort
    while (updatedIndex < INITIAL_TEAM_COUNT - 1 &&
           mainTeams[updatedIndex].AverageBattingStrikerate < mainTeams[updatedIndex + 1].AverageBattingStrikerate)
    {
        MainTeam temp = mainTeams[updatedIndex];
        mainTeams[updatedIndex] = mainTeams[updatedIndex + 1];
        mainTeams[updatedIndex + 1] = temp;
        updatedIndex++;
    }

    while (updatedIndex > 0 &&
           mainTeams[updatedIndex].AverageBattingStrikerate > mainTeams[updatedIndex - 1].AverageBattingStrikerate)
    {
        MainTeam temp = mainTeams[updatedIndex];
        mainTeams[updatedIndex] = mainTeams[updatedIndex - 1];
        mainTeams[updatedIndex - 1] = temp;
        updatedIndex--;
    }
}

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