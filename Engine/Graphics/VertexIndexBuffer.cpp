#include "VertexIndexBuffer.h"

namespace gme {
	VertexIndexBuffer::~VertexIndexBuffer() 	{
		//<if ibo is not 0 then glDeleteBuffers>
		if (ibo) {
			glDeleteBuffers(1, &ibo);
		}
	}

	bool VertexIndexBuffer::Load(const std::string& name, void* null) 	{
		VertexBuffer::Load(name, null);
		return true;
	}

	void VertexIndexBuffer::CreateIndexBuffer(GLenum indexType, GLsizei indexCount, void* data) 	{
		this->indexType = indexType;
		this->indexCount = indexCount;

		//<glGenBuffers with ibo>
		//<glBindBuffer GL_ELEMENET_ARRAY_BUFFER with ibo>
		glGenBuffers(1,&ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		
		size_t indexSize = (indexType == GL_UNSIGNED_SHORT) ? sizeof(GLushort) : sizeof(GLuint);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * indexSize, data, GL_STATIC_DRAW);
	}

	void VertexIndexBuffer::Draw(GLenum primitiveType) 	{
		//<glBindVertexArray vao>
		//<glDrawElements>
		glBindVertexArray(vao);
		glDrawElements(primitiveType,indexCount,GL_UNSIGNED_INT,0);
	}
}
