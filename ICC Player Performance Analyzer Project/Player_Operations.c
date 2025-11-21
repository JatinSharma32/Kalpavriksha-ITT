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
    strcpy(newPlayer->TeamName, (mainTeams + findTeamNo(teams[userInputTeamID]))->Name);
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

    int teamNo = findTeamNo(teams[userInputTeamID]);
    MainTeam *team = mainTeams + teamNo;
    insertInTeamList(team, newTeamMember);

    // Fixing the team order after nw player added
    fixTeamOrder(teamNo);
    printf("Player added successfully to Team %s!", (mainTeams + findTeamNo(teams[userInputTeamID]))->Name);
}

// Add player to global player list in decending order of performance index
void insertInPlayerList(MainPlayer *newPlayer)
{
    // If the list is empty, insert as the first node
    if (mainPlayerList->head == NULL)
    {
        mainPlayerList->head = newPlayer;
        mainPlayerList->tail = newPlayer;
        newPlayer->prev = NULL;
        newPlayer->next = NULL;
        return;
    }

    MainPlayer *curr = mainPlayerList->head;

    // the correct position in descending order
    while (curr != NULL && curr->PerformanceIndex > newPlayer->PerformanceIndex)
    {
        curr = curr->next;
    }

    if (curr == mainPlayerList->head)
    {
        // Insert at the start
        newPlayer->next = mainPlayerList->head;
        newPlayer->prev = NULL;
        mainPlayerList->head->prev = newPlayer;
        mainPlayerList->head = newPlayer;
    }
    else if (curr == NULL)
    {
        // Insert at the last
        newPlayer->next = NULL;
        newPlayer->prev = mainPlayerList->tail;
        mainPlayerList->tail->next = newPlayer;
        mainPlayerList->tail = newPlayer;
    }
    else
    {
        // Insert in the middle
        newPlayer->next = curr;
        newPlayer->prev = curr->prev;
        curr->prev->next = newPlayer;
        curr->prev = newPlayer;
    }
}

void displayTopKPlayers()
{
    printf("Enter Team ID: ");
    int userInputTeamID;
    if (scanf("%d", &userInputTeamID) != 1 || userInputTeamID > 10 || userInputTeamID < 1)
    {
        printf("Team ID should be between 1 to 10.\n");
        return;
    }
    userInputTeamID--;

    printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    int userInputRoleID;
    if (scanf("%d", &userInputRoleID) != 1 || userInputRoleID > 3 || userInputRoleID < 1)
    {
        printf("Role should be between 1 to 3.\n");
        return;
    }

    printf("Enter number of players: ");
    int k;
    if (scanf("%d", &k) != 1 || k < 1)
    {
        printf("Number of players should be positive.\n");
        return;
    }

    const char *roleStr;
    if (userInputRoleID == 1)
        roleStr = "Batsman";
    else if (userInputRoleID == 2)
        roleStr = "Bowler";
    else
        roleStr = "All-rounder";

    printf("Top %d %s of Team %s:\n", k, roleStr, (mainTeams + userInputTeamID)->Name);
    printf("====================================================================================\nID\tName\tRole\tRuns\tAvg\tSR\tWkts\tER\tPerf.Index\n====================================================================================\n");

    int count = 0;
    int teamNo = findTeamNo(teams[userInputTeamID]);
    PlayerInTeamNode *temp = (mainTeams + teamNo)->TeamListHead;
    while (temp != NULL && count < k)
    {
        if (strcmp(temp->data->Role, roleStr) == 0)
        {
            printf("%d\t%s\t%s\t%d\t%.2f\t%.2f\t%d\t%.2f\t%.2f\n",
                   temp->data->PlayerId,
                   temp->data->Name,
                   temp->data->Role,
                   temp->data->TotalRuns,
                   temp->data->BattingAverage,
                   temp->data->StrikeRate,
                   temp->data->Wickets,
                   temp->data->EconomyRate,
                   temp->data->PerformanceIndex);
            count++;
        }
        temp = temp->next;
    }
    printf("====================================================================================\n");
}

void displayAllPlayers()
{
    printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    int userInputRoleID;
    if (scanf("%d", &userInputRoleID) != 1 || userInputRoleID > 3 || userInputRoleID < 1)
    {
        printf("Role should be between 1 to 3.\n");
        return;
    }

    const char *roleStr;
    if (userInputRoleID == 1)
        roleStr = "Batsman";
    else if (userInputRoleID == 2)
        roleStr = "Bowler";
    else
        roleStr = "All-rounder";

    printf("%s of all teams:\n", roleStr);
    printf("======================================================================================\nID\tName\tTeam\tRole\tRuns\tAvg\tSR\tWkts\tER\tPerf.Index\n======================================================================================\n");

    MainPlayer *temp = mainPlayerList->head;
    while (temp != NULL)
    {
        if (strcmp(temp->Role, roleStr) == 0)
        {
            printf("%d\t%s\t%s\t%s\t%d\t%.2f\t%.2f\t%d\t%.2f\t%.2f\n",
                   temp->PlayerId,
                   temp->Name,
                   temp->TeamName,
                   temp->Role,
                   temp->TotalRuns,
                   temp->BattingAverage,
                   temp->StrikeRate,
                   temp->Wickets,
                   temp->EconomyRate,
                   temp->PerformanceIndex);
        }
        temp = temp->next;
    }
    printf("======================================================================================\n");
}

void displayAllPlayersOfTeam()
{
    printf("Enter Team ID: ");
    int userInputTeamID;
    if (scanf("%d", &userInputTeamID) != 1 || userInputTeamID > 10 || userInputTeamID < 1)
    {
        printf("Team ID should be between 1 to 10.\n");
        return;
    }

    userInputTeamID--;
    printf("====================================================================================\nID\tName\tRole\tRuns\tAvg\tSR\tWkts\tER\tPerf.Index\n====================================================================================\n");

    int teamNo = findTeamNo(teams[userInputTeamID]);
    PlayerInTeamNode *temp = (mainTeams + teamNo)->TeamListHead;
    while (temp != NULL)
    {
        printf("%d\t%s\t%s\t%d\t%.2f\t%.2f\t%d\t%.2f\t%.2f\n", temp->data->PlayerId, temp->data->Name, temp->data->Role, temp->data->TotalRuns, temp->data->BattingAverage, temp->data->StrikeRate, temp->data->Wickets, temp->data->EconomyRate, temp->data->PerformanceIndex);
        temp = temp->next;
    }
    printf("Total Players: %d\nAverage Batting Strike Rate: %2f\n", (mainTeams + teamNo)->TotalPlayers, (mainTeams + teamNo)->AverageBattingStrikerate);
}
