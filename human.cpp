#include "human.h"

void human::load(bool t) {
	type = t;
	leg.readTexture("leg.png");
	leg.loadModel("leg.obj");

	arm.readTexture("torso.png");
	arm.loadModel("arm.obj");

	torso.readTexture("torso.png");
	torso.loadModel("torso.obj");
	if (t)
	{
		head.readTexture("head.png");
		head.loadModel("head.obj");
	}
	else {
		head.readTexture("Female Blonde Hair.png");
		head.loadModel("Female Blonde Hair.obj");
		torso.readTexture("purmesh.png");
	}
	
}

void human::DeleteTextures() {
	leg.DeleteTextures();
	arm.DeleteTextures();
	torso.DeleteTextures();
	head.DeleteTextures();
}

void human::setM(glm::mat4 startM) {
	SM = startM;
}

void human::set_limit(float l1, float l2) {
	limit1 = l1;
	limit2 = l2;
	limit = l1;
}


void human::draw(GLFWwindow* window, ShaderProgram* sp) {

	glm::mat4 M = SM;
	M = glm::translate(M, glm::vec3(0.0f, -speed, 0.0f));
	SM = M;
	if (move < limit)
		move += 10;
	else
	{
		if (limit == limit1)
			limit = limit2;
		else
			limit = limit1;
		move = 0;
		//M = glm::translate(M, glm::vec3(0.0f, -1500.0f, 0.0f));
		M = glm::rotate(M, -90 * PI / 180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		SM = M;
		//M = glm::translate(M, glm::vec3(0.0f, -move, 0.0f));
	}
	M = glm::scale(M, glm::vec3(0.2f, 0.2f, 0.18f));
	glm::mat4 tempM = M;

	M = glm::translate(M, glm::vec3(0.0f, 0.0f, 200.0f));
	M = glm::rotate(M, 180 * PI / 180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	M = glm::rotate(M, (j + 180) * PI / 180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	M = glm::translate(M, glm::vec3(0.0f, -50.0f, -200.0f));

	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
	leg.draw(window, sp);

	M = tempM;
	M = glm::translate(M, glm::vec3(100.0f, 0.0f, 0.0f));
	tempM = M;

	M = glm::translate(M, glm::vec3(0.0f, 0.0f, 200.0f));
	M = glm::rotate(M, 180 * PI / 180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	M = glm::rotate(M, (-(35 + j) + 190) * PI / 180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	M = glm::translate(M, glm::vec3(0.0f, -35.0f, -200.0f));

	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
	leg.draw(window, sp);

	if (dir_j) {
		j++;
		if (j == 1)
			dir_j = false;
	}
	else
	{
		j--;
		if (j == -35)
			dir_j = true;
	}

	M = tempM;
	M = glm::translate(M, glm::vec3(-50.0f, 60.0f, 200.0f));
	M = glm::scale(M, glm::vec3(0.5f, 0.5f, 0.5f));

	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
	torso.draw(window, sp);

	tempM = M;
	
	M = glm::translate(M, glm::vec3(150.0f, 0.0f, 825.0f));
	M = glm::rotate(M, 180 * PI / 180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	M = glm::rotate(M, (i+180)* PI / 180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	M = glm::rotate(M, 90 * PI / 180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	M = glm::translate(M, glm::vec3(0.0f, 0.0f, -600.0f));
	M = glm::scale(M, glm::vec3(5.0f, 5.0f, 3.0f));

	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
	arm.draw(window, sp);

	M = tempM;

	M = glm::translate(M, glm::vec3(-150.0f, 0.0f, 825.0f));
	M = glm::rotate(M, 180 * PI / 180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	M = glm::rotate(M, (-(35+i) + 180) * PI / 180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	M = glm::rotate(M, -90 * PI / 180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	M = glm::translate(M, glm::vec3(0.0f, 0.0f, -600.0f));
	M = glm::scale(M, glm::vec3(5.0f, 5.0f, 3.0f));

	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
	arm.draw(window, sp);

	if (dir_i) {
		i++;
		if (i == 1)
			dir_i = false;
	}
	else
	{
		i--;
		if (i == -35)
			dir_i = true;
	}

	M = tempM;

	M = glm::translate(M, glm::vec3(25.0f, -50.0f, 775.0f));
	if (type) {
		M = glm::rotate(M, 45 * PI / 180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	}
	else {
		M = glm::translate(M, glm::vec3(675.0f, 0.0f, 0.0f));
	}
	M = glm::scale(M, glm::vec3(25.0f, 25.0f, 25.0f));

	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
	head.draw(window, sp);
	
}