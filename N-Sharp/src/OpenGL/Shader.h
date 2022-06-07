#include "NS.h"

#pragma once
class Shader
{
public:
	Shader(const Shader&) = delete;
	Shader(Shader&) = delete;
	Shader(Shader&&) = delete;
	~Shader();

	static Shader& instance() {
		static Shader shader;
		return shader;
	}

	Shader& use();
	Shader& setUniform(string name, glm::mat4 v);

private:
	GLuint ID;
	
	Shader();
};