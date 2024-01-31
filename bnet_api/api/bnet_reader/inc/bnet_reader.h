//*****************************************************************************
// bnet_reader.h - File Read Functionality for BNET save data
//
//      Basic usage is to set the data source to a save folder produced by
//      bnet_interface. Data can then be read out into MESAK_Data objects
//      which can then be casted to their actual data types.
//
// Author: Bryan Womack
// Date: May 1, 2019
// Company: Echodyne Corp.
//
//*****************************************************************************

#ifndef BNET_READER_H
#define BNET_READER_H

#include "bnet_data.h"

#include <memory>
#include <array>
#include <string>
#include "filesystem_ns.h"

class file_handler;
class read_handler;
class mesak_data_configurator;
class MESAK_Data;

class bnet_reader
{
	std::unique_ptr<file_handler> file_manager;
	std::array<std::unique_ptr<read_handler>, N_MESAK_DTypes> read_handlers;
	std::unique_ptr<mesak_data_configurator> config_helper;
	std::unique_ptr<read_handler>& getReadHandler(mesa_data_t d_type);

    bool initialized;
    bool realtime_playback;

	public:
        /******************************************************************************
         *  @brief  Create a new instance of bnet reader with a folder source
         *
         *  @return  NA
         *  @note NA
         *  @warning NA
        ******************************************************************************/
		bnet_reader(const fs::path& source_folder);
		bnet_reader(const std::string& source_folder);

        /******************************************************************************
         *  @brief  Create a new instance of bnet reader
         *
         *  @return  NA
         *  @note NA
         *  @warning NA
        ******************************************************************************/
		bnet_reader(void);

        /******************************************************************************
         *  @brief  Destructor of bnet reader
         *
         *  @return  NA
         *  @note NA
         *  @warning NA
        ******************************************************************************/
		~bnet_reader(void);

        /******************************************************************************
         *  @brief  Get the version and build time
         *
         *  @return  (version, build time)
         *  @note NA
         *  @warning NA
        ******************************************************************************/
		std::pair<std::string, std::string> version(void);

        /******************************************************************************
         *  @brief  Set the data source
         *
         *  @param[in] source_folder    Path to folder to read from
         *
         *  @return NA
         *  @note NA
         *  @warning NA
        ******************************************************************************/
        void source(const std::string& source_folder);

        /******************************************************************************
         *  @brief  Get data of a certain type
         *
         *  @param[in] d_type	data type for this operation
         *
         *  @return Data packet (raw)
         *  @note NA
         *  @warning NA
        ******************************************************************************/
		MESAK_Data get_data(mesa_data_t d_type);

        /******************************************************************************
         *  @brief  Throw away a data packet of a certain type
         *
         *  @param[in] d_type	data type for this operation
         *
         *  @return NA
         *  @note NA
         *  @warning NA
        ******************************************************************************/
		void seek(mesa_data_t d_type);

        /******************************************************************************
         *  @brief  Get the path of the folder currently being read from
         *
         *  @param[in] d_type	data type for this operation
         *
         *  @return std::filesystem::path of the directory
         *  @note NA
         *  @warning NA
        ******************************************************************************/
        fs::path get_directory(mesa_data_t d_type);

        /******************************************************************************
         *  @brief  Get the data prefix of a certain data type
         *
         *  @param[in] d_type	data type for this operation
         *
         *  @return prefix as a string
         *  @note NA
         *  @warning NA
        ******************************************************************************/
        std::string get_prefix(mesa_data_t d_type);


        /******************************************************************************
         *  @brief  Check if data has been read to completion
         *
         *  @param[in] d_type	data type for this operation
         *
         *  @return true if empty, false otherwise
         *  @note NA
         *  @warning NA
        ******************************************************************************/
		bool isEmpty(mesa_data_t d_type);
};

#endif /* BNET_READER_H */

/* EOF */

