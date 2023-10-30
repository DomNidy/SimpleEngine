#pragma once

#include <cstdint>
#include <stdexcept>
#include <vector>
#include <Windows.h>

enum logger_level {
	OFF,
	LOW_VERBOSITY,
	HIGH_VERBOSITY
};

namespace whim {
	class Logger {
	public:
		// Returns the logger instance
		static Logger& getInstance();

		void log(const std::string& message);
		void log_error(const std::string& message);

		// Changes the logger prefix (NOTE: A space is automatically added to the end of the prefix)
		void set_logger_prefix(const std::string& prefix);

		// Updates color of logger prefix
		void set_prefix_color(WORD color_attribute);

	private:
		// Constructors
		explicit Logger();

		// Verbosity level of logs
		logger_level level_;
		// Stores windows color flag for prefix
		WORD color_attribute_;
		// Stores windows color flag for prefix when logging an error message
		WORD color_attribute_error_;
		// Pointer to console handle
		HANDLE hConsole_;
		// The log message
		std::vector<std::string> messages_;
		// The prefix our console logs will have, ex: [WHIM LOGGER]:
		std::string logger_prefix_ = "[WHIM LOGGER]: ";

		/// <summary>
		/// Returns a formatted string containing the current timestamp
		/// </summary>
		std::string get_timestamp_string();

		// Outputs a message on logger initialization
		void write_initialization_message_(logger_level level);
	};

}