/*
 * TCPSockClient.h
 *
 *  Created on: Jan 26, 2016
 *      Author: hwj
 */

#ifndef SRC_TCPSOCKCLIENT_H_
#define SRC_TCPSOCKCLIENT_H_

#include "SocketClient.h"

class TCPSockClient: public SocketClient {
public:
	TCPSockClient(char *server_addr, int port, int index);
	virtual ~TCPSockClient();

	int receiverMsg(int sockfd, char *buf, int len);

	int do_register(void);
private:
	int m_index;
};

#endif /* SRC_TCPSOCKCLIENT_H_ */
