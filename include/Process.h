#ifndef ARCADE_MACHINE_PROCESS_H
#define ARCADE_MACHINE_PROCESS_H
#include <string>
#include <unistd.h>

pid_t spawnProcess(std::string directory, std::string fileName);
bool processRunning(pid_t processId);

#endif