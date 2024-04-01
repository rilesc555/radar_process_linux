//*****************************************************************************
// mesa_status.h - Status types for MESA Comms
//
// Author: Bryan Womack
// Date: October 6, 2017
// Company: Echodyne Corp.
//
//*****************************************************************************

#ifndef MESA_STATUS_H
#define MESA_STATUS_H

typedef enum mesa_command_status_t
{
	MESA_OK = 0,
	MESA_IC,
	MESA_CE,
	MESA_IP,
	MESA_TO,
	MESA_NA,
	MESA_UE,
	MESA_UNKNOWN
} mesa_command_status_t;


#endif /* MESA_STATUS_H */

/* EOF */
