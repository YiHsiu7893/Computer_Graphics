#include <iostream>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Object.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void loadMaterialLight();
unsigned int createShader(const char* filename, const char* type);
unsigned int createProgram(unsigned int vertexShader, unsigned int geometryShader, unsigned int fragmentShader);
unsigned int ModelVAO(Object* model);
void loadTexture(unsigned int& texture, const char* tFileName);
glm::mat4 getPerspective();
glm::mat4 getView();

Object* deerModel = new Object("obj/deer.obj");

Material material;
Light light;
glm::vec3 cameraPos = glm::vec3(0, 3.5, 3.5);

int windowWidth = 800, windowHeight = 600;
int shadingType;
float angle = 0;
bool rotating = true;
float dissolveFactor = -30;  


int main()
{
	// Initialization
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFE_OPENGL_FORWARD_COMPACT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "HW3", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	/* TODO: Create vertex shader, geometry shader( for flat shading ),fragment shader, shader program for each shading approach 
	 * Hint:
	 *       createShader, createProgram, glUseProgram
	 * Note:
	 *       Blinn-Phong shading: "Blinn-Phong.vert", "Blinn-Phong.frag"
	 *       Gouraud shading: "Gouraud.vert", "Gouraud.frag"
	 *		 Toon shading: "Toon.vert", "Toon.frag"
	 *       Flat shading: "Flat.vert", "Flat.geom", "Flat.frag"
	 *		 Border effect : "Border.vert", "Border.frag"
	 *		 Dissolve effect : "Dissolve.vert", "Dissolve.frag"
	 */
	// Blinn-Phong shading
	unsigned int BPvertex, BPfragment, BPprogram;
	BPvertex = createShader("shaders/Blinn-Phong.vert", "vert");
	BPfragment = createShader("shaders/Blinn-Phong.frag", "frag");
	BPprogram = createProgram(BPvertex, 0, BPfragment);

	// Gouraud shading
	unsigned int Gouraudvertex, Gouraudfragment, Gouraudprogram;
	Gouraudvertex = createShader("shaders/Gouraud.vert", "vert");
	Gouraudfragment = createShader("shaders/Gouraud.frag", "frag");
	Gouraudprogram = createProgram(Gouraudvertex, 0, Gouraudfragment);

	// Toon shading
	unsigned int Toonvertex, Toonfragment, Toonprogram;
	Toonvertex = createShader("shaders/Toon.vert", "vert");
	Toonfragment = createShader("shaders/Toon.frag", "frag");
	Toonprogram = createProgram(Toonvertex, 0, Toonfragment);

	// Flat shading
	unsigned int Flatvertex, Flatgeometry, Flatfragment, Flatprogram;
	Flatvertex = createShader("shaders/Flat.vert", "vert");
	Flatgeometry = createShader("shaders/Flat.geom", "geom");
	Flatfragment = createShader("shaders/Flat.frag", "frag");
	Flatprogram = createProgram(Flatvertex, Flatgeometry, Flatfragment);

	// Border shading
	unsigned int Bordervertex, Borderfragment, Borderprogram;
	Bordervertex = createShader("shaders/Border.vert", "vert");
	Borderfragment = createShader("shaders/Border.frag", "frag");
	Borderprogram = createProgram(Bordervertex, 0, Borderfragment);

	// Dissolve shading
	unsigned int Dissolvevertex, Dissolvefragment, Dissolveprogram;
	Dissolvevertex = createShader("shaders/Dissolve.vert", "vert");
	Dissolvefragment = createShader("shaders/Dissolve.frag", "frag");
	Dissolveprogram = createProgram(Dissolvevertex, 0, Dissolvefragment);
	
	// sets Blinn-Phong shading as the default
	glUseProgram(BPprogram);
	unsigned int shaderProgram = BPprogram;
	shadingType = 1;


	// Texture
	unsigned int deerTexture;
	loadTexture(deerTexture, "obj/deer_diffuse.jpg");

	// VAO, VBO
	unsigned int deerVAO;
	deerVAO = ModelVAO(deerModel);

	// Display loop
	loadMaterialLight();
	glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
	glViewport(0, 0, windowWidth, windowHeight);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	double dt;
	double lastTime = glfwGetTime();
	double currentTime;

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.98f, 0.97f, 0.82f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glm::mat4 perspective = getPerspective();
		glm::mat4 view = getView();
		glm::mat4 model = glm::mat4(1.0f);
		
		model = glm::rotate(model, angle, glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(-120.0f), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		model = glm::scale(model, glm::vec3(0.04, 0.04, 0.04));


		/* TODO: Create all variable and pass them to shaders through uniform
		 * Hint:
		 *		glUniformMatrix4fv, glUniform3fv, glUniform1f
		 */

		unsigned int mLoc = glGetUniformLocation(shaderProgram, "M");
		unsigned int vLoc = glGetUniformLocation(shaderProgram, "V");
		unsigned int pLoc = glGetUniformLocation(shaderProgram, "P");
		unsigned int cLoc = glGetUniformLocation(shaderProgram, "C");

		unsigned int k_ambientLoc = glGetUniformLocation(shaderProgram, "Ka");
		unsigned int k_diffuseLoc = glGetUniformLocation(shaderProgram, "Kd");
		unsigned int k_specularLoc = glGetUniformLocation(shaderProgram, "Ks");
		unsigned int k_shinessLoc = glGetUniformLocation(shaderProgram, "Alpha");

		unsigned int l_ambientLoc = glGetUniformLocation(shaderProgram, "La");
		unsigned int l_diffuseLoc = glGetUniformLocation(shaderProgram, "Ld");
		unsigned int l_specularLoc = glGetUniformLocation(shaderProgram, "Ls");
		unsigned int l_posLoc = glGetUniformLocation(shaderProgram, "LightPos");

		unsigned int texLoc = glGetUniformLocation(shaderProgram, "Texture");
		unsigned int dLoc = glGetUniformLocation(shaderProgram, "dissolveFactor"); 

		// Connect locations for all variable
		glUniformMatrix4fv(mLoc, 1, GL_FALSE, value_ptr(model));
		glUniformMatrix4fv(vLoc, 1, GL_FALSE, value_ptr(view));
		glUniformMatrix4fv(pLoc, 1, GL_FALSE, value_ptr(perspective));
		glUniform3fv(cLoc, 1, value_ptr(cameraPos));

		glUniform3fv(k_ambientLoc, 1, value_ptr(material.ambient));
		glUniform3fv(k_diffuseLoc, 1, value_ptr(material.diffuse));
		glUniform3fv(k_specularLoc, 1, value_ptr(material.specular));
		glUniform1f(k_shinessLoc, material.shininess);

		glUniform3fv(l_ambientLoc, 1, value_ptr(light.ambient));
		glUniform3fv(l_diffuseLoc, 1, value_ptr(light.diffuse));
		glUniform3fv(l_specularLoc, 1, value_ptr(light.specular));
		glUniform3fv(l_posLoc, 1, value_ptr(light.position));

		glUniform1i(texLoc, 0);
		glUniform1f(dLoc, dissolveFactor);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, deerTexture);
		glBindVertexArray(deerVAO);
		glDrawArrays(GL_TRIANGLES, 0, deerModel->positions.size());
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);

		// Status update
		currentTime = glfwGetTime();
		dt = currentTime - lastTime;
		lastTime = currentTime;
		if (rotating)	angle += glm::radians(45.0f) * dt;
		if (angle > glm::radians(360.0f))	angle -= glm::radians(360.0f);
		if (shadingType == 6) dissolveFactor += 10*dt;

		glUseProgram(0);
		switch (shadingType)
		{
			case 1:
				glUseProgram(BPprogram);
				shaderProgram = BPprogram;
				dissolveFactor = -30;	
				break;
			case 2:
				glUseProgram(Gouraudprogram);
				shaderProgram = Gouraudprogram;
				dissolveFactor = -30;
				break;
			case 3:
				glUseProgram(Flatprogram);
				shaderProgram = Flatprogram;
				dissolveFactor = -30;
				break;
			case 4:
				glUseProgram(Toonprogram);
				shaderProgram = Toonprogram;
				dissolveFactor = -30;
				break;
			case 5:
				glUseProgram(Borderprogram);
				shaderProgram = Borderprogram;
				dissolveFactor = -30;
				break;
			case 6:
				glUseProgram(Dissolveprogram);
				shaderProgram = Dissolveprogram;
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glUseProgram(0);

	glfwTerminate();
	return 0;
}

