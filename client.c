#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server_ip> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sock, port;
    struct sockaddr_in server;
    char buffer[BUFFER_SIZE];

    port = atoi(argv[2]);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(argv[1]);

    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        perror("connect");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Connected to %s:%d\n", argv[1], port);

    while (1) {
        printf("Command> ");
        fgets(buffer, BUFFER_SIZE, stdin);
        write(sock, buffer, strlen(buffer));

        if (strncmp(buffer, "exit", 4) == 0)
            break;

        memset(buffer, 0, BUFFER_SIZE);

        while (1) {
            int bytes = read(sock, buffer, BUFFER_SIZE - 1);
            if (bytes <= 0)
                break;
            buffer[bytes] = '\0';

            char *end_marker = strstr(buffer, "__END__");
            if (end_marker != NULL) {
                *end_marker = '\0';
                printf("%s", buffer);
                break;
            }

            printf("%s", buffer);
            memset(buffer, 0, BUFFER_SIZE);
        }
    }

    close(sock);
    return 0;
}

