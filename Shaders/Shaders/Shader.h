#pragma once
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <memory>

std::string getFileContentsAsString(const std::string& fileName);

class Shader
{
	GLuint m_programID = -1;
	void SetProgramId(GLuint id) { m_programID = id;	 }

public:
	Shader(const std::string& vertexShaderFile, const std::string& fragmentShaderFile);
	GLuint GetProgramId() const {
		return m_programID;
	}

	//Activate
	void Activate() const {
		glUseProgram(GetProgramId());
	}

	//De-activate
	void Delete() const {
		glDeleteProgram(GetProgramId());
	}
};

