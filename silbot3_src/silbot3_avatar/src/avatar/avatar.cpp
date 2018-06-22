/*
 * avatarcir3d.cpp
 *
 *  Created on: 2014. 1. 15.
 *      Author: msyoo
 */

#include <cstdio>
#include <ros/ros.h>
#include <avatar/server/AvatarServer.h>

std::string exec(const char* cmd) {
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != NULL)
                result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
};

int main(int argc, char **argv) {
	ros::init(argc, argv, "silbot3_avatar");
	ros::Time::init();

	avatar::server::AvatarServer server;
	server.start();
	server.initROSService(argc, argv);
	string command = "ps -ef | grep flashplayerdebugger | grep -v grep | awk '{print $2}'";

	string result = exec(command.c_str());
	cout << result << endl;

	while(result.length() > 1) {
		string c2 = "kill -9 "+result;
		cout << c2 << endl;
		exec(c2.c_str());
		result = exec(command.c_str());
		cout << result << endl;
	}

	return -1;
}