/* TODO: Key callback
 *    1. Press '1' : switch to blinn-phong shading program
 *    2. Press '2' : switch to gouraud shading program
 *    3. Press '3' : switch to flat shading program
 *    4. Press '4' : switch to toon shading program
 */
 /* Advanced
  *   1. Press '5' : switch to border effect program
  *	  2. Press '6' : switch to dissolve effect program
  */
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		rotating = !rotating;
		cout << "KEY SPACE PRESSED\n";
	}

	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		shadingType = 1;
		cout << "KEY 1 PRESSED\n";
	}
	if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		shadingType = 2;
		cout << "KEY 2 PRESSED\n";
	}
	if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
		shadingType = 3;
		cout << "KEY 3 PRESSED\n";
	}
	if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
		shadingType = 4;
		cout << "KEY 4 PRESSED\n";
	}
	if (key == GLFW_KEY_5 && action == GLFW_PRESS) {
		shadingType = 5;
		cout << "KEY 5 PRESSED\n";
	}
	if (key == GLFW_KEY_6 && action == GLFW_PRESS) {
		shadingType = 6;
		cout << "KEY 6 PRESSED\n";
	}	
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	windowWidth = width;
	windowHeight = height;
}


unsigned int createShader(const char* filename, const char* type)
{
	unsigned int shader;
	if (strcmp(type, "vert") == 0)
		shader = glCreateShader(GL_VERTEX_SHADER);
	else if (strcmp(type, "frag") == 0)
		shader = glCreateShader(GL_FRAGMENT_SHADER);
	else if (strcmp(type, "geom") == 0)
		shader = glCreateShader(GL_GEOMETRY_SHADER);
	else
	{
		cout << "Unknown Shader Type.\n";
		return 0;
	}
	
	FILE* fp = fopen(filename, "rb");
	fseek(fp, 0, SEEK_END);
	long fsize = ftell(fp);
	fseek(fp, 0, SEEK_SET);  //same as rewind(fp);

	char* source = (char*)malloc(sizeof(char) * (fsize + 1));
	fread(source, fsize, 1, fp);
	fclose(fp);

	source[fsize] = 0;

	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::" << type << "::COMPLIATION_FAILED\n" << infoLog << endl;
		return -1;
	}

	return shader;
}


