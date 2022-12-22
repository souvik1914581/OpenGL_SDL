#include"shaderClass.h"
#include <map>
#define INFO_LOG_SIZE	1024
// Reads a text file and outputs a string with everything in the text file
std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

static const std::map<ShaderType, std::string> ShaderTypeString{
	{ShaderType::FRAGMENT_SHADER,"Fragment Shader"},
	{ShaderType::VERTEX_SHADER,"Vertex Shader"},
	{ShaderType::PROGRAM,"Program"},
	{ShaderType::MAX,"Unknown Type"}
};

std::string toString(ShaderType type)
{
	const auto& entry = ShaderTypeString.find(type);
	if (entry != ShaderTypeString.end())
	{
		return entry->second;
	}
	return "";
}

std::ostream& operator << (std::ostream& out, ShaderType type)
{
	out << toString(type);
	return out;
}

// Constructor that build the Shader Program from 2 different shaders
Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	// Read vertexFile and fragmentFile and store the strings
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	// Convert the shader source strings into character arrays
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	// Create Vertex Shader Object and get its reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach Vertex Shader source to the Vertex Shader Object
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(vertexShader);
	CheckCompilationErrors(vertexShader, ShaderType::VERTEX_SHADER);

	// Create Fragment Shader Object and get its reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attach Fragment Shader source to the Fragment Shader Object
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(fragmentShader);
	CheckCompilationErrors(fragmentShader, ShaderType::FRAGMENT_SHADER);

	// Create Shader Program Object and get its reference
	ID = glCreateProgram();
	// Attach the Vertex and Fragment Shaders to the Shader Program
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	// Wrap-up/Link all the shaders together into the Shader Program
	glLinkProgram(ID);
	CheckCompilationErrors(ID, ShaderType::PROGRAM);

	// Delete the now useless Vertex and Fragment Shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

// Activates the Shader Program
void Shader::Activate()
{
	glUseProgram(ID);
}

// Deletes the Shader Program
void Shader::Delete()
{
	glDeleteProgram(ID);
}

void Shader::CheckCompilationErrors(GLuint shaderId, ShaderType type)
{
	GLint hasCompiled;
	std::unique_ptr<GLchar> infoLog;
	infoLog.reset(new GLchar[INFO_LOG_SIZE]);
	switch (type)
	{
	case ShaderType::VERTEX_SHADER:
	case ShaderType::FRAGMENT_SHADER:
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shaderId, INFO_LOG_SIZE, NULL, infoLog.get());
			std::cout << "Shader Compilation Error for : " << type << ":\n" << infoLog << std::endl;
		}
		break;
	case ShaderType::PROGRAM:
		glGetProgramiv(shaderId, GL_LINK_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetProgramInfoLog(shaderId, INFO_LOG_SIZE, NULL, infoLog.get());
			std::cout << "Shader Linking failed for : " << type << ":\n<<" << infoLog << std::endl;
		}
		break;
	default:
		std::cout << "WTF are you??" << std::endl;
	}
}
