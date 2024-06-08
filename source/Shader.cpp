#include "Shader.h"
#include <fstream>
#include <sstream>
#include "error.h"

std::string load_shader_from_file(const char* path)
{
	std::ifstream input;

	input.open(path);

	if (input.fail())
	{
		std::cerr << " error  shader not open \n " + std::string(path) << std::endl;
	}

	std::string result;
	std::string line;

	while (getline(input, line))
	{
		result += line; 	result += "\n";
	}

	return result;
}


	shaderCompilation::shaderCompilation(const char* vertex_filepath, const char* fragment_filepath, const char* name)
	{
		auto vertex = load_shader_from_file(vertex_filepath);	auto fragment = load_shader_from_file(fragment_filepath);

		unsigned int program = glCreateProgram();

		GL_ERROR_MACRO(unsigned shader_obj = glCreateShader(GL_VERTEX_SHADER));
		const char* src = vertex.c_str();
		GL_ERROR_MACRO(glShaderSource(shader_obj, 1, &src, nullptr));
		GL_ERROR_MACRO(glCompileShader(shader_obj));
		
		int status = 0;
		GL_ERROR_MACRO(glad_glGetShaderiv(shader_obj, GL_COMPILE_STATUS, &status));

		if (status == GL_FALSE)
		{
			int length;
			GL_ERROR_MACRO(glad_glGetShaderiv(shader_obj, GL_INFO_LOG_LENGTH, &length));

			char* message = (char*)alloca(sizeof(char) * length);
			GL_ERROR_MACRO(glad_glGetShaderInfoLog(shader_obj, length, &length, message));

			std::cout << m_name + " error -> " << "[vertex shader error: ]"  << message << std::endl;
		}

		unsigned int m_vs = shader_obj;

		shader_obj = 0;
		src = nullptr;
		GL_ERROR_MACRO(shader_obj = glCreateShader(GL_FRAGMENT_SHADER));
		src = fragment.c_str();
		GL_ERROR_MACRO(glShaderSource(shader_obj, 1, &src, nullptr));
		GL_ERROR_MACRO(glCompileShader(shader_obj));

		status = 0;
		GL_ERROR_MACRO(glad_glGetShaderiv(shader_obj, GL_COMPILE_STATUS, &status));

		if (status == GL_FALSE)
		{
			int length;
			GL_ERROR_MACRO(glad_glGetShaderiv(shader_obj, GL_INFO_LOG_LENGTH, &length));

			char* message = (char*)alloca(sizeof(char) * length);
			GL_ERROR_MACRO(glad_glGetShaderInfoLog(shader_obj, length, &length, message));

			std::cout << m_name + " error -> " << "[Fragment shader error: ]"  << message << std::endl;
		}

		unsigned int m_fs = shader_obj;

		GL_ERROR_MACRO(glAttachShader(program, m_vs));
		GL_ERROR_MACRO(glAttachShader(program, m_fs));

		GL_ERROR_MACRO(glLinkProgram(program));

		
		GL_ERROR_MACRO(glValidateProgram(program));

		object_ID_ = program;
	}


	shaderCompilation::~shaderCompilation()
	{

	}

	void shaderCompilation::Bind()const 
	{
		GL_ERROR_MACRO(glUseProgram(object_ID_));
	}

	void shaderCompilation::Unbind()const
	{
		GL_ERROR_MACRO(glUseProgram(0));
	}

	void shaderCompilation::setUniformMat4(const char* name, const glm::mat4 data, unsigned int count)
	{
		Bind();
		GL_ERROR_MACRO(glUniformMatrix4fv(getUniformLocation(name), (int)count, GL_FALSE, &( data[0][0]) ));
	}

	void shaderCompilation::setUniformFloat3(const char* name, const glm::vec3& data, unsigned int count)
	{
		Bind();
		GL_ERROR_MACRO(glUniform3fv(getUniformLocation(name), (int)count, &data[0]));
	}



	void shaderCompilation::setUniformInt(const char* name, int r_data, unsigned int count)
	{
		Bind();
		GL_ERROR_MACRO(glUniform1iv(getUniformLocation(name), (int)count, &r_data));
	}


	void shaderCompilation::setUniformFloat(const char* name, float data)
	{
		Bind();
		GL_ERROR_MACRO(glUniform1f(getUniformLocation(name), data));
	}




	int shaderCompilation::getUniformLocation(const char* name)
	{
		if (uniform_Locations_cache.find(name) != uniform_Locations_cache.end())
		{
			return  uniform_Locations_cache[name];
		}

		GL_ERROR_MACRO(int location = glGetUniformLocation(object_ID_, name));
		std::cout << (location < 0 || location == -1? name : "") << (location < 0 ? " is not a valid uniform" : "") << std::endl;
		uniform_Locations_cache[name] = location;
		return location;
	}

