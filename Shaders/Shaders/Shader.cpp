#include "Shader.h"
#include<iostream>
#include <errno.h>
std::string getFileContentsAsString(const std::string& fileName) 
{
	std::ifstream in(fileName, std::ios::binary);
	if (in)
	{
		std::string contents;

		//calculate file size
		in.seekg(0, std::ios::end);
		//resize the contents string to size of file
		contents.resize(in.tellg());
		//move fd to beginning of file
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return contents;
	}
	char errBuff[32] = { 0 };
	strerror_s(errBuff, errno);
	std::string exceptMsg = "Failed to read file : '" + fileName + "' error: " +   std::string(errBuff);
	throw(std::runtime_error(exceptMsg));
}

Shader::Shader(const std::string& vertexShaderFile, const std::string& fragmentShaderFile)
{
	try {
		const std::string vertexShaderStr = getFileContentsAsString(vertexShaderFile);
		const std::string  fragmentShaderStr = getFileContentsAsString(fragmentShaderFile);
		const char* vertexShader = vertexShaderStr.c_str();
		const char* fragmentShader = fragmentShaderStr.c_str();

		GLuint vs = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vs, 1, &vertexShader, NULL);
		glCompileShader(vs);

		GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fs, 1, &fragmentShader, NULL);
		glCompileShader(fs);

		const GLuint id = glCreateProgram();
		SetProgramId(id);

		glAttachShader(GetProgramId(), vs);
		glAttachShader(GetProgramId(), fs);

		glLinkProgram(GetProgramId());

		glDeleteShader(vs);
		glDeleteShader(fs);
	}
	catch (std::exception& e) {
		std::cout << "Caught Exception: " << e.what() << std::endl;
	}
}

