//*****************************************************************************
// mesak_track_packet.h	MESA-K Data class for accessing track data
//
// Author: Bryan Womack
// Date: May 15, 2019
// Company: Echodyne Corp.
//
//*****************************************************************************

#ifndef MESAK_TRACK_H
#define MESAK_TRACK_H

#include "mesak_data.h"
#include "data_types.h"
#include <vector>

class MESAK_Track : private MESAK_Data
{
    private:
        const track_packet* const packet;
        std::vector<track_data> tracks;

    public:
        MESAK_Track(MESAK_Data&& data);
        ~MESAK_Track(void);

        const track_header* const header;
        const std::vector<track_data>& data;
        track_packet pod(void);
};

#endif /* MESAK_TRACK_H */

/* EOF */

