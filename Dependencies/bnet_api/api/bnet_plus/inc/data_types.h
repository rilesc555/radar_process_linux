//*****************************************************************************
// data_types.h - Templated types for MESA data of maximum size
//
// Author: Bryan Womack
// Date: October 9, 2017
// Company: Echodyne Corp.
//
//*****************************************************************************

#ifndef DATA_TYPES_H
#define DATA_TYPES_H

extern "C" 
{
	#include "daa_2drv.h"
	#include "daa_detec.h"
	#include "daa_sstat.h"
	#include "daa_track.h"
	#include "daa_meas.h"
}

// status Packet
struct status_packet
{
	sstat_header	header;
	sstat_data		data;
};

// rvmap packet - data size varies based on waveform
struct rvmap_packet
{
	rvmap_header	header;
	uint32_t		data[MAX_RVMAP_SIZE];	
};

// detection packet
struct detection_packet
{
	detec_header	header;
	detec_data		data[MAX_DETECTIONS];
};

struct track_packet
{
	track_header	header;
	track_data		data[MAX_TRACKS];	
};

struct meas_packet
{
	meas_header		header;
	meas_data		data[MAX_MEASUREMENTS];	
};

#endif /* DATA_TYPES_H */
