#ifndef ARCADE_MACHINE_PROCESS_H
#define ARCADE_MACHINE_PROCESS_H
#include <string>
#include <unistd.h>

pid_t spawnProcess(std::string processFp);
bool processRunning(pid_t processId);

#endif