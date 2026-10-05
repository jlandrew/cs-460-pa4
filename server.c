#include "server.h"

int main(void) {
    // set socket address for the server
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // create socket
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("Could not create socket.\n");
		return 1;
	}

    // bind socket
    if ((bind(sockfd, (const struct sockaddr *)&server_addr, 
        sizeof(server_addr))) < 0) {
            printf("Could not bind socket.\n");
            return 1;
	}
    else {
        printf("UDP server listening on port %d...\n", PORT);
    }

    // store client address
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[100];

    // receive data packet from client
    int n = recvfrom(sockfd, (char *)buffer, 100, MSG_WAITALL,
        (struct sockaddr *)&client_addr, &addr_len);
    buffer[n] = '\0';

    // server loop
	while (true) {
        // get daytime info
        time_t now = time(NULL);                // get time in seconds
        struct tm *tm_info = gmtime(&now);      // convert time to UTC

        // format: YY-MM-DD HH:MM:SS
        // %y = 2-digit year, %m = 2-digit month, %d = 2-digit day
        // %H = 24-hour hour, %M = minutes, %S = seconds
        strftime(buffer, sizeof(buffer), "Daytime Server\n\n%y-%m-%d %H:%M:%S UTC\n\nDone.\n", tm_info);

		// send daytime info to the client
		sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr,
		    sizeof(client_addr));
	}
    return 0;
}