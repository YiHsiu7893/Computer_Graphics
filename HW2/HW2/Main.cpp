#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Object.h"

using namespace std;

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
unsigned int createShader(const string& filename, const string& type);
unsigned int createProgram(unsigned int vertexShader, unsigned int fragmentShader);
unsigned int modelVAO(Object& model);
unsigned int loadTexture(const char* tFileName);

Object penguinModel("obj/penguin.obj");
Object boardModel("obj/surfboard.obj");

int windowWidth = 800, windowHeight = 600;

// You can use these parameters
float swingAngle = 0;
float swingPos = 0;
float swingAngleDir = -1;
float swingPosDir = 1;
float squeezeFactor = 0;
float flashFactor = 0;
bool squeezing = false;
bool useGrayscale = false;
bool flashing = false;

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
	/* TODO#0: Change window title to "HW2 - your student id"
   *        Ex. HW2 - 312550000
   */
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "HW2-110550136", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// You are encouraged to complete TODO#1~3 by finishing the functions createShader, createProgram, loadTexture, and modelVAO, but it's not mandatory.
	// In other words, you can complete TODO#1~3 without using these four functions.

	/* TODO#1: Create vertex shader, fragment shader, shader program
	 * Hint:
	 *       createShader, createProgram, glUseProgram
	 * Note:
	 *       vertex shader filename: "vertexShader.vert"
	 *		 fragment shader filename: "fragmentShader.frag"
	 */
	unsigned int vertexShader, fragmentShader, shaderProgram;
	vertexShader = createShader("vertexShader.vert", "vert");
	fragmentShader = createShader("fragmentShader.frag", "frag");
	shaderProgram = createProgram(vertexShader, fragmentShader);
	glUseProgram(shaderProgram);

	/* TODO#2: Load texture
     * Hint:
     *       loadTexture
	 * Note:
	 *       penguin texture filename: "obj/penguin_diffuse.jpg"
	 *       board texture filename : "obj/surfboard_diffuse.jpg"
     */
	unsigned int penguinTexture, boardTexture;
	penguinTexture = loadTexture("obj/penguin_diffuse.jpg");
	boardTexture = loadTexture("obj/surfboard_diffuse.jpg");

	/* TODO#3: Set up VAO, VBO
	 * Hint:
	 *       modelVAO
	 */
	unsigned int penguinVAO, boardVAO;
	penguinVAO = modelVAO(penguinModel);
	boardVAO = modelVAO(boardModel);

	// Display loop
	glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
	glViewport(0, 0, windowWidth, windowHeight);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	double dt;
	double lastTime = glfwGetTime();
	double currentTime;

	/* TODO#4: Data connection - Retrieve uniform variable locations
	 *    1. Retrieve locations for model, view, and projection matrices.
	 *    2. Obtain locations for squeezeFactor, grayscale, and other parameters.
	 * Hint:
	 *    glGetUniformLocation
	 */
	 
	// Retrieve locations for model, view, and projection matrices.
	unsigned int mLoc, vLoc, pLoc;
	mLoc = glGetUniformLocation(shaderProgram, "M");
	vLoc = glGetUniformLocation(shaderProgram, "V");
	pLoc = glGetUniformLocation(shaderProgram, "P");
	// Obtain locations for squeezeFactor, grayscale, and texture.
	unsigned int sfLoc, gsLoc, texLoc;
	sfLoc = glGetUniformLocation(shaderProgram, "squeezeFactor");
	gsLoc = glGetUniformLocation(shaderProgram, "useGrayscale");
	texLoc = glGetUniformLocation(shaderProgram, "ourTexture");
	// Obtain location for timer.
	unsigned int tLoc;
	tLoc = glGetUniformLocation(shaderProgram, "timer");
	

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model(1.0f);
		glm::mat4 view = glm::lookAt(glm::vec3(0, 5, 5), glm::vec3(0, 0.5, 0), glm::vec3(0, 1, 0));
		glm::mat4 perspective = glm::perspective(glm::radians(45.0f),(float)windowWidth / (float)windowHeight,1.0f, 100.0f);

		/* TODO#5-1: Render Board
		 *    1. Set up board model matrix.
		 *    2. Send model, view, and projection matrices to the program.
		 *    3. Send squeezeFactor, useGrayscale, or other parameters to the program.(for key 's')
		 *    4. Set the value of the uniform variable "useGrayscale" and render the board.(for key 'g')
		 * Hint:
		 *	  rotate, translate, scale
		 *    glUniformMatrix4fv, glUniform1i
		 *    glUniform1i, glActiveTexture, glBindTexture, glBindVertexArray, glDrawArrays
		 */
		// Build the board model
		glm::mat4 board = model;
		board = glm::translate(board, glm::vec3(0.0f, 0.0f, swingPos));
		board = glm::rotate(board, glm::radians(swingAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		board = glm::translate(board, glm::vec3(0.0f, -0.5f, 0.0f));
		board = glm::rotate(board, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		board = glm::scale(board, glm::vec3(0.03f, 0.03f, 0.03f));
		
		// Connect locations for board, view, and projection matrices
		glUniformMatrix4fv(mLoc, 1, GL_FALSE, value_ptr(board));
		glUniformMatrix4fv(vLoc, 1, GL_FALSE, value_ptr(view));
		glUniformMatrix4fv(pLoc, 1, GL_FALSE, value_ptr(perspective));
		// Connect locations for squeezeFactor, grayscale, and texture
		glUniform1f(sfLoc, 0);
		glUniform1i(gsLoc, useGrayscale);
		glUniform1i(texLoc, 0);
		// Connect location for timer
		glUniform1f(tLoc, flashFactor);

		// Bind texture
		glActiveTexture(GL_TEXTURE0);  
		glBindTexture(GL_TEXTURE_2D, boardTexture);
		
		// Bind boardVAO and render primitives
		glBindVertexArray(boardVAO);  
		glDrawArrays(GL_TRIANGLES, 0, boardModel.positions.size());
		glBindVertexArray(0);  

	    /* TODO#5-2: Render Penguin
	     *    1. Set up penguin model matrix.
	     *    2. Send model, view, and projection matrices to the program.
	     *    3. Send squeezeFactor, useGrayscale, or other parameters to the program.(for key 's')
	     *    4. Set the value of the uniform variable "useGrayscale" and render the penguin.(for key 'g')
	     * Hint:
	     *	 rotate, translate, scale
	     *   glUniformMatrix4fv, glUniform1i
	     *   glUniform1i, glActiveTexture, glBindTexture, glBindVertexArray, glDrawArrays
	     */
	    // Build the penguin model
		glm::mat4 penguin = model;
		penguin = glm::translate(penguin, glm::vec3(0.0f, 0.0f, swingPos));
		penguin = glm::rotate(penguin, glm::radians(swingAngle), glm::vec3(0.0f, 1.0f, 0.f));  
		penguin = glm::translate(penguin, glm::vec3(0.0f, 0.0f, 0.0f));
		penguin = glm::rotate(penguin, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		penguin = glm::scale(penguin, glm::vec3(0.025f, 0.025f, 0.025f));

		glUniformMatrix4fv(mLoc, 1, GL_FALSE, value_ptr(penguin));
		glUniformMatrix4fv(vLoc, 1, GL_FALSE, value_ptr(view));
		glUniformMatrix4fv(pLoc, 1, GL_FALSE, value_ptr(perspective));
		glUniform1f(sfLoc, glm::radians(squeezeFactor));
		glUniform1i(gsLoc, useGrayscale);
		glUniform1i(texLoc, 0);
		glUniform1f(tLoc, 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, penguinTexture);
		
		glBindVertexArray(penguinVAO);
		glDrawArrays(GL_TRIANGLES, 0, penguinModel.positions.size());
		glBindVertexArray(0);

		// Status update
		currentTime = glfwGetTime();
		dt = currentTime - lastTime;
		lastTime = currentTime;
	    /* TODO#6: Update "swingAngle", "swingPos", "squeezeFactor"
	     * Speed:
	     *	  swingAngle   : 20 degrees/sec
		 *    swingPos     : 1.0f / sec
		 *    squeezeFactor: 90 degrees/sec
	     */
		swingAngle += swingAngleDir * 20.0 * dt;
		if (swingAngle > 20.0f) 
		{
			swingAngle = 20.0f;
			swingAngleDir *= -1.0f;  // 反轉方向
		}
		else if (swingAngle < -20.0f)
		{
			swingAngle = -20.0f;
			swingAngleDir *= -1.0f;  // 反轉方向
		}

		swingPos += swingPosDir * 1.0 * dt;
		if (swingPos > 2.0f)
		{
			swingPos = 2.0f;
			swingPosDir *= -1.0f;
		}
		else if (swingPos < 0.0f)
		{
			swingPos = 0.0f;
			swingPosDir *= -1.0f;
		}

		if (squeezing) squeezeFactor += 90.0 * dt;
		
		if (flashing) flashFactor += 25.0 * dt;

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glUseProgram(0);
	
	glfwTerminate();
	return 0;
}

// Add key callback

/* TODO#7: Key callback
 *    1. Press 's' to squeeze the penguin.
 *    2. Press 'g' to change to grayscale.
 *    3. Print "KEY S PRESSED" when key 's' is pressed, and similarly for other keys.
 */
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		cout << "KEY S PRESSED\n";
		squeezing = !squeezing;
	}
	if (key == GLFW_KEY_G && action == GLFW_PRESS)
	{
		cout << "KEY G PRESSED\n";
		useGrayscale = !useGrayscale;
	}
	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		cout << "KEY F PRESSED\n";
		// When flashing terminates, return to the original color
		if (flashing) flashFactor = 0;
		flashing = !flashing;
	}
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	windowWidth = width;
	windowHeight = height;
}

