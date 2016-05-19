/*
 * WebSockClient.h
 *
 *  Created on: Jan 22, 2016
 *      Author: hwj
 */

#ifndef SRC_WEBSOCKCLIENT_H_
#define SRC_WEBSOCKCLIENT_H_

#include "SocketClient.h"

class WebSockClient: public SocketClient {
public:
	WebSockClient(char *server_addr, int port, int index);
	virtual ~WebSockClient();

	virtual int receiverMsg(int sockfd, char *buf, int len);

	int handshake(void);

private:
	int do_register(void);
	int m_index;
};

#endif /* SRC_WEBSOCKCLIENT_H_ */