unsigned int createProgram(unsigned int vertexShader, unsigned int geometryShader, unsigned int fragmentShader)
{
	unsigned int program = glCreateProgram();

	//Attach our shaders to our program
	glAttachShader(program, vertexShader);
	glAttachShader(program, geometryShader);
	glAttachShader(program, fragmentShader);

	//Link our program
	glLinkProgram(program);

	//Note the different functions here: glGetProgram* instead of glGetShader*.
	int success = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success)
	{
		int maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		char* infoLog = (char*)malloc(sizeof(char) * (maxLength));
		glGetProgramInfoLog(program, maxLength, &maxLength, infoLog);

		//We don't need the program anymore.
		glDeleteProgram(program);
		//Don't leak shaders either.
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		cout << "LINKING ERROR: ";
		puts(infoLog);
		free(infoLog);

		return -1;
	}

	//Always detach shaders after a successful link.
	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);

	return program;
}

void loadMaterialLight() {
	material.ambient = glm::vec3(1.0, 1.0, 1.0);
	material.diffuse = glm::vec3(1.0, 1.0, 1.0);
	material.specular = glm::vec3(0.7, 0.7, 0.7);
	material.shininess = 10.5;

	light.ambient = glm::vec3(0.2, 0.2, 0.2);
	light.diffuse = glm::vec3(0.8, 0.8, 0.8);
	light.specular = glm::vec3(0.5, 0.5, 0.5);
	light.position = glm::vec3(10, 10, 10);
}

unsigned int ModelVAO(Object* model)
{
	unsigned int VAO, VBO[3];
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(3, VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * (model->positions.size()), &(model->positions[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 3, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * (model->normals.size()), &(model->normals[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 3, 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * (model->texcoords.size()), &(model->texcoords[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 2, 0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	return VAO;
}

void loadTexture(unsigned int& texture, const char* tFileName) {
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(tFileName, &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
}

glm::mat4 getPerspective()
{
	return glm::perspective(
		glm::radians(45.0f),
		(float)windowWidth / (float)windowHeight, 
		1.0f, 100.0f);
}

glm::mat4 getView()
{
	return glm::lookAt(
		cameraPos,
		glm::vec3(0, 0.5, 0),
		glm::vec3(0, 1, 0));
}
