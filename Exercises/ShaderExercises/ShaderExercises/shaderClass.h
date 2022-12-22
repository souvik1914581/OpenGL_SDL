#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

std::string get_file_contents(const char* filename);
enum class ShaderType
{
	VERTEX_SHADER = 0,
	FRAGMENT_SHADER,
	PROGRAM,
	MAX
};

std::string toString(ShaderType type);
std::ostream& operator << (std::ostream& out, ShaderType type);


class Shader
{
public:
	// Reference ID of the Shader Program
	GLuint ID;
	// Constructor that build the Shader Program from 2 different shaders
	Shader(const char* vertexFile, const char* fragmentFile);

	// Activates the Shader Program
	void Activate();
	// Deletes the Shader Program
	void Delete();
private:
	void CheckCompilationErrors(GLuint shaderId, ShaderType type);
};
#endif