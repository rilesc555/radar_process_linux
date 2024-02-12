// radar_process_linux.cpp : Defines the entry point for the application.
//

#include <iostream>
#include "bnet_async.h"
#include <string> 
#include <pthread.h>
#include <vector>
#include "utils.h"



using namespace std;

string ip = "192.168.1.2";
unsigned short port = 23;
string custom_directory = ".";

int main()
{
	std::cout << "Let's track some bad guys\n" << endl;

	bnet_interface bnet_commands;
	bnet_commands.connect(ip, port, custom_directory);
    // Configures radar
	startupScript(bnet_commands);
	
	string command, output;

	std::cout << "Tracking buffer length: " << bnet_commands.get_buffer_length(TRACK_DATA) << endl;
	std::cout << "Amount of items being tracked: " << bnet_commands.get_n_buffered(TRACK_DATA) << endl;
	while (true) {
		std::cout << "Enter command: ";
		std::getline(std::cin, command);
		if (command == "exit") {
			break;
		}
		output = bnet_commands.send_command(command).second;
		std::cout << output << endl;
	}

	//output = bnet_commands.send_command("MODE:SWT:START").second;
	//std::cout << output << endl;

	//outputTracks(bnet_commands);

	bnet_commands.disconnect();
	std::cout << "Disconnected" << endl;
	bnet_commands.~bnet_interface();
	return 0;
}
 


