// radar_process_linux.cpp : Defines the entry point for the application.
//

#include <iostream>
#include "bnet_async.h"
#include <string> 
#include <thread>
#include <future>
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

	monitorStatus(bnet_commands);

	//THIS IS ASYNC

	string command;

	while (command != "Q" && command != "q") {
		std::cout << "Enter a command (LIST for available commands, Q/q to quit):\n";
		cin >> command;		
		if (command != "Q" && command != "q") {
			string output = bnet_commands.send_command(command).second; 
			std::cout << output << endl;
		}
	}
		
	try {
		vector<track_data> tracks = bnet_commands.get_track().data;
		std::cout << tracks.at(0).azest << endl;
	}
	catch (...) {
		std::cout << "No tracks available" << endl;
	}
		
	bnet_commands.disconnect();
	std::cout << "Disconnected" << endl;

	bnet_commands.~bnet_interface();

	return 0;

	}



