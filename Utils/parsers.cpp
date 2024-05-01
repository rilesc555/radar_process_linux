#include <iostream>
#include "parsers.h"




parsed_packet parseTrackPacket(uint8_t* data, size_t size) {
    parsed_packet result;

    // Check if the packet has enough data for the header
    if (size >= sizeof(track_header))
    {
        std::memcpy(&result.header, data, sizeof(track_header));

        // Check if the packet has tracks
        if (result.header.nTracks > 0)
        {
            // Calculate the total size of the track data
            size_t trackDataSize = result.header.nTracks * sizeof(track_data);

            // Check if the packet has enough data for the tracks
            if (size >= sizeof(track_header) + trackDataSize)
            {
                result.tracks.resize(result.header.nTracks);
                std::memcpy(result.tracks.data(), data + sizeof(track_header), trackDataSize);
            }
        }
    }

    return result;
};

