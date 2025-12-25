#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main()
{
    int sock = 0;
    struct sockaddr_in serv_addr;
    int choice, amount, success, balance;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    while (1)
    {
        printf("\n--- ATM Menu ---\n");
        printf("1. Withdraw\n");
        printf("2. Deposit\n");
        printf("3. Display Balance\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        send(sock, &choice, sizeof(int), 0);

        if (choice == 4)
        {
            printf("Exiting...\n");
            break;
        }

        if (choice == 1)
        {
            printf("Enter amount to withdraw: ");
            scanf("%d", &amount);
            send(sock, &amount, sizeof(int), 0);
        }
        else if (choice == 2)
        {
            printf("Enter amount to deposit: ");
            scanf("%d", &amount);
            send(sock, &amount, sizeof(int), 0);
        }

        recv(sock, &success, sizeof(int), 0);
        recv(sock, &balance, sizeof(int), 0);

        if (choice == 1)
        {
            if (success)
            {
                printf("Withdrawal successful. New Balance: %d\n", balance);
            }
            else
            {
                printf("Insufficient funds. Current Balance: %d\n", balance);
            }
        }
        else if (choice == 2)
        {
            printf("Deposit successful. New Balance: %d\n", balance);
        }
        else if (choice == 3)
        {
            printf("Current Balance: %d\n", balance);
        }
    }

    close(sock);
    return 0;
}