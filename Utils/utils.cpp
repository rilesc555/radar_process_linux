#include <iostream>
#include "bnet_interface.h"
#include "utils.h"
#include "thread"
#include "future"
#include "atomic"


void monitorStatus( bnet_interface& bnet) {
	std::atomic<bool> running(true);
	bnet.set_collect(STATUS_DATA, true);
	while (running) {
		std::this_thread::sleep_for(std::chrono::seconds(5)); 
		std::cout << "System state: " << bnet.get_status().data->sys_state << endl;
	}
}