//*****************************************************************************
// daa_track.h - Linked list data structure to hold track data
//
// Author: Bryan Womack
// Date: February 3, 2017
// Company: Echodyne Corp.
//
//*****************************************************************************

#ifndef DAA_TRACK_H
#define DAA_TRACK_H

#include <stdint.h>

#define HEADER_SIZE_TRACK		    (28)
#define MAX_TRACKS				    (128)
#define MAX_MEASUREMENTS_2_TRK_MAIN (3)
#define MAX_MEASUREMENTS_2_TRK_EXT  (3)

typedef struct track_header
{
	char		packet_tag[12];
	uint32_t 	packetSize;
	uint32_t	nTracks;
	uint32_t 	sys_time_days;
	uint32_t 	sys_time_ms;
    uint32_t    profile_atracker;
    uint32_t    profile_atracker_main;
    uint32_t    packet_type;
} track_header;

typedef struct track_data
{
	uint32_t	ID;
	uint32_t	state;
	float		azest;
	float		elest;
	float		rest;
	float		xest;
	float		yest;
	float		zest;
	float		velxest;
	float		velyest;
	float		velzest;
    uint32_t    assocMeas_id_main[MAX_MEASUREMENTS_2_TRK_MAIN];
    float       assocMeas_chi2_main[MAX_MEASUREMENTS_2_TRK_MAIN];
	int32_t 	TOCA_days;
	int32_t 	TOCA_ms;
	float		DOCA;
	float		lifetime;
	uint32_t	lastUpdateTime_days;
	uint32_t	lastUpdateTime_ms;
	uint32_t	lastAssociatedDataTime_days;
	uint32_t	lastAssociatedDataTime_ms;
	uint32_t	acquiredTime_days;
	uint32_t	acquiredTime_ms;
	float		estConfidence;
	uint32_t    numAssocMeasurements;
    float       estRCS;
    float       probabilityOther;
    float       probabilityUAV;
} track_data;

typedef struct track_data_extended
{
    track_data  standard_data;
    float       prob_placeholder[9];
    int32_t     lastBeamTime_days;
    int32_t     lastBeamTime_ms;
    float       Pest[6][6];
    float       rangeMeasurementVariance;
    float       vradialMeasurementVariance;
    float       vradialAssocVariance;
    float       innovationEst;
    float       stateVaraince;
    uint32_t    assocMeas_id_ext[MAX_MEASUREMENTS_2_TRK_EXT];
    float       assocMeas_chi2_ext[MAX_MEASUREMENTS_2_TRK_EXT];
    uint32_t    trk_data_ext_unused0[16];
} track_data_extended;

typedef struct daa_track_packet
{
	track_header	header_data;
	track_data*		track_data_list;
} daa_track_packet;


#endif /* DAA_TRACK_H */
