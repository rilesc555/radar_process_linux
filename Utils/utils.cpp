#include <iostream>
#include "bnet_interface.h"
#include "utils.h"
#include "thread"
#include "future"
#include "atomic"


void monitorStatus( bnet_interface& bnet) {
	std::atomic<bool> running(true);
	bnet.set_collect(STATUS_DATA, true);
	int i = 0;
	while (i < 10) {
		std::this_thread::sleep_for(std::chrono::seconds(5)); 
		std::cout << "System state: " << bnet.get_status().data->sys_state << endl;
		i++;
	}
}

void startupScript(bnet_interface& bnet) {
	string command, output;
	command = "RESET:PARAMETERS";
	bnet.send_command(command);
	command = "MODE:SWT:OPERATIONMODE 1";
	bnet.send_command(command);
	command = "enable_track";
	bnet.send_command(command);
	command = "enable_track_logging";
	bnet.send_command(command);
	command = "MODE:SWT:TRACK:ELFOVMIN -20";
	bnet.send_command(command);
	command = "RSP:RCSMASK:MAXRCS 10";
	bnet.send_command(command);
	command = "PLATFORM:STATE:ORIENTATION eldorado 0, 20, 0";
	bnet.send_command(command);
	command = "PLATFORM:STATE:ELEVATIONAGL eldorado 2";
	bnet.send_command(command);
	command = "RANGE:MASK eldorado 5,131,134,0,31";
	bnet.send_command(command);
	command = "AGLMASK:MINAGL eldorado 5";
	bnet.send_command(command);
	command = "AGLMASK:MAXAGL eldorado 500";
	bnet.send_command(command);
	command = "AGLMASK:ENABLE eldorado TRUE";
	bnet.send_command(command);
}

void outputTracks(bnet_interface& bnet) {



}