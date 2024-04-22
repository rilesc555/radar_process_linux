// radar_process_linux.cpp : Defines the entry point for the application.
//

#include <iostream>
#include "bnet_async.h"
#include <string> 
#include <pthread.h>
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
int piSock, processSock = 0;
struct sockaddr_in pi_serv_add, process_serv_add;
sig_atomic_t exitLoop = 0;
std::string command;
unsigned char trackBuffer[8] = { 0 };
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
    int processSocketCreated = createProcessSocket(processSock, process_serv_add);
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

	//set up the csv file for logging
	std::ofstream outfile;
	std::string filename;
	std::cout << "Enter test name: " << std::endl;
	std::cin >> filename;
	filename += getTimeString();
	outfile.open(filename);
	outfile << "time,rvx,rvy,rvz,raz,rel,rrange,kvx,kvy,kvz,kaz,kel" << std::endl;

	//start tracking and give it a second to pick up a track
	send_command(bnet_commands, "MODE:SWT:START");
	
	/*Set preliminary stuff: set signal ending the tracking loop, create kalman filter object,
	and some other random variables it'll use*/
	signal(SIGINT, sig_handler);
	std::cout << "Press CTRL + C to stop tracking" << std::endl;
	KalmanFilter* kf = new KalmanFilter;
	Eigen::VectorXd x0(7);
	Eigen::VectorXd z(5);
	double lastTime;
	coordinateStruct toTrack;
	int trackID = -1;
	int bufferedTracks = 0;
    uint8_t trackData[2600];

	/*main tracking loop. CTRL+C will exit this when done. Sends radar data to rpi, and also logs both radar data
	and Kalman filtered data*/
	while (!exitLoop) {
        int bytesReceived = recv(processSock, trackData, 2600, 0);
        if (bytesReceived < 0) {
            std::cout << "Error receiving data from process" << std::endl;
            continue;
        }
        else if (bytesReceived == 0) {
            std::cout << "Process disconnected" << std::endl;
            break;
        }
        else {
            parsed_packet packet = parseTrackPacket(trackData, bytesReceived);
            bufferedTracks = packet.header.nTracks;
            //if there's a packet in the buffer, do this:
            if (bufferedTracks > 0) {
                toTrack = getMostUAV(packet);

                /*if the packet has at least one track, and the most probable track in the packet is not the current track, re-initialize the kalman filter
                and give it a 50 millisecond update*/
                if (toTrack.id != trackID) {
                    trackID = toTrack.id;
                    lastTime = toTrack.lastTime;
                    std::cout << "Now tracking UAV " << trackID << std::endl;
                    x0 << toTrack.vx, toTrack.vy, toTrack.vz, toTrack.az, toTrack.el, 0, 0;
                    z << toTrack.vx, toTrack.vy, toTrack.vz, toTrack.az, toTrack.el;
                    kf->init(x0, 1, trackID);
                    kf->predict(.05);
                    kf->update(z);
                    std::cout << "kalman updated" << std::endl;
                }

                /*if the packet grabbed is tracking the same object as before, update the kalman filter based on difference between last recorded track time
                and the previously last recorded track time*/
                else if (toTrack.id == trackID) {
                    std::cout << "still tracking " << trackID << std::endl;
                    z << toTrack.vx, toTrack.vy, toTrack.vz, toTrack.az, toTrack.el;
                    double dt = (toTrack.lastTime - lastTime) / 1000;
                    kf->predict(dt);
                    kf->update(z);
                }

                if (socketCreated == 1) {
                    serializeCoordinates(toTrack, trackBuffer);
                    std::cout << send(piSock, trackBuffer, sizeof(float) * 2, 0) << " bytes sent to gimbal"
                              << std::endl;
                }

                //print radar and kalman coordinates to csv
                outfile << toTrack.lastTime << "," << toTrack.vx << "," << toTrack.vy << "," << toTrack.vz << ","
                        << toTrack.az << "," << toTrack.el << "," << toTrack.range << ",";
                outfile << kf->get_x_hat()[0] << "," << kf->get_x_hat()[1] << "," << kf->get_x_hat()[2] << ","
                        << kf->get_x_hat()[3] << "," << kf->get_x_hat()[4] << std::endl;
            }
                //if there's no packet in the buffer, wait a moment and check for another packet
            else {
                continue;
            }
        }
	}
	
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
 


