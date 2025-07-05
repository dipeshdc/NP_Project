#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int server_fd, client_fd;
    struct sockaddr_in addr;
    char buffer[BUFFER_SIZE];
    int port = atoi(argv[1]);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 5) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", port);

    client_fd = accept(server_fd, NULL, NULL);
    if (client_fd < 0) {
        perror("accept");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Client connected.\n");

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes = read(client_fd, buffer, BUFFER_SIZE);
        if (bytes <= 0 || strncmp(buffer, "exit", 4) == 0)
            break;

        FILE *fp = popen(buffer, "r");
        if (!fp) {
            snprintf(buffer, BUFFER_SIZE, "Failed to execute command.\n");
            write(client_fd, buffer, strlen(buffer));
        } else {
            while (fgets(buffer, BUFFER_SIZE, fp)) {
                write(client_fd, buffer, strlen(buffer));
            }
            pclose(fp);
        }

        write(client_fd, "__END__", strlen("__END__"));
    }

    close(client_fd);
    close(server_fd);
    return 0;
}

