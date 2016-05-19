/*
 * TCPSockClient.cpp
 *
 *  Created on: Jan 26, 2016
 *      Author: hwj
 */

#include "TCPSockClient.h"
#include "VncDataPackage.h"
#include "VncUtils.h"
#include "sys_def.h"
#include <sstream>

TCPSockClient::TCPSockClient(char *server_addr, int port, int index) :
	SocketClient(server_addr, port),
	m_index(index)
{

}

TCPSockClient::~TCPSockClient() {
}

int TCPSockClient::receiverMsg(int sockfd, char *buf, int len) {
	DEBUG("receive from sockfd %d message length %d", sockfd, len);
	return 0;
}

int TCPSockClient::do_register(void)
{
	int push_id = PUSH_BEGIN + m_index;

	std::stringstream sstream;
	sstream << push_id;
	std::string server_id;
	sstream >> server_id;

	unsigned source = crc32_hash((const unsigned char*) server_id.c_str(),
			server_id.length());
	VncDataPackage package;
	package.setSourceAndTarget(source, SERVER_ID);
	package.setCommandId(0x00010004, 0);

	package.setStringParam(server_id);
	std::string returnbuf = package.getProtocolBuffer();
	writeMsg(this->m_sockfd, (char *) returnbuf.c_str(),
			returnbuf.length());
	return 0;
}
