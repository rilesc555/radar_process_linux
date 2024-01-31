// radar_process_linux.cpp : Defines the entry point for the application.
//

#include "radar_process_linux.h"
#include <iostream>
#include "bnet_async.h"
#include <string> 




using namespace std;

int main()
{
	cout << "Let's track some bad guys\n" << endl;

	bnet_async bnet_commands;

	string ip = "192.168.1.2";
	unsigned short port = 23;
	string custom_directory = ".";

	//cout << "ip: ";
	//cin >> ip;
	//cout << "directory: ";
	//cin >> custom_directory;

	
	bnet_commands.connect(ip, port, custom_directory);

	string command = "LIST";

	while (command != "Q" && command != "q") {
		
		string output = bnet_commands.send_command(command).second;

		cout << output << "\nEnter a command (Q/q to quit): ";

		cin >> command;


		size_t tracks = bnet_commands.get_n_buffered(TRACK_DATA);

		cout << tracks << endl;

	}

	bnet_commands.disconnect();
	cout << "Disconnected" << endl;

	bnet_commands.~bnet_interface();

	return 0;
}
