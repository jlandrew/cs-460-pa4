#include "client.h"

int main(void) {
    // set socket address
    struct sockaddr_in server_addr, sender_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    socklen_t sender_len = sizeof(sender_addr);

    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr);
    server_addr.sin_port = htons(PORT);

    // open socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd == -1) {
        perror("\n Failed to create socket \n");
        return 1;
    }

    // send empty packet to server
    sendto(sockfd, NULL, 0, 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // receive daytime information from server
    char buffer[100];
    recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&sender_addr, &sender_len);

    // if sender IP is not the same as the server IP,
    // discard current packet & continue until package is received
    while (sender_addr.sin_addr.s_addr != server_addr.sin_addr.s_addr) {
        recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&sender_addr, &sender_len);
    }

    // display information
	printf("%s", buffer);

    // close the socket
    close(sockfd);
    return 0;
}