#ifndef UTILS_H
#define UTILS_H


#include <iostream>
#include "bnet_interface.h"
#include "thread"
#include "future"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <csignal>
#include <ctime>
#include "parsers.h"

struct coordinateStruct
{
	float vx;
	float vy;
	float vz;
	float az;
	float el;
	float range;
	int id;
	//tracks the last time the UAV track was updated in milliseconds
	long lastTime;
	bool tracking;

	coordinateStruct() : vx(0.0f), vy(0.0f), vz(0.0f), az(0.0f), el(0.0f), range(0), lastTime(0L), tracking(false) {}
	coordinateStruct(float vx, float vy, float vz, float az, float el, float range, int ID, long lastTime, bool tracking) : vx(vx), vy(vy), vz(vz), az(az), el(el), range(range), id(ID), lastTime(lastTime), tracking(tracking) {}
};

void send_command(bnet_interface& bnet, std::string command);

void startupScript(bnet_interface& bnet);

void setTime(bnet_interface& bnet);

int createPiSocket(int& sock, struct sockaddr_in& serv_addr);

int createProcessSocket(int& sock, struct sockaddr_in& serv_addr);

std::string getTimeString();

void serializeCoordinates(coordinateStruct& coords, unsigned char* buffer);

coordinateStruct getMostUAV(parsed_packet& packet);


#endif