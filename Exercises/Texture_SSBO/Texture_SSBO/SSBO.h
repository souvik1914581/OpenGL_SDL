#pragma once
#include <glad/glad.h>

/*SSBO*/
static constexpr int MAX_TEX_WIDTH{ 1920 };
static constexpr int MAX_TEX_HEIGHT{ 1080 };
struct shader_data {
	int tex_width{ 0 };
	int tex_height{ 0 };
	int tex_data[MAX_TEX_WIDTH * MAX_TEX_HEIGHT];
};

class SSBO
{
private:
	GLuint m_id{ 0 };
	GLuint* IdW() { return &m_id; }
	bool m_isBinded{ false };
public:
	SSBO() {
		glGenBuffers(1, IdW());
	}

	bool IsBound() const { return m_isBinded; }
	GLuint Id() const { return m_id; }
	void Load(void* data,size_t size);
	
	GLvoid* Map(GLenum access) {
		return glMapBuffer(GL_SHADER_STORAGE_BUFFER, access);
	}

	void Unmap() {
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	}

	void Bind() {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, Id());
		m_isBinded = true;
	}

	void Unbind() {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		m_isBinded = false;
	}
	void Delete() {
		glDeleteBuffers(1, IdW());
	}
};

