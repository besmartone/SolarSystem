#pragma once
#include "error.h"

class Buffer
{
public:
	Buffer() = default;

	Buffer(void* data, unsigned int size);

	~Buffer();


	void operator=(const Buffer& va)
	{
		this->Object_ID_ = va.Object_ID_;
	}

	void updateBuffer(float* data, size_t size, int offset);

	void Bind(GLenum bufferType = GL_ARRAY_BUFFER);
	void unBind(GLenum bufferType = GL_ARRAY_BUFFER);

private:
	void initialize_buffer_(void* data, unsigned int size)
	{
		GL_ERROR_MACRO(glGenBuffers(1, &Object_ID_));
		GL_ERROR_MACRO(glBindBuffer(GL_ARRAY_BUFFER, Object_ID_));
		GL_ERROR_MACRO(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
	}

	unsigned int Object_ID_;
};


class Element_buffer
{
public:
	Element_buffer() = default;
	Element_buffer(unsigned int* data, unsigned int size);
	
	void operator =(Element_buffer&& data)
	{
		this->Object_ID_ = data.Object_ID_;
		data.Object_ID_ = 0;
	}

	Element_buffer(Element_buffer& copy)
	{
		this->Object_ID_ = copy.Object_ID_;
		copy.Object_ID_ = 0;
	}

	~Element_buffer();

	void operator =(const Element_buffer& va)
	{
		this->Object_ID_ = va.Object_ID_;
	}

	
	void Bind();
	void unBind();

private:
	unsigned int Object_ID_;
	unsigned m_size;
};


