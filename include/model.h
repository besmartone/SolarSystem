#pragma once

#include <string>
#include <iostream>
#include <vector>
#include "Shader.h"
#include "tiny_obj_loader.h"

#include "error.h"
#include <iostream>
#include "stb_image.h"

namespace Models
{
	bool LoadObj(tinyobj::attrib_t& attrib, std::vector<tinyobj::shape_t>& shapes, std::vector<tinyobj::material_t>& materials, const char* filename, const char* basepath = NULL, bool triangulate = true);

	class MeshObject
	{
		//Data
		unsigned int Vao;
		unsigned int Vbo;
		unsigned int TVB;
		unsigned int NVB;
		unsigned int Texture_object; 
		
		shaderCompilation& program;

		unsigned int Hsize = 0;
		unsigned int Hsize2 = 0;
		unsigned int Hsize3 = 0;


		int vertex_count = 0;

	public:
		MeshObject() = default;

		MeshObject(std::string objectPath, std::string basePath, std::string Texture, shaderCompilation& GPUprogram);
		~MeshObject();
		void draw(glm::mat4 Model, int slot);
	};


}
