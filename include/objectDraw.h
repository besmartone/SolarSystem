#pragma once
#include <iostream>
#include <string>
#include "buffers.h"
#include "Shader.h"
#include "Animation.h"
#include "freeLookCamera.h"



class Object
{
public:
	Object(float* vertex_data, unsigned int vsize, float* normal_data, unsigned int norm_size,  float* tex_coord_data, unsigned int tex_size,  unsigned int* index_data = nullptr, unsigned int index_size = 0)
	{
		GL_ERROR_MACRO(glGenVertexArrays(1, &VAO));
		GL_ERROR_MACRO(glBindVertexArray(VAO));

		m_buff = Buffer(vertex_data, vsize + norm_size + tex_size);
		m_buff.updateBuffer(normal_data, norm_size, vsize);
		m_buff.updateBuffer(tex_coord_data, tex_size, vsize + norm_size);

		GL_ERROR_MACRO(glEnableVertexAttribArray(0));
		GL_ERROR_MACRO(glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, (const void*)(0)));

		GL_ERROR_MACRO(glEnableVertexAttribArray(1));
		GL_ERROR_MACRO(glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, (const void*)(vsize)));

		GL_ERROR_MACRO(glEnableVertexAttribArray(2));
		GL_ERROR_MACRO(glVertexAttribPointer(2, 2, GL_FLOAT, false, 0, (const void*)(vsize + norm_size)));

		if (index_data)
		{
			index_data_ = true;
			m_elements = Element_buffer(index_data, index_size);
			m_elements.Bind();
		}
	}

	Object(float* vertex_data, unsigned int vsize)
	{
		GL_ERROR_MACRO(glGenVertexArrays(1, &VAO));
		GL_ERROR_MACRO(glBindVertexArray(VAO));

		m_buff = Buffer(vertex_data, vsize);

		GL_ERROR_MACRO(glEnableVertexAttribArray(0));
		GL_ERROR_MACRO(glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(float), (const void*)(0)));

		GL_ERROR_MACRO(glEnableVertexAttribArray(1));
		GL_ERROR_MACRO(glVertexAttribPointer(1, 3, GL_FLOAT, false, 8 * sizeof(float), (const void*)(3 * sizeof(float))));

		GL_ERROR_MACRO(glEnableVertexAttribArray(2));
		GL_ERROR_MACRO(glVertexAttribPointer(2, 2, GL_FLOAT, false, 8 * sizeof(float), (const void*)(6 * sizeof(float))));
	}

	void Draw_object(unsigned int count)
	{
		GL_ERROR_MACRO(glBindVertexArray(VAO));
		m_buff.Bind();

		if (index_data_)
		{
			m_elements.Bind();
			GL_ERROR_MACRO(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr));
			return;
		}

		GL_ERROR_MACRO(glDrawArrays(GL_TRIANGLE_STRIP, 0, count));
	}

	void drawInterLeaved(unsigned int count)
	{
		GL_ERROR_MACRO(glBindVertexArray(VAO));
		m_buff.Bind();
		GL_ERROR_MACRO(glDrawArrays(GL_TRIANGLES, 0, count));
	}

private:
	bool index_data_ = 0;
	Buffer m_buff;
	Element_buffer m_elements;
	unsigned int VAO;
};
