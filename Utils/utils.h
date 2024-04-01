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
	float az;
	float el;

	coordinateStruct() : az(0.0f), el(0.0f) {}
	coordinateStruct(float az, float el) : az(az), el(el) {}
};

void send_command(bnet_interface& bnet, std::string command);

void startupScript(bnet_interface& bnet);

int createSocket(int& sock, struct sockaddr_in& serv_addr);

void serializeCoordinates(coordinateStruct& coords, unsigned char* buffer);

coordinateStruct getMostUAV(bnet_interface& bnet);


#endif