#include "SSBO.h"

void SSBO::Load(void* data,size_t size) {
	glBufferData(GL_SHADER_STORAGE_BUFFER,size, (void*)data, GL_DYNAMIC_COPY);
}
