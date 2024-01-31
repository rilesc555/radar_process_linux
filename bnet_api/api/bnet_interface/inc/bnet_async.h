//*****************************************************************************
// bnet_async.h - Asynchronous interface to MESA-K Radar using bnet_interface
//
// Author: Bryan Womack
// Date: May 8, 2019
// Company: Echodyne Corp.
//
//*****************************************************************************

#ifndef BNET_ASYNC_INTERFACE_H
#define BNET_ASYNC_INTERFACE_H

#include "bnet_interface.h"
#include <thread>
#include <mutex>
#include <functional>
#include <condition_variable>


class bnet_async : public bnet_interface
{
    public:
        typedef std::function<void(bool)> ConnectCallback_t;
        typedef std::function<void(bool, std::string, mesa_command_status_t)> CommandCallback_t;

        bnet_async(void);
		~bnet_async(void);

        void disconnect(void);

        bool connect_async(const std::string& IP, unsigned short port,
                const std::string& custom_directory, ConnectCallback_t cb,
                const std::string& deviceSerial = "", long timeout_ms = 200);
        bool send_command_async(const std::string& command, CommandCallback_t cb);

    private:
        enum command_state_t
        {
            STATE_DISCONNECTED,         // Command port is disconnected from radar.
            STATE_CONNECTING,           // Command port is connecting to radar.
            STATE_CONNECTED_IDLE,       // Command port is connected to radar and is idle.
            STATE_CONNECTED_BUSY,       // Command port is connected to radar and busy sending command and receiving response.
        };

        command_state_t     command_state;
        std::string         async_command;
        CommandCallback_t   cmd_cb;
        bool                command_ready;
        bool                command_handler_exiting;
        bool                command_handler_joined;

        std::condition_variable cv;
        std::mutex              thread_mutex;       // Protects command_state, command_ready command_handler_exiting
        std::mutex              command_mutex;      // Protects cmd_cb, async_command
        std::thread             command_handle;

        void close_command_thread(void);
        void command_thread(const std::string& IP, unsigned short port,
                const std::string& custom_directory, ConnectCallback_t cb,
                const std::string& deviceSerial, long timeout_ms);

};


#endif /* BNET_ASYNC_INTERFACE_H */

/* EOF */

