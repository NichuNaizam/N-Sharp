#pragma once

#include "NS.h"
#include "Window.h"
#include "Shader.h"
#include "Input.h"

void initialize();
int generateTexture(string path);
void renderTexture(int id, glm::vec2 pos, float rot, glm::vec2 scale);
glm::mat4 getModelMatrix(glm::vec2 pos, float rot, glm::vec2 scale);