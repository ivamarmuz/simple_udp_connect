#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define SERVER_ADDR "192.168.1.2"
#define PORT 7890

int main(int argc, char **argv)
{
    int fd;
    char addr[16] = SERVER_ADDR;
    struct sockaddr_in server_addr;
    socklen_t slen = sizeof(server_addr);
    unsigned char buff[BUFFER_SIZE];

    if (argc > 1) {
        strncpy(addr, argv[1], 16);
    }

    fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (fd == -1) {
        perror("Socket error");
    }

    server_addr.sin_family = AF_INET;
    inet_aton(addr, &server_addr.sin_addr);
    server_addr.sin_port = htons(PORT);

    if ((bind(fd, (struct sockaddr*)&server_addr, slen)) == -1) {
        perror("Bind error");
    }

    printf("Connected to %s on port %d...\n > ", addr, PORT);
    fgets(buff, BUFFER_SIZE - 1, stdin);

    while (1) {
        sendto(fd, buff, BUFFER_SIZE, 0, (struct sockaddr *)&server_addr, slen);
        if (!strncmp("/quit", buff, 5)) {
            close(fd);
            exit(1);
        }
        printf(" > ");
        fgets(buff, BUFFER_SIZE - 1, stdin);
    }

    return 0;
}