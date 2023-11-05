#pragma once
#include <map>
#include <string>

namespace whim {
	class Input
	{
	public:
		static Input& getInstance() {
			static Input instance;
			return instance;
		}
	private:
		explicit Input();
		std::map<int, std::string> input_command_map_;
	};
}

