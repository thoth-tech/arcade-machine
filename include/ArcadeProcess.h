#ifndef ARCADE_MACHINE_PROCESS_H
#define ARCADE_MACHINE_PROCESS_H

#include <array>
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <thread>

class ArcadeProcess {
	private:
		std::string _command;
		std::vector<std::string> _args;
		std::thread _execution_context;
		bool _is_running = false;
		void _execute_async_thread();

	public:
		std::vector<std::string> output_stream;
		ArcadeProcess(std::string command, std::vector<std::string> args);
		bool is_running() const { return this->_is_running; }
		std::string execute_sync();
		void execute_async();
};

#endif