#include "AssetLoader.hpp"
#include "../logger/Logger.hpp"
#include <fstream>
#include <iostream>
#include <map>



namespace whim {
	AssetLoader::AssetLoader() {
		Logger::getInstance().log("Initialized the asset loader!");
	}

	AssetLoader& AssetLoader::getInstance()
	{
		static AssetLoader instance;
		return instance;
	}

	std::string AssetLoader::load_shader_string(const std::string& filePath)
	{
		Logger logger = Logger::getInstance();
		std::ifstream shader_file;
		std::string shader_string;
		std::string line;

		logger.log("Attempting to load shader from file " + filePath);
		shader_file.open(filePath);

		if (!shader_file.is_open())
		{
			logger.log_error("Failed to open shader file " + filePath);
			throw std::domain_error("Failed to open file");
		}

		while (std::getline(shader_file, line)) {
			logger.log(line);
			shader_string += line + "\n";
		}
		return shader_string;
	}

};

