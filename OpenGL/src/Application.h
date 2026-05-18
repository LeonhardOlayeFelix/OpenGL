#pragma once
#include "Common.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

GLFWwindow* init();

void processInput(GLFWwindow* window);

void main(void);

ShaderProgramSource ParseShaderProgram(const std::string& filepath);
