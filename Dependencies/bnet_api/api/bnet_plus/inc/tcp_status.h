//*****************************************************************************
// tcp_status.h - Status info for TCP Interface
//
// Author: Bryan Womack
// Date: October 6, 2017
// Company: Echodyne Corp.
//
//*****************************************************************************

#ifndef TCP_STATUS_H
#define TCP_STATUS_H

typedef enum tcp_status_t
{
	TCP_OK = 0,
	TCP_TIMEOUT_SEND,
	TCP_TIMEOUT_RECEIVE,
	TCP_TIMEOUT_CONNECT,
	TCP_NO_CONNECTION,
	TCP_INVALID_SOCKET,
	TCP_INVALID_PORT,
	TCP_INVALID_IP,
	TCP_SETSOCKETOPT_FAIL_TIMEOUT,
	TCP_SETSOCKETOPT_FAIL_BUFSIZE,
	TCP_SETSOCKETOPT_FAIL_BLOCKING,
	TCP_SOCKET_SHUTDOWN_ERROR,
	TCP_SOCKET_CLOSE_ERROR,
	TCP_CONNECTION_TIMEDOUT,
	TCP_CONNECTION_ABORTED,
	TCP_CONNECTION_RESET,
	TCP_SEND_ERROR_BYTESMISMATCH,
	TCP_BAD_POINTER,
	TCP_GETBYTESAVAILABLE_FAIL,
	TCP_INSUFFICIENT_BUFFER,
	TCP_READ_INCOMPLETE,
	TCP_UNKNOWN_ERROR
} tcp_status_t;

typedef enum connection_state_t
{
	TCP_CONNECTED = 0,
	TCP_DISCONNECTED
} connection_state_t;

#endif /* TCP_STATUS_H */

/* EOF */
