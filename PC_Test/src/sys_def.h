#ifndef __SYS_DEF__
#define __SYS_DEF__

#include <stdio.h>

#define __DEBUG__

#ifndef __DEBUG__
#define DEBUG(format,...) printf("File: " __FILE__ ", Line: %05d: " format "\n", __LINE__, ##__VA_ARGS__)
#else
#define DEBUG(format,...)
#endif

#define MAX_EVENTS 100
#define BUFF_SIZE 1024 * 10

#define WEBSOCK_PORT 9000
#define TCPSOCK_PORT 9001
#define PHPSOCK_PORT 9002
#define FILESOCK_PORT 9003
#define LOGSOCK_PORT 9004
#define LOGWEBSOCK_PORT 9005
#define SERVER_ID 0000
#define FILE_PATH "/var/www/"
#define PUSH_ID "1111"

//#define IP_ADDR "223.202.11.125"
#define IP_ADDR "192.168.1.25"
#define PUSH_BEGIN 10000

#define CONNECT_LIMIT 1

#define DEFAULT_LOG_PATH ""
#define DEFAULT_LOG_FILE "tvdoctor.log"
#define DEFAULT_LOG_MAXSIZE 1024 * 10

#define LOG_DEBUG_LEVEL 4

#endif
