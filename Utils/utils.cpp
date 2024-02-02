#include <iostream>
#include "bnet_interface.h"
#include "utils.h"
#include "thread"
#include "future"


void static monitorStatus(const bnet_interface& bnet) {
	
	while (true) {
		std::cout << "System state: " << bnet.get_status().data->sys_state << endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}