#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Source/Shader.h"
#include "Source/Camera.hpp"
#include "Source/Model.hpp"

#include "Source/Vendor/stb_image/stb_image.h"


#define SCREENWIDTH 1000
#define SCREENHEIGHT 800

// Prototype
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void userInput(GLFWwindow* window);
void mouseCursorPosition(GLFWwindow* window, double xPos, double yPos);
void mouseScrollPosition(GLFWwindow* window, double xOffset, double yOffset);

glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;

/* Frames */
float deltaTime = 0.0f;
float lastFrame = 0.0f;

/* Camera */
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = float(SCREENWIDTH) / 2.0f;
float lastY = float(SCREENHEIGHT) / 2.0f;
bool isFirstMouse = true;

/* Light */
glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 1.5f);

int main()
{
	/* Initialize GLFW */
	glfwInit();

	/* Initialize Version 3.3 */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCREENWIDTH, SCREENHEIGHT, "OpenGL Window", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to Create a window\n";
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetCursorPosCallback(window, mouseCursorPosition);
	glfwSetScrollCallback(window, mouseScrollPosition);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize Glew\n";
		glfwTerminate();
	}

	glEnable(GL_DEPTH_TEST);

	// Make sure it's true before loading model.
	stbi_set_flip_vertically_on_load(true);

	/* Model */
	Model myBackPack("Resource/Object/Backpack/backpack.obj");

	Shader shader("Resource/Shader/VertexShader.glsl", "Resource/Shader/FragmentShader.glsl");
	shader.use();

	while (!glfwWindowShouldClose(window))
	{
		/* Update */
		userInput(window);
		float time = glfwGetTime();
		deltaTime = time - lastFrame;
		lastFrame = time;

		float radius = 5.0f;
		float camX = std::sin(time) * radius;
		float camZ = std::cos(time) * radius;

		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		

		/* First Object */
		shader.use();

		projection = glm::perspective(glm::radians(camera.Zoom), float(SCREENWIDTH) / float(SCREENHEIGHT), 0.1f, 100.0f);
		shader.setMat4("projection", projection);

		view = glm::mat4(1.0f);
		view = camera.GetViewMatrix();
		shader.setMat4("view", view);

		model = glm::mat4(1.0f);
		shader.setMat4("model", model);

		// Draw your model
		myBackPack.Draw(shader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void userInput(GLFWwindow* window)
{
	const float camera_speed = 2.5f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // To exit the program
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_TRUE) // Forward
		camera.ProcessKeyboard(FORWARD, deltaTime);
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_TRUE) // Backward
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_TRUE) // Right
		camera.ProcessKeyboard(RIGHT, deltaTime);
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_TRUE) // Left
		camera.ProcessKeyboard(LEFT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_TRUE) // Up
		camera.ProcessKeyboard(UP, deltaTime);
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_TRUE) // Down
		camera.ProcessKeyboard(DOWN, deltaTime);
}

void mouseCursorPosition(GLFWwindow* window, double xPos, double yPos)
{
	if (isFirstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		isFirstMouse = false;
	}

	float xoffset = xPos - lastX;
	float yoffset = lastY - yPos;
	lastX = xPos;
	lastY = yPos;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_TRUE)
		camera.ProcessMouseMovement(xoffset, yoffset);
}

void mouseScrollPosition(GLFWwindow* window, double xOffset, double yOffset)
{
	camera.ProcessMouseScroll(yOffset);
}