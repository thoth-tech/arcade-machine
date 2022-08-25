#include "Process.h"
#include <string>
#include <unistd.h>
#include <stdexcept>
#include <iostream>
#include <array>
 #include <sys/types.h>
 #include <signal.h>

pid_t spawnProcess(std::string processFp) {
	pid_t processId = fork();
	if (processId > 0)
		return processId;

    std::array<char, 128> buffer;
    std::string result;

	auto pipe = popen(processFp.c_str(), "r");
	if (! pipe) {
		std::cerr << "Error executing popen" << std::endl;
		exit(EXIT_FAILURE);
	}

	while (fgets(buffer.data(), 128, pipe) != NULL)
		result += buffer.data();

	auto returnCode = pclose(pipe);

	exit(EXIT_SUCCESS);
}

bool processRunning(pid_t processId) {
	return getpgid(processId) >= 0;
}