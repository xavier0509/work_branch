/*
 * SocketClient.h
 *
 *  Created on: Jan 21, 2016
 *      Author: hwj
 */

#ifndef SRC_SOCKETCLIENT_H_
#define SRC_SOCKETCLIENT_H_


class SocketClient {
public:
	SocketClient(char *server_addr, int port);
	virtual ~SocketClient();

	void process(void);

	virtual int receiverMsg(int sockfd, char *buf, int len) = 0;

	int writeMsg(int sockfd, char *buf, int len);
	int writeMsgToWebSocket(int sockfd, char *buf, int len);
protected:
	int m_sockfd;
	int m_epollfd;
	char *m_server_addr;
	int m_port;
private:
	int init(void);
};

#endif /* SRC_SOCKETCLIENT_H_ */
