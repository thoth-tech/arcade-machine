#include "ArcadeProcess.h"

#include <array>
#include <string>
#include <vector>
#include <iostream>
#include <thread>


void ArcadeProcess::_execute_async_thread() {
	// Don't allow duplicate threads to execute.
	if (this->_is_running)
		return;

	std::string command = std::string(this->_command);
	for (auto arg : this->_args)
		command.append(" " + arg);

	this->_is_running = true;
	auto pipe = popen(command.c_str(), "r");
	if (! pipe)
		throw std::runtime_error("Error running process (async) `" + command + "`");

	std::array<char, 256> buffer;
	while (! feof(pipe)) {
		if (fgets(buffer.data(), 256, pipe) != nullptr)
			this->output_stream.push_back(buffer.data());
	}

	if (pclose(pipe) !=  EXIT_SUCCESS)
		std::cerr << "Unable to close process (async), possible memory leak" << std::endl;

	this->_is_running = false;
}

ArcadeProcess::ArcadeProcess(std::string command, std::vector<std::string> args) {
	this->_command = command;
	this->_args = args;
	this->output_stream = std::vector<std::string>();
}

std::string ArcadeProcess::execute_sync() {
	// This should never happen, but you never know...
	if (this->_is_running)
		throw std::runtime_error("Instance of process is already running.");

	std::string command = std::string(this->_command);
	for (auto arg : this->_args)
		command.append(" " + arg);

	this->_is_running = true;
	auto pipe = popen(command.c_str(), "r");
	if (! pipe)
		throw std::runtime_error("Error running process `" + command + "`");

	std::array<char, 256> buffer;
	std::string str_buffer;
	while (! feof(pipe)) {
		if (fgets(buffer.data(), 256, pipe) != nullptr)
			str_buffer += buffer.data();
	}

	if (pclose(pipe) !=  EXIT_SUCCESS)
		std::cerr << "Unable to close process, possible memory leak" << std::endl;

	this->_is_running = false;
	return str_buffer;
}

void ArcadeProcess::execute_async() {
	this->_execution_context = std::thread(&ArcadeProcess::_execute_async_thread, this);
	this->_execution_context.detach();
}
