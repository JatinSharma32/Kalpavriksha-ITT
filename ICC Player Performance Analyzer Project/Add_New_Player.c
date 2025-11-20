#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "Players_data.h"
#include "Definitions.h"

void addNewPlayer()
{
    // User input
    printf("Enter Team ID to add player: ");
    int userInputTeamID;
    if (scanf("%d", &userInputTeamID) != 1 || userInputTeamID > 10 || userInputTeamID < 1)
    {
        printf("Team ID should be between 1 to 10.\n");
        return;
    }
    // Zero based indexing
    userInputTeamID--;

    printf("Enter Player Details:\nPlayer ID: ");
    int userInputPlayerID;
    if (scanf("%d", &userInputPlayerID) != 1 || userInputPlayerID > 1500 || userInputPlayerID < 1)
    {
        printf("User ID should be between 1 to 1500.\n");
        return;
    }

    printf("Name: ");
    char *userInputPlayerName = (char *)malloc(51 * sizeof(char));
    if (userInputPlayerName == NULL)
    {
        printf("Memory can't be allocated\n");
        exit(0);
    }

    scanf(" ");

    fgets(userInputPlayerName, 50, stdin);
    userInputPlayerName[strcspn(userInputPlayerName, "\n")] = '\0';
    if (strlen(userInputPlayerName) == 0)
    {
        printf("Invalid Player Name.\n");
        return;
    }

    printf("Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    int userInputRoleID;
    if (scanf("%d", &userInputRoleID) != 1 || userInputRoleID > 3 || userInputRoleID < 1)
    {
        printf("Role should be between 1 to 3.\n");
        return;
    }

    printf("Total Runs: ");
    int userInputTotalRuns;
    if (scanf("%d", &userInputTotalRuns) != 1)
    {
        printf("Invalid Runs.\n");
        return;
    }

    printf("Batting Average: ");
    float userInputBattingAverage;
    if (scanf("%f", &userInputBattingAverage) != 1)
    {
        printf("Invalid Batting Average.\n");
        return;
    }

    printf("Strike Rate: ");
    float userInputStrikeRate;
    if (scanf("%f", &userInputStrikeRate) != 1)
    {
        printf("Invalid Strike Rate.\n");
        return;
    }

    printf("Wickets: ");
    int userInputWickets;
    if (scanf("%d", &userInputWickets) != 1)
    {
        printf("Invalid Wickets.\n");
        return;
    }

    printf("Economy Rate: ");
    float userInputEconomyRate;
    if (scanf("%f", &userInputEconomyRate) != 1)
    {
        printf("Invalid Economy Rate.\n");
        return;
    }

    // Addition part
    MainPlayer *newPlayer = (MainPlayer *)malloc(sizeof(MainPlayer));
    if (newPlayer == NULL)
    {
        printf("Memory can't be allocated\n");
        exit(0);
    }
    newPlayer->PlayerId = userInputPlayerID;
    newPlayer->Name = (char *)malloc(51 * sizeof(char));
    if (newPlayer->Name == NULL)
    {
        printf("Memory can't be allocated\n");
        exit(0);
    }
    strcpy(newPlayer->Name, userInputPlayerName);
    newPlayer->TeamName = (char *)malloc(51 * sizeof(char));
    if (newPlayer->TeamName == NULL)
    {
        printf("Memory can't be allocated\n");
        exit(0);
    }
    strcpy(newPlayer->TeamName, (mainTeams + userInputTeamID)->Name);
    newPlayer->TotalRuns = userInputTotalRuns;
    newPlayer->BattingAverage = userInputBattingAverage;
    newPlayer->StrikeRate = userInputStrikeRate;
    newPlayer->Wickets = userInputWickets;
    newPlayer->EconomyRate = userInputEconomyRate;
    newPlayer->Role = (char *)malloc(51 * sizeof(char));
    if (newPlayer->Role == NULL)
    {
        printf("Memory can't be allocated\n");
        exit(0);
    }

    // Add the performance matrix
    if (userInputRoleID == 2)
    {
        strcpy(newPlayer->Role, "Bowler");
        newPlayer->PerformanceIndex = ((double)newPlayer->Wickets * 2) + (100 - newPlayer->EconomyRate);
    }
    else if (userInputRoleID == 1)
    {
        strcpy(newPlayer->Role, "Batsman");
        newPlayer->PerformanceIndex = ((double)newPlayer->BattingAverage * newPlayer->StrikeRate) / 100;
    }
    else if (userInputRoleID == 3)
    {
        strcpy(newPlayer->Role, "All-rounder");
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

    int teamNo = userInputTeamID;
    MainTeam *team = mainTeams + teamNo;
    insertInTeamList(team, newTeamMember);

    // Fixing the team order after nw player added
    fixTeamOrder(teamNo);
    printf("Player added successfully to Team %s!", (mainTeams + userInputTeamID)->Name);
}