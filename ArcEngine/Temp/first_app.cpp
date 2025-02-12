#include "first_app.h"

#include "simple_render_system.h"
#include "camera.h"
#include "keyboard_movement_controller.h"
#include "Rendering/buffer.h"

#include "Util/texture.h"

// libs
#define GLM_FORCE_RADIANS
#define FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "filesystem"

// std
#include <stdexcept>
#include <array>
#include <chrono>

namespace arc
{

	struct GlobalUBO
	{
		glm::mat4 projectionView{ 1.0f };
		glm::vec3 lightDirection = glm::normalize(glm::vec3(1.0f, -3.0f, -1.0f));
	};

	cFirstApp::cFirstApp()
	{
		loadGameObjects();
	}

	cFirstApp::~cFirstApp()
	{
	}

	void arc::cFirstApp::run()
	{
		std::vector<std::unique_ptr<cBuffer>> uboBuffers(arcSwapChain::MAX_FRAMES_IN_FLIGHT);
		for (int i = 0; i < uboBuffers.size(); i++)
		{
			uboBuffers[i] = std::make_unique<cBuffer>(
				arc_device,
				sizeof(GlobalUBO),
				1,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
			);
			uboBuffers[i]->map();
		}


		simpleRenderSystem simple_render_system{ arc_device, arc_renderer.getSwapChainRenderPass() };
		arcCamera camera{};

		auto viewObject = arcGameObject::createGameObject();
		KeyboardMovementController cameraController{};

		auto current_time = std::chrono::high_resolution_clock::now();

		while (!arc_window.shouldClose())
		{
			glfwPollEvents();

			auto new_time = std::chrono::high_resolution_clock::now();
			float frame_time = std::chrono::duration<float, std::chrono::seconds::period>(new_time - current_time).count();
			current_time = new_time;

			cameraController.moveInPlaneXZ(arc_window.getGLFWWindow(), frame_time, viewObject);

			camera.setViewYXZ(viewObject.transform.translation, viewObject.transform.rotation);

			float aspect = arc_renderer.getAspectRatio();
			//camera.setOrthographicProjection(-aspect, aspect, -1, 1, -1, 1);
			camera.setPerspectiveProjection(glm::radians(45.0f), aspect, 0.1f, 10.0f);

			if (auto command_buffer = arc_renderer.beginFrame())
			{
				int frame_index = arc_renderer.getFrameIndex();

				frameInfo frame_info{ frame_index, frame_time, command_buffer, camera };

				// Update
				GlobalUBO ubo{};
				ubo.projectionView = camera.getProjectionMatrix() * camera.getViewMatrix();
				uboBuffers[frame_index]->writeToBuffer(&ubo);
				uboBuffers[frame_index]->flush();

				// Render
				arc_renderer.beginSwapChainRenderPass(command_buffer);
				simple_render_system.renderGameObjects(frame_info, arc_game_objects);
				arc_renderer.endSwapChainRenderPass(command_buffer);
				arc_renderer.endFrame();
			}
		}

		vkDeviceWaitIdle(arc_device.device());
	}

	void cFirstApp::loadGameObjects()
	{
		std::shared_ptr<arcModel> arc_model = arcModel::createOBJModelFromFile(arc_device, "ArcEngine/Models/smooth_vase.obj");

		auto gameObj = arcGameObject::createGameObject();
		gameObj.model = arc_model;
		gameObj.transform.translation = { 0.0f, 0.0f, 2.5f };
		gameObj.transform.scale = glm::vec3{ 2.0f, 3.0f, 2.0f };

		arc_game_objects.push_back(std::move(gameObj));

		std::shared_ptr<arcModel> arc_model1 = arcModel::createGLTFModelFromFile(arc_device, "ArcEngine\\Models\\Rat.glb");

		/*auto test2 = std::filesystem::current_path().string();
		auto path2 = test2 + "\\" + "ArcEngine\\Models\\T_Rat_BC.png";*/

		auto gameObj1 = arcGameObject::createGameObject();
		gameObj1.model = arc_model1;
		gameObj1.transform.translation = { 2.0f, 0.0f, 2.5f };
		gameObj1.transform.scale = glm::vec3{ 0.05f,  0.05f,  0.05f };
		//gameObj1.texture = std::make_shared<cTexture>(arc_device, path2);

		arc_game_objects.push_back(std::move(gameObj1));
		
	}
}