#pragma once
#include <string>
#include <unordered_map>
#include "../logger/Logger.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include "../../thirdparty/stb_image/stb_image.h"


struct ImageTextureData {
	/// <summary>
	/// The actual texture data we read from the file
	/// </summary>
	unsigned char* data;
	/// <summary>
	/// Width of the image file we read this texture from
	/// </summary>
	int width;
	/// <summary>
	/// Height of the image file we read this texture from
	/// </summary>
	int height;
	/// <summary>
	/// Number of channels the file we read this texture from used
	/// </summary>
	int num_channels;
	/// <summary>
	/// Frees the memory used to store the texture data
	/// </summary>
	void free_image();
	~ImageTextureData();
};

namespace whim {


	class AssetLoader
	{
	public:
		explicit AssetLoader();
		static AssetLoader& getInstance();

		/// <summary>
		/// Loads a GLSL shader string from the given file
		/// </summary>
		/// <param name="filePath"></param>
		/// <returns>A string containing the shader in the defined file</returns>
		static std::string load_shader_string(const std::string& file_path);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="texture_file_path"></param>
		/// <returns></returns>
		static ImageTextureData load_texture_data_from_image(const std::string& texture_file_path);
	};
}

