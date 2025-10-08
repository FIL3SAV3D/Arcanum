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

OGLSystem::OGLSystem()
{
	sptr_OGLWindow		= std::make_shared<OGLWindow>(screenWidth, screenHeight, windowName);
	sptr_shaderManager	= std::make_shared<ShaderManager>();

	// Manual changing of vertex color
	Rect2D.verticesBuffer.at(0).color = glm::vec3(1.0f, 0.0f, 0.0f);
	Rect2D.verticesBuffer.at(1).color = glm::vec3(0.0f, 1.0f, 0.0f);
	Rect2D.verticesBuffer.at(2).color = glm::vec3(0.0f, 0.0f, 1.0f);
	Rect2D.verticesBuffer.at(3).color = glm::vec3(1.0f, 1.0f, 0.0f);

	Rect2D.verticesBuffer.at(0).texCoords = glm::vec2(1.0f, 1.0f);
	Rect2D.verticesBuffer.at(1).texCoords = glm::vec2(1.0f, 0.0f);
	Rect2D.verticesBuffer.at(2).texCoords = glm::vec2(0.0f, 0.0f);
	Rect2D.verticesBuffer.at(3).texCoords = glm::vec2(0.0f, 1.0f);
}

OGLSystem::~OGLSystem()
{
	sptr_OGLWindow.reset();
	sptr_shaderManager.reset();
}

void FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void OGLSystem::Run()
{
	Shader defaultShader("default.vert", "default.frag");

	// Generate buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Rect2D.GetVerticesBufferSize(), Rect2D.verticesBuffer.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Rect2D.GetIndiciesBufferSize(), Rect2D.indicesBuffer.data(), GL_STATIC_DRAW);

	// Vertex Attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vertex::pos)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vertex::pos) + sizeof(Vertex::color)));
	glEnableVertexAttribArray(2);

	//Texture sampling
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("D:\\PersonalProjects\\Arcanum\\Data\\Models\\Src_Images\\container.jpg", &width, &height, &nrChannels, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	// Debind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	glViewport(0, 0, screenWidth, screenHeight);

	glfwSetFramebufferSizeCallback(sptr_OGLWindow->GetWindow(), FrameBufferSizeCallback);
	
	// Simple FPS Counter
	std::chrono::steady_clock::time_point current_time = std::chrono::steady_clock::now();

	int frames = 0;
	double starttime = 0;
	float fps = 0.0f;
	double timepassed = 0;

	while (!glfwWindowShouldClose(sptr_OGLWindow->GetWindow()))
	{
		auto new_time = std::chrono::steady_clock::now();
		double delta_time = std::chrono::duration<float, std::chrono::seconds::period>(new_time - current_time).count();
		current_time = new_time;

		timepassed += delta_time;

		if (timepassed - starttime > 0.25 && frames > 10)
		{
			fps = static_cast<float>((double)frames / (timepassed - starttime));
			starttime = timepassed;
			frames = 0;
			printf("FPS: %f \n", fps);
		}

		// Input
		ProcessInput(sptr_OGLWindow->GetWindow());

		// Rendering commands
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		defaultShader.use();

		// Example on changing uniforms 
		float timeValue = glfwGetTime();
		float sine = (sin(timeValue) / 2.0f) + 0.5f;

		defaultShader.setFloat("opacity", sine);

		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Check and call events and swap buffers
		glfwSwapBuffers(sptr_OGLWindow->GetWindow());
		glfwPollEvents();

		frames++;
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();

	return;
}
