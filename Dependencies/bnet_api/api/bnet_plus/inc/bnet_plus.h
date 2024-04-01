//*****************************************************************************
// bnet_plus.h - Interface to MESA Radar for command/control and data reception
//
// Author: Bryan Womack
// Date: April 17, 2019
// Company: Echodyne Corp.
//
//*****************************************************************************

#ifndef BNET_PLUS_H
#define BNET_PLUS_H

#include "bnet_data.h"
#include "bnet_config.h"
#include "tcp_status.h"
#include "mesa_status.h"
#include "data_status.h"
#include "data_types.h"
#include "thread_helper.h"
#include <mutex>
#include <functional>
#include <condition_variable>
#include <string>
#include <memory>
#include <assert.h>

class bnet_reader;
class bnet_async;

#define SIZEOF_ARRAY(a_) (sizeof(a_) / sizeof((a_)[0]))
#define STRNCPY(dest_, src_, len_) { assert(len_); strncpy((dest_), (src_), (len_)-1); (dest_)[(len_)-1] = 0; }

class bnet_plus
{
	std::unique_ptr<bnet_async> radar;
	std::unique_ptr<bnet_reader> reader;
	std::string IP;
    data_source_t data_source;

	public:
		bnet_plus();
		~bnet_plus();

		// Version Info
		void version(char* version_, char* build_time);

		// Command and Control Interface
		tcp_status_t connect(const char* IP, int port, const char* custom_directory,
                long timeout_ms = DEFAULT_CONNECT_TIMEOUT_MS);
		tcp_status_t disconnect(void);
		tcp_status_t reconnect(void);
		void get_IP(char* IP_dest, int length);

		tcp_status_t send_command(const char* command, char* output, int output_buffer_length, mesa_command_status_t* m_status);
		tcp_status_t send_blob(const char* blob, int blob_size);
		tcp_status_t receive_async(char* output, int output_buffer_length, int* bytes_received);

		// File manager control
		int set_dataSource(char* source_);
		void new_folder(void);
		void save_folder(void);
		void get_folder(char* directory);

		// Get Data
		data_status_t get_status(status_packet* data);
		data_status_t get_rvmap(rvmap_packet* data);
		data_status_t get_detection(detection_packet* data);
		data_status_t get_track(track_packet* data);
		data_status_t get_meas(meas_packet* data);

		// Buffer Control
		data_status_t set_buffer_length(mesa_data_t d_type, int length);
		int get_buffer_length(mesa_data_t d_type);
		int get_n_buffered(mesa_data_t d_type);
		data_status_t flush_buffer(mesa_data_t d_type);

		// Collection/Logging Control
		data_status_t set_chunk(bool chunk);
		data_status_t set_save(mesa_data_t d_type, bool log);
		bool get_save(mesa_data_t d_type);
		data_status_t set_collect(mesa_data_t d_type, bool collect);
		bool get_collect(mesa_data_t d_type);

		// Diagnostics
		void disable_logging(void);
		void enable_api_log(bool val);
		void enable_cmd_log(bool val);
		void log_action(const char* action);

		// Lawrence Async Stuff
        bool connect_async(const char* IP, int port, const char* custom_directory,
                std::function<void(tcp_status_t)>cb, const std::string& deviceSerial = "",
                long timeout_ms = DEFAULT_CONNECT_TIMEOUT_MS);
        bool send_command_async(const char* command, char* output, int output_buffer_length,
            std::function<void(tcp_status_t, char* output, mesa_command_status_t)>cb);
};

#endif /* BNET_PLUS_H */
/* EOF */

