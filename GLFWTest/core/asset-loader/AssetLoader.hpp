#pragma once
#include <string>
#include <unordered_map>

namespace whim {


	static class AssetLoader
	{
	public:
		explicit AssetLoader();
		static AssetLoader& getInstance();

		/// <summary>
		/// Loads a GLSL shader string from the given file
		/// </summary>
		/// <param name="filePath"></param>
		/// <returns>A string containing the shader in the defined file</returns>
		static std::string load_shader_string(const std::string& filePath);
	};
}