/* TODO#1-1: createShader
* Hint:
*       glCreateShader, glShaderSource, glCompileShader
*/
unsigned int createShader(const string& filename, const string& type)
{
	unsigned int shader;

	// Create either vertex shader or fragment shader
	if (type == "vert") shader = glCreateShader(GL_VERTEX_SHADER);
	else shader = glCreateShader(GL_FRAGMENT_SHADER);

	// Set the source code in the shader
	ifstream fs(filename);
	stringstream ss;
	string s;
	while (getline(fs, s))
	{
		ss << s << "\n";
	}
	string temp = ss.str();
	const char* source = temp.c_str();
	glShaderSource(shader, 1, &source, NULL);
	
	// Compile the shader
	glCompileShader(shader);

	return shader;
}

/* TODO#1-2: createProgram
* Hint:
*       glCreateProgram, glAttachShader, glLinkProgram, glDetachShader
*/
unsigned int createProgram(unsigned int vertexShader, unsigned int fragmentShader)
{
	// Create a program object
	unsigned int program = glCreateProgram();

	// Attach the shader object to the program
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	// Link this program
	glLinkProgram(program);

	// Detatch the shader object from the program
	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);

	return program;
}

/* TODO#2: Load texture
 * Hint:
 *       glEnable, glGenTextures, glBindTexture, glTexParameteri, glTexImage2D
 */
