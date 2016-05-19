/*
 * SocketClient.cpp
 *
 *  Created on: Jan 21, 2016
 *      Author: hwj
 */

#include "SocketClient.h"

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "sys_def.h"

SocketClient::SocketClient(char *server_addr, int port) :
		m_sockfd(-1),
		m_epollfd(-1),
		m_server_addr(server_addr),
		m_port(port)

{
	init();
}

SocketClient::~SocketClient() {
}

void setnonblocking(int fd) {
	int opts;
	opts = fcntl(fd, F_GETFL);
	if (opts < 0) {
		perror("fcntl(F_GETFL)");
		exit(1);
	}
	opts = (opts | O_NONBLOCK);
	if (fcntl(fd, F_SETFL, opts) < 0) {
		perror("fcntl(F_SETFL)");
		exit(1);
	}
}

int SocketClient::writeMsg(int sockfd, char *buf, int len)
{
	int ret = write(sockfd, buf, len);
	DEBUG("write to %d  bytes %d, data length %d", sockfd, ret, len);
	return ret;
}

int SocketClient::writeMsgToWebSocket(int sockfd, char *buf, int len)
{
	char message[BUFF_SIZE];
	bzero(message, BUFF_SIZE);

	message[0] = 0x82;
	int len_pos = 2;

	if (len >= 126) {
		message[1] = 126;
		message[2] = (len >> 8) & 0xFF;
		message[3] = (len) & 0xFF;
		len_pos += 2;
	} else {
		message[1] = len & 0x7f;
	}
	memcpy(message + len_pos, buf, len);
	writeMsg(sockfd, message,  len + len_pos);
	return 0;
}

int SocketClient::init(void)
{
	struct sockaddr_in client_addr;
	bzero(&client_addr, sizeof(client_addr));
	client_addr.sin_family = AF_INET;
	client_addr.sin_addr.s_addr = htons(INADDR_ANY);
	client_addr.sin_port = htons(0);

	m_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sockfd < 0) {
		perror("Create Socket Failed!\n");
		exit(1);
	}

	signal(SIGPIPE, SIG_IGN);


	int on = 1;
	setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	struct sockaddr_in server_addr;
	bzero(&server_addr, sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	if (inet_aton(m_server_addr, &server_addr.sin_addr) == 0)
			{
		perror("Server IP Address Error!\n");
		exit(1);
	}
	server_addr.sin_port = htons(m_port);
	socklen_t server_addr_length = sizeof(server_addr);

	if (connect(m_sockfd, (struct sockaddr*) &server_addr,
			server_addr_length) < 0) {
		perror("Can Not Connect To!\n");
		exit(1);
	}
	setnonblocking(m_sockfd);
	struct epoll_event ev;
	m_epollfd = epoll_create(MAX_EVENTS);
	if (-1 == m_epollfd) {
		perror("epoll_create ERROR");
		return -1;
	}

	ev.events = EPOLLIN;
	ev.data.fd = m_sockfd;
	if (epoll_ctl(m_epollfd, EPOLL_CTL_ADD, m_sockfd, &ev) == -1) {
		perror("epoll_ctl: listen_sock ERROR");
		return -1;
	}
	return 0;
}

void SocketClient::process(void)
{
	struct epoll_event events[MAX_EVENTS];
	int nfds, nread, i, n;
	char buf[BUFF_SIZE];
	for (;;) {

		nfds = epoll_wait(this->m_epollfd, events, MAX_EVENTS, -1);
		if (nfds == -1) {
			DEBUG("epoll_pwait ERROR");
			exit(EXIT_FAILURE);
		}

		for (i = 0; i < nfds; ++i) {
			int sockfd = events[i].data.fd;

			if (events[i].events & EPOLLIN) {
				n = 0;
				nread = 0;
				bzero(buf, BUFF_SIZE);
				while ((nread = read(sockfd, buf + n, BUFF_SIZE - (n + 1))) > 0) {
					n += nread;
				}
				if (nread == -1&& errno != EAGAIN && errno != EWOULDBLOCK
				&& errno != EINTR) {
					DEBUG("read error: sockfd = %d, errno = %d", sockfd, errno);
					struct epoll_event event_del;
					event_del.data.fd = events[i].data.fd;
					event_del.events = events[i].events;
					epoll_ctl(this->m_epollfd, EPOLL_CTL_DEL, event_del.data.fd,
							&event_del);
					//disconnect(sockfd);
				} else {
					if (0 == n) {
						continue;
					}
					receiverMsg(sockfd, buf, n);
				}
			}
			if (events[i].events & EPOLLOUT) {

			}
		}
	}
}
