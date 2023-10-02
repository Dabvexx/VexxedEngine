#include "shaderClass.h"

// Reads a text file and outputs a string with everything in the text file
std::string GetFileContents(const char* filename)
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

Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	std::string vertexCode = GetFileContents(vertexFile);
	std::string fragmentCode = GetFileContents(fragmentFile);

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	// Create Vertex Shader Object and get reference.
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach Vertex Shader source to Vertex Shader Object.
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	// Compile Vertex Shader.
	glCompileShader(vertexShader);
	CompileErrors(vertexShader, "VERTEX");


	// Create Fragment Shader Object and get reference.
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attach Fragment Shader source to Fragment Shader Object.
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	// Compile Fragment Shader.
	glCompileShader(fragmentShader);
	CompileErrors(fragmentShader, "FRAGMENT");


	// Create Shader Program Object and get reference.
	ID = glCreateProgram();
	// Attach Vertex and Fragment Shaders to Shader Program.
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	// Link shaders into Shader Program.
	glLinkProgram(ID);
	CompileErrors(ID, "PROGRAM");


	// Dispose of now redundant shader objects.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Activate() 
{
	glUseProgram(ID);
}

void Shader::Delete()
{
	glDeleteProgram(ID);
}


void Shader::CompileErrors(unsigned int shader, const char* type)
{
	GLint hasCompiled;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR for: " << type << "\n" << std::endl;
		}
	}
	else
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_LINKING_ERROR for: " << type << "\n" << std::endl;
		}
	}
}