unsigned int loadTexture(const char* tFileName)
{
	unsigned int texture;
	
	// Enable texture
	glEnable(GL_TEXTURE_2D);
	// Generate textures and bind to the texturing target
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// Do texture wrappping and filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load texture images as dada
	int width, height, channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(tFileName, &width, &height, &channels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
	
	return texture;
}

/* TODO#3: Set up VAO, VBO
 * Hint:
 *       glGenVertexArrays, glBindVertexArray, glGenBuffers, glBindBuffer, glBufferData,
 *       glVertexAttribPointer, glEnableVertexAttribArray, 
 */
unsigned int modelVAO(Object& model)
{
	unsigned int VAO, VBO[3];
	
	// Generate vertex array object
	glGenVertexArrays(1, &VAO);
	// Bind the vertex array object
	glBindVertexArray(VAO);
	
	// Generate three vertex buffer objects
	glGenBuffers(3, VBO);

	// Bind the target buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	// Copy the data into the target buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * (model.positions.size()), &(model.positions[0]), GL_STATIC_DRAW);
	// Link the vertex buffer with the vertex shader input
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 3, 0);
	glEnableVertexAttribArray(0);
	// Unbind the target buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * (model.normals.size()), &(model.normals[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 3, 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * (model.texcoords.size()), &(model.texcoords[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 2, 0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Unbind the array object
	glBindVertexArray(0);

	return VAO;
}
