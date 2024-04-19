//*****************************************************************************
// daa_detec.h - Linked list data structure to hold detection data
//
// Author: Bryan Womack
// Date: February 3, 2017
// Company: Echodyne Corp.
//
//*****************************************************************************

#ifndef DAA_DETEC_H
#define DAA_DETEC_H

#include <stdint.h>

#define HEADER_SIZE_DETEC		24
#define MAX_DETECTIONS			(128)

typedef struct detec_header
{
	char		packet_tag[12];
	uint32_t 	packetSize;
	uint32_t	nDetections;
	uint32_t    beam_purpose;
	float 		az;
	float 		el;
	uint32_t 	detec_time_days;
	uint32_t	detec_time_sec;
	uint8_t		unused[4];
} detec_header;


typedef struct detec_data
{
	uint32_t 	detec_time_days;
	uint32_t	detec_time_sec;
	float 		power;
	float 		snr;
	float 		r;
	float 		az;
	float 		el;
	float 		vradial;
	float 		r_interp;
	uint32_t 	detection_ID;
	uint8_t 	unused0[4];
	float 		rcs;
	uint8_t		unused1[16];
} detec_data;


typedef struct daa_detec_packet
{
	detec_header	header_data;
	detec_data*		detec_data_list;
} daa_detec_packet;

#endif /* DAA_DETEC_H */
