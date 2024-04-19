//*****************************************************************************
// mesak_status_packet.h	MESA-K Data class for accessing status data
//
// Author: Bryan Womack
// Date: May 15, 2019
// Company: Echodyne Corp.
//
//*****************************************************************************

#ifndef MESAK_STATUS_H
#define MESAK_STATUS_H

#include "mesak_data.h"
#include "data_types.h"

class MESAK_Status : private MESAK_Data
{
    private:
        const status_packet* const packet;

    public:
        MESAK_Status(MESAK_Data&& raw_data);
        ~MESAK_Status(void);

        const sstat_header* const header;
        const sstat_data* const data;

        status_packet pod(void);
};

#endif /* MESAK_STATUS_H */

/* EOF */

