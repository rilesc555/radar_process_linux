// radar_process_linux.cpp : Defines the entry point for the application.
//

#include <iostream>
#include "bnet_async.h"
#include <string> 
#include <pthread.h>
#include <vector>
#include "utils.h"
#include "ThreadSafeQueue.h"



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

	ThreadSafeQueue<coordinateStruct> coordinateQueue;


	while (!exitLoop) {
		std::cout << "Enter command (\"continue\" to start tracking) : ";
		std::getline(std::cin, command);
		if (command == "continue" || command == "Continue" || command == "CONTINUE") {
			std::cout << "starting tracking" << std::endl;
			break;
		}

		send_command(bnet_commands, command);
	}

	send_command(bnet_commands, "MODE:SWT:START");
	
	// Set ctrl+c to exit loop
	signal(SIGINT, sig_handler);

	while (!exitLoop) {
		if (bnet_commands.get_track().header->nTracks) {
			coordinateStruct toTrack = getMostUAV(bnet_commands);
			unsigned char* trackBuffer = serializeCoordinates(toTrack);
			send(sock, trackBuffer, sizeof(float) * 3, 0);
		}
	}

	send_command(bnet_commands, "MODE:SWT:STOP");

	bnet_commands.disconnect();
	std::cout << "Disconnected" << std::endl;
	bnet_commands.~bnet_interface();

	close(sock);
	std::cout << "Socket closed" << std::endl;
	return 0;
}
 


