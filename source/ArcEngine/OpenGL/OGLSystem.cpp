#include "OGLSystem.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream> 
#include <chrono>
#include <filesystem>

#include "Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Camera.h"

#include "Model.h"

OGLSystem::OGLSystem()
{
	sptr_OGLWindow		= std::make_shared<OGLWindow>(screenWidth, screenHeight, windowName);
	inputHandler = std::make_shared<InputHandler>();

	std::vector<float> verts = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

}

OGLSystem::~OGLSystem()
{
	sptr_OGLWindow.reset();
	inputHandler.reset();
}

void FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void OGLSystem::Run()
{
	Shader defaultShader("default.vert", "default.frag");
	Shader lightingShader("lightingShader.vert", "lightingShader.frag");
	Shader lightCubeShader("lightCubeShader.vert", "lightCubeShader.frag");

	//// Generate buffers
	//glGenVertexArrays(1, &cubeVAO);
	//glGenBuffers(1, &VBO);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, Cube.GetVerticesBufferSize(), Cube.verticesBuffer.data(), GL_STATIC_DRAW);

	//glBindVertexArray(cubeVAO);

	//// Vertex Attribute
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	//glEnableVertexAttribArray(0);

	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vertex::position)));
	//glEnableVertexAttribArray(1);

	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vertex::position) + sizeof(Vertex::normal)));
	//glEnableVertexAttribArray(2);

	Model backpack = Model(std::string("D:\\PersonalProjects\\Arcanum\\Data\\Models\\OBJ_Models\\cube.obj").c_str());

	// Light array
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vertex::position)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vertex::position) + sizeof(Vertex::normal)));
	glEnableVertexAttribArray(2);

	// Debind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	glViewport(0, 0, screenWidth, screenHeight);

	glfwSetFramebufferSizeCallback(sptr_OGLWindow->GetWindow(), FrameBufferSizeCallback);
	
	glEnable(GL_DEPTH_TEST);

	// Simple FPS Counter
	std::chrono::steady_clock::time_point current_time = std::chrono::steady_clock::now();

	int frames = 0;
	double starttime = 0;
	float fps = 0.0f;
	double timepassed = 0;

	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(sptr_OGLWindow->GetWindow(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

	camera = std::make_shared<Camera>();
	camera->SetPosition(glm::vec3(0.0f, 1.0f, 0.0f));

	inputHandler->AddListener(camera);

	while (!glfwWindowShouldClose(sptr_OGLWindow->GetWindow()))
	{
		if (glfwGetKey(sptr_OGLWindow->GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(sptr_OGLWindow->GetWindow(), true);
		}

		// Input Events
		inputHandler->ProcessInput(sptr_OGLWindow->GetWindow());

		currentFrameTime = (float)glfwGetTime();
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		timepassed += deltaTime;

		if (timepassed - starttime > 0.25 && frames > 10)
		{
			fps = static_cast<float>((double)frames / (timepassed - starttime));
			starttime = timepassed;
			frames = 0;
			printf("FPS: %f \n", fps);
		}

		camera->Update(deltaTime);

		// Rendering commands
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightingShader.use();
		lightingShader.setVec3f("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
		lightingShader.setVec3f("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
		lightingShader.setVec3f("viewPos", camera->GetPosition());

		glm::mat4 projection = camera->GetProjectionMatrix();
		glm::mat4x4 view = camera->GetViewMatrix();
		lightingShader.setMatrix4x4f("projection", projection);
		lightingShader.setMatrix4x4f("view", view);

		glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

		lightingShader.setVec3f("lightPos", lightPos);
		lightingShader.setVec3f("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
		lightingShader.setVec3f("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
		lightingShader.setVec3f("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
		lightingShader.setFloat("material.shininess", 32.0f);

		glm::vec3 lightColor;
		lightColor.x = sin(glfwGetTime() * 2.0f);
		lightColor.y = sin(glfwGetTime() * 0.7f);
		lightColor.z = sin(glfwGetTime() * 1.3f);

		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);

		lightingShader.setVec3f("light.ambient", ambientColor);
		lightingShader.setVec3f("light.diffuse", diffuseColor);

		//lightingShader.setVec3f("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		//lightingShader.setVec3f("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f)); // darken diffuse light a bit
		lightingShader.setVec3f("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

		glm::mat4 modelback = glm::mat4(1.0f);
		lightingShader.setMatrix4x4f("model", modelback);
		backpack.Draw(lightingShader);

		lightCubeShader.use();
		lightCubeShader.setMatrix4x4f("projection", projection);
		lightCubeShader.setMatrix4x4f("view", view);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		
		lightCubeShader.setMatrix4x4f("model", model);

		backpack.Draw(lightCubeShader);


		// Check and call events and swap buffers
		glfwSwapBuffers(sptr_OGLWindow->GetWindow());
		glfwPollEvents();

		frames++;
	}

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();

	return;
}
