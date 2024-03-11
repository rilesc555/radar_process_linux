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

int main()
{
	createSocket(sock, serv_addr);

	std::cout << "Let's track some bad guys\n" << std::endl;
	bnet_interface bnet_commands;
	bnet_commands.connect(ip, port, custom_directory);
    // Configures radar
	startupScript(bnet_commands);
	std::string command, output;

	ThreadSafeQueue<coordinateStruct> coordinateQueue;

	while (true) {
		std::cout << "Enter command (\"continue\" to start tracking) : ";
		std::getline(std::cin, command);
		if (command == "continue" || command == "Continue" || command == "CONTINUE") {
			std::cout << "starting tracking" << std::endl;
			break;
		}

		output = bnet_commands.send_command(command).second;
		std::cout << output << std::endl;
	}

	output = bnet_commands.send_command("MODE:SWT:START").second;
	std::cout << output << std::endl;

	while (true) {
		if (bnet_commands.get_track().header->nTracks) {
			coordinateStruct toTrack = getMostUAV(bnet_commands);
			unsigned char* trackBuffer = serializeCoordinates(toTrack);
			send(sock, trackBuffer, sizeof(float) * 3, 0);
		}
	}

	output = bnet_commands.send_command("MODE:SWT:STOP").second;
	std::cout << output << std::endl;

	bnet_commands.disconnect();
	std::cout << "Disconnected" << std::endl;
	bnet_commands.~bnet_interface();

	close(sock);
	std::cout << "Socket closed" << std::endl;
	return 0;
}
 


