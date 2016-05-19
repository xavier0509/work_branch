
#include <stddef.h>

#ifndef COOCAA_OS_VNC_COMMAND_ID_H_
#define COOCAA_OS_VNC_COMMAND_ID_H_

#define T2S_CMD_HEARTBEAT                               (0x00010002)
#define T2S_CMD_REG_TV                                  (0x00010004)
#define T2S_RET_REG_TV                                  (0x00010005)
#define T2S_CMD_TELLME_SRV_ID                           (0x00010006)
#define T2S_RET_TELLME_SRV_ID                           (0x00010007)
#define T2S_CMD_TELLME_PC_ID                            (0x00010008)
#define T2S_RET_TELLME_PC_ID                            (0x00010009)
#define T2S_CMD_NOTIFY_PC_OFFLINE                       (0x0001000A)
#define T2S_CMD_REJECT_HELP                             (0x0001000C)

#define T2S_CMD_SEND_VIRKEY                             (0x00020000)	
#define T2S_RET_SEND_VIRKEY                             (0x00020001)
#define T2P_CMD_START_TELNETD                           (0x00020002)
#define T2P_RET_START_TELNETD                           (0x00020003)
#define T2P_CMD_STOP_TELNETD                            (0x00020004)
#define T2P_RET_STOP_TELNETD                            (0x00020005)
#define T2P_CMD_SNATCH_LOG                              (0x00020006)
#define T2P_RET_SNATCH_LOG                              (0x00020007)
#define T2P_CMD_START_SNATCH_LOG                        (0x00020008)
#define T2P_CMD_TELNET_DATA                             (0x0002000A)
#define T2P_CMD_PRINTE_SCREEN                           (0x0002000C)
#define T2P_RET_PRINTE_SCREEN                           (0x0002000D)
#define T2P_CMD_PRINTE_CONTINUE_SCREEN                  (0x0002000E)
#define T2P_CMD_STOP_SCREEN                             (0x00020010)
#define T2P_CMD_STOP_SNATCH_LOG                         (0x00020012)
#define T2P_CMD_REMOTE_PULL_FILE                        (0x00020014)
#define T2P_RET_REMOTE_PULL_FILE						(0x00020015)
#define T2P_CMD_REMOTE_PUSH_FILE						(0x00020016)
#define T2P_RET_REMOTE_PUSH_FILE						(0x00020017)


#define T2P_CMD_START_PRINT_LOG							(0x00020018)
#define T2P_CMD_PARAM_PRINT_LOG							(0x0002001A)
#define T2P_CMD_STOP_PRINT_LOG							(0x0002001C)


#define T2P_CMD_LOGCAT_START_SCREEN                     (0x00020018)
#define T2P_RET_LOGCAT_START_SCREEN                     (0x00020019)
#define T2P_CMD_LOGCAT_PARAM_SCREEN                     (0x0002001A)
#define T2P_RET_LOGCAT_PARAM_SCREEN                     (0x0002001B)
#define T2P_CMD_LOGCAT_STOP_SCREEN                      (0x0002001C)

#endif


