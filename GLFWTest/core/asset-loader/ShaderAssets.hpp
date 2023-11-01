#pragma once
#include <string>
#include "AssetLoader.hpp"
#include "../logger/Logger.hpp"

namespace whim {

	namespace Assets {

		namespace Shaders {

			struct Shader {
			public:
				const char* shader_string;
				Shader(const std::string& filePath) {
					std::string temp_shader_string_ = whim::AssetLoader::load_shader_string(filePath);
					// Copy the shader data to shader_data_ member
					shader_data_ = temp_shader_string_;
					// Assign the pointer to the internal data of shader_data
					shader_string = shader_data_.c_str();
				}

			private:
				std::string shader_data_;
			};

			inline Shader default_vertex{ "assets/shaders/default.vertex.glsl" };
			inline Shader default_fragment{ "assets/shaders/default.fragment.glsl" };
		}
	}
}