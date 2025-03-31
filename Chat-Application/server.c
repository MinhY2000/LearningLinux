#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define LISTEN_BACKLOG 10
#define BUFFER_SIZE 1024

typedef struct 
{
    int socket;
    struct sockaddr_in address;
    char ip[INET_ADDRSTRLEN];
    int port;
} Client;

Client clients[LISTEN_BACKLOG];
int client_count = 0;
int addr_size, opt;
int server_fd, new_socket_fd;
struct sockaddr_in server_addr, client_addr;


void print_help() 
{
    printf("Các lệnh hỗ trợ:\n");
    printf("help: Hiển thị hướng dẫn sử dụng.\n");
    printf("myip: Hiển thị địa chỉ IP của máy.\n");
    printf("myport: Hiển thị cổng mà server đang lắng nghe.\n");
    printf("connect <destination> <port>: Kết nối đến peer.\n");
    printf("list: Hiển thị danh sách các kết nối hiện tại.\n");
    printf("terminate <connection id>: Ngắt kết nối với peer.\n");
    printf("send <connection id> <message>: Gửi tin nhắn đến peer.\n");
    printf("exit: Đóng tất cả kết nối và thoát.\n");
}
void connect_to_peer(const char *ip, int port) 
{
    if (client_count >= LISTEN_BACKLOG) 
    {
        printf("Unable to connect, maximum connection limit reached.\n");
        return;
    }
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) 
    {
        printf("Can't create socket");
        return;
    }
    struct sockaddr_in peer_addr;
    peer_addr.sin_family = AF_INET;
    peer_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &peer_addr.sin_addr) <= 0) 
    {
        printf("Socket creation failed\n");
        close(client_socket);
        return;
    }

    if (connect(client_socket, (struct sockaddr*)&peer_addr, sizeof(peer_addr)) < 0) {
        perror("Unable to connect to peer\n");
        close(client_socket);
        return;
    }
    clients[client_count].socket = client_socket;
    clients[client_count].address = peer_addr;
    client_count++;
    printf("Connected successfully to %s:%d\n", ip, port);
}
void terminate_connection(int connection_id) 
{
    if (connection_id < 0 || connection_id >= client_count) 
    {
        printf("Invalid connection ID.\n");
        return;
    }
    close(clients[connection_id].socket);
    printf("Disconnected from %s:%d\n", clients[connection_id].ip, clients[connection_id].port);
    for (int i = connection_id; i < client_count - 1; i++) 
    {
        clients[i] = clients[i + 1];
    }
    client_count--;
}
void *client_handler(void *arg) 
{
    int client_socket = *((int *)arg);
    char buffer[BUFFER_SIZE];
    int len;

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        len = recv(client_socket, buffer, sizeof(buffer), 0);
        if (len <= 0) {
            break;
        }
        printf("Receive messages from clients: %s\n", buffer);
    }
    close(client_socket);
}
void server_init(int port)
{
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) 
    {
        printf("Fail create server socket\n");
        exit(1);
    }
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) 
    {
        printf("Can't bind socket\n");
        exit(1);
    }

    if (listen(server_fd, LISTEN_BACKLOG) < 0) 
    {
        printf("Can't listen on socket\n");
        exit(1);
    }
    printf("Server's listening on port: %d\n", port);
}
void send_message(int connection_id, char* message) 
{
    if (connection_id < 0 || connection_id >= client_count) 
    {
        printf("Invalid connection ID.\n");
        return;
    }

    send(clients[connection_id].socket, message, strlen(message), 0);
    printf("Message sent to %s:%d\n", clients[connection_id].ip, clients[connection_id].port);
}
void print_my_ip_and_port(int server_socket, char name[]) 
{
    struct sockaddr_in addr;
    int len;
    if (getsockname(server_socket, (struct sockaddr*)&addr, (socklen_t *)&len) == -1) 
    {
        printf("getsockname failed");
        return;
    }
    if (strncmp(name, "myip", 4) == 0)
    {
        printf("IP of server: %s\n", inet_ntoa(addr.sin_addr));
    }else if (strncmp(name, "myport", 6) == 0) 
    {
        printf("Port of server: %d\n", ntohs(addr.sin_port));
    }
}
void list_connections() 
{
    if (client_count == 0) 
    {
        printf("No connection yet.\n");
        return;
    }
    printf("List of connections:\n");
    for (int i = 0; i < client_count; i++) {
        printf("ID: %d, IP: %s, Port: %d\n", i, clients[i].ip, clients[i].port);
    }
}
void control(int server_socket)
{
    while (1)
    {
        char command[BUFFER_SIZE];
        printf("Enter command: ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = '\0';  
        if (strncmp(command, "help", 4) == 0) 
        {
            print_help();
        } else if (strncmp(command, "myip", 4) == 0) 
        {
            print_my_ip_and_port(server_fd, "myip");
        } else if (strncmp(command, "myport", 6) == 0) 
        {
            print_my_ip_and_port(server_fd, "myport");
        } else if (strncmp(command, "connect", 7) == 0) 
        {
            char ip[INET_ADDRSTRLEN];
            int dest_port;
            sscanf(command + 8, "%s %d", ip, &dest_port);
            connect_to_peer(ip, dest_port);
        } else if (strncmp(command, "list", 4) == 0) 
        {
            list_connections();
        } else if (strncmp(command, "terminate", 9) == 0) 
        {
            int id;
            sscanf(command + 10, "%d", &id);
            terminate_connection(id);
        } else if (strncmp(command, "send", 4) == 0) 
        {
            int id;
            char message[BUFFER_SIZE];
            sscanf(command + 5, "%d %[^\n]", &id, message);
            send_message(id, message);
        } else if (strncmp(command, "exit", 4) == 0) 
        {
            for (int i = 0; i < client_count; i++) 
            {
                close(clients[i].socket);
            }
            close(server_socket);
            printf("Program exited.\n");
            exit(1);
        } else printf("Invalid command. Type 'help' for instructions.\n");
    }
}
int main()
{
    pthread_t client_id, control_choose;
    server_init(8080);
    while (1) 
    {
        new_socket_fd = accept(server_fd, (struct sockaddr *)&client_addr, (socklen_t *)&addr_size);
        if (new_socket_fd < 0) 
        {
            printf("Accept failed\n");
            continue;
        }
        if (client_count < LISTEN_BACKLOG) 
        {
            clients[client_count].socket = new_socket_fd;
            clients[client_count].address = client_addr;
            inet_ntop(AF_INET, &(client_addr.sin_addr), clients[client_count].ip, INET_ADDRSTRLEN);
            clients[client_count].port = ntohs(client_addr.sin_port);
            client_count++;
            printf("New connection from %s:%d\n", clients[client_count - 1].ip, clients[client_count - 1].port);

            pthread_create(&client_id, NULL, client_handler, &new_socket_fd);
        } else 
        {
            printf("Maximum client connections reached, connections refused.\n");
            close(new_socket_fd);
        }
        pthread_create(&control_choose, NULL, control, NULL);
        pthread_join(control_choose, NULL);
    }
    close(server_fd);
    return 0;
}