//*****************************************************************************
// mesak_detection_packet.h	MESA-K Data class for accessing detection data
//
// Author: Bryan Womack
// Date: May 15, 2019
// Company: Echodyne Corp.
//
//*****************************************************************************

#ifndef MESAK_DETECTION_H
#define MESAK_DETECTION_H

#include "mesak_data.h"
#include "data_types.h"
#include <vector>

class MESAK_Detection : private MESAK_Data
{
    private:
        const detection_packet* const packet;
        std::vector<detec_data> detections;

    public:
        MESAK_Detection(MESAK_Data&& data);
        ~MESAK_Detection(void);

        const detec_header* const header;
        const std::vector<detec_data>& data;

        detection_packet pod(void);
};

#endif /* MESAK_DETECTION_H */

/* EOF */

