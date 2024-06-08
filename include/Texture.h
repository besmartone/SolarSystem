#pragma once

#include <string>
#include <vector>
#include "error.h"
#include <array>

class ITexture
{
public:
	//default ctor and dtor
	ITexture()	{};
	~ITexture() {};

	//set nothing to a texture slot
	void unbind(unsigned int slot = 0);
	//returns name
	void setTexturesName(const std::string& name);
	//returns name
	void setTexturesName(const std::string&& name);
	//binds texture to a slot 
	//no of available slots to bind to is variable
	virtual void bindTextureToSlot(unsigned int slot = 0) = 0;

	const std::string& getName() { return m_textureName; }
	unsigned int getId()const { return m_rendererID; }
	unsigned int GetTextureSlot() { return m_presentTextureSlot; }

protected:
	unsigned int m_rendererID;
	std::string m_textureName;
	int m_width, m_hieght, m_BPP;
	unsigned int m_presentTextureSlot;
	static unsigned int presentlyBounded;
	static unsigned int  presentTextureType;
};

	class Texture_2D : public ITexture
	{
	public:
		Texture_2D() = default;
		Texture_2D(unsigned char* data);
		Texture_2D(const std::string& texture_filePath, const std::string& name = "m_texture", unsigned int format = GL_RGBA, unsigned internalFormat = GL_RGBA8);
		Texture_2D(GLenum internalFormat, GLenum format, int width, int hieght);
		~Texture_2D();

		void bindTextureToSlot(unsigned int slot = 0)override;
	};


	class CubeMap : public ITexture
	{
	public:
		CubeMap() = default;
		CubeMap(std::vector<std::string>& texture_filePaths, std::string& name, GLenum format = GL_RGBA, GLenum internalFormat = GL_RGBA8);
		CubeMap(std::array<Texture_2D, 6> textures);
		 ~CubeMap();

		void operator=(const CubeMap&& texture);
		void bindTextureToSlot(unsigned int slot = 0)override;
	};


	