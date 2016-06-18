/*
 * SocketProcess.h
 *
 *  Created on: Jun 13, 2016
 *      Author: hwj
 */

#ifndef SRC_SOCKETPROCESS_H_
#define SRC_SOCKETPROCESS_H_

class SocketProcess {
public:
	SocketProcess(char *server_addr, int port);
	virtual ~SocketProcess();

	void process();

private:
	void handshake(int sockfd);

	int acceptConnect(int sockfd);
	int disconnect(int sockfd);

	int writeMsg(int sockfd, char *buf, int len);
	int writeMsgToWebSocket(int sockfd, char *buf, int len);

	int receiverMsg(int sockfd, char *buf, int len);

	int do_register(int sockfd);
	char *m_server_addr;
	int m_port;
	int m_epollfd;
	int m_index;
};

#endif /* SRC_SOCKETPROCESS_H_ */
