#ifndef UTILS_H
#define UTILS_H


#include <iostream>
#include "bnet_interface.h"
#include "thread"
#include "future"

using namespace std;

void monitorStatus(bnet_interface& bnet);

void startupScript(bnet_interface& bnet);

void outputTracks(bnet_interface& bnet); 



#endif