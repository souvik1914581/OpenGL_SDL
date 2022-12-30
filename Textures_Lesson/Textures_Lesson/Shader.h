#pragma once
#include <glad/glad.h>
#include <iostream>


enum class ShaderState
{
	ERROR = -1,
	UNINITIALIZED,
	LOADED,
	COMPILED,
	MAX
};

std::string fileToString(const std::string& fileName);

class Shader
{
private:
	GLenum m_shaderType;
	GLuint m_shaderId = 0;
	ShaderState m_state{ ShaderState::UNINITIALIZED };
	const char* m_shaderSrc{ nullptr };

public:
	Shader(const std::string& shaderFileName, GLenum shaderType);
	Shader();
	void Load(const std::string& shaderFileName);
	void Compile();
	void SetType(GLenum type) { m_shaderType = type; }
	GLenum Type() const { return m_shaderType; }
	GLuint Id() const { return m_shaderId; }
	ShaderState State() const { return m_state; }
	void CheckCompileErrors();
	bool isLoaded() const { return (m_state == ShaderState::LOADED); }
	bool isCompiled() const { return (m_state == ShaderState::COMPILED); }
	
	void Reset();
	void Delete() {
		glDeleteShader(Id());
		Reset();
	}
};

