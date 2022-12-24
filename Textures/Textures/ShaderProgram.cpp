#include "ShaderProgram.h"

static constexpr unsigned INFO_LOG_SIZE{ 1024 };
ShaderProgram::ShaderProgram() {
	m_programId = glCreateProgram();
	m_programState = ShaderProgramState::CREATED;
}

void ShaderProgram::AddVertexShader(const std::string& vertexShaderFile) {
	m_vertexShader.SetType(GL_VERTEX_SHADER);
	CreateAndAddShader(m_vertexShader,vertexShaderFile);
}

void ShaderProgram::AddFragmentShader(const std::string& fragmentShaderFile) {
	m_fragmentShader.SetType(GL_FRAGMENT_SHADER);
	CreateAndAddShader(m_fragmentShader,fragmentShaderFile);
}

void ShaderProgram::CreateAndAddShader(Shader& shader,const std::string& shaderFile) {
	
	//Load the shader
	try {
		shader.Load(shaderFile);
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	//Compile shader
	shader.Compile();
	if (!shader.isCompiled()) {
		std::cout << "Failed to Compile Shader " << shader.Type()<< std::endl;
		return;
	}

	//shader is compiled. Attach it to our program
	glAttachShader(ProgramId(), shader.Id());
}

void ShaderProgram::Link() {
	glLinkProgram(ProgramId());
	CheckLinkageErrors();

	//Delete the now useless shaders
	m_vertexShader.Delete();
	m_fragmentShader.Delete();
}

void ShaderProgram::CheckLinkageErrors() {
	GLint hasLinked{ GL_FALSE };
	std::unique_ptr<char> infoLog;
	infoLog.reset(new char[INFO_LOG_SIZE]);
	
	glGetProgramiv(ProgramId(), GL_LINK_STATUS, &hasLinked);
	if (hasLinked == GL_FALSE) {
		m_programState = ShaderProgramState::ERROR;
		glGetProgramInfoLog(ProgramId(), INFO_LOG_SIZE, NULL, infoLog.get());
		std::cout << "Shader Program Linking failed \"" << infoLog <<"\"" <<std::endl;
	}
	else {
		m_programState = ShaderProgramState::LINKED;
	}
}