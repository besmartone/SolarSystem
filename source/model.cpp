#include "model.h"
#include <iostream>
#include <string>

namespace Models
{
	bool LoadObj(tinyobj::attrib_t& attrib, std::vector<tinyobj::shape_t>& shapes, std::vector<tinyobj::material_t>& materials, const char* filename, const char* basepath, bool triangulate ) {
	
		std::string err;

		bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename, basepath, triangulate);
	
		std::cout << "[ERRORS:] " << err << std::endl;

		if (!ret) {
			printf("Failed to load/parse .obj.\n");
			return false;
		}

		return true;
	}


	MeshObject::MeshObject(std::string objectPath, std::string basePath, std::string Texture, shaderCompilation& GPUprogram)
		:program(GPUprogram)
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		Models::LoadObj(attrib, shapes, materials, objectPath.c_str(), basePath.c_str(), true);

		std::vector<float> vertex;
		std::vector<float> normals;
		std::vector<float> texCoords;


		for (auto var : shapes)
		{
			int indexOffset = 0;
			for (int i = 0; i < var.mesh.num_face_vertices.size(); i++)
			{
				int ii = var.mesh.num_face_vertices[i];


				for (int v = 0; v < ii; v++)
				{
					auto j = var.mesh.indices[indexOffset + v];

					vertex.push_back(attrib.vertices[(3 * j.vertex_index) + 0]);
					vertex.push_back(attrib.vertices[(3 * j.vertex_index) + 1]);
					vertex.push_back(attrib.vertices[(3 * j.vertex_index) + 2]);

					normals.push_back(attrib.normals[(3 * j.normal_index) + 0]);
					normals.push_back(attrib.normals[(3 * j.normal_index) + 1]);
					normals.push_back(attrib.normals[(3 * j.normal_index) + 2]);

					texCoords.push_back(attrib.texcoords[(2 * j.texcoord_index) + 0]);
					texCoords.push_back(attrib.texcoords[(2 * j.texcoord_index) + 1]);
				}

				indexOffset += ii;
			}



		}

		Hsize = vertex.size() * sizeof(float);
		Hsize2 = normals.size() * sizeof(float);
		Hsize3 = texCoords.size() * sizeof(float);
		
		GL_ERROR_MACRO(glGenBuffers(1, &Vbo));
		GL_ERROR_MACRO(glBindBuffer(GL_ARRAY_BUFFER, Vbo));
		GL_ERROR_MACRO(glBufferData(GL_ARRAY_BUFFER, Hsize, vertex.data(), GL_STATIC_DRAW));


		GL_ERROR_MACRO(glGenBuffers(1, &NVB));
		GL_ERROR_MACRO(glBindBuffer(GL_ARRAY_BUFFER, NVB));
		GL_ERROR_MACRO(glBufferData(GL_ARRAY_BUFFER, Hsize2, normals.data(), GL_STATIC_DRAW));

		GL_ERROR_MACRO(glGenBuffers(1, &TVB));
		GL_ERROR_MACRO(glBindBuffer(GL_ARRAY_BUFFER, TVB));
		GL_ERROR_MACRO(glBufferData(GL_ARRAY_BUFFER, Hsize3, texCoords.data(), GL_STATIC_DRAW));

		vertex_count = vertex.size() / 3.0f;

		GL_ERROR_MACRO(glGenVertexArrays(1, &Vao));
		GL_ERROR_MACRO(glBindVertexArray(Vao));
		GL_ERROR_MACRO(glBindBuffer(GL_ARRAY_BUFFER, Vbo));

		
		GL_ERROR_MACRO(glEnableVertexAttribArray(0));
		GL_ERROR_MACRO(glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), 0));

		GL_ERROR_MACRO(glBindBuffer(GL_ARRAY_BUFFER, NVB));
		GL_ERROR_MACRO(glEnableVertexAttribArray(1));
		GL_ERROR_MACRO(glVertexAttribPointer(1, 3, GL_FLOAT, false, 3 * sizeof(float), 0));

		GL_ERROR_MACRO(glBindBuffer(GL_ARRAY_BUFFER, TVB));
		GL_ERROR_MACRO(glEnableVertexAttribArray(2));
		GL_ERROR_MACRO(glVertexAttribPointer(2, 2, GL_FLOAT, false, 2 * sizeof(float), 0));


		int m_width = 0; int m_hieght = 0;
		unsigned char* _char_ = nullptr;
		if (Texture.find("png") != std::string::npos || Texture.find(".png") != std::string::npos)
		{
			stbi_set_flip_vertically_on_load(1);
		}

		int m_BPP;
		_char_ = stbi_load(Texture.c_str(), &m_width, &m_hieght, &m_BPP, 4);//TODO chek the meaning of bits per pixel

		if (!_char_)
		{
			std::cerr << "[ERROR:] failed to load texture:  " << std::endl;
		}

		GL_ERROR_MACRO(glGenTextures(1, &Texture_object));
		GL_ERROR_MACRO(glBindTexture(GL_TEXTURE_2D, Texture_object));

		GL_ERROR_MACRO(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GL_ERROR_MACRO(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GL_ERROR_MACRO(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GL_ERROR_MACRO(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

		GL_ERROR_MACRO(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_hieght, 0, GL_RGBA, GL_UNSIGNED_BYTE, _char_));
		GL_ERROR_MACRO(glBindTexture(GL_TEXTURE_2D, 0));


		if (_char_)
		{
			stbi_image_free(_char_);
		}


	}

	MeshObject::~MeshObject()
	{
	}

	void MeshObject::draw(glm::mat4 Model, int slot)
	{
		program.Bind();
		program.setUniformFloat3("color", glm::vec3(0.0f));
		program.setUniformMat4("model", Model);

		GL_ERROR_MACRO(glActiveTexture((GL_TEXTURE0 + slot)));
		GL_ERROR_MACRO(glBindTexture(GL_TEXTURE_2D, Texture_object));

		program.setUniformInt("Texture", slot);

		GL_ERROR_MACRO(glBindVertexArray(Vao));

		GL_ERROR_MACRO(glBindBuffer(GL_ARRAY_BUFFER, Vbo));

		GL_ERROR_MACRO(glDrawArrays(GL_TRIANGLES, 0, vertex_count));
	}

}