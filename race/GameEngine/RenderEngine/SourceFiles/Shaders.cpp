/*===================================================================================*//**
	Shaders
	
	Utility class for loading shaders.
    
    Copyright 2017 Erick Fernandez de Arteaga. All rights reserved.
        https://www.linkedin.com/in/erick-fda
        https://bitbucket.org/erick-fda

	@author Chris Leclair, Erick Fernandez de Arteaga
	@version 0.0.0
	@file
	
	@see Shaders
	@see Shaders.h
	
*//*====================================================================================*/

/*========================================================================================
	Dependencies
========================================================================================*/
#include "Shaders.h"
#include "FileHelper.h"

/*========================================================================================
	Class Fields
========================================================================================*/
 char* Shaders::vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 iPos;\n"
"layout (location = 1) in vec3 iNorm;\n"
"layout (location = 2) in vec2 iTexC;\n"
"out vec2 oTexC;\n"
"out vec3 oNormal;\n"
"out vec3 oWorldPos;\n"
"uniform mat4 iModelViewProjectionMatrix;\n"
"uniform mat4 iModelMatrix;\n"
"void main()\n"
"{\n"
"   gl_Position = iModelViewProjectionMatrix * vec4(iPos.x, iPos.y, iPos.z, 1.0);\n"
"   oTexC = iTexC;\n"
"   oNormal = (iModelMatrix * vec4(iNorm, 0.0)).xyz;\n"
"   oWorldPos = (iModelMatrix * vec4(iPos, 1.0)).xyz;\n"
"}\0";

char* Shaders::fragmentShaderSource = "#version 330 core\n"
"in vec2 oTexC;\n"
"in vec3 oNormal;\n"
"in vec3 oWorldPos;\n"
"layout (location = 0) out vec3 FragColor;\n"
"layout (location = 1) out vec3 FragPosition;\n"
"layout (location = 2) out vec3 FragNormal;\n"
"uniform sampler2D iTexImage;\n"
"void main()\n"
"{\n"
"   FragColor = texture(iTexImage, oTexC).rgb;\n"
"   FragPosition = oWorldPos;\n"
"   FragNormal = oNormal;\n"
"}\n\0";

char* Shaders::vertexShader2Source = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"out vec2 uv;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"   uv = (aPos.xy + vec2(1, 1)) / 2.0;\n"
"}\0";

char* Shaders::fragmentShader2Source = "#version 330 core\n"
"in vec2 uv;\n"
"out vec3 color;\n"
"uniform sampler2D fColor;\n"
"uniform sampler2D fPosition;\n"
"uniform sampler2D fNormal;\n"
"uniform sampler2D fDepth;\n"
"void main()\n"
"{\n"
"   color = texture(fColor, uv).rgb;\n"
"}\n\0";

std::string Shaders::VSH_01_PATH = "ResourceFiles/Shaders/shader01.vsh";
std::string Shaders::FSH_01_PATH = "ResourceFiles/Shaders/shader01.fsh";
std::string Shaders::VSH_02_PATH = "ResourceFiles/Shaders/shader02.vsh";
std::string Shaders::FSH_02_PATH = "ResourceFiles/Shaders/shader02.fsh";

/*----------------------------------------------------------------------------------------
	Class Methods
----------------------------------------------------------------------------------------*/
///
///
///
GLuint Shaders::LoadShaders()
{
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	std::string vsh = FileHelper::loadFileFromString(Shaders::VSH_01_PATH);
	const char* vshContent = vsh.c_str();
	glShaderSource(vertexShader, 1, &(vshContent), NULL);
	glCompileShader(vertexShader);

	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		SDL_Log("%s", infoLog);
	}

	// fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	std::string fsh = FileHelper::loadFileFromString(Shaders::FSH_01_PATH);
	const char* fshContent = fsh.c_str();
	glShaderSource(fragmentShader, 1, &(fshContent), NULL);
	glCompileShader(fragmentShader);

	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		SDL_Log("%s", infoLog);
	}

	// link shaders
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		SDL_Log("%s", infoLog);
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

///
///
///
GLuint Shaders::LoadShadersFBDraw()
{
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//glShaderSource(vertexShader, 1, &(Shaders::vertexShader2Source), NULL);
	std::string vsh = FileHelper::loadFileFromString(Shaders::VSH_02_PATH);
	const char* vshContent = vsh.c_str();
	glShaderSource(vertexShader, 1, &(vshContent), NULL);
	glCompileShader(vertexShader);

	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		SDL_Log("%s", infoLog);
	}

	// fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//glShaderSource(fragmentShader, 1, &fragmentShader2Source, NULL);
	std::string fsh = FileHelper::loadFileFromString(Shaders::FSH_02_PATH);
	const char* fshContent = fsh.c_str();
	glShaderSource(fragmentShader, 1, &(fshContent), NULL);
	glCompileShader(fragmentShader);

	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		SDL_Log("%s", infoLog);
	}

	// link shaders
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		SDL_Log("%s", infoLog);
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}
