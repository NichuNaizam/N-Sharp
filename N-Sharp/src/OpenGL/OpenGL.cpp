#include "OpenGL.h"

GLuint quadVAO;

void initialize()
{
	GLuint VBO;
	float vertices[] = {
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glBindVertexArray(quadVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

int generateTexture(string path) {
	int width, height, channels;
	unsigned char* pixels = stbi_load(path.c_str(), &width, &height, &channels, 4);

	if (pixels == nullptr) {
		return -1;
	}

	unsigned int ID = -1;
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	glBindTexture(GL_TEXTURE_2D, 0);

	return ID;
}

void renderTexture(int id, glm::vec2 pos, float rot, glm::vec2 scale)
{
	glm::mat4 model = getModelMatrix(pos, rot, scale);
	glm::mat4 projection = glm::ortho(0.0f, (float)Window::instance().width, (float)Window::instance().height, 0.0f);

	Shader::instance().use()
		.setUniform("projection", projection)
		.setUniform("model", model);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, id);

	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

glm::mat4 getModelMatrix(glm::vec2 pos, float rot, glm::vec2 scale)
{
	glm::mat4 m = glm::mat4(1.0f);

	m = glm::translate(m, glm::vec3(pos, 0.0f));
	m = glm::translate(m, glm::vec3(scale.x * 0.5f, scale.y * 0.5f, 0.0f));
	m = glm::rotate(m, rot, { 0, 0, 1 });
	m = glm::translate(m, glm::vec3(scale.x * -0.5f, scale.y * -0.5f, 0.0f));
	m = glm::scale(m, glm::vec3(scale, 1.0f));

	return m;
}
