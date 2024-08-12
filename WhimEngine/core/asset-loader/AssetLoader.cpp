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

	ImageTextureData AssetLoader::load_texture_data_from_image(const std::string& texture_file_path)
	{

		int width, height, num_channels;
		unsigned char* texture_data = stbi_load(texture_file_path.c_str(), &width, &height, &num_channels, 0);
		if (!texture_data) {
			whim::Logger::log_error("Faled to load texture from " + texture_file_path);
		}
		else {
			whim::Logger::log("Loaded texture");
			whim::Logger::log("Texture dimensions: " + std::to_string(width) + "x" + std::to_string(height) + "," + std::to_string(num_channels));
		}

		ImageTextureData image_data = ImageTextureData();
		image_data.height = height;
		image_data.width = width;
		image_data.num_channels = num_channels;

		return ImageTextureData();
	}

};

void ImageTextureData::free_image()
{
	whim::Logger::log("Freeing texture data");
	stbi_image_free(data);
}

ImageTextureData::~ImageTextureData()
{
	whim::Logger::log("Destructor");
}
