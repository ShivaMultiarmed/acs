#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

enum ControllerInteractionType : uint8_t
{
    ENTER, EXIT
};

struct ControllerInteractionRequest
{
    uint32_t key_id;
    uint16_t object_id;
    enum ControllerInteractionType type;
};

enum ControllerInteractionFailure : uint8_t
{
    NONE, NOT_AUTHENTICATED, NOT_FOUND, FORBIDDEN, NOT_EXPECTED
};

struct ControllerInteractionResponse
{
    enum ControllerInteractionFailure failure;
};

void* handle_client(void* arg)
{
    const uint32_t client_socket = (uint32_t) arg;
    while (1)
    {
        struct ControllerInteractionRequest request;
        int total_bytes_count = 0;
        while (total_bytes_count < sizeof(struct ControllerInteractionRequest))
        {
            int bytes_received = recv(client_socket, &request, sizeof(request), 0);
            if (bytes_received == -1) continue;
            if (bytes_received == 0) goto exit;
            total_bytes_count += bytes_received;
        }
        // TODO: handle the request
        struct ControllerInteractionResponse response = {NONE}; // TODO: replace by the actual failure
        send(client_socket, &response, sizeof(response), 0);
    }
    exit:
    close(client_socket);
    return 0;
}

void run_controller_server(void)
{
    const uint32_t con_count = 10;
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (!server_socket)
    {
        printf("Socket failed to open.\n");
        return;
    }
    struct sockaddr_in * addr;
    if (bind(server_socket, (struct sockaddr*) addr, sizeof(addr)) != 0)
    {
        printf("Failed to resolve the addr.\n");
        goto exit;
    }
    if (listen(server_socket, con_count) != 0)
    {
        printf("Failed to start listening.\n");
        goto exit;
    }
    while (1)
    {
        socklen_t client_addr_len;
        int client_socket = accept(server_socket, (struct sockaddr*) addr, &client_addr_len);
        if (client_socket == 0)
        {
            printf("A problem occurred while accepting a client.");
            continue;
        }
        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handle_client, (void*) client_socket) == 0)
        {
            printf("A problem occurred while accepting a client.");
            continue;
        }
        pthread_detach(thread_id);
    }
exit:
    close(server_socket);
}