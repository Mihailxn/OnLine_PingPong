#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

struct cntmsg{
	char nickname[15];
	short usr;
};

struct getmsg{
	char nick[15];
};

int main(){
	struct sockaddr_in serv_addr, cli_addr;
	struct cntmsg test;
	struct getmsg getm;
	short num = 1;
	int sockfd, i, slen = sizeof(cli_addr), recv_len, portnum = 8888;
	char buf[15];
	sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portnum);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	fflush(stdout);
	while (num <= 30){
		recv_len = recvfrom(sockfd, &getm, sizeof(getm), MSG_WAITALL, (struct sockaddr *)&cli_addr, &slen);
		if (recv_len > 0){
			printf("Got msg: %s", getm.nick);
			strncpy (test.nickname, getm.nick, sizeof(getm.nick));
			test.usr = num;
			sendto(sockfd, &test, sizeof(test), 0, (struct sockaddr *)&cli_addr, slen);
			num++;
		}
	}
	return 0;
}