#include "Shader.h"
#include <fstream>
#include <sstream>
#include <string>

static constexpr unsigned INFO_LOG_SIZE{ 512 };
std::string fileToString(const std::string& fileName)
{
	std::string fileData{};
	std::ifstream in(fileName, std::ios::binary);
	if (in)
	{
		in.seekg(0, std::ios::end);
		const size_t fileLength = in.tellg();
		fileData.resize(fileLength);
		in.seekg(0, std::ios::beg);
		in.read(&fileData[0], fileData.size());
		in.close();
		return fileData;
	}
	else
	{
		char errMsgBuf[128] = { 0 };
		strerror_s(errMsgBuf,127, errno);
		std::string exceptionMsg = "Failed to Open file '" + fileName + "' Error: " + errMsgBuf;
		std::exception fileOpenException(exceptionMsg.c_str());
		throw(fileOpenException);
	}
}

Shader::Shader() {
	Reset();
}

Shader::Shader(const std::string& fileName, GLenum shaderType) : m_shaderType(shaderType)
{	
	bool success = true;	
	Reset();

	switch (Type()) {
	case GL_VERTEX_SHADER:
	case GL_FRAGMENT_SHADER:
		Load(fileName);
		Compile();
		break;
	default:
		throw("Unsupported Shader!");
	}
}

void Shader::Reset() {
	m_shaderId = 0;
	m_shaderType = GL_VERTEX_SHADER;
	m_state = ShaderState::UNINITIALIZED;
	m_shaderSrc = nullptr;
}

/*
	@name: Shader::Load
	@brief: Reads Shader source code from a file, creates Shader and sets the Shader source
	@returns: void
*/
void Shader::Load(const std::string& shaderFile) {
	std::string shaderSrcStr;
	m_shaderId = glCreateShader(Type());
	try {
		shaderSrcStr = fileToString(shaderFile);
	}
	catch (std::exception& e) {
		std::cout << "Failed to load shader :" << Type() << ":" << shaderFile << ": Error: " << e.what() << std::endl;
		m_state = ShaderState::UNINITIALIZED;
		return;
	}
	
	m_shaderSrc = shaderSrcStr.c_str();
	glShaderSource(Id(), 1, &m_shaderSrc, NULL);
	m_state = ShaderState::LOADED;
}

void Shader::Compile() {
	if (isLoaded()) {
		glCompileShader(Id());
		CheckCompileErrors();
	}
}

void Shader::CheckCompileErrors() {

	GLint hasCompiled{ GL_FALSE };
	std::unique_ptr<GLchar> infoLog;
	infoLog.reset(new GLchar[INFO_LOG_SIZE]);
	

	glGetShaderiv(Id(), GL_COMPILE_STATUS, &hasCompiled);
	if (hasCompiled == GL_FALSE) {
		glGetShaderInfoLog(Id(), INFO_LOG_SIZE, NULL, infoLog.get());
		m_state = ShaderState::ERROR;
		std::cout << "Shader Compilation Error for : " << Type() << ":\n\"" << infoLog <<"\""<< std::endl;
	
	}
	else
		m_state = ShaderState::COMPILED;
}
