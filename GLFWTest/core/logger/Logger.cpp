#include <string>
#include "logger.hpp"
#include <iostream>
#include <ctime>
#include <sstream>
#include <chrono>
#include <GL/glew.h>

namespace whim {
	Logger& Logger::getInstance()
	{
		static Logger instance;
		return instance;
	}

	Logger::Logger() : level_(LOW_VERBOSITY)
	{
		this->color_attribute_ = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		this->color_attribute_error_ = FOREGROUND_RED | FOREGROUND_INTENSITY;
		this->hConsole_ = GetStdHandle(STD_OUTPUT_HANDLE);
		write_initialization_message_(LOW_VERBOSITY);
	}

	void Logger::log(const std::string& message)
	{
		// Create log entry string
		std::string log_entry = get_timestamp_string() + message;

		// Append log message to log array
		this->messages_.push_back(log_entry);

		// Apply coloring for the prefix and write prefix
		SetConsoleTextAttribute(this->hConsole_, this->color_attribute_);
		std::cout << logger_prefix_;

		// Set color for log message back to white and write log message
		SetConsoleTextAttribute(this->hConsole_, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
		std::cout << log_entry << std::endl;

	}

	void Logger::log_error(const std::string& message)
	{
		std::string log_entry = get_timestamp_string() + "ERROR: " + message;
		this->messages_.push_back(log_entry);

		// Apply coloring for the prefix and write prefix
		SetConsoleTextAttribute(this->hConsole_, this->color_attribute_error_);
		std::cout << logger_prefix_ << log_entry << std::endl;
	}

	void Logger::set_logger_prefix(const std::string& prefix)
	{
		logger_prefix_ = prefix + " ";
		Logger::log("Changed logger prefix to" + prefix + " ");
	}

	void Logger::set_prefix_color(WORD color_attribute)
	{
		color_attribute_ = color_attribute;
		log("Changed logger prefix color!");
	}


	std::string Logger::get_timestamp_string()
	{
		// Get current time
		std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
		std::time_t log_time = std::chrono::system_clock::to_time_t(now);
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());

		// Format the timestamp as a string
		std::tm timestamp;
		localtime_s(&timestamp, &log_time);
		char timestamp_string[44];
		std::strftime(timestamp_string, sizeof(timestamp_string), "[%Y-%m-%d %H:%M:%S.", &timestamp);

		return std::string(timestamp_string) + std::to_string(ms.count() % 1000) + "] ";
	}

	void Logger::write_initialization_message_(logger_level level)
	{
		switch (level) {
		case LOW_VERBOSITY:
			this->log("Initialized whim logger with LOW_VERBOSITY");
			break;
		case HIGH_VERBOSITY:
			this->log("Initialized whim logger with HIGH_VERBOSITY");
			break;
		case OFF:
			this->log("Initialized whim logger with OFF (logger is disabled)");
			break;
		default:
			this->log("Initialized whim logger with an unknown logger level");
		}
	}
}
