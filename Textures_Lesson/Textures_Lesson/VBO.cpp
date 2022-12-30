#include "VBO.h"

void VBO::Load(const GLfloat* vertices,const GLsizeiptr size) {
	if (!IsBound())
		Bind();
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}