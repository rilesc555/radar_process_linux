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

struct coordinateStruct
{
	float az;
	float el;
	float range;

	coordinateStruct() : az(0.0f), el(0.0f), range(0.0f) {}
	coordinateStruct(float az, float el, float range) : az(az), el(el), range(range) {}
};


void startupScript(bnet_interface& bnet);

int createSocket(int& sock, struct sockaddr_in& serv_addr);

unsigned char* serializeCoordinates(coordinateStruct coords);

coordinateStruct getMostUAV(bnet_interface& bnet);


#endif