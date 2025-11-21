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

void insertInTeamList(MainTeam *team, PlayerInTeamNode *newTeamMember)
{
    // If the team list is empty
    if (team->TeamListHead == NULL)
    {
        team->TeamListHead = newTeamMember;
        team->TeamListTail = newTeamMember;
    }
    else
    {
        PlayerInTeamNode *curr = team->TeamListHead;
        // Find the correct position (descending order)
        while (curr != NULL && curr->data->PerformanceIndex >= newTeamMember->data->PerformanceIndex)
        {
            curr = curr->next;
        }

        if (curr == team->TeamListHead)
        {
            // Insert at the beginning
            newTeamMember->next = team->TeamListHead;
            team->TeamListHead->prev = newTeamMember;
            team->TeamListHead = newTeamMember;
        }
        else if (curr == NULL)
        {
            // Insert at the end
            newTeamMember->prev = team->TeamListTail;
            team->TeamListTail->next = newTeamMember;
            team->TeamListTail = newTeamMember;
        }
        else
        {
            // Insert in the middle
            newTeamMember->next = curr;
            newTeamMember->prev = curr->prev;
            curr->prev->next = newTeamMember;
            curr->prev = newTeamMember;
        }
    }

    // run onyl for allrounder or Batsman
    if (strcmp(newTeamMember->data->Role, "Batsman") == 0 || strcmp(newTeamMember->data->Role, "All-rounder") == 0)
    {
        team->AverageBattingStrikerate =
            ((team->AverageBattingStrikerate * team->TotalPlayers) + newTeamMember->data->StrikeRate) / (team->TotalPlayers + 1);
    }
    team->TotalPlayers += 1;
}

void initilizeTeams()
{
    for (int teamNo = 0; teamNo < INITIAL_TEAM_COUNT; teamNo++)
    {
        MainTeam *newTeam = (MainTeam *)malloc(sizeof(MainTeam));
        if (newTeam == NULL)
        {
            printf("Memory can't be allocated\n");
            exit(0);
        }
        newTeam->Name = (char *)malloc(51 * sizeof(char));
        if (newTeam->Name == NULL)
        {
            printf("Memory can't be allocated\n");
            exit(0);
        }

        strcpy(newTeam->Name, teams[teamNo]);
        newTeam->TeamId = teamNo + 1;
        newTeam->AverageBattingStrikerate = 0;
        newTeam->TeamListHead = NULL;
        newTeam->TeamListTail = NULL;
        newTeam->TotalPlayers = 0;
        *(mainTeams + teamNo) = *newTeam;
    }
}

void displayTeamsByAvgStr()
{
    printf("Teams Sorted by Average Batting Strike Rate\n=========================================================\nID\tTeam Name\tAvg Bat SR\tTotal Players\n=========================================================\n");

    for (int i = 0; i < INITIAL_TEAM_COUNT; i++)
    {
        printf("%d\t%s\t%.2f\t\t%d\n",
               (mainTeams + i)->TeamId,
               (mainTeams + i)->Name,
               (mainTeams + i)->AverageBattingStrikerate,
               (mainTeams + i)->TotalPlayers);
    }

    printf("=========================================================\n");
}
