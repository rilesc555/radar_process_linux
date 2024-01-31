//*****************************************************************************
// mesak_chunk.h -	Class for creating and reading chunks of MESA-K Data to and
//					from binary representations
//
// Author: Bryan Womack
// Date: April 8, 2019
// Company: Echodyne Corp.
//
//*****************************************************************************

#ifndef MESAK_CHUNK_UTILITY_H
#define MESAK_CHUNK_UTILITY_H

#include "mesak_data.h"
#include <string>
#include <cstddef>
#include "filesystem_ns.h"

class MESAK_Chunk : public MESAK_Data
{
    std::size_t pos;

	public:
		MESAK_Chunk(size_t raw_size);
		MESAK_Chunk(MESAK_Data&& data);

		void add(const MESAK_Data& packet);
        std::byte* read(std::size_t length, bool advance = true);
		void save(const fs::path& filename);
		void load(const fs::path& filename);
};

#endif /* MESAK_CHUNK_UTILITY_H */

/* EOF */

