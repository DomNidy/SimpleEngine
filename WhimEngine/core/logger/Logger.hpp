#pragma once
#include <glad/glad.h>
#include <iostream>
#include <sstream>
#include <cstdint>
#include <stdexcept>
#include <Windows.h>
#include <vector>
#include <string>
#include "logger.hpp"
#include <chrono>
#include <ctime>

enum logger_level
{
	OFF,
	LOW_VERBOSITY,
	HIGH_VERBOSITY
};

namespace whim
{
	class Logger
	{
	public:
		static void log(const std::string& message);
		static void log_error(const std::string& message);
		static void log_warning(const std::string& message);

		// Changes the logger prefix (NOTE: A space is automatically added to the end of the prefix)
		static void set_logger_prefix(const std::string& prefix);

		// Updates color of logger prefix
		static void set_prefix_color(WORD color_attribute);

	private:
		// Constructors
		explicit Logger();

		// Verbosity level of logs
		static logger_level level_;
		// Stores windows color flag for prefix
		static WORD color_attribute_;
		// Stores windows color flag for prefix when logging an error message
		static WORD color_attribute_error_;
		// Stores warning color flag
		static WORD color_attribute_warn_;
		// Pointer to console handle
		static HANDLE hConsole_;
		// The log message
		static std::vector<std::string> messages_;
		// The prefix our console logs will have, ex: [WHIM LOGGER]:
		static std::string logger_prefix_;

		/// <summary>
		/// Returns a formatted string containing the current timestamp
		/// </summary>
		static std::string get_timestamp_string();

		// Outputs a message on logger initialization
		static void write_initialization_message_(logger_level level);
	};

}