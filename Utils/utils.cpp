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

//sends a command to a radar and prints the result. In case you're into that sort of thing
void send_command(bnet_interface& bnet, std::string command)
{
	std::string result = bnet.send_command(command).second;
	std::cout << result << std::endl;
}

//sets a bunch of parameters on the radar. Edit this to change masking, etc.
void startupScript(bnet_interface& bnet) {
	std::string command, output;
	command = "RESET:PARAMETERS";
	bnet.send_command(command);

	setTime(bnet);

	command = "MODE:SWT:OPERATIONMODE 1";
	bnet.send_command(command);
	bnet.set_save(TRACK_DATA, true);
	bnet.set_collect(TRACK_DATA, true);
	bnet.set_save(RVMAP_DATA, true);
	bnet.set_collect(RVMAP_DATA, true);
	bnet.set_save(STATUS_DATA, true);
	bnet.set_collect(STATUS_DATA, true);
	bnet.set_logging(true);


	command = "MODE:SWT:TRACK:ELFOVMIN -10";
	bnet.send_command(command);
	command = "RSP:RCSMASK:MAXRCS 10";
	bnet.send_command(command);
	command = "PLATFORM:STATE:ORIENTATION eldorado 0, 20, 0";
	bnet.send_command(command);
	command = "PLATFORM:STATE:ELEVATIONAGL eldorado 0";
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

//sets the time of the radar to match the system time
void setTime(bnet_interface &bnet)
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

void setMaxRangeMask(bnet_interface& bnet, int maxRange)
{
	send_command(bnet, "MODE:SWT:START");
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	int range0;
	float dR;
	while (true) {
		if (bnet.get_n_buffered(RVMAP_DATA) > 0) {
			auto rvmap = bnet.get_rvmap();
			range0 = rvmap.header->zero_range_bin;
			dR = rvmap.header->dR;
			std::cout << "Zero range bin: " << range0 << std::endl;
			std::cout << "Range Resolution: " << dR << std::endl;
			break;
		}
		else {
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
			std::cout << "Waiting for RVMap data" << std::endl;
		}
	}
	send_command(bnet, "MODE:SWT:STOP");
	bnet.set_save(RVMAP_DATA, false);
	bnet.set_collect(RVMAP_DATA, false);
	auto maxRangeToBlock = (maxRange / dR) + range0;
	std::ostringstream ss;
	ss << maxRangeToBlock;
	std::string maxRangeBin(ss.str());
	std::string command = "RANGE:MASK eldorado 0," + maxRangeBin + ",2047,0,31";
	send_command(bnet, command);
}

//creates a socket and connects to the pi
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
		std::cout << "Connection to Pi failed" << std::endl;
		return -1;
	}
	std::cout << "Socket created" << std::endl;
	return 1;
}

//gets the current system time and returns it in YYYY-MM-DDTHHMMSS format
std::string getTimeString()
{
	std::stringstream ss;
	time_t now = time(0);
	tm* pnow = localtime(&now);
	ss << (pnow->tm_year + 1900) << "-" << (pnow->tm_mon + 1) << "-" << pnow->tm_mday << "T" << pnow->tm_hour << pnow->tm_min << pnow->tm_sec;
	return ss.str();
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

//takes a coordinate structure and puts azimuth and elevation in a buffer for sending over socket
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

//get coordinates of most likely UAV. Assumes there is at least one buffered track packet (could be empty or have data). Returns coordinate struct of that
coordinateStruct getMostUAV(bnet_interface& bnet)
{
	float vx, vy, vz, az, el = 0;
	int id = -1;
	long lastTime = 0;
	float pUAV = 0;
	int target = 0;
	bool tracking = false;
	MESAK_Track track = bnet.get_track();
	if (track.header->nTracks == 0) {
		return coordinateStruct(vx, vy, vz, az, el, id, lastTime, tracking);
	}
	else if (track.header->nTracks > 1) {
		for (size_t i = 0; i < track.header->nTracks; i++) {
			if (track.data.at(i).probabilityUAV > pUAV) {
				pUAV = track.data.at(i).probabilityUAV;
				target = i;
			}
		}
	}

	vx = track.data.at(target).velxest;
	vz = track.data.at(target).velzest;
	vy = track.data.at(target).velyest;
	az = track.data.at(target).azest;
	el = track.data.at(target).elest;
	id = track.data.at(target).ID;
	lastTime = track.data.at(target).lastAssociatedDataTime_ms;
	tracking = true;
	
	return coordinateStruct(vx, vy, vz, az, el, id, lastTime, tracking);
}






