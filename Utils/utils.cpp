#include <iostream>
#include "bnet_interface.h"
#include "utils.h"
#include "thread"
#include "future"
#include "atomic"


void monitorStatus( bnet_interface& bnet) {
	std::atomic<bool> running(true);
	bnet.set_collect(STATUS_DATA, true);
	int i = 0;
	while (i < 10) {
		std::this_thread::sleep_for(std::chrono::seconds(5)); 
		std::cout << "System state: " << bnet.get_status().data->sys_state << std::endl;
		i++;
	}
}

void startupScript(bnet_interface& bnet) {
	std::string command, output;
	command = "RESET:PARAMETERS";
	bnet.send_command(command);
	command = "MODE:SWT:OPERATIONMODE 1";
	bnet.send_command(command);
	command = "enable_track";
	bnet.send_command(command);
	command = "enable_track_logging";
	bnet.send_command(command);
	bnet.set_collect(TRACK_DATA, true);
	command = "MODE:SWT:TRACK:ELFOVMIN -20";
	bnet.send_command(command);
	command = "RSP:RCSMASK:MAXRCS 10";
	bnet.send_command(command);
	command = "PLATFORM:STATE:ORIENTATION eldorado 0, 20, 0";
	bnet.send_command(command);
	command = "PLATFORM:STATE:ELEVATIONAGL eldorado 2";
	bnet.send_command(command);
	command = "RANGE:MASK eldorado 5,131,134,0,31";
	bnet.send_command(command);
	command = "AGLMASK:MINAGL eldorado 5";
	bnet.send_command(command);
	command = "AGLMASK:MAXAGL eldorado 500";
	bnet.send_command(command);
	command = "AGLMASK:ENABLE eldorado TRUE";
	bnet.send_command(command);
	std::cout << "Radar initialized" << std::endl;
}



int createSocket(int& sock, struct sockaddr_in& serv_addr) {
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		std::cout << "Socket creation error" << std::endl;
		return -1;
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(1111);

	if (inet_pton(AF_INET, "192.168.1.11", &serv_addr.sin_addr) < 0) {
		std::cout << "Invalid address" << std::endl;
		return -1;
	}

	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
		std::cout << "Connection failed" << std::endl;
		return -1;
	}

	std::cout << "Socket created" << std::endl;

	return 1;
}

coordinateStruct getCoordinates(bnet_interface& bnet) {
	coordinateStruct coords{};
	if (bnet.get_track().header->nTracks > 0) {
		coords.az = bnet.get_track().data.at(0).azest;
		coords.el = bnet.get_track().data.at(0).elest;
		coords.range = bnet.get_track().data.at(0).rest;
	}
	else {}
	return coords;
}

unsigned char* serializeCoordinates(coordinateStruct coords)
{
	unsigned char buffer[sizeof(float) * 3];
	unsigned char* azimuthBytes = reinterpret_cast<unsigned char*>(&coords.az);
	for (size_t i = 0; i < sizeof(float); ++i) {
		buffer[i] = azimuthBytes[sizeof(float) - 1 - i];
	}
	unsigned char* elevationBytes = reinterpret_cast<unsigned char*>(&coords.el);
	for (size_t i = 0; i < sizeof(float); ++i) {
		buffer[sizeof(float) + i] = elevationBytes[sizeof(float) - 1 - i];
	}
	unsigned char* rangeBytes = reinterpret_cast<unsigned char*>(&coords.range);
	for (size_t i = 0; i < sizeof(float); ++i) {
		buffer[2 * sizeof(float) + i] = rangeBytes[sizeof(float) - 1 - i];
	}
	return buffer;
}

//get coordinates of most likely UAV. Assumes there is at least one active track
coordinateStruct getMostUAV(bnet_interface& bnet)
{
	float az, el, range = 0;
	float pUAV = 0;
	int target = 0;
	for (int i = 0; i < bnet.get_track().header->nTracks; i++) {
		if (bnet.get_track().data.at(i).probabilityUAV > pUAV) {
			pUAV = bnet.get_track().data.at(i).probabilityUAV;
			target = i;
		}
	}
	az = bnet.get_track().data.at(target).azest;
	el = bnet.get_track().data.at(target).elest;
	range = bnet.get_track().data.at(target).rest;
	
	return coordinateStruct(az, el, range);
}






