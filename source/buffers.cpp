#include "buffers.h"


Buffer::Buffer(void* data, unsigned int size)
{
	this->initialize_buffer_(data, size);
}

Buffer::~Buffer()
{
}


void Buffer::Bind(GLenum bufferType)
{
	GL_ERROR_MACRO(glBindBuffer(GL_ARRAY_BUFFER, Object_ID_));
}

void Buffer::updateBuffer(float* data, size_t size, int offset)
{
	Bind();
	int val = 1;
	GL_ERROR_MACRO(glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &val));
	GL_ERROR_MACRO(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
}

void Buffer::unBind(GLenum bufferType)
{
	GL_ERROR_MACRO(glBindBuffer(GL_ARRAY_BUFFER, 0));
}


Element_buffer::Element_buffer(unsigned int* data, unsigned int size)
{
	
	GL_ERROR_MACRO(glGenBuffers(1, &Object_ID_));

	GL_ERROR_MACRO(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Object_ID_));

	GL_ERROR_MACRO(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

Element_buffer::~Element_buffer()
{
	//if(Object_ID_ != 0)	GL_ERROR_MACRO(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Object_ID_));	GL_ERROR_MACRO(glDeleteBuffers(1, &Object_ID_));

}


void Element_buffer::Bind()
{
	GL_ERROR_MACRO(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Object_ID_));
}

void Element_buffer::unBind()
{
	GL_ERROR_MACRO(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
