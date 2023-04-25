/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <vector>
#include "model3D.h"
#include "human.h"

float speed_x = 0;//[radiany/s]
float speed_y = 0;//[radiany/s]
float speed_walk = 0;
float aspectRatio=1;

ShaderProgram *sp;


constexpr int grassNumber = 150;
constexpr int groundNumber = 156;

model3D grass;
model3D school;
model3D ground;
model3D roof;
model3D room;
model3D table;
model3D chair;
model3D board;
model3D bus;
human teacher1;
human teacher2;
human student1;
human student2;
human student3;
human student4;
model3D sitting1;
model3D sitting2;
model3D sitting3;
model3D sitting4;
model3D student;

//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}


glm::vec3 pos = glm::vec3(0, 1, -10);


glm::vec3 computeDir(float angle_x, float angle_y) {
	glm::vec4 dir(0, 0, 1, 0);
	glm::mat4 M = glm::mat4(1.0f);
	M = glm::rotate(M, angle_y, glm::vec3(0, 1, 0));
	M = glm::rotate(M, angle_x, glm::vec3(1, 0, 0));
	dir = M * dir;
	return glm::vec3(dir);
}

glm::mat4 resetM() {
	glm::mat4 M = glm::mat4(1.0f);
	M = glm::rotate(M, 270.0f * PI / 180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	M = glm::scale(M, glm::vec3(0.01f, 0.01f, 0.01f));
	return M;
}

void key_callback(
	GLFWwindow* window,
	int key,
	int scancode,
	int action,
	int mod
) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_A) speed_y = 2;
		if (key == GLFW_KEY_D) speed_y = -2;
		if (key == GLFW_KEY_W) speed_walk = 3;
		if (key == GLFW_KEY_S) speed_walk = -3;
		if (key == GLFW_KEY_UP) speed_x = -1;
		if (key == GLFW_KEY_DOWN) speed_x = 1;
	}
	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_A) speed_y = 0;
		if (key == GLFW_KEY_D) speed_y = 0;
		if (key == GLFW_KEY_W) speed_walk = 0;
		if (key == GLFW_KEY_S) speed_walk = 0;
		if (key == GLFW_KEY_UP) speed_x = 0;
		if (key == GLFW_KEY_DOWN) speed_x = 0;
	}
}

void windowResizeCallback(GLFWwindow* window,int width,int height) {
    if (height==0) return;
    aspectRatio=(float)width/(float)height;
    glViewport(0,0,width,height);
}

