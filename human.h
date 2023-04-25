#pragma once

#include "model3D.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "constants.h"

class human
{
	model3D leg;
	model3D arm;
	model3D torso;
	model3D head;
	int i = 0;
	int j = 0;
	bool dir_i = true;
	bool dir_j = true;
	float move = 0;
	float speed = 2;
	bool type;
public:
	glm::mat4 SM;
	float limit = 7500;
	float limit1 = 7500;
	float limit2 = 4500;
	void load(bool t);
	void draw(GLFWwindow* window, ShaderProgram* sp);
	void DeleteTextures();
	void setM(glm::mat4 startM);
	void set_limit(float l1, float l2);
};