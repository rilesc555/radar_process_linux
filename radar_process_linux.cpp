// radar_process_linux.cpp : Defines the entry point for the application.
//

#include <iostream>
#include "bnet_async.h"
#include <string> 
#include <pthread.h>
#include <vector>
#include "utils.h"
#include "KalmanFilter.h"



std::string ip = "192.168.1.2";
unsigned short port = 23;
std::string custom_directory = ".";
int sock = 0;
struct sockaddr_in serv_addr;
sig_atomic_t exitLoop = 0;
std::string command;
unsigned char trackBuffer[8] = { 0 };

void sig_handler(int sig)
{ 
	exitLoop = 1; 
	signal(SIGINT, SIG_DFL);
}

int main()
{
	createSocket(sock, serv_addr);

	std::cout << "Let's track some bad guys\n" << std::endl;
	bnet_interface bnet_commands;
	bnet_commands.connect(ip, port, custom_directory);
    // Configures radar
	startupScript(bnet_commands);

	// space to enter and preliminary commands before tracking
	while (true) {
		std::cout << "Enter command (\"C\" to start tracking): ";
		std::getline(std::cin, command);
		if (command == "C" || command == "c" || command == "continue" || command == "Continue" || command == "CONTINUE") {
			std::cout << "starting tracking" << std::endl;
			break;
		}
		if (command == "MODE:SWT:START" || command == "MODE:SEARCH:START") {
			std::cout << "Calm down, I'll start tracking when I'm ready. Anything I should do BEFORE starting to track?" << std::endl;
			continue;
		}
		send_command(bnet_commands, command);
	}

	// Set ctrl+c to exit loop
	signal(SIGINT, sig_handler);

	KalmanFilter kf;
	kf.init()

	send_command(bnet_commands, "MODE:SWT:START");

	std::this_thread::sleep_for(std::chrono::seconds(1));

	while (!exitLoop) {
		if (bnet_commands.get_n_buffered(TRACK_DATA)) {
			coordinateStruct toTrack = getMostUAV(bnet_commands);
			
			serializeCoordinates(toTrack, trackBuffer);
			std::cout << send(sock, trackBuffer, sizeof(float) * 2, 0);
			std::this_thread::sleep_for(std::chrono::milliseconds(104));
		}
		else {

			std::this_thread::sleep_for(std::chrono::milliseconds(104));
		}
	}

	send_command(bnet_commands, "MODE:SWT:STOP");

	std::cout << "Stopping tracking" << std::endl;
	bnet_commands.set_save(TRACK_DATA, false);
	bnet_commands.set_collect(TRACK_DATA, false);

	bnet_commands.disconnect();
	std::cout << "Disconnected" << std::endl;
	bnet_commands.~bnet_interface();

	close(sock);
	std::cout << "Socket closed" << std::endl;
	return 0;
}
 


