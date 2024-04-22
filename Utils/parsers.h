#ifndef PARSERS_H
#define PARSERS_H

#include <iostream>
#include <vector>
#include <cstdint>
#include <cstring>
#include "daa_track.h"

struct parsed_packet
{
    track_header header;
    std::vector<track_data> tracks;
};

parsed_packet parseTrackPacket(uint8_t* buffer, size_t size);

#endif
