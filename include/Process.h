#ifndef ARCADE_MACHINE_PROCESS_H
#define ARCADE_MACHINE_PROCESS_H

#include <array>
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>

namespace Arcade {
	class Process {
		private:
			std::string _command;
			std::vector<std::string> _args;
			bool _is_running = false;

		public:
			Process(std::string command, std::vector<std::string> args) {
				this->_command = command;
				this->_args = args;
			}

			bool is_running() const { return this->_is_running; }

			/**
			 * @brief Executes the process synchronously.
			 * @return std::string The buffered stream from stdout / stderr
			 */
			std::string execute_sync() {
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

				std::array<char, 128> buffer;
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

			/**
			 * @brief Executes the process asynchronously in a thread.
			 */
			void execute_async(std::vector<std::string> &output_stream) {
				throw std::runtime_error("This method is currently not supported.");
			}
	};
}

#endif