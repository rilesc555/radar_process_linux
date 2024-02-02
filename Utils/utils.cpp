#include <iostream>
#include "bnet_interface.h"
#include "utils.h"
#include "thread"
#include "future"


void monitorStatus( bnet_interface& bnet) {
	
	for (int i = 0; i < 10; i++) {
		std::this_thread::sleep_for(std::chrono::seconds(5)); 
		std::cout << "System state: " << bnet.get_status().data->eth_link_rate << endl;
	}
}