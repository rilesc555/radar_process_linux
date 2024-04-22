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

	bnet.set_save(STATUS_DATA, true);
	bnet.set_collect(STATUS_DATA, true);
	bnet.set_logging(true);
	bnet.set_logging(STATUS_DATA);

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
	command = "PLATFORM:STATE:ELEVATIONAGL eldorado 0";
	bnet.send_command(command);
	command = "AGLMASK:MINAGL eldorado 0";
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
	long milliseconds = (unixTime % 86400) * 1000L;
	std::cout << "System time: " << days << ", " << milliseconds << std::endl;

	std::pair<int, int> bootTime;
	std::string token;
	command = "SYS:TIME?";
	output = bnet.send_command(command).second;
	std::cout << "Time before setting: " << output;
	std::istringstream tokenStream(output);
	while (std::getline(tokenStream, token, ',')) {
		bootTime.first = std::stoi(token);
		std::getline(tokenStream, token, ',');
		bootTime.second = std::stoi(token);
	}
	milliseconds -= (bootTime.second + 5);
	days -= bootTime.first;
	if (milliseconds < 0) {
		milliseconds += 86400000;
		days--;
	}
	command = "SYS:TIME " + std::to_string(days) + "," + std::to_string(milliseconds);
	bnet.send_command(command);
	output = bnet.send_command("SYS:TIME?").second;
	std::cout << "Radar time: " << output;
}

int createPiSocket(int& sock, struct sockaddr_in& serv_addr) {
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		std::cout << "Pi socket creation error" << std::endl;
		return -1;
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(8080);

	if (inet_pton(AF_INET, "192.168.1.11", &serv_addr.sin_addr) < 0) {
		std::cout << "Invalid Pi address" << std::endl;
		return -1;
	}
	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
		std::cout << "Connection to pi failed" << std::endl;
		return -1;
	}
	std::cout << "Socket created" << std::endl;
	return 1;
}

int createProcessSocket(int &sock, sockaddr_in &serv_addr) {
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout << "Process socket creation error" << std::endl;
        return -1;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9999);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) < 0) {
        std::cout << "Invalid Process address" << std::endl;
        return -1;
    }
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "Connection to Process failed" << std::endl;
        return -1;
    }
    std::cout << "Socket created" << std::endl;
    return 1;
}

std::string getTimeString()
{
	std::stringstream ss;
	time_t now = time(0);
	tm* pnow = localtime(&now);
	ss << (pnow->tm_year + 1900) << "-" << (pnow->tm_mon + 1) << "-" << pnow->tm_mday << "T" << pnow->tm_hour << pnow->tm_min << pnow->tm_sec;
	
	return ss.str();
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

//get coordinates of most likely UAV. Assumes there is at least one buffered track packet (could be empty or have data)
//
coordinateStruct getMostUAV(parsed_packet& track)
{
	float vx, vy, vz, az, el, range = 0;
	int id = -1;
	long lastTime = 0;
	float pUAV = 0;
	int target = 0;
	bool tracking = false;
	if (track.header.nTracks == 0) {
		return coordinateStruct(vx, vy, vz, az, el, range, id, lastTime, tracking);
	}
	else if (track.header.nTracks > 1) {
		for (size_t i = 0; i < track.header->nTracks; i++) {
			if (track.tracks.at(i).probabilityUAV > pUAV) {
				pUAV = track.tracks.at(i).probabilityUAV;
				target = i;
			}
		}
	}

	vx = track.tracks.at(target).velxest;
	vz = track.tracks.at(target).velzest;
	vy = track.tracks.at(target).velyest;
	az = track.tracks.at(target).azest;
	el = track.tracks.at(target).elest;
	id = track.tracks.at(target).ID;
	range = track.tracks.at(target).rest;
	lastTime = track.tracks.at(target).lastAssociatedDataTime_ms;
	tracking = true;

	return coordinateStruct(vx, vy, vz, az, el, range, id, lastTime, tracking);
}








