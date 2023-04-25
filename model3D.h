#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "lodepng.h"
#include<iostream>
#include "shaderprogram.h"


class model3D
{
	std::vector<glm::vec4> verts;
	std::vector<glm::vec4> norms;
	std::vector<glm::vec2> texCoords;
	std::vector<unsigned int> indices;

	GLuint tex;

public:
	void readTexture(const char* filename);
	void loadModel(std::string file);
	void DeleteTextures();
	void draw(GLFWwindow* window, ShaderProgram* sp);
};

