//*****************************************************************************
// data_status.h - Status for data_handler related classes
//
// Author: Bryan Womack
// Date: October 9, 2017
// Company: Echodyne Corp.
//
//*****************************************************************************

#ifndef DATA_STATUS_H
#define DATA_STATUS_H

typedef enum data_status_t
{
	DATA_OK = 0,
	DATA_BUFFER_EMPTY,
	DATA_INVALID_TYPE,
	DATA_CONNECTION_ERROR,
	DATA_OK_RESYNC,
	DATA_INVALID_OBJECT,
	DATA_BAD_PACKET_SIZE,
	DATA_UNKNOWN_ERROR
} data_status_t;

#endif /* DATA_STATUS_H */
