#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

//glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

 //GLFW
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Camera.h"

 //SOIL2
#include <SOIL2/SOIL2.h>

const GLuint WIDTH = 800, HEIGHT = 600;

Camera camera;
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}
}
void MouseCallback(GLFWwindow *window, double xPos, double yPos) {
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouse(xOffset, yOffset);
}
void DoMovement() {
	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.processKeyboard(FORWARD, deltaTime);
	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.processKeyboard(BACKWARD, deltaTime);
	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.processKeyboard(LEFT, deltaTime);
	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.processKeyboard(RIGHT, deltaTime);
	}

	if (keys[GLFW_KEY_SPACE] && camera.jumping == false)
	{
		camera.processJump();
	}
}

void Update() {
	camera.Update(deltaTime);
}

int main() {
	// Init GLFW

	glfwInit();

	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Project", nullptr, nullptr);

	int ScreenWidth, ScreenHeight;
	glfwGetFramebufferSize(window, &ScreenWidth, &ScreenHeight);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, ScreenWidth, ScreenHeight);

	Shader ourShader("core.vert", "core.frag");

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH);
	glEnable(GL_DEPTH_TEST);

	GLfloat ground[] = 
	{
		10.0f, 0.0f, 10.0f, 1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f, 0.0f, 0.0f,
		-10.0f, 0.0f, -10.0f, 0.0f, 1.0f,
		-10.0f, 0.0f, -10.0f, 0.0f, 1.0f,
		10.0f, 0.0f, 10.0f, 1.0f, 0.0f,
		10.0f, 0.0f, -10.0f, 1.0f, 1.0f
	};

	GLfloat vertices[] =
	{
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};


	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	//initialize texture
	GLuint texture[2];
	int t_ScreenWidth, t_hight;

	
	glGenTextures(2, texture);

	//binds and generates texture
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	unsigned char *image = SOIL_load_image("../../bricks.jpg", &t_ScreenWidth, &t_hight, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t_ScreenWidth, t_hight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);

	int t_ScreenWidth1, t_hight1;

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	unsigned char *image2 = SOIL_load_image("../../concrete.jpg", &t_ScreenWidth1, &t_hight1, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t_ScreenWidth1, t_hight1, 0, GL_RGBA, GL_UNSIGNED_BYTE, image2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image2);


	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = (GLfloat)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		Update();

		// Render
		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		//shape
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid *)0);
		glEnableVertexAttribArray(0);

		//texture
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid *)(3 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(2);

		//	draw
		ourShader.Use();

		//activates the 0th texture unit
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		//tells the fragment shader to look at the 0th texture unit when sampling color
		glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture"), 0);

		glm::mat4 projection(1);
		projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 1000.0f);

		glm::mat4 view(1);
		view = camera.GetViewMatrix();

		glm::mat4 model(1);
		model = glm::translate(model, vec3(0.0f, 4.0f, -7.0f));

		GLuint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLuint projectionLoc = glGetUniformLocation(ourShader.Program, "projection");
		GLuint modelLoc = glGetUniformLocation(ourShader.Program, "model");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(ground), ground, GL_STATIC_DRAW);

		//shape ground
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid *)0);

		//texture ground
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid *)(3 * sizeof(GL_FLOAT)));

		//activates the first texture unit
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		//tells the fragment shader to look at the 0th texture unit when sampling color
		glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture"), 1);
		

		glm::mat4 model2(1);
		model2 = glm::translate(model2, vec3(0.0f, 0.0f, -7.0f));

		GLuint viewLoc2 = glGetUniformLocation(ourShader.Program, "view");
		GLuint projectionLoc2 = glGetUniformLocation(ourShader.Program, "projection");
		GLuint modelLoc2 = glGetUniformLocation(ourShader.Program, "model");

		glUniformMatrix4fv(viewLoc2, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc2, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc2, 1, GL_FALSE, glm::value_ptr(model2));

		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}


	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);


	glfwTerminate();
	return EXIT_SUCCESS;
}
