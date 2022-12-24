#include "EBO.h"

void EBO::Load(const GLuint* indices,const GLsizeiptr size) {
	if (!IsBound())
		Bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}