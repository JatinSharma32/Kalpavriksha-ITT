#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define DB_FILE "../../resource/accountDB.txt"

pthread_mutex_t lock;

void *client_handler(void *socket_desc)
{
    int new_socket = *(int *)socket_desc;
    int choice, amount;
    int current_balance;
    int success = 0;
    FILE *file;

    free(socket_desc);

    while (1)
    {
        if (recv(new_socket, &choice, sizeof(int), 0) <= 0)
            break;

        if (choice == 4)
            break;

        pthread_mutex_lock(&lock);

        file = fopen(DB_FILE, "r");
        if (file)
        {
            fscanf(file, "%d", &current_balance);
            fclose(file);
        }
        else
        {
            current_balance = 0;
        }

        if (choice == 1)
        {
            recv(new_socket, &amount, sizeof(int), 0);
            if (amount > current_balance)
            {
                success = 0;
            }
            else
            {
                current_balance -= amount;
                success = 1;
            }
        }
        else if (choice == 2)
        {
            recv(new_socket, &amount, sizeof(int), 0);
            current_balance += amount;
            success = 1;
        }
        else if (choice == 3)
        {
            success = 1;
        }

        if (choice == 1 && success)
        {
            file = fopen(DB_FILE, "w");
            fprintf(file, "%d", current_balance);
            fclose(file);
        }
        else if (choice == 2)
        {
            file = fopen(DB_FILE, "w");
            fprintf(file, "%d", current_balance);
            fclose(file);
        }

        pthread_mutex_unlock(&lock);

        send(new_socket, &success, sizeof(int), 0);
        send(new_socket, &current_balance, sizeof(int), 0);
    }

    close(new_socket);
    return NULL;
}

int main()
{
    int server_fd, new_socket, *new_sock;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("Mutex init failed\n");
        return 1;
    }

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    while (1)
    {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("Accept failed");
            continue;
        }

        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = new_socket;

        if (pthread_create(&sniffer_thread, NULL, client_handler, (void *)new_sock) < 0)
        {
            perror("Could not create thread");
            return 1;
        }
    }

    pthread_mutex_destroy(&lock);
    return 0;
}