//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glClearColor(0,0,0,1);
	glEnable(GL_DEPTH_TEST);
	glfwSetWindowSizeCallback(window,windowResizeCallback);
	glfwSetKeyCallback(window, key_callback);

	sp=new ShaderProgram("v_simplest.glsl",NULL,"f_simplest.glsl");


	grass.readTexture("Grass lawn.png");
	grass.loadModel("Grass lawn.obj");	
	

	ground.readTexture("wooden_wall.png");
	ground.loadModel("Grass lawn.obj");

	school.readTexture("wood_wall.png");
	school.loadModel("school.obj");
	
	roof.readTexture("gray_wall.png");
	roof.loadModel("Grass lawn.obj");

	room.readTexture("wood_wall.png");
	room.loadModel("classroom.obj");

	table.readTexture("table.png");
	table.loadModel("table.obj");

	chair.readTexture("wooden_chair.png");
	chair.loadModel("Wooden Chair.obj");

	board.readTexture("Chalkboard.png");
	board.loadModel("Chalkboard.obj");

	bus.readTexture("School Bus.png");
	bus.loadModel("School Bus.obj");

	teacher1.load(true);
	teacher2.load(true);

	glm::mat4 startM = resetM();
	startM = glm::translate(startM, glm::vec3(200.0f, 0.0f, 2.5f));
	teacher1.setM(startM);
	startM = glm::translate(startM, glm::vec3(100.0f, 0.0f,0.0f));
	student1.setM(glm::scale(startM, glm::vec3(0.5f, 0.5f, 0.5f)));
	startM = glm::translate(startM, glm::vec3(100.0f, 0.0f, 0.0f));
	student3.setM(glm::scale(startM, glm::vec3(0.5f, 0.5f, 0.5f)));
	

	startM = glm::translate(startM, glm::vec3(-1000.0f, -1500.0f, 2.5f));
	startM = glm::rotate(startM, 180.0f * PI / 180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	teacher2.setM(startM);
	startM = glm::translate(startM, glm::vec3(100.0f, 0.0f, 0.0f));
	student2.setM(glm::scale(startM, glm::vec3(0.5f, 0.5f, 0.5f)));
	startM = glm::translate(startM, glm::vec3(100.0f, 100.0f, 0.0f));
	student4.setM(glm::scale(startM, glm::vec3(0.5f, 0.5f, 0.5f)));


	student1.set_limit(17000, 9000);
	student2.set_limit(15000, 9000);
	student3.set_limit(15000, 12000);
	student4.set_limit(17000, 10000);

	student1.load(false);
	student2.load(false);
	student3.load(false);
	student4.load(false);

	sitting1.readTexture("Sitting Male.png");
	sitting1.loadModel("Sitting Male.obj");

	sitting2.readTexture("Sitting Male 2.png");
	sitting2.loadModel("Sitting Male 2.obj");

	sitting3.readTexture("Sitting Female.png");
	sitting3.loadModel("Sitting Female.obj");

	sitting4.readTexture("Sitting Female 2.png");
	sitting4.loadModel("Sitting Female 2.obj");

	student.readTexture("Student.png");
	student.loadModel("Student.obj");
}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
    //************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************
	grass.DeleteTextures();
	school.DeleteTextures();
	ground.DeleteTextures();
	roof.DeleteTextures();
	room.DeleteTextures();
	chair.DeleteTextures();
	table.DeleteTextures();
	board.DeleteTextures();
	bus.DeleteTextures();
	teacher1.DeleteTextures();
	teacher2.DeleteTextures();
	student1.DeleteTextures();
	student2.DeleteTextures();
	student3.DeleteTextures();
	student4.DeleteTextures();
	sitting1.DeleteTextures();
	sitting2.DeleteTextures();
	sitting3.DeleteTextures();
	sitting4.DeleteTextures();
	student.DeleteTextures();
    delete sp;
}


void drawChairs(GLFWwindow* window, glm::mat4 M) {
	glm::mat4 Ms;
	for (int i = 0; i < 9; i++) {
		float x;
		float y;
		if (i % 3 == 0) {
			x = 40.0f;
			y = -20.0f;
		}
		else
		{
			x = -20.0f;
			y = 0.0f;
		}
		M = glm::translate(M, glm::vec3(x, y, 0.0f));
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
		chair.draw(window, sp);
		Ms = glm::scale(M, glm::vec3(0.01f, 0.01f, 0.01f));
		Ms = glm::rotate(Ms, 90.0f * PI / 180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		Ms = glm::translate(Ms, glm::vec3(0.0f, -100.0f, 130.0f));
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(Ms));
		if (i % 4 == 0)
			sitting1.draw(window, sp);
		if (i % 4 == 1)
			sitting2.draw(window, sp);
		if (i % 4 == 2)
			sitting3.draw(window, sp);
		if (i % 4 == 3)
			sitting4.draw(window, sp);
	}
}

void drawTables(GLFWwindow* window, glm::mat4 M) {
	for (int i = 0; i < 9; i++) {
		float x;
		float y;
		if (i % 3 == 0) {
			x = 340.0f;
			y = -190.0f;
		}
		else
		{
			x = -170.0f;
			y = 0.0f;
		}
		M = glm::translate(M, glm::vec3(x, y, 0.0f));
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
		table.draw(window, sp);
	}
}

