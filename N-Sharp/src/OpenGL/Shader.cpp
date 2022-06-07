#include "Shader.h"

Shader::Shader() {
	string vertexShader =
		"#version 330 core\n"
		"\n"
		"layout (location = 0) in vec4 vertex;\n"
		"\n"
		"uniform mat4 projection = mat4(1);\n"
		"uniform mat4 model = mat4(1);"
		"\n"
		"out vec2 uvCoord;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	gl_Position = projection * model * vec4(vertex.xy, 0, 1);\n"
		"	uvCoord = vertex.zw;\n"
		"}\n";

	string fragmentShader =
		"#version 330 core\n"
		"\n"
		"in vec2 uvCoord;\n"
		"\n"
		"uniform sampler2D sampler;\n"
		"\n"
		"out vec4 FragColor;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	FragColor = texture2D(sampler, uvCoord) * vec4(1, 1, 1, 1);\n"
		"}\n";

	const char* vSource = vertexShader.c_str();
	const char* fSource = fragmentShader.c_str();

	GLuint vs, fs;
	int success;
	char infoLog[512];

	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vSource, nullptr);
	glCompileShader(vs);

	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fSource, nullptr);
	glCompileShader(fs);

	ID = glCreateProgram();

	glAttachShader(ID, vs);
	glAttachShader(ID, fs);

	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, nullptr, infoLog);
		logError(infoLog);
	}

	glDeleteShader(vs);
	glDeleteShader(fs);
}

Shader::~Shader() {
	glDeleteProgram(ID);
}

Shader& Shader::use()
{
	glUseProgram(ID);
	return *this;
}

Shader& Shader::setUniform(string name, glm::mat4 v)
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, false, &v[0][0]);
	return *this;
}
