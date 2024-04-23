// radar_process_linux.cpp : Defines the entry point for the application.
//

#include <iostream>
#include "bnet_interface.h"
#include <string> 
#include <thread>
#include <vector>
#include <fstream>
#include "utils.h"
#include "KalmanFilter.h"
#include "parsers.h"


//!!!BEFORE RUNNING THIS MAKE SURE YOU'VE SYNCED THE LAPTOP WITH GPS TIME!!!

//variables for connecting to the radar, the raspberry pi, and setting up the buffer that'll send the data to rpi
std::string ip = "192.168.1.2";
unsigned short port = 23;
std::string custom_directory = ".";
int piSock = 0;
struct sockaddr_in pi_serv_add;
sig_atomic_t exitLoop = 0;
std::string command;
int heading;

//this will be used to let CTRL+C stop the main tracking loop without exiting the program
static void sig_handler(int sig)
{ 
	exitLoop = 1; 
	signal(SIGINT, SIG_DFL);
}

int main()
{
	std::string answer;
	std::cout << "Press H for hack, otherwise direct connect to radar: ";
	std::getline(std::cin, answer);
	if (answer == "H" || answer == "h") {
		ip = "127.0.0.1";
	}
	//create the socket to the raspberry pi (rpi), create a bnet object and connect to radar, and issue initial startup script
	int socketCreated = createPiSocket(piSock, pi_serv_add);

	std::string filename = "Test";
	filename += getTimeString();

	std::thread mainThread(mainLoop, filename, std::ref(piSock), socketCreated, std::ref(exitLoop));

	std::cout << "Let's track some bad guys\n" << std::endl;
	bnet_interface bnet_commands;
	bnet_commands.connect(ip, port, custom_directory, 60000L);
	startupScript(bnet_commands);

	// loop to enter any preliminary commands before tracking. C or c will start to the main tracking loop
	while (true) {
		std::cout << "Enter command (\"C\" to start tracking): ";
		std::getline(std::cin, command);
		if (command == "C" || command == "c" || command == "continue" || command == "Continue" || command == "CONTINUE") {
			break;
		}
		if (command == "MODE:SWT:START" || command == "MODE:SEARCH:START") {
			std::cout << "Calm down, I'll start tracking when I'm ready. Anything I should do BEFORE starting to track?" << std::endl;
			continue;
		}
		send_command(bnet_commands, command);
	}

	//start tracking and give it a second to pick up a track
	send_command(bnet_commands, "MODE:SWT:START");


	/*main tracking loop. CTRL+C will exit this when done. Sends radar data to rpi, and also logs both radar data
    and Kalman filtered data*/
	mainThread.join();
	
	//stop tracking, stop logging, and disconnect from the radar
	send_command(bnet_commands, "MODE:SWT:STOP");
	std::cout << "Stopping tracking" << std::endl;
	bnet_commands.set_save(TRACK_DATA, false);
	bnet_commands.set_collect(TRACK_DATA, false);
	bnet_commands.disconnect();
	std::cout << "Disconnected" << std::endl;
	bnet_commands.~bnet_interface();

	//close the socket with the rpi
	close(piSock);
	std::cout << "Socket closed" << std::endl;
	return 0;
}
 


