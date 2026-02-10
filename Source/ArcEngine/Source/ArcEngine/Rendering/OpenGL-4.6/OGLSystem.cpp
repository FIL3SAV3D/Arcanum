//#include "OGLSystem.h"
//
//#include <imgui.h>
//#include <backends/imgui_impl_opengl3.h>
//#include <backends/imgui_impl_glfw.h>
//
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//
//#include <iostream>
//#include <fstream> 
//#include <chrono>
//#include <filesystem>
//
//#include "Shader.h"
//
//#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image.h>
//
//#include "Camera.h"
//
//#include "Model.h"
//#include <map>
//
//#include "FrameBuffer.h"
//
//#include <glm/gtc/type_ptr.hpp>
//#include "Asset/Types/Cubemap.h"
//
//
//#include <glm/detail/type_float.hpp>
//#include "DeferredRenderer.h"
//
//OGLSystem::OGLSystem()
//{
//	screenWidth = 1200;
//	screenHeight = 800;
//	sptr_OGLWindow		= std::make_shared<Window>(screenWidth, screenHeight, windowName);
//	inputHandler = std::make_shared<InputHandler>();
//}
//
//OGLSystem::~OGLSystem()
//{
//	sptr_OGLWindow.reset();
//	inputHandler.reset();
//}
//
//void OGLSystem::Run()
//{
//	glfwSetWindowUserPointer(sptr_OGLWindow->GetNativeWindow(), this);
//	glfwSetFramebufferSizeCallback(sptr_OGLWindow->GetNativeWindow(), FrameBufferSizeCallback);
//
//	glfwSetCursorPosCallback(sptr_OGLWindow->GetNativeWindow(), CursorCallback);
//	glfwSetScrollCallback(sptr_OGLWindow->GetNativeWindow(), ScrollCallback);
//	glfwSetKeyCallback(sptr_OGLWindow->GetNativeWindow(), KeyCallback);
//
//	//glEnable(GL_STENCIL_TEST);
//	//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
//	//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
//
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	
//	glEnable(GL_CULL_FACE);
//	glCullFace(GL_BACK);
//
//	//glEnable(GL_MULTISAMPLE);
//	//glEnable(GL_FRAMEBUFFER_SRGB);
//
//	int frames = 0;
//	double starttime = 0;
//	float fps = 0.0f;
//	double timepassed = 0;
//
//	if (glfwRawMouseMotionSupported())
//		glfwSetInputMode(sptr_OGLWindow->GetNativeWindow(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
//
//	camera = std::make_shared<Camera>();
//	camera->SetPosition(glm::vec3(0.0f, 1.0f, 0.0f));
//
//	inputHandler->AddListener(camera);
//
//	IMGUI_CHECKVERSION();
//	ImGui::CreateContext();
//	ImGuiIO& io = ImGui::GetIO(); (void)io;
//	ImGui::StyleColorsDark();
//	ImGui_ImplGlfw_InitForOpenGL(sptr_OGLWindow->GetNativeWindow(), true);
//	ImGui_ImplOpenGL3_Init("#version 460");
//
//	renderer = std::make_shared<DeferredRenderer>();
//	renderer->Initialize(glm::vec2(screenWidth, screenHeight));
//
//	glm::vec2 test = glm::vec2(0.0f, 1.0f);
//
//	while (!glfwWindowShouldClose(sptr_OGLWindow->GetNativeWindow()))
//	{
//		currentFrameTime = (float)glfwGetTime();
//		deltaTime = currentFrameTime - lastFrameTime;
//		lastFrameTime = currentFrameTime;
//
//		// Input Events
//		inputHandler->ProcessInput(sptr_OGLWindow->GetNativeWindow());
//
//		timepassed += deltaTime;
//
//		if (timepassed - starttime > 0.10 && frames > 10)
//		{
//			fps = static_cast<float>((double)frames / (timepassed - starttime));
//			starttime = timepassed;
//			frames = 0;
//		}
//
//		camera->Update(deltaTime);
//
//		renderer->RenderSceneCB(camera->GetProjectionMatrix(), camera->GetViewMatrix(), camera->GetPosition(), debugMode, test);
//
//		// Imgui begin render
//		ImGui_ImplOpenGL3_NewFrame();
//		ImGui_ImplGlfw_NewFrame();
//		ImGui::NewFrame();
//
//		if (ImGui::BeginMainMenuBar())
//		{
//			if (ImGui::BeginMenu("File"))
//			{
//				if (ImGui::MenuItem("Save", "CTRL+S")) {  }
//				if (ImGui::MenuItem("Exit", "ESCAPE")) { glfwSetWindowShouldClose(sptr_OGLWindow->GetNativeWindow(), true); }
//				ImGui::EndMenu();
//			}
//			if (ImGui::BeginMenu("Edit"))
//			{
//				if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
//				if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {} // Disabled item
//				ImGui::Separator();
//				if (ImGui::MenuItem("Cut", "CTRL+X")) {}
//				if (ImGui::MenuItem("Copy", "CTRL+C")) {}
//				if (ImGui::MenuItem("Paste", "CTRL+V")) {}
//				ImGui::EndMenu();
//			}
//			if (ImGui::BeginMenu("Debug"))
//			{
//				if (ImGui::Checkbox("Debug Rendering", &debugMode)) {}
//				if (ImGui::SliderFloat2("Depth Range", glm::value_ptr(test), 0.0f, 1.0f)) {}
//
//				ImGui::EndMenu();
//			}
//			if (ImGui::BeginMenu("Post Process"))
//			{
//				ImGui::EndMenu();
//			}
//			
//			ImGui::Text((std::string("FPS: ") + std::to_string(static_cast<int>(fps))).c_str());
//			ImGui::EndMainMenuBar();
//		}
//
//		//ImGui::ShowDemoWindow();
//
//		ImGui::Render();
//		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//		// Imgui end render
//
//
//		// Check and call events and swap buffers
//		glfwSwapBuffers(sptr_OGLWindow->GetNativeWindow());
//		glfwPollEvents();
//
//		frames++;
//	}
//
//	ImGui_ImplOpenGL3_Shutdown();
//	ImGui_ImplGlfw_Shutdown();
//	ImGui::DestroyContext();
//
//	glDeleteVertexArrays(1, &cubeVAO);
//	glDeleteBuffers(1, &VBO);
//	glDeleteBuffers(1, &EBO);
//
//	glfwTerminate();
//
//	return;
//}
//
//void OGLSystem::FrameBufferSizeCallback(GLFWwindow* _window, int _width, int _height)
//{
//	OGLSystem* ptr = static_cast<OGLSystem*>(glfwGetWindowUserPointer(_window));
//
//	ptr->screenWidth = _width;
//	ptr->screenHeight = _height;
//
//	if (ptr->camera)
//	{
//		ptr->camera->screenWidth = _width;
//		ptr->camera->screenHeight = _height;
//	}
//
//	ptr->renderer->Resize(glm::vec2(_width, _height));
//
//	glViewport(0, 0, _width, _height);
//}
//
//void OGLSystem::CursorCallback(GLFWwindow* _window, double _xpos, double _ypos)
//{
//	OGLSystem* ptr = static_cast<OGLSystem*>(glfwGetWindowUserPointer(_window));
//	if (ptr->inputHandler)
//	{
//		ptr->inputHandler->CursorCallBackImpl(_window, _xpos, _ypos);
//	}
//}
//
//void OGLSystem::ScrollCallback(GLFWwindow* _window, double _xoffset, double _yoffset)
//{
//	OGLSystem* ptr = static_cast<OGLSystem*>(glfwGetWindowUserPointer(_window));
//	if (ptr->inputHandler)
//	{
//		ptr->inputHandler->ScrollCallBackImpl(_window, _xoffset, _yoffset);
//	}
//}
//
//void OGLSystem::KeyCallback(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods)
//{
//	OGLSystem* ptr = static_cast<OGLSystem*>(glfwGetWindowUserPointer(_window));
//	if (ptr->inputHandler)
//	{
//		ptr->inputHandler->KeyCallBackImpl(_window, _key, _scancode, _action, _mods);
//	}
//}
