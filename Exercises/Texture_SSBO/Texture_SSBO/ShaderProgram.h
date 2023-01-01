#pragma once
#include <glad/glad.h>
#include "Shader.h"
#include <vector>

enum class ShaderProgramState
{
	ERROR = -1,
	CREATED,
	LINKED,
	ACTIVATED
};

class ShaderProgram
{
private:
	GLuint m_programId;
	Shader m_vertexShader;
	Shader m_fragmentShader;
	ShaderProgramState m_programState;
	void CreateAndAddShader(Shader& shader,const std::string& shaderFile);
	void CheckLinkageErrors();

public:
	GLuint ProgramId() const { return m_programId; }
	ShaderProgram();
	
	void Link();

	void Activate() {
		if (State() == ShaderProgramState::LINKED) {
			glUseProgram(ProgramId());
			m_programState = ShaderProgramState::ACTIVATED;
		}
	}

	void Delete() {
		glDeleteProgram(ProgramId());
	}
	void AddVertexShader(const std::string& vertexShaderFile);
	void AddFragmentShader(const std::string& fragmentShaderFile);

	ShaderProgramState State() const { return m_programState; }
};

