//*****************************************************************************
// daa_2drv.h - Linked list data structure to hold status data
//
// Author: Bryan Womack
// Date: January 19, 2017
// Company: Echodyne Corp.
//
//*****************************************************************************

#ifndef DAA_SSTAT_H
#define DAA_SSTAT_H

#include <stdint.h>

#define HEADER_SIZE_SSTAT		24


typedef struct sstat_header
{
	char		packet_tag[12];
	uint32_t 	packetSize;
    uint8_t     unused[8];
    uint8_t     packetSchemaVerArr[4];
    char        serialNumber[8];
} sstat_header;


typedef struct sstat_data
{
	uint32_t 	sys_state;
	float 		search_fps;
	float 		platform_height;
	uint8_t 	unused0[4];
	float 		quat_x;
	float 		quat_y;
	float 		quat_z;
	float 		quat_w;
	uint32_t 	sys_time_days;
	uint32_t 	sys_time_ms;
	float 		platform_velocity_x;
	float 		platform_velocity_y;
	float 		platform_velocity_z;
	uint32_t	time_channel_state;
	uint8_t		unused1[4];
	uint32_t 	eth_link_rate;
    uint8_t     healthMon[36];
    uint32_t    cbitSummary;
    uint8_t     cbit_unused0[8];
    float       mars_primeFltInput;
    float       mars_primeImon;
    float       mars_krfeImon;
    float       krfe_6F_Vsense;
    float       krfe_6F_Vin;
    float       krfe_6F_ADin;
    float       krfe_69_Vsense;
    float       krfe_69_Vin;
    float       krfe_69_ADin;
    int32_t     mars_4F_internalTemp;
    int32_t     mars_pcbTemp;
    int32_t     krfe_TR1C;
    int32_t     krfe_TR2C;
    int32_t     txAntennaTemp;
    int32_t     rxAntennaTemp;
    int32_t     krbe_4D_internalTemp;
    uint8_t     unused2[56];
	char 		nemaGprmcMsg[84];
} sstat_data;


typedef struct daa_sstat_packet
{
	sstat_header	header;
	sstat_data		data;
} daa_sstat_packet;


#endif /* DAA_SSTAT_H */
