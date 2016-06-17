/*
 * SocketProcess.cpp
 *
 *  Created on: Jun 13, 2016
 *      Author: hwj
 */

#include "SocketProcess.h"

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

#include "VncDataPackage.h"
#include "VncUtils.h"
#include "sys_def.h"
#include "CommandId.h"
#include "utils/logger.h"

SocketProcess::SocketProcess(char *server_addr, int port) :
	m_server_addr(server_addr),
	m_port(port),
	m_epollfd(-1),
	m_index(0)
{

}

SocketProcess::~SocketProcess() {

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

int addfd(int epollfd, int sockfd) {
	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = sockfd;
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev) == -1) {
		perror("epoll_ctl: listen_sock ERROR");
		return -1;
	}
	setnonblocking(sockfd);

	return 0;
}

void SocketProcess::process()
{
	m_epollfd = epoll_create(MAX_EVENTS);

	struct sockaddr_in server_addr;
	bzero(&server_addr, sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	if (inet_aton(m_server_addr, &server_addr.sin_addr) == 0) {
		perror("Server IP Address Error!\n");
		exit(1);
	}
	server_addr.sin_port = htons(m_port);
	socklen_t server_addr_length = sizeof(server_addr);

	signal(SIGPIPE, SIG_IGN);

	int i = 0;
	for (i = 0; i < CONNECT_LIMIT; ++i) {
		int sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd < 0) {
			perror("socket () : \n");
		}

		if (connect(sockfd, (struct sockaddr*) &server_addr, server_addr_length) < 0) {
			perror("connect () : \n");
		}
		addfd(m_epollfd, sockfd);
		this->do_register(sockfd);
	}

	struct epoll_event events[MAX_EVENTS];
		int nfds, nread, n;
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

void SocketProcess::handshake(int sockfd) {
	char buf[BUFF_SIZE];
	bzero(buf, BUFF_SIZE);

	sprintf(buf, "GET / HTTP/1.1\r\nHost: %s:%d\r\nUpgrade: websocket\r\nConnection: Upgrade\r\n\
				Sec-WebSocket-Version: 13\r\nSec-WebSocket-Key: jIVkkZ7w7y7rcSr22XKkhQ==\r\n\r\n",
				m_server_addr, m_port);

	writeMsg(sockfd, buf, strlen(buf));
}

int SocketProcess::writeMsg(int sockfd, char *buf, int len)
{
	int ret = write(sockfd, buf, len);
	DEBUG("write to %d  bytes %d, data length %d", sockfd, ret, len);
	return ret;
}

int SocketProcess::writeMsgToWebSocket(int sockfd, char *buf, int len)
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

int SocketProcess::receiverMsg(int sockfd, char *buf, int len)
{
	DEBUG("receive from sockfd %d message length %d", sockfd, len);
	//writeMsg(sockfd, buf, len);
	VncDataPackage * package = new VncDataPackage((const unsigned char* )buf, len);
	if (!package) {
		LOG_DEBUG("socket fd = %d package data is Error", sockfd);
		return -1;
	}
	unsigned int source = package->getSource();
	unsigned int target = package->getTarget();
	unsigned int command_id = package->getCommandId();
	LOG_DEBUG("socket fd = %d source = 0x%08x; target = 0x%08x, command id =  0x%08x", sockfd, source, target, command_id);
	if (SERVER_ID != source) {
		package->setSourceAndTarget(target, source);
		std::string returnbuf = package->getProtocolBuffer();
		write(sockfd, returnbuf.c_str(), returnbuf.length());
	}
	return 0;

}

#include <sstream>
int SocketProcess::do_register(int sockfd)
{
	int push_id = PUSH_BEGIN + m_index++;


	std::stringstream sstream;
	sstream << push_id;
	std::string server_id;
	sstream >> server_id;

	unsigned source = crc32_hash((const unsigned char*) server_id.c_str(),
			server_id.length());
	VncDataPackage package;
	package.setSourceAndTarget(source, SERVER_ID);
	package.setCommandId(T2S_CMD_REG_TV_TEST, 0);

	package.setStringParam(server_id);
	std::string returnbuf = package.getProtocolBuffer();
	writeMsg(sockfd, (char *) returnbuf.c_str(),
			returnbuf.length());

	return 0;
}
