// SFML_Setup.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <string>

using namespace sf;

void setup();
GLuint createShader(GLenum eShaderType, const std::string &strShaderFile);
GLuint createProgram(const std::vector<GLuint> &shaderList);
void initializeProgram();
void initializeVertexBuffer();

ContextSettings* settings;
Window* window;
GLuint glProgram, positionBufferObject, vao;

const float vertexPositions[] = {
	0.75f, 0.75f, 0.0f, 1.0f,
	0.75f, -0.75f, 0.0f, 1.0f,
	-0.75f, -0.75f, 0.0f, 1.0f,
};

const std::string strVertexShader(
	"#version 330\n"
	"layout(location = 0) in vec4 position;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = position;\n"
	"}\n"
);

const std::string strFragmentShader(
	"#version 330\n"
	"out vec4 outputColor;\n"
	"void main()\n"
	"{\n"
	"	outputColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
	"}\n"
);

int main()
{
	setup();

	initializeProgram();
	initializeVertexBuffer();
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	bool running = true;
	while (running)
	{
		Event event;
		while (window->pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				running = false;
			}
			else if (event.type == Event::Resized)
			{
				glViewport(0, 0, event.size.width, event.size.height);
			}
			else if (event.type == Event::KeyPressed)
			{
				Keyboard::Key key = event.key.code;
				//if (key == Keyboard::Num1)
				
			}
		}

		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(glProgram);

		glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glDisableVertexAttribArray(0);
		glUseProgram(0);

		window->display();
		sleep(milliseconds(8)); // at most, 125 fps
	}

	delete settings;
	delete window;

	return 0;
}

void setup()
{
	settings = new ContextSettings();
	settings->antialiasingLevel = 8;
	settings->attributeFlags = 0;
	settings->depthBits = 24;
	settings->majorVersion = 4;
	settings->minorVersion = 2;
	settings->stencilBits = 8;

	window = new Window(VideoMode(800, 600), "OpenGL", Style::Default, *settings);
	window->setVerticalSyncEnabled(true);

	glewInit();
}

void initializeProgram()
{
	std::vector<GLuint> shaderList;
	shaderList.push_back(createShader(GL_VERTEX_SHADER, strVertexShader));
	shaderList.push_back(createShader(GL_FRAGMENT_SHADER, strFragmentShader));

	glProgram = createProgram(shaderList);
}

GLuint createShader(GLenum shaderType, const std::string &strShaderFile)
{
	GLuint shader = glCreateShader(shaderType);
	const char *strFileData = strShaderFile.c_str();
	glShaderSource(shader, 1, &strFileData, NULL);

	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

		const char *strShaderType = NULL;
		switch (shaderType)
		{
			case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
			case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
			case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
		}

		fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
		delete[] strInfoLog; //deletes every item in the array! Neat.
	}

	return shader;
}

GLuint createProgram(const std::vector<GLuint> &shaderList)
{
	GLuint program = glCreateProgram();

	for (unsigned int i = 0; i < shaderList.size(); ++i)
	{
		glAttachShader(program, shaderList[i]);
	}

	glLinkProgram(program);

	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}

	return program;
}

void initializeVertexBuffer()
{
	glGenBuffers(1, &positionBufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
