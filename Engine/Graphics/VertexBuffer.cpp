#include "VertexBuffer.h"

namespace gme {
	VertexBuffer::~VertexBuffer() 	{
		//<if vao is not 0 then glDeleteVertexArrays>
		//<if vbo is not 0 then glDeleteBuffers>
		if (vao) {
			glDeleteVertexArrays(1, &vao);
		}
		if (vbo) {
			glDeleteBuffers(1, &vbo);
		}
	}

	bool VertexBuffer::Load(const std::string& name, void* null) 	{
		//<glGenVertexArrays with vao>
		//<glBindVertexArray vao>
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		return true;
	}

	void VertexBuffer::CreateVertexBuffer(GLsizei size, GLsizei vertexCount, void* data) 	{
		this->vertexCount = vertexCount;

		//<glGenBuffers with vbo>
		//<glBindBuffer GL_ARRAY_BUFFER with vbo>
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	void VertexBuffer::SetAttribute(int index, GLint size, GLsizei stride, size_t offset) 	{
		glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (void*)(offset));
		glEnableVertexAttribArray(index);
	}

	void VertexBuffer::Draw(GLenum primitiveType) 	{
		//<glBindVertexArray vao>
		//<glDrawArrays use vertex count>
		glBindVertexArray(vao);
		glDrawArrays(primitiveType, 0 ,vertexCount);
	}
}
