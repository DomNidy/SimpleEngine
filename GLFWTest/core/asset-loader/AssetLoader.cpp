#include "AssetLoader.hpp"

namespace whim {
	AssetLoader::AssetLoader() {
		whim::Logger::log("Initialized the asset loader!");
	}

	AssetLoader& AssetLoader::getInstance()
	{
		static AssetLoader instance;
		return instance;
	}

	std::string AssetLoader::load_shader_string(const std::string& filePath)
	{
		std::ifstream shader_file;
		std::string shader_string;
		std::string line;

		whim::Logger::log("Attempting to load shader from file " + filePath);
		shader_file.open(filePath);

		if (!shader_file.is_open())
		{
			whim::Logger::log_error("Failed to open shader file " + filePath);
			throw std::domain_error("Failed to open file");
		}

		while (std::getline(shader_file, line)) {
			whim::Logger::log(line);
			shader_string += line + "\n";
		}
		return shader_string;
	}

};

