#include "Process.h"
#include <string>
#include <unistd.h>
#include <iostream>
#include <array>
#include <sys/types.h>
#include <signal.h>

pid_t spawnProcess(std::string directory, std::string fileName) {

#ifndef _WIN32

	// First, fork the current process into a new process.
	// This is required to ensure that process execution occurs concurrently.
	pid_t processId = fork();
	if (processId > 0)
		return processId;

    std::array<char, 128> buffer;

	// The working directory must be changed to the root directory of the game
	// to ensure that SplashKit resources are correctly pathed when the process 
	// executes.
	chdir(directory.c_str());

	std::string cmd = "./" + fileName;
	auto pipe = popen(cmd.c_str(), "r");
	if (! pipe) {
		std::cerr << "Error executing popen" << std::endl;
		exit(EXIT_FAILURE);
	}

	// At this point, we're doing nothing with the stdout and stderr
	// streams. This could be extended by piping them into shared
	// memory for consumption by the main process.
	while (fgets(buffer.data(), 128, pipe) != NULL) {
		;
	}

	pclose(pipe);

	exit(EXIT_SUCCESS);

#endif

}

bool processRunning(pid_t processId) {
#ifndef _WIN32
	return getpgid(processId) >= 0;
#endif
}