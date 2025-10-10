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
#include <map>

OGLSystem::OGLSystem()
{
	sptr_OGLWindow		= std::make_shared<OGLWindow>(screenWidth, screenHeight, windowName);
	inputHandler = std::make_shared<InputHandler>();
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
	Shader lightingShaderInstancing("lightingShaderInstancing.vert", "lightingShader.frag");

	

	Shader lightCubeShader("lightCubeShader.vert", "lightCubeShader.frag");
	Shader outlineShader("simpleOutline.vert", "simpleOutline.frag");

	Model backpack = Model(std::string("D:\\PersonalProjects\\Arcanum\\Data\\Models\\OpenGL\\Cube.fbx").c_str());

	Model planet = Model(std::string("D:\\PersonalProjects\\Arcanum\\Data\\Models\\OpenGL\\planet.obj").c_str());
	Model asteroid = Model(std::string("D:\\PersonalProjects\\Arcanum\\Data\\Models\\OpenGL\\rock.obj").c_str());

	unsigned int amount = 100000;
	glm::mat4* modelMatrices;
	modelMatrices = new glm::mat4[amount];
	srand(glfwGetTime()); // initialize random seed	
	float radius = 60.0f;
	float offset = 20.0f;
	for (unsigned int i = 0; i < amount; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		// 1. translation: displace along circle with 'radius' in range [-offset, offset]
		float angle = (float)i / (float)amount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f; // keep height of field smaller compared to width of x and z
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));

		// 2. scale: scale between 0.05 and 0.25f
		float scale = (rand() % 20) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));

		// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
		float rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// 4. now add to list of matrices
		modelMatrices[i] = model;
	}

	glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
	};

	//unsigned int Diffuse = TextureFromFile("rock.png", "D:\\PersonalProjects\\Arcanum\\Data\\Models\\OpenGL\\");
	unsigned int DiffuseAsteroid = TextureFromFile("rock.png", "D:\\PersonalProjects\\Arcanum\\Data\\Models\\OpenGL\\");
	unsigned int Specular = TextureFromFile("container2_specular.png", "D:\\PersonalProjects\\Arcanum\\Data\\Models\\Src_Images\\");
	unsigned int Grass = TextureFromFile("blending_transparent_window.png", "D:\\PersonalProjects\\Arcanum\\Data\\Models\\Src_Images\\");
	
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
	glDepthFunc(GL_LESS);

	//glEnable(GL_STENCIL_TEST);
	//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

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

	std::map<float, glm::mat4> sorted;


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
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		lightingShader.use();

		glm::mat4 projection = camera->GetProjectionMatrix();
		glm::mat4x4 view = camera->GetViewMatrix();
		lightingShader.setMatrix4x4f("projection", projection);
		lightingShader.setMatrix4x4f("view", view);
		lightingShader.setVec3f("viewPos", camera->GetPosition());


		lightingShader.setInt("material.diffuse", 0);
		lightingShader.setInt("material.specular", 1);
		lightingShader.setFloat("material.shininess", 32.0f);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		

		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, Specular);

		// Light stuff

		lightingShader.setVec3f("dirLight.direction", glm::vec3(0.3f, -0.5f, 0.3f));
		lightingShader.setVec3f("dirLight.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
		lightingShader.setVec3f("dirLight.diffuse", glm::vec3(0.3f, 0.3f, 0.3f));
		lightingShader.setVec3f("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

		lightingShader.setVec3f("pointLights[0].position", pointLightPositions[0] * 10.0f);
		lightingShader.setVec3f("pointLights[0].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		lightingShader.setVec3f("pointLights[0].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		lightingShader.setVec3f("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));

		lightingShader.setFloat("pointLights[0].constant", 1.0f);
		lightingShader.setFloat("pointLights[0].linear", 0.09f);
		lightingShader.setFloat("pointLights[0].quadratic", 0.032f);

		lightingShader.setVec3f("pointLights[1].position", pointLightPositions[1] * 10.0f);
		lightingShader.setVec3f("pointLights[1].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		lightingShader.setVec3f("pointLights[1].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		lightingShader.setVec3f("pointLights[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));

		lightingShader.setFloat("pointLights[1].constant", 1.0f);
		lightingShader.setFloat("pointLights[1].linear", 0.09f);
		lightingShader.setFloat("pointLights[1].quadratic", 0.032f);

		lightingShader.setVec3f("pointLights[2].position", pointLightPositions[2] * 10.0f);
		lightingShader.setVec3f("pointLights[2].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		lightingShader.setVec3f("pointLights[2].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		lightingShader.setVec3f("pointLights[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));

		lightingShader.setFloat("pointLights[2].constant", 1.0f);
		lightingShader.setFloat("pointLights[2].linear", 0.09f);
		lightingShader.setFloat("pointLights[2].quadratic", 0.032f);

		lightingShader.setVec3f("pointLights[3].position", pointLightPositions[3] * 10.0f);
		lightingShader.setVec3f("pointLights[3].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		lightingShader.setVec3f("pointLights[3].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		lightingShader.setVec3f("pointLights[3].specular", glm::vec3(1.0f, 1.0f, 1.0f));

		lightingShader.setFloat("pointLights[3].constant", 1.0f);
		lightingShader.setFloat("pointLights[3].linear", 0.09f);
		lightingShader.setFloat("pointLights[3].quadratic", 0.032f);

		//sorted.clear();

		//for (unsigned int i = 0; i < amount; i++)
		//{
		//	float distance = glm::length(camera->GetPosition() - glm::vec3(modelMatrices[i][3]));
		//	sorted[distance] = modelMatrices[i];
		//}

		//for (std::map<float, glm::mat4>::iterator it = sorted.begin(); it != sorted.end(); ++it)
		//{
		//	lightingShader.setMatrix4x4f("model", it->second);
		//	backpack.Draw(lightingShader);
		//}

		

		lightingShader.use();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		lightingShader.setMatrix4x4f("model", model);
		planet.Draw(lightingShader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, DiffuseAsteroid);

		// draw meteorites
		for (unsigned int i = 0; i < amount; i++)
		{
			lightingShader.setMatrix4x4f("model", modelMatrices[i]);
			asteroid.Draw(lightingShader);
		}

		//for (int i = 0; i < amount; i++)
		//{
		//	lightingShader.setMatrix4x4f("model", modelMatrices.at(i));
		//	backpack.Draw(lightingShader);
		//}

		//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		//glStencilMask(0x00); // disable writing to the stencil buffer
		//glDisable(GL_DEPTH_TEST);

		//outlineShader.use();

		//outlineShader.setMatrix4x4f("projection", projection);
		//outlineShader.setMatrix4x4f("view", view);

		//for (int i = 0; i < rats; i++)
		//{
		//	outlineShader.setMatrix4x4f("model", glm::scale(ratMats.at(i), glm::vec3(1.1f)));
		//	backpack.Draw(outlineShader);
		//}

		//glStencilMask(0xFF);
		//glStencilFunc(GL_ALWAYS, 1, 0xFF);
		//glEnable(GL_DEPTH_TEST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		lightCubeShader.use();
		lightCubeShader.setMatrix4x4f("projection", projection);
		lightCubeShader.setMatrix4x4f("view", view);
		for (int i = 0; i < pointLightPositions->length(); i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i] * 10.0f);
			model = glm::scale(model, glm::vec3(0.1f)); // a smaller cube

			lightCubeShader.setMatrix4x4f("model", model);

			backpack.Draw(lightCubeShader);
		}


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
