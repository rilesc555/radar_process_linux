//*****************************************************************************
// bnet_data.h - Type definitions for BNET data sources and types
//
// Author: Bryan Womack
// Date: April 15, 2019
// Company: Echodyne Corp.
//
//*****************************************************************************

#ifndef BNET_DATA_ENUM_DEFINITIONS_H
#define BNET_DATA_ENUM_DEFINITIONS_H

typedef enum data_source_t
{
	MESA_RADAR_SOURCE = 0,
	FOLDER_SOURCE
} data_source_t;

typedef enum mesa_data_t
{
	STATUS_DATA = 0,
	RVMAP_DATA,
	DETECTION_DATA,
	TRACK_DATA,
	MEAS_DATA,
	N_MESAK_DATA_TYPES
} mesa_data_t;

const int N_MESAK_DTypes = static_cast<int>(N_MESAK_DATA_TYPES);

#endif /* BNET_DATA_ENUM_DEFINITIONS_H */

/* EOF */

