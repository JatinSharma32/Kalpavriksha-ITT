#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "Players_data.h"
#include "Definitions.h"

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