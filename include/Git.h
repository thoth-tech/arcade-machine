#ifndef ARCADE_MACHINE_GIT_H
#define ARCADE_MACHINE_GIT_H
#include "Process.h"

namespace Arcade::Git {
	void clone(std::string url, std::string directory) {
		auto p = Arcade::Process(
			"git", 
			std::vector<std::string> {
				"clone", 
				url,
				directory
			}
		);

		p.execute_sync();
	}

	void pull(std::string url, std::string directory) {
		auto p = Arcade::Process(
			"git", 
			std::vector<std::string> {
				"-C", 
				directory,
				"pull",
				url
			}
		);

		p.execute_sync();
	}
}

#endif