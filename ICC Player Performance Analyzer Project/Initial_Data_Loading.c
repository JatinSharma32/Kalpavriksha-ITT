#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "Players_data.h"
#include "Definitions.h"

void initilizePlayers()
{
    for (int playerNo = 0; playerNo < INITIAL_PLAYER_COUNT; playerNo++)
    {
        // Adding new player to list
        MainPlayer *newPlayer = (MainPlayer *)malloc(sizeof(MainPlayer));
        if (newPlayer == NULL)
        {
            printf("Memory can't be allocated\n");
            exit(0);
        }
        newPlayer->PlayerId = (players + playerNo)->id;
        newPlayer->Name = (char *)malloc(51 * sizeof(char));
        if (newPlayer->Name == NULL)
        {
            printf("Memory can't be allocated\n");
            exit(0);
        }
        strcpy(newPlayer->Name, (players + playerNo)->name);
        newPlayer->TeamName = (char *)malloc(51 * sizeof(char));
        if (newPlayer->TeamName == NULL)
        {
            printf("Memory can't be allocated\n");
            exit(0);
        }
        strcpy(newPlayer->TeamName, (players + playerNo)->team);
        newPlayer->Role = (char *)malloc(51 * sizeof(char));
        if (newPlayer->Role == NULL)
        {
            printf("Memory can't be allocated\n");
            exit(0);
        }
        strcpy(newPlayer->Role, (players + playerNo)->role);
        newPlayer->TotalRuns = (players + playerNo)->totalRuns;
        newPlayer->BattingAverage = (players + playerNo)->battingAverage;
        newPlayer->StrikeRate = (players + playerNo)->strikeRate;
        newPlayer->Wickets = (players + playerNo)->wickets;
        newPlayer->EconomyRate = (players + playerNo)->economyRate;

        // Add the performance matrix
        if (strcmp(newPlayer->Role, "Bowler") == 0)
        {
            newPlayer->PerformanceIndex = ((double)newPlayer->Wickets * 2) + (100 - newPlayer->EconomyRate);
        }
        else if (strcmp(newPlayer->Role, "Batsman") == 0)
        {
            newPlayer->PerformanceIndex = ((double)newPlayer->BattingAverage * newPlayer->StrikeRate) / 100;
        }
        else if (strcmp(newPlayer->Role, "All-rounder") == 0)
        {
            newPlayer->PerformanceIndex = (((double)newPlayer->BattingAverage * newPlayer->StrikeRate) / 100) + (newPlayer->Wickets * 2);
        }

        insertInPlayerList(newPlayer);

        // Adding new player to team.
        PlayerInTeamNode *newTeamMember = (PlayerInTeamNode *)malloc(sizeof(PlayerInTeamNode));
        if (newTeamMember == NULL)
        {
            printf("Memory can't be allocated\n");
            exit(0);
        }
        newTeamMember->data = newPlayer;
        newTeamMember->next = NULL;
        newTeamMember->prev = NULL;
        int teamNo = findTeamNo(newPlayer->TeamName);
        MainTeam *team = mainTeams + teamNo;
        insertInTeamList(team, newTeamMember);

        // Fixing the team order after nw player added
        fixTeamOrder(teamNo);
    }
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