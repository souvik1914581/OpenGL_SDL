#pragma once
#pragma once
#include <glad/glad.h>
#include "Shader.h"
#include "ShaderProgram.h"

class ComputeShader : public Shader
{
private:
	GLuint m_programId{ 0 };
	ShaderProgramState m_state{ ShaderProgramState::ERROR };
public:
	ComputeShader(const std::string& shaderFileName);
	GLuint ShaderId() const {
		return Id();
	}
	GLuint ProgramId() const { return m_programId; }
	void Use() {
		glUseProgram(ProgramId());
	}

	void Delete() {
		glDeleteShader(Id());
		Reset();
		glDeleteProgram(ProgramId());
	}
};

