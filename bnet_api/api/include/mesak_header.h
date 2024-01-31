//*****************************************************************************
// mesak_header.h	Template struct for parsing binary data of MESAK_Data
//
// Author: Bryan Womack
// Date: April 15, 2019
// Company: Echodyne Corp.
//
//*****************************************************************************

#ifndef MESAK_DATA_PACKET_HEADER_STRUCT_DEF_H
#define MESAK_DATA_PACKET_HEADER_STRUCT_DEF_H

#include <cstdint>

typedef uint32_t mesaPacketSize_t;

template <unsigned int N_TAG_BYTES>
struct MESAK_HeaderStart
{
	char tag[N_TAG_BYTES];
	mesaPacketSize_t packetSize;
};

#endif /* MESAK_DATA_PACKET_HEADER_STRUCT_DEF_H */

/* EOF */