void drawGround(GLFWwindow* window, glm::mat4 M) {
	for (int i = 0; i < grassNumber; i++) {
		float x;
		float y;
		if (i % 15 == 0) {
			x = -280.0f;
			y = -20.0f;
		}
		else
		{
			x = 20.0f;
			y = 0.0f;
		}
		M = glm::translate(M, glm::vec3(x, y, 0.0f));
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
		grass.draw(window, sp);
	}

	M = glm::translate(M, glm::vec3(-44.0f, 00.0f, 0.0f));
	for (int i = 0; i < groundNumber; i++) {
		float x;
		float y;
		if (i % 13 == 0) {
			x = -240.0f;
			y = -20.0f;
		}
		else
		{
			x = 20.0f;
			y = 0.0f;
		}
		M = glm::translate(M, glm::vec3(x, y, 0.0f));
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
		ground.draw(window, sp);

		float z = 40;
		M = glm::translate(M, glm::vec3(0.0f, 0.0f, z));
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
		roof.draw(window, sp);
		M = glm::translate(M, glm::vec3(0.0f, 0.0f, -z));
	}
}

glm::mat4 moveStudent(glm::mat4 M) {
	static int x = 0;
	if (x <= 400)
		M = glm::translate(M, glm::vec3(x, 0.0f, 0.0f));
	else {
		M = glm::translate(M, glm::vec3(400.0f, 0.0f, 0.0f));
		M = glm::rotate(M, 180.0f * PI / 180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		M = glm::translate(M, glm::vec3(x-400, 0.0f, 0.0f));
	}
	if (x > 800)
		x = 0;
	x++;
	return M;
}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window, float angle_x, float angle_y) {
	//************Tutaj umieszczaj kod rysujący obraz******************ls	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 V = glm::lookAt(pos, pos + computeDir(angle_x, angle_y), glm::vec3(0.0f, 1.0f, 0.0f)); //Wylicz macierz widoku

    glm::mat4 P = glm::perspective(50.0f*PI/180.0f, aspectRatio, 0.01f, 50.0f); //Wylicz macierz rzutowania

    glm::mat4 M = resetM();
	

    sp->use();//Aktywacja programu cieniującego
    //Przeslij parametry programu cieniującego do karty graficznej
    glUniformMatrix4fv(sp->u("P"),1,false,glm::value_ptr(P));
    glUniformMatrix4fv(sp->u("V"),1,false,glm::value_ptr(V));
    glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(M));
	glUniform1i(sp->u("textureMap"), 0);
	glUniform4f(sp->u("lp1"), 0, 20, -30, 1);
	glUniform4f(sp->u("lp2"), 10, 0, 10, 1);
	glUniform4f(sp->u("lp3"), -10, 0, 10, 1);

	

	M = glm::rotate(M, 180.0f * PI / 180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	M = glm::scale(M, glm::vec3(4.0f, 6.0f, 8.0f));
	M = glm::translate(M, glm::vec3(0.0f, 200.0f, 0.0f));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
	room.draw(window, sp);

	M = resetM();

	M = glm::rotate(M, 180.0f * PI / 180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	M = glm::translate(M, glm::vec3(-1200.0f, 0.0f, 0.0f));
	M = glm::scale(M, glm::vec3(1.0f, 1.0f, 2.0f));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

	school.draw(window, sp);


	M = resetM();

	M = glm::scale(M, glm::vec3(10.0f, 10.0f, 10.0f));
	M = glm::translate(M, glm::vec3(150.0f, 250.0f, 0.0f));
	
	drawGround(window, M);
	
	M = glm::mat4(1.0f);
	M = glm::rotate(M, 270.0f * PI / 180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	M = glm::rotate(M, 270.0f * PI / 180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	M = glm::scale(M, glm::vec3(0.09f, 0.09f, 0.09f));

	M = glm::translate(M, glm::vec3(-20.0f, 140.0f, 0.0f));
	
	drawChairs(window, M);

	M = glm::translate(M, glm::vec3(125.0f, 0.0f, 0.0f));

	drawChairs(window, M);

	M = glm::mat4(1.0f);
	M = glm::rotate(M, 270.0f * PI / 180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	M = glm::rotate(M, 270.0f * PI / 180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	M = glm::scale(M, glm::vec3(0.09f, 0.09f, 0.09f));

	M = glm::translate(M, glm::vec3(-20.0f, -90.0f, 0.0f));

	drawChairs(window, M);

	M = glm::translate(M, glm::vec3(125.0f, 0.0f, 0.0f));
	drawChairs(window, M);

	M = glm::mat4(1.0f);
	M = glm::rotate(M, 270.0f * PI / 180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	M = glm::rotate(M, 270.0f * PI / 180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	M = glm::scale(M, glm::vec3(0.09f, 0.09f, 0.09f));

	M = glm::translate(M, glm::vec3(65.0f, 15.0f, 0.0f));

	drawChairs(window, M);

	M = resetM();
	M = glm::translate(M, glm::vec3(725.0f, 350.0f, 0.0f));

	drawTables(window, M);

	M = glm::translate(M, glm::vec3(0.0f, -1125.0f, 0.0f));
	drawTables(window, M);

	M = resetM();
	M = glm::translate(M, glm::vec3(-1340.0f, 350.0f, 0.0f));

	drawTables(window, M);

	M = glm::translate(M, glm::vec3(0.0f, -1125.0f, 0.0f));
	drawTables(window, M);

	M = resetM();
	M = glm::translate(M, glm::vec3(-395.0f, -425.0f, 0.0f));

	drawTables(window, M);

	M = resetM();
	M = glm::scale(M, glm::vec3(0.7f, 0.7f, 0.7f));

	M = glm::translate(M, glm::vec3(1200.0f, -800.0f, 0.0f));
	
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
	board.draw(window, sp);

	M = glm::translate(M, glm::vec3(0.0f, -1800.0f, 0.0f));

	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
	board.draw(window, sp);

	M = glm::translate(M, glm::vec3(-2800.0f, 0.0f, 0.0f));

	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
	board.draw(window, sp);

	M = glm::translate(M, glm::vec3(0.0f, 1800.0f, 0.0f));

	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
	board.draw(window, sp);

	M = glm::translate(M, glm::vec3(1300.0f, -900.0f, 0.0f));

	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
	board.draw(window, sp);

	M = resetM();
	M = glm::scale(M, glm::vec3(0.4f, 0.4f, 0.4f));
	M = glm::translate(M, glm::vec3(-1200.0f, 1900.0f, 0.0f));

	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
	bus.draw(window, sp);

	M = resetM();
	M = glm::scale(M, glm::vec3(2.0f, 2.0f, 2.0f));
	M = glm::translate(M, glm::vec3(-300.0f, -100.0f, 1.0f));
	M = moveStudent(M);

	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
	student.draw(window, sp);

	
	teacher1.draw(window, sp);
	teacher2.draw(window, sp);

	student1.draw(window, sp);
	student2.draw(window, sp);
	student3.draw(window, sp);
	student4.draw(window, sp);

    glfwSwapBuffers(window); //Przerzuć tylny bufor na przedni
}


int main(void)
{
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(1024, 768, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjujące

	//Główna pętla
	float angle = 0; //zadeklaruj zmienną przechowującą aktualny kąt obrotu
	float angle_y = 0;
	float angle_x = 0;
	glfwSetTime(0); //Zeruj timer
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		angle_y += speed_y * glfwGetTime(); //Oblicz kąt o jaki obiekt obrócił się podczas poprzedniej klatki
		angle_x += speed_x * glfwGetTime(); //Oblicz kąt o jaki obiekt obrócił się podczas poprzedniej klatki
		pos += (float)(speed_walk * glfwGetTime()) * computeDir(0, angle_y);
        glfwSetTime(0); //Zeruj timer
		drawScene(window, angle_x, angle_y); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
