//*****************************************************************************
// mesak_rvmap_packet.h	MESA-K Data class for accessing status data
//
// Author: Bryan Womack
// Date: May 15, 2019
// Company: Echodyne Corp.
//
//*****************************************************************************

#ifndef MESAK_RVMAP_H
#define MESAK_RVMAP_H

#include "mesak_data.h"
#include "data_types.h"

class MESAK_Rvmap : private MESAK_Data
{
    private:
        const rvmap_packet* const packet;

    public:
        MESAK_Rvmap(MESAK_Data&& data);
        ~MESAK_Rvmap(void);

        const rvmap_header* const header;
        const uint32_t* const data;

        rvmap_packet pod(void);
};

#endif /* MESAK_RVMAP_H */

/* EOF */

