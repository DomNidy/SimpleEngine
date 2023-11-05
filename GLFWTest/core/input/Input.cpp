#include "Input.h"
#include <map>
#include <string>
namespace whim {
	Input::Input() {
		Input::input_command_map_.emplace(87, "MOVE_FWD");
	}
}