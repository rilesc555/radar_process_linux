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

	string output = bnet_commands.send_command("SCRIPT: ./Utils/input_script.txt").second;

	std::cout << output << endl;

	//monitorStatus(bnet_commands);

	//THIS IS ASYNC

	string command;

	while (command != "Q" && command != "q") {
		std::cout << "Enter a command (LIST for available commands, Q/q to quit):\n";
		cin >> command;		
		if (command != "Q" && command != "q") {
			if (command == "MODE:SWT:START") {
				bnet_commands.set_collect(TRACK_DATA, true);

				output = bnet_commands.send_command(command).second;

				for (int i = 0; i < 90; i++) {
					std::this_thread::sleep_for(std::chrono::seconds(1));
					
					if (bnet_commands.get_track().header->nTracks) {
						track_data first = bnet_commands.get_track().data.at(0);
						std::cout << "Position is: (" << first.xest << "," << first.yest << "," << first.zest << ")" << endl;
					}
					else {
						std::cout << "No tracks" << endl;
					}
				}
				output = bnet_commands.send_command("MODE:SWT:STOP").second;
				std::cout << output << endl;
			}
			else {
				string output = bnet_commands.send_command(command).second;
				std::cout << output << endl;
			}
		}
			/*if (command == "MODE:SWT:START") {
				for (int i = 0; i < 10; i++) {
					std::this_thread::sleep_for(std::chrono::seconds(1));
					if (bnet_commands.get_track().header->nTracks == 0) {
						std::cout << "No tracks available" << endl;
					}
					else {
						vector<track_data> tracks = bnet_commands.get_track().data;
						std::cout << "x position is: " << tracks.at(0).xest << endl;
						std::cout << "y position is: " << tracks.at(0).yest << endl;
						std::cout << "z position is: " << tracks.at(0).zest << endl;

						std::cout << "x velocity is: " << tracks.at(0).velxest << endl;
						std::cout << "y velocity is: " << tracks.at(0).velyest << endl;
						std::cout << "z velocity is: " << tracks.at(0).velzest << endl;
					}
				}
				string output = bnet_commands.send_command("MODE:SWT:STOP").second;
				std::cout << output << endl;
			}*/
	}

		
	bnet_commands.disconnect();
	std::cout << "Disconnected" << endl;

	bnet_commands.~bnet_interface();

	return 0;

}
 


