//*****************************************************************************
// bnet_interface.h - MESA-K Radar Interface for Command, control and data reception
//
// Author: Bryan Womack
// Date: April 15, 2019
// Company: Echodyne Corp.
//
//*****************************************************************************

#ifndef BNET_INTERFACE_H
#define BNET_INTERFACE_H

#include "bnet_data.h"
#include "bnet_config.h"
#include "mesa_status.h"

#include "mesak_status_packet.h"
#include "mesak_rvmap_packet.h"
#include "mesak_detection_packet.h"
#include "mesak_track_packet.h"
#include "mesak_measurement_packet.h"

#include <memory>
#include <array>
#include <string>
#include <map>

class mesa_interface;
class file_handler;
class data_handler;
class command_logger;
class mesak_data_configurator;
class MESAK_Data;

class bnet_interface
{
    protected:
        std::unique_ptr<mesa_interface> radar;

    private:
        std::unique_ptr<file_handler> file_manager;
        std::unique_ptr<command_logger> logger;
        std::array<std::unique_ptr<data_handler>, N_MESAK_DTypes> data_handlers;
        std::unique_ptr<mesak_data_configurator> config_helper;
        bool bIsConnected;

        std::unique_ptr<data_handler>& getDataHandler(mesa_data_t d_type);
        void LogHeader(const std::string& devSerial, const std::string& IP, unsigned short port);
        void checkConnection(const std::string& errMsg);
        void checkDataConnection(const std::string& errMsg);
        void connectionHelper(const std::string& IP, unsigned short port,
                const std::string& custom_directory, const std::string& deviceSerial);

		MESAK_Data get_data(mesa_data_t d_type);

	public:
		bnet_interface(void);
		bnet_interface(std::map<mesa_data_t, unsigned short> portMap);
		~bnet_interface(void);

		std::pair<std::string, std::string> version(void);
        std::string getDeviceSerial(void);
        std::string get_IP(void);

		void connect(const std::string& IP, unsigned short port,
                const std::string& custom_directory, long timeout_ms = DEFAULT_CONNECT_TIMEOUT_MS);
		void connect(const std::string& IP, unsigned short port,
                const std::string& custom_directory, const std::string& deviceSerial,
                long timeout_ms = DEFAULT_CONNECT_TIMEOUT_MS);
		void disconnect(void);
		void reconnect(void);

		std::pair<mesa_command_status_t, std::string> send_command(const std::string& command);
		void send_blob(const char* blob, std::size_t length);
		std::string receive_async(void);

		MESAK_Status get_status(void);
		MESAK_Rvmap get_rvmap(void);
		MESAK_Detection get_detection(void);
		MESAK_Track get_track(void);
		MESAK_Measurement get_meas(void);

		void new_folder(void);
		void new_folder(const std::string& folderName);
		void save_folder(void);
		std::string get_folder(void);

		void set_buffer_length(mesa_data_t d_type, std::size_t length);
		std::size_t get_buffer_length(mesa_data_t d_type);
		std::size_t get_n_buffered(mesa_data_t d_type);
		void flush_buffer(mesa_data_t d_type);

		void set_save(mesa_data_t d_type, bool flag);
		void set_collect(mesa_data_t d_type, bool flag);
		void set_chunk(bool flag);
		bool get_save(mesa_data_t d_type);
		bool get_collect(mesa_data_t d_type);
		bool get_chunk(void);

		void set_logging(bool enabled);
		void log_action(const std::string& action);
};

#endif /* BNET_INTERFACE_H */

/* EOF */

