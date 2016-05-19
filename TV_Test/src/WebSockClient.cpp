/*
 * WebSockClient.cpp
 *
 *  Created on: Jan 22, 2016
 *      Author: hwj
 */

#include <string.h>
#include <unistd.h>

#include "WebSockClient.h"
#include "VncDataPackage.h"
#include "sys_def.h"
#include "VncUtils.h"

WebSockClient::WebSockClient(char *server_addr, int port, int index)  :
	SocketClient(server_addr, port),
	m_index(index)
{

}

WebSockClient::~WebSockClient() {
}

int WebSockClient::receiverMsg(int sockfd, char *buf, int len)
{
	DEBUG("receive from sockfd %d message length %d", sockfd, len);
	if (0 != strstr(buf, "\r\n\r\n")) {
		do_register();
	} else {

	}

	return 0;
}

int WebSockClient::handshake(void)
{
	char buf[BUFF_SIZE];
	bzero(buf, BUFF_SIZE);

	sprintf(buf, "GET / HTTP/1.1\r\nHost: %s:%d\r\nUpgrade: websocket\r\nConnection: Upgrade\r\n\
			Sec-WebSocket-Version: 13\r\nSec-WebSocket-Key: jIVkkZ7w7y7rcSr22XKkhQ==\r\n\r\n", m_server_addr, m_port);

	writeMsg(m_sockfd, buf, strlen(buf));
	return 0;
}
#include <sstream>
int WebSockClient::do_register(void)
{
	int push_id = PUSH_BEGIN + m_index;

	std::stringstream sstream;
	sstream << push_id;
	std::string server_id;
	sstream >> server_id;

	unsigned source = crc32_hash((const unsigned char*)server_id.c_str(), server_id.length());
	VncDataPackage package;
	package.setSourceAndTarget(source, SERVER_ID);
	package.setCommandId(0x00000004, 0);

	package.setStringParam(server_id);
	std::string returnbuf = package.getProtocolBuffer();
	writeMsgToWebSocket(this->m_sockfd, (char *)returnbuf.c_str(), returnbuf.length());
	return 0;
}
