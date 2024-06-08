#ifndef OPENGL_SHADER_H
#define  OPENGL_SHADER_H

#include <unordered_map>
#include <string>
#include <iostream>
#include <optional>
#include "glm/glm.hpp"

	class shaderCompilation
		{
		public:
			shaderCompilation(const char* vertex_filepath, const char* fragment_filepath, const char* name);
			shaderCompilation(){}
			~shaderCompilation();

			void Bind()const ;
			void Unbind()const ;
			void setUniformMat4(const char* name, const glm::mat4 data, unsigned int count = 1);
			void setUniformFloat3(const char* name, const glm::vec3& data, unsigned int count = 1);
			void setUniformInt(const char* name, int r_data, unsigned int count = 1);
			void setUniformFloat(const char* name, float data);

			unsigned int return_object() {
				return object_ID_;
			}

		private:
			std::string m_name;
			unsigned int object_ID_;
			std::string m_shader_filePath;
			std::unordered_map <std::string, int> uniform_Locations_cache;
			int getUniformLocation(const char* name);
	};

#endif
