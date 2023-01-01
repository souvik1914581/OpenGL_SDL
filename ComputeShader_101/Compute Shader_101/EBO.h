#pragma once
#include <glad/glad.h>
class EBO
{
	GLuint  m_id;
	GLuint* IdW() { return &m_id; }
	bool m_binded = false;
public:
	EBO() {
		glGenBuffers(1, IdW());
	}
	bool IsBound() const { return m_binded; }
	void Load(const GLuint* indices,const GLsizeiptr size);

	GLuint Id() const { return m_id; }

	void Bind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Id());
		m_binded = true;
	}

	void Unbind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
		m_binded = false;
	}

	void Delete() {
		glDeleteBuffers(1, IdW());
	}
};

