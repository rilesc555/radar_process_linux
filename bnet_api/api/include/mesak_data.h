//*****************************************************************************
// mesak_data.h	Parent class for MESA-K data types. Manage interpretation of
//				raw bytes into MESA-K Data
//
// Author: Bryan Womack
// Date: April 8, 2019
// Company: Echodyne Corp.
//
//*****************************************************************************

#ifndef MESAK_DATA_PARENT_H
#define MESAK_DATA_PARENT_H

#include <vector>
#include <cstddef>

//*****************************************************************************
// All MESA-K Data types start with a uniquely identifying tag followed by the
// size of the packet in bytes.
//
// All MESA-K Data types are composed of a header and 0 to N data elements:
// 		Status has a header and 1 data element
// 		RV map has a header and 1 data element
//
// 		Detections have a header and 0 to N data elements
// 		Measurements have a header and 0 to N data elements
// 		Tracks have a header and 0 to N data elements
//
//*****************************************************************************
class MESAK_Data
{
	protected:
		std::vector<std::byte> raw;

	public:
		MESAK_Data(std::size_t raw_size);
		MESAK_Data(MESAK_Data&& other);

		void push(std::byte* bytes, std::size_t length);
		void push(const std::vector<std::byte>& data);

		const std::vector<std::byte>& bytes(void) const;
		std::byte* copyInto(std::size_t length);
		std::size_t size(void) const;
};

#endif /* MESAK_DATA_PARENT_H */

/* EOF */

