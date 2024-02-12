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
    // Configures radar and starts tracking
	string output = bnet_commands.send_command("SCRIPT: ./Utils/input_script.txt").second;
	std::cout << output << endl;

    pthread_rwlock_t lock = PTHREAD_RWLOCK_INITIALIZER;
    pthread.create


	bnet_commands.disconnect();
	std::cout << "Disconnected" << endl;

	bnet_commands.~bnet_interface();

	return 0;

}
 


