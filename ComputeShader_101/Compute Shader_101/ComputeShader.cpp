#include "ComputeShader.h"
ComputeShader::ComputeShader(const std::string& shaderFileName) {
	SetType(GL_COMPUTE_SHADER);
	Load(shaderFileName);
	Compile();

	m_programId = glCreateProgram();
	glAttachShader(ProgramId(), Id());
	glLinkProgram(ProgramId());
}