//*****************************************************************************
// mesak_measurement_packet.h	MESA-K Data class for accessing measurement data
//
// Author: Bryan Womack
// Date: May 15, 2019
// Company: Echodyne Corp.
//
//*****************************************************************************

#ifndef MESAK_MEASUREMENT_H
#define MESAK_MEASUREMENT_H

#include "mesak_data.h"
#include "data_types.h"
#include <vector>

class MESAK_Measurement : private MESAK_Data
{
    private:
        const meas_packet* const packet;
        std::vector<meas_data> measurements;

    public:
        MESAK_Measurement(MESAK_Data&& data);
        ~MESAK_Measurement(void);

        const meas_header* const header;
        const std::vector<meas_data>& data;
        meas_packet pod(void);
};

#endif /* MESAK_MEASUREMENT_H */

/* EOF */

