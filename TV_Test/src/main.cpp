/*
 * main.cpp
 *
 *  Created on: Dec 14, 2015
 *      Author: hwj
 */

#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <pthread.h>

#include "TCPSockClient.h"

#include "sys_def.h"

static char *server_addr = NULL;
static int port = 0;

static void init_daemon(const char *dir)
{
	int pid = fork();
	if (pid)
		exit(0);
	else if (pid < 0)
		exit(1);

	setsid();
	pid = fork();
	if (pid)
		exit(0);
	else if (pid < 0)
		exit(1);

	umask(0);

	int fd = open("/dev/null", O_RDWR);
	if (fd == -1) {
		DEBUG("open(\"/dev/null\") failed");
		return;
	}

	if (dup2(fd, STDIN_FILENO) == -1) {
		DEBUG("dup2(STDIN) failed");
		return;
	}

	if (dup2(fd, STDOUT_FILENO) == -1) {
		DEBUG("dup2(STDOUT) failed");
		return;
	}
	if (fd > STDERR_FILENO) {
		if (close(fd) == -1) {
			DEBUG("close() failed");
			return;
		}
	}
}

volatile static int index = 0;
static void *start_proc(void *arg) {

	TCPSockClient *client = new TCPSockClient(server_addr, port, index++);

	//client->handshake();
	client->do_register();
	client->process();

	return NULL;
}

int main(int argc, char *argv[])
{
	//SocketClient *client = new SocketClient(argv[1], atoi(argv[2]));
	server_addr = argv[1];
	port = atoi(argv[2]);
//	WebSockClient *client = new WebSockClient(argv[1], atoi(argv[2]));
//	client->handshake();
//	client->process();

	pthread_t webclient[PTHREAD_LIMIT];
	int i = 0;
	for(i = 0; i < PTHREAD_LIMIT; ++i) {
		pthread_create(&(webclient[i]), NULL, start_proc, NULL);
	}

	for(i = 0; i < PTHREAD_LIMIT; ++i) {
		//pthread_create(&(webclient[i]), NULL, start_proc, NULL);
		pthread_join(webclient[i], 0);
	}


	return 0;
}


