#pragma once
#include <glad/glad.h>
#include <string>
#include "VBO.h"
class VAO
{
private:
	GLuint m_id;
	GLuint* IdW() { return &m_id; }
public:
	GLuint Id() { return m_id; }
	VAO() {
		glGenVertexArrays(1, IdW());
	}
	
	void Bind() {
		glBindVertexArray(Id());
	}

	void Unbind() {
		glBindVertexArray(0);
	}

	/*	layout 0 = X, Y, Z
		layout 1 = R,G,B
	*/
	void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizei stride, void* offset);

	void Delete() {
		glDeleteVertexArrays(1, IdW());
	}

	void Enable() {
		glEnableVertexAttribArray(Id());
	}
	void Disable() {
		glDisableVertexAttribArray(Id());
	}
};

