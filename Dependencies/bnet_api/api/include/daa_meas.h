//*****************************************************************************
// daa_meas.h - Linked list data structure to hold track data
//
// Author: Bryan Womack
// Date: November 9, 2018
// Company: Echodyne Corp.
//
//*****************************************************************************

#ifndef DAA_MEAS_H
#define DAA_MEAS_H

#include <stdint.h>

#define MAXNUMDETECTIONSPERMEASUREMENT	(64)
#define MAX_MEASUREMENTS				(256)

typedef struct meas_header
{
	char		packet_tag[16];
	uint32_t 	packetSize;
	uint32_t	nMeasurements;
    uint32_t    systemTime_days;
    uint32_t    systemTime_ms;
    uint8_t    	UNUSED0[32];
} meas_header;


typedef struct meas_data
{
    uint32_t    id;
    uint8_t    	UNUSED0[8];
    float       az;
    float       el;
    float       r;
    float       rcsEst;
    float       vradial;
    uint32_t    nDetections;
    uint32_t    detIds[MAXNUMDETECTIONSPERMEASUREMENT];
    uint8_t    	UNUSED1[88];
} meas_data;


typedef struct daa_meas_packet
{
	meas_header		header_data;
	meas_data*		meas_data_list;
} daa_meas_packet;


#endif /* DAA_MEAS_H */
