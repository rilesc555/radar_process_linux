#include <iostream>
#include "bnet_interface.h"
#include "utils.h"
#include "thread"
#include "future"
#include "atomic"
#include <ctime>
#include <utility>
#include <string>
#include <sstream>

void send_command(bnet_interface& bnet, std::string command)
{
	std::string result = bnet.send_command(command).second;
	std::cout << result << std::endl;
}

void startupScript(bnet_interface& bnet) {
	std::string command, output;
	command = "RESET:PARAMETERS";
	bnet.send_command(command);

	setTime(bnet);

	command = "MODE:SWT:OPERATIONMODE 1";
	bnet.send_command(command);
	bnet.set_save(TRACK_DATA, true);
	bnet.set_collect(TRACK_DATA, true);
	bnet.set_logging(true);

	bnet.set_logging(TRACK_DATA);

	command = "enable_track_logging";
	bnet.send_command(command);
	command = "MODE:SWT:TRACK:ELFOVMIN -10";
	bnet.send_command(command);
	command = "MODE:SWT:TRACK:AZFOVMAX 10";
	bnet.send_command(command);
	command = "MODE:SWT:TRACK:AZFOVMAX 10";
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

void setTime(bnet_interface& bnet)
{
	std::string command, output;

	command = "SYS:TIME 0,0";
	bnet.send_command(command);

	auto now = std::chrono::system_clock::now();
	auto unixTime = std::chrono::system_clock::to_time_t(now);
	auto days = unixTime / 86400;
	long long milliseconds = (unixTime % 86400) * 1000LL;

	std::pair<int, int> bootTime;
	std::string token;
	command = "SYS:TIME?";
	output = bnet.send_command(command).second;
	std::istringstream tokenStream(output);
	while (std::getline(tokenStream, token, ',')) {
		bootTime.first = std::stoi(token);
		std::getline(tokenStream, token, ',');
		bootTime.second = std::stoi(token);
	}
	milliseconds -= bootTime.second;
	days -= bootTime.first;
	if (milliseconds < 0) {
		milliseconds += 86400000;
		days--;
	}
	command = "SYS:TIME " + std::to_string(days) + "," + std::to_string(milliseconds);
	bnet.send_command(command);
	output = bnet.send_command("SYS:TIME?").second;
	std::cout << "Radar time: " << output << std::endl;
	std::cout << "System time: " << days << ", " << milliseconds << std::endl;
}



int createSocket(int& sock, struct sockaddr_in& serv_addr) {
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		std::cout << "Socket creation error" << std::endl;
		return -1;
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(8080);

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
	}
	else {}
	return coords;
}

void serializeCoordinates(coordinateStruct& coords, unsigned char* buffer)
{
	unsigned char* azimuthBytes = reinterpret_cast<unsigned char*>(&coords.az);
	for (size_t i = 0; i < sizeof(float); ++i) {
		buffer[i] = azimuthBytes[sizeof(float) - 1 - i];
	}
	unsigned char* elevationBytes = reinterpret_cast<unsigned char*>(&coords.el);
	for (size_t i = 0; i < sizeof(float); ++i) {
		buffer[sizeof(float) + i] = elevationBytes[sizeof(float) - 1 - i];
	}
}

//get coordinates of most likely UAV. Assumes there is at least one active track
coordinateStruct getMostUAV(bnet_interface& bnet)
{
	float vx, vy, vz, az, el = 0;
	int id = 0;
	float pUAV = 0;
	int target = 0;
	if (bnet.get_track().header->nTracks > 1) {
		for (int i = 0; i < bnet.get_track().header->nTracks; i++) {
			if (bnet.get_track().data.at(i).probabilityUAV > pUAV) {
				pUAV = bnet.get_track().data.at(i).probabilityUAV;

				target = i;
			}
		}
	}
	vx = bnet.get_track().data.at(target).velxest;
	vy = bnet.get_track().data.at(target).velyest;
	vz = bnet.get_track().data.at(target).velzest;
	az = bnet.get_track().data.at(target).azest;
	el = bnet.get_track().data.at(target).elest;
	id = bnet.get_track().data.at(target).ID;
	
	return coordinateStruct(vx, vy, vz, az, el, id);
}






