#include "OGLSystem.h"

#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

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

#include "FrameBuffer.h"

#include <glm/gtc/type_ptr.hpp>
#include "Cubemap.h"

OGLSystem::OGLSystem()
{
	screenWidth = 800;
	screenHeight = 600;
	sptr_OGLWindow		= std::make_shared<OGLWindow>(screenWidth, screenHeight, windowName);
	inputHandler = std::make_shared<InputHandler>();
}

OGLSystem::~OGLSystem()
{
	sptr_OGLWindow.reset();
	inputHandler.reset();
}

void OGLSystem::Run()
{
	Shader defaultShader("default");

	Shader lightingShader("lightingShader.vert", "lightingShader.frag");
	Shader lightingShaderInstancing("lightingShaderInstancing.vert", "lightingShaderInstancing.frag");
	Shader screenShader("screenShader.vert", "screenShader.frag");

	Shader lightCubeShader("lightCubeShader.vert", "lightCubeShader.frag");
	Shader outlineShader("simpleOutline.vert", "simpleOutline.frag");

	Shader envShader("envMapping.vert", "envMapping.frag");

	Shader skyboxShader("skybox.vert", "skybox.frag");

	Shader explosionGeometryShader("geoShader.vert", "geoShader.frag", "geoShader.gs");

	Shader normals("normalViewShader.vert", "normalViewShader.frag", "normalViewShader.gs");

	Shader blinnPhongShader("blinnPhong.vert", "blinnPhong.frag");

	Shader PBRShader("PBRNoTextures.vert", "PBRNoTextures.frag");
	Shader PBRTextureShader("PBRTextures.vert", "PBRTextures.frag");

	Shader PBRInstancedShader("PBRNoTexturesInstancing.vert", "PBRNoTexturesInstancing.frag");

	std::string directory;
	std::string currentPath = std::filesystem::current_path().string();
	const size_t last_slash_idx = currentPath.rfind('\\');
	if (std::string::npos != last_slash_idx)
	{
		directory = currentPath.substr(0, last_slash_idx);
	}

	Model backpack = Model(std::string("F:\\PersonalProjects\\Arcanum\\Data\\Models\\OpenGL\\Cube.fbx").c_str());

	Model Rat = Model(std::string("F:\\PersonalProjects\\Arcanum\\Data\\Models\\GLB_Models\\Rat.glb").c_str());

	Model invertedCube = Model(std::string("F:\\PersonalProjects\\Arcanum\\Data\\Models\\OpenGL\\InvertedCube.fbx").c_str());
	Model Cube = Model(std::string("F:\\PersonalProjects\\Arcanum\\Data\\Models\\OpenGL\\Cube.fbx").c_str());

	Model planet = Model(std::string("F:\\PersonalProjects\\Arcanum\\Data\\Models\\OpenGL\\planet.obj").c_str());
	Model asteroid = Model(std::string("F:\\PersonalProjects\\Arcanum\\Data\\Models\\OpenGL\\rock.obj").c_str());

	unsigned int amount = 100000;
	glm::mat4* modelMatrices;
	modelMatrices = new glm::mat4[amount];
	srand(glfwGetTime()); // initialize random seed	
	float radius = 100.0f;
	float offset = 50.0f;
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

	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
	};

	glm::vec3 lightPositions[] = {
		glm::vec3(10.0f,  0.0f, 10.0f),
		glm::vec3(-10.0f,  0.0f, 10.0f),
		glm::vec3(-10.0f, 0.0f, -10.0f),
		glm::vec3(10.0f, 0.0f, -10.0f),
	};
	glm::vec3 lightColors[] = {
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(0.0f, 300.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 0.0f)
	};

	//unsigned int Diffuse = TextureFromFile("rock.png", "D:\\PersonalProjects\\Arcanum\\Data\\Models\\OpenGL\\");
	unsigned int DiffuseAsteroid = TextureFromFile("rock.png", directory + "\\Arcanum\\Data\\Models\\OpenGL\\");
	unsigned int Specular = TextureFromFile("container2_specular.png", directory + "\\Arcanum\\Data\\Models\\Src_Images\\");
	unsigned int Grass = TextureFromFile("blending_transparent_window.png", directory + "\\Arcanum\\Data\\Models\\Src_Images\\");

	unsigned int BC = TextureFromFile("T_Rat_BC.png", directory + "\\Arcanum\\Data\\Models\\Src_Images\\");
	unsigned int N = TextureFromFile("T_Rat_N.png", directory + "\\Arcanum\\Data\\Models\\Src_Images\\");
	unsigned int ORM = TextureFromFile("T_Rat_ORM.png", directory + "\\Arcanum\\Data\\Models\\Src_Images\\");
	
	Cubemap cubemap{"TestCubeMap"};

	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

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

	glfwSetWindowUserPointer(sptr_OGLWindow->GetWindow(), this);
	glfwSetFramebufferSizeCallback(sptr_OGLWindow->GetWindow(), FrameBufferSizeCallback);

	glfwSetCursorPosCallback(sptr_OGLWindow->GetWindow(), CursorCallback);
	glfwSetScrollCallback(sptr_OGLWindow->GetWindow(), ScrollCallback);
	glfwSetKeyCallback(sptr_OGLWindow->GetWindow(), KeyCallback);

	//glEnable(GL_STENCIL_TEST);
	//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_MULTISAMPLE);

	//glEnable(GL_FRAMEBUFFER_SRGB);

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

	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));


	// vertex buffer object
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

	for (unsigned int i = 0; i < asteroid.GetMeshCount(); i++)
	{
		unsigned int VAO = asteroid.GetMeshes()->at(i).GetVAO();
		glBindVertexArray(VAO);
		// vertex attributes
		std::size_t vec4Size = sizeof(glm::vec4);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(sptr_OGLWindow->GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 460");
	
	frameBuffer = std::make_shared<FrameBuffer>(glm::vec2(screenWidth, screenHeight));
	// framebuffer

	unsigned int intermediateFBO;
	glGenFramebuffers(1, &intermediateFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);

	unsigned int screenTexture;
	glGenTextures(1, &screenTexture);
	glBindTexture(GL_TEXTURE_2D, screenTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);

	while (!glfwWindowShouldClose(sptr_OGLWindow->GetWindow()))
	{
		currentFrameTime = (float)glfwGetTime();
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		// Input Events
		inputHandler->ProcessInput(sptr_OGLWindow->GetWindow());

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
		frameBuffer->Bind();
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = camera->GetProjectionMatrix();
		glm::mat4x4 view = camera->GetViewMatrix();

		PBRTextureShader.Use();

		PBRTextureShader.setMat4("view", view);
		PBRTextureShader.setMat4("projection", projection);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, BC);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, N);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, ORM);

		PBRTextureShader.setVec3("camPos", camera->GetPosition());

		for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
		{
			glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
			newPos = lightPositions[i];
			PBRTextureShader.setVec3(std::string("lightPositions[" + std::to_string(i) + "]").c_str(), newPos);
			PBRTextureShader.setVec3(std::string("lightColors[" + std::to_string(i) + "]").c_str(), lightColors[i]);

			auto model = glm::mat4(1.0f);
			model = glm::translate(model, newPos);
			model = glm::scale(model, glm::vec3(0.5f));
			PBRTextureShader.setMat4("model", model);
			PBRTextureShader.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
			Cube.Draw(PBRTextureShader);
		}

		PBRTextureShader.setMat4("model", glm::mat4(1.0f));
		Rat.Draw(PBRTextureShader);

		//sorted.clear();

		//for (unsigned int i = 0; i < amount; i++)
		//{
		//	float distance = glm::length(camera->GetPosition() - glm::vec3(modelMatrices[i][3]));
		//	sorted[distance] = modelMatrices[i];
		//}

		//for (std::map<float, glm::mat4>::iterator it = sorted.begin(); it != sorted.end(); ++it)
		//{
		//	lightingShader.setMat4("model", it->second);
		//	backpack.Draw(lightingShader);
		//}

		//glm::mat4 model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		//blinnPhongShader.setVec3("cameraPos", camera->GetPosition());

		////blinnPhongShader.setFloat("time", glfwGetTime());

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap.textureID);
		//planet.Draw(blinnPhongShader);

		//normals.Use();
		//normals.setMat4("model", model);
		//normals.setMat4("view", view);
		//normals.setMat4("projection", projection);
		//normals.setVec3("cameraPos", camera->GetPosition());

		//planet.Draw(normals);

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, DiffuseAsteroid);

		// draw meteorites
		//for (unsigned int i = 0; i < amount; i++)
		//{
		//	lightingShader.setMat4("model", modelMatrices[i]);
		//	asteroid.Draw(lightingShader);
		//}


		PBRInstancedShader.Use();

		PBRShader.setMat4("view", view);
		PBRShader.setMat4("projection", projection);

		PBRShader.setVec3("albedo", glm::vec3(1.0f, 1.0f, 1.0f));
		PBRShader.setFloat("metallic", 0.5f);
		PBRShader.setFloat("ao", 1.0f);
		PBRShader.setFloat("roughness", 0.5f);

		PBRShader.setVec3("camPos", camera->GetPosition());

		for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
		{
			glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
			newPos = lightPositions[i];
			PBRShader.setVec3(std::string("lightPositions[" + std::to_string(i) + "]").c_str(), newPos);
			PBRShader.setVec3(std::string("lightColors[" + std::to_string(i) + "]").c_str(), lightColors[i]);

			//auto model = glm::mat4(1.0f);
			//model = glm::translate(model, newPos);
			//model = glm::scale(model, glm::vec3(0.5f));
			//PBRShader.setMat4("model", model);
			//PBRShader.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
			//Cube.Draw(PBRShader);
		}

		for (unsigned int i = 0; i < asteroid.GetMeshCount(); i++)
		{
			glBindVertexArray(asteroid.GetMeshes()->at(i).GetVAO());
			glDrawElementsInstanced(GL_TRIANGLES, asteroid.GetMeshes()->at(i).indices.size(), GL_UNSIGNED_INT, 0, amount);
		}

		//for (int i = 0; i < amount; i++)
		//{
		//	lightingShader.setMat4("model", modelMatrices.at(i));
		//	backpack.Draw(lightingShader);
		//}

		//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		//glStencilMask(0x00); // disable writing to the stencil buffer
		//glDisable(GL_DEPTH_TEST);

		//outlineShader.use();

		//outlineShader.setMat4("projection", projection);
		//outlineShader.setMat4("view", view);

		//for (int i = 0; i < rats; i++)
		//{
		//	outlineShader.setMat4("model", glm::scale(ratMats.at(i), glm::vec3(1.1f)));
		//	backpack.Draw(outlineShader);
		//}

		//glStencilMask(0xFF);
		//glStencilFunc(GL_ALWAYS, 1, 0xFF);
		//glEnable(GL_DEPTH_TEST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		lightCubeShader.Use();
		lightCubeShader.setMat4("projection", projection);
		lightCubeShader.setMat4("view", view);
		for (int i = 0; i < pointLightPositions->length(); i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i] * 10.0f);
			model = glm::scale(model, glm::vec3(0.1f)); // a smaller cube

			lightCubeShader.setMat4("model", model);

			backpack.Draw(lightCubeShader);
		}

		glDepthFunc(GL_LEQUAL);
		skyboxShader.Use();
		skyboxShader.setMat4("projection", projection);
		skyboxShader.setMat4("view", glm::mat4(glm::mat3(view)));

		glBindVertexArray(skyboxVAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap.textureID);

		invertedCube.Draw(skyboxShader);
		glDepthFunc(GL_LESS);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBuffer->textureColorbuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFBO);
		glBlitFramebuffer(0, 0, screenWidth, screenHeight, 0, 0, screenWidth, screenHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		screenShader.Use();
		glBindVertexArray(quadVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, screenTexture);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Imgui begin render
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::SetNextWindowSize(ImVec2(screenWidth, 100));
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::Begin("Debug Menu", nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_::ImGuiWindowFlags_NoResize);
		ImGui::Button("Spawn asteroid");
		ImGui::Button("Spawn planet");
		ImGui::End();

		//ImGui::ShowDemoWindow();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		// Imgui end render


		// Check and call events and swap buffers
		glfwSwapBuffers(sptr_OGLWindow->GetWindow());
		glfwPollEvents();

		frames++;
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();

	return;
}

