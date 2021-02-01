#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define BUFF_SIZE 1024
#define PORT 7890

int main(void)
{
    int fd;
    unsigned char buff[BUFF_SIZE];
    struct sockaddr_in client_addr, server_addr;
    socklen_t slen = sizeof(client_addr);

    fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (fd == -1) {
        perror("Socket error");
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if ((bind(fd, (struct sockaddr *)&server_addr, slen)) == -1) {
        perror("Bind error");
    }

    printf("Server was started on port %d...\n", PORT);

    recvfrom(fd, buff, BUFF_SIZE, 0, (struct sockaddr *)&client_addr, &slen);

    while (1) {
        if (!strncmp("/quit", buff, 5)) {
            close(fd);
            exit(1);
        }
        printf(" > %s", buff);
        recvfrom(fd, buff, BUFF_SIZE, 0, (struct sockaddr *)&client_addr, &slen);
    }
    
    return 0;
}