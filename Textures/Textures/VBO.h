#pragma once
#include <glad/glad.h>
#include <string>
class VBO
{
private:
	GLuint m_id;
	GLuint* IdW() {
		return &m_id;
	}
	bool m_isBinded = false;
public:
	VBO() {
		glGenBuffers(1, IdW());
	}
	bool IsBound() const { return m_isBinded; }
	GLuint Id() const { return m_id; }
	
	void Load(const GLfloat* vertices, const GLsizeiptr size);

	void Bind() {
		glBindBuffer(GL_ARRAY_BUFFER, Id());
		m_isBinded = true;
	}

	void Unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		m_isBinded = false;
	}

	void Delete() {
		glDeleteBuffers(1, IdW());
	}
};