void OGLSystem::FrameBufferSizeCallback(GLFWwindow* _window, int _width, int _height)
{
	OGLSystem* ptr = static_cast<OGLSystem*>(glfwGetWindowUserPointer(_window));

	ptr->screenWidth = _width;
	ptr->screenHeight = _height;

	if (ptr->camera)
	{
		ptr->camera->screenWidth = _width;
		ptr->camera->screenHeight = _height;
	}

	ptr->frameBuffer->Resize(glm::vec2(_width, _height));

	glViewport(0, 0, _width, _height);
}

void OGLSystem::CursorCallback(GLFWwindow* _window, double _xpos, double _ypos)
{
	OGLSystem* ptr = static_cast<OGLSystem*>(glfwGetWindowUserPointer(_window));
	if (ptr->inputHandler)
	{
		ptr->inputHandler->CursorCallBackImpl(_window, _xpos, _ypos);
	}
}

void OGLSystem::ScrollCallback(GLFWwindow* _window, double _xoffset, double _yoffset)
{
	OGLSystem* ptr = static_cast<OGLSystem*>(glfwGetWindowUserPointer(_window));
	if (ptr->inputHandler)
	{
		ptr->inputHandler->ScrollCallBackImpl(_window, _xoffset, _yoffset);
	}
}

void OGLSystem::KeyCallback(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods)
{
	OGLSystem* ptr = static_cast<OGLSystem*>(glfwGetWindowUserPointer(_window));
	if (ptr->inputHandler)
	{
		ptr->inputHandler->KeyCallBackImpl(_window, _key, _scancode, _action, _mods);
	}
}
