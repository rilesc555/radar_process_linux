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
	std::cout << "Buffer length: " << bnet_commands.get_buffer_length(TRACK_DATA) << std::endl;

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

	send_command(bnet_commands, "MODE:SWT:START");

	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	
	// Set ctrl+c to exit loop
	signal(SIGINT, sig_handler);
	std::cout << "Press CTRL + C to stop tracking" << std::endl;

	KalmanFilter* kf = new KalmanFilter;
	Eigen::VectorXd x0(7);
	Eigen::VectorXd z(5);
	double lastTime;
	coordinateStruct toTrack;

	int trackID = 0;
	while (!exitLoop) {
		if (bnet_commands.get_n_buffered(TRACK_DATA)) {
			toTrack = getMostUAV(bnet_commands);
			if (!toTrack.tracking) {
				std::cout << "Waiting" << std::endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(104));
				continue;
			}
			else if (toTrack.id != trackID) {
				trackID = toTrack.id;
				lastTime = toTrack.lastTime;
				std::cout << "Now tracking UAV " << trackID << std::endl;
				x0 << toTrack.vx, toTrack.vy, toTrack.vz, toTrack.az, toTrack.el, 0, 0;
				z << toTrack.vx, toTrack.vy, toTrack.vz, toTrack.az, toTrack.el;
				kf->init(x0, 1, trackID);
				kf->predict(0.104);
				kf->update(z);
			}
			else {
				z << toTrack.vx, toTrack.vy, toTrack.vz, toTrack.az, toTrack.el;
				double dt = (toTrack.lastTime - lastTime) / 1000;
				kf->predict(dt);
				kf->update(z);
			}
			
			serializeCoordinates(toTrack, trackBuffer);
			std::cout << send(sock, trackBuffer, sizeof(float) * 2, 0) << " bytes sent to gimbal" << std::endl;
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
 


