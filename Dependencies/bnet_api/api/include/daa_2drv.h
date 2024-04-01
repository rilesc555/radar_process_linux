//*****************************************************************************
// daa_2drv.h - Linked list data structure to hold 2D Range Velocity FFT data
//
// Author: Bryan Womack
// Date: January 19, 2017
// Company: Echodyne Corp.
//
//*****************************************************************************

#ifndef DAA_2DRV_H
#define DAA_2DRV_H

#include <stdint.h>

#define GETBEAM_FILLED_DETECTION	0b01
#define GETBEAM_FILLED_RVMAP		0b10

#define MAX_RVMAP_SIZE				(0x20000)

typedef struct rvmap_header
{
	char		packet_tag[16];
	uint32_t 	packetSize;
	float		beam_az;
	float		beam_el;
	uint32_t	trig_day;
	uint32_t	trig_ms;
	float		dR;
	float		nRanges;
	float		dV;
	float		nVelocities;
	float		quat_x;
	float		quat_y;
	float		quat_z;
	float		quat_w;
	float		search_fps;
	uint32_t	zero_range_bin;
	uint32_t	zero_doppler_bin;
	float		platform_height;
	float 		platform_velocity_x;
	float 		platform_velocity_y;
	float 		platform_velocity_z;
	uint8_t		unused[11];
	uint8_t		status;
} rvmap_header;

typedef struct daa_rvmap_packet
{
	rvmap_header	header_data;
	uint32_t		rvmap_data[MAX_RVMAP_SIZE];
} daa_rvmap_packet;

#endif /* DAA_2DRV_H */

