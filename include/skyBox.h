#pragma once
#include <array>
#include "Shader.h"
#include "buffers.h"
#include "Texture.h"
#include "freeLookCamera.h"



unsigned int vao;

Buffer SkyboxVertices;
Element_buffer skyboxIndexes;
shaderCompilation SkyboxShader;
CubeMap skybox;

//initializes a skybox
void genSkyBox(freeCamera Camera)
{
	float skyBoxMesh[] =
	{
		-1.0f, -1.0f,  -0.1f, //0
		 1.0f, -1.0f,  -0.1f, //1
		 1.0f,  1.0f,  -0.1f, //2
		-1.0f,  1.0f,  -0.1f, //3

		 1.0f, -1.0f, 20.0f, ///4
		-1.0f, -1.0f, 20.0f, ///5
		-1.0f,  1.0f, 20.0f, ///6
		 1.0f,  1.0f, 20.0f  //7
	};

	unsigned int skyIndicies[] =
	{
		0, 1, 2,
		2, 3, 0,

		1, 4, 7,
		7, 2, 1,

		4, 5, 6,
		6, 7, 4,

		5, 0, 3,
		3, 6, 5,

		3, 2, 7,
		7, 6, 3,

		0, 1, 4,
		4, 5, 0
	};

	GL_ERROR_MACRO(glGenVertexArrays(1, &vao));
	GL_ERROR_MACRO(glBindVertexArray(vao));
	
	SkyboxVertices = Buffer(skyBoxMesh, sizeof(skyBoxMesh));
	SkyboxVertices.Bind();

	skyboxIndexes = Element_buffer(skyIndicies, sizeof(skyIndicies));
	skyboxIndexes.Bind();

	GL_ERROR_MACRO(glEnableVertexAttribArray(0));
	GL_ERROR_MACRO(glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), 0));

	SkyboxShader = shaderCompilation("shaders/SkyBoxVertex.glsl", "shaders/SkyBoxFragment.glsl", "skybox_shader");
	SkyboxShader.Bind();

	std::vector<std::string> skyBoxPaths;
	skyBoxPaths.push_back("resources/skybox/posx.png");
	skyBoxPaths.push_back("resources/skybox/negx.png");
	skyBoxPaths.push_back("resources/skybox/negy.png");
	skyBoxPaths.push_back("resources/skybox/posy.png");
	skyBoxPaths.push_back("resources/skybox/posz.png");
	skyBoxPaths.push_back("resources/skybox/negz.png");

	std::string name = "skybox";
	skybox = CubeMap(skyBoxPaths, name);
	skybox.bindTextureToSlot(3);

	SkyboxShader.setUniformInt("skyBox", skybox.GetTextureSlot());
	SkyboxShader.setUniformMat4("projection", glm::perspective(glm::radians(45.0f), (float)1280 / (float)720, 0.1f, 20.0f));
	SkyboxShader.setUniformMat4("model", glm::mat4(1.0f));
	SkyboxShader.setUniformMat4("view", Camera.GetViewMatrix());
}


//draws a skybox
void renderSkyBox(freeCamera Camera)
{
	GL_ERROR_MACRO(glBindVertexArray(vao));

	SkyboxVertices.Bind();
	skyboxIndexes.Bind();
	SkyboxShader.Bind();
	skybox.bindTextureToSlot(3);

	SkyboxShader.setUniformInt("skyBox", skybox.GetTextureSlot());
	//SkyboxShader.setTextureSampler("skyBox", 3);
	SkyboxShader.setUniformMat4("projection", glm::perspective(glm::radians(90.0f), (float)1280 / (float)720, 0.1f, 20.0f));
	SkyboxShader.setUniformMat4("model", glm::mat4(1.0f));
	SkyboxShader.setUniformMat4("view", Camera.GetViewMatrix());

	GL_ERROR_MACRO(glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr));
}

