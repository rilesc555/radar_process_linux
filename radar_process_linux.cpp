// radar_process_linux.cpp : Defines the entry point for the application.
//

#include <iostream>
#include "bnet_async.h"
#include <string> 
#include <pthread.h>
#include <vector>
#include "utils.h"


std::string ip = "192.168.1.2";
unsigned short port = 23;
std::string custom_directory = ".";
int sock = 0;
struct sockaddr_in serv_addr;
sig_atomic_t exitLoop = 0;

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
	std::string command;

	while (true) {
		std::cout << "Enter command (\"continue\" to start tracking): ";
		std::getline(std::cin, command);
		if (command == "continue" || command == "Continue" || command == "CONTINUE") {
			std::cout << "starting tracking" << std::endl;
			break;
		}
		send_command(bnet_commands, command);
	}


	// Set ctrl+c to exit loop
	signal(SIGINT, sig_handler);

	send_command(bnet_commands, "MODE:SWT:START");

	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << "started loop" << std::endl;
	int loopCounter = 1;
	unsigned char trackBuffer[8] = { 0 };
	while (!exitLoop) {
		if (bnet_commands.get_track().header->nTracks > 0) {
			coordinateStruct toTrack = getMostUAV(bnet_commands);
			
			serializeCoordinates(toTrack, trackBuffer);
			std::cout << send(sock, trackBuffer, sizeof(float) * 3, 0);
			std::cout << "tracking drone - " << loopCounter << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		else {
			std::cout << "not tracking - " << loopCounter << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		loopCounter++;
	}

	send_command(bnet_commands, "MODE:SWT:STOP");

	bnet_commands.disconnect();
	std::cout << "Disconnected" << std::endl;
	bnet_commands.~bnet_interface();

	close(sock);
	std::cout << "Socket closed" << std::endl;
	return 0;
}
 


