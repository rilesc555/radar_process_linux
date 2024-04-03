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

struct coordinateStruct
{
	float vx;
	float vy;
	float vz;
	float az;
	float el;
	int id;
	//tracks the last time the UAV track was updated in milliseconds
	long lastTime;


	coordinateStruct() : vx(0.0f), vy(0.0f), vz(0.0f), az(0.0f), el(0.0f), lastTime(0L) {}
	coordinateStruct(float vx, float vy, float vz, float az, float el, int ID, long lastTime) : vx(vx), vy(vy), vz(vz), az(az), el(el), id(ID),  lastTime(lastTime) {}
};

void send_command(bnet_interface& bnet, std::string command);

void startupScript(bnet_interface& bnet);

void setTime(bnet_interface& bnet);

int createSocket(int& sock, struct sockaddr_in& serv_addr);

void serializeCoordinates(coordinateStruct& coords, unsigned char* buffer);

coordinateStruct getMostUAV(bnet_interface& bnet);


#endif