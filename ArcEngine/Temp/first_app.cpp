#include "first_app.h"

#include "camera.h"
#include "keyboard_movement_controller.h"
#include "Rendering/buffer.h"

#include "systems/simple_render_system.h"
#include "systems/point_light_system.h"


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
		glm::mat4 projection{ 1.0f };
		glm::mat4 view{ 1.0f };
		glm::vec4 ambientLightColor{ 1.0f, 1.0f, 1.0f, 0.02f }; // w is light intensity
		glm::vec3 lightPosition{ -1.0f };
		alignas(16) glm::vec4 lightColor{ 1.0f, 1.0f, 1.0f, 1.0f }; // w is light intensity
	};

	struct Texture
	{
		VkSampler sampler;
	};

	cFirstApp::cFirstApp()
	{
		global_pool = cDescriptorPool::Builder(arc_device)
			.setMaxSets(arcSwapChain::MAX_FRAMES_IN_FLIGHT)
			.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, arcSwapChain::MAX_FRAMES_IN_FLIGHT)
			.build();
		loadGameObjects();
	}

	cFirstApp::~cFirstApp()
	{
		delete tex;
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

		std::vector<std::unique_ptr<cBuffer>> uboImageBuffers(arcSwapChain::MAX_FRAMES_IN_FLIGHT);
		for (int i = 0; i < uboImageBuffers.size(); i++)
		{
			uboImageBuffers[i] = std::make_unique<cBuffer>(
				arc_device,
				sizeof(Texture),
				1,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
			);
			uboImageBuffers[i]->map();
		}

		auto global_set_layout = cDescriptorSetLayout::Builder(arc_device)
			.addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
			.addBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
			.addBinding(2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
			.build();

		VkDescriptorImageInfo info;
		info.imageLayout = tex->getImageLayout();
		info.imageView = tex->getImageView();
		info.sampler = tex->getSampler();

		VkDescriptorImageInfo info_normal;
		info_normal.imageLayout = tex_normal->getImageLayout();
		info_normal.imageView = tex_normal->getImageView();
		info_normal.sampler = tex_normal->getSampler();

		std::vector<VkDescriptorSet> globalDescriptorSets(arcSwapChain::MAX_FRAMES_IN_FLIGHT);
		for (int i = 0; i < globalDescriptorSets.size(); i++)
		{
			auto bufferInfo = uboBuffers[i]->descriptorInfo();
			cDescriptorWriter(*global_set_layout, *global_pool)
				.writeBuffer(0, &bufferInfo)
				.writeImage(1, &info)
				.writeImage(2, &info_normal)
				.build(globalDescriptorSets[i]);
		}


		simpleRenderSystem simple_render_system{ arc_device, arc_renderer.getSwapChainRenderPass(), global_set_layout->getDescriptorSetLayout() };
		cPointLightSystem point_light_system   { arc_device, arc_renderer.getSwapChainRenderPass(), global_set_layout->getDescriptorSetLayout()};
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
			//camera.setOrthographicProjection(-aspect, aspect, -1, 1, -10, 1000.0f);
			camera.setPerspectiveProjection(glm::radians(120.0f), aspect, 0.1f, 1000.0f);
			game_objects.at(0).transform.rotation.y += 1.0f * frame_time;

			if (auto command_buffer = arc_renderer.beginFrame())
			{
				int frame_index = arc_renderer.getFrameIndex();

				frameInfo frame_info{
					frame_index,
					frame_time,
					command_buffer,
					camera,
					globalDescriptorSets[frame_index],
					game_objects
				};

				// Update
				GlobalUBO ubo{};
				ubo.projection = camera.getProjectionMatrix();
				ubo.view = camera.getViewMatrix();
				uboBuffers[frame_index]->writeToBuffer(&ubo);
				uboBuffers[frame_index]->flush();

				Texture texture{};
				uboImageBuffers[frame_index]->writeToBuffer(&texture);
				uboImageBuffers[frame_index]->flush();

				// Render
				arc_renderer.beginSwapChainRenderPass(command_buffer);
				simple_render_system.renderGameObjects(frame_info);
				point_light_system.render(frame_info);
				arc_renderer.endSwapChainRenderPass(command_buffer);
				arc_renderer.endFrame();
			}
		}

		vkDeviceWaitIdle(arc_device.device());
	}

	void cFirstApp::loadGameObjects()
	{
		//std::shared_ptr<arcModel> arc_model = arcModel::createOBJModelFromFile(arc_device, "ArcEngine/Models/smooth_vase.obj");

		//auto gameObj = arcGameObject::createGameObject();
		//gameObj.model = arc_model;
		//gameObj.transform.translation = { 0.0f, 0.0f, 2.5f };
		//gameObj.transform.scale = glm::vec3{ 2.0f, 3.0f, 2.0f };

		//arc_game_objects.push_back(std::move(gameObj));
		//"E:\Arcanum\ArcEngine\Models\Source images\T_MIMIC_BC.png"
		auto test2 = std::filesystem::current_path().string();
		auto path2 = test2 + "\\" + "ArcEngine\\Models\\T_Rat_BC.png";
		auto path_normal = test2 + "\\" + "ArcEngine\\Models\\T_Rat_N.png";

		tex = new cTexture{ arc_device, path2 };
		tex_normal = new cTexture{ arc_device, path_normal };

		//std::shared_ptr<arcModel> rat_model = arcModel::createGLTFModelFromFile(arc_device, "ArcEngine\\Models\\MIMICAAAAH.glb");

		//auto rat01 = arcGameObject::createGameObject();
		//rat01.model = rat_model;
		//rat01.transform.translation = { 0.0f, 0.0f, 0.0f };
		//rat01.transform.scale = glm::vec3{ 0.05f,  0.05f,  0.05f };
		//rat01.transform.rotation = { 180.0f * (3.14/180), 0.0f, 0.0f};
		////gameObj1.texture = std::make_shared<cTexture>(arc_device, path2);

		//game_objects.emplace(rat01.getId(), std::move(rat01));

		std::shared_ptr<arcModel> rat_model = arcModel::createGLTFModelFromFile(arc_device, "ArcEngine\\Models\\Rat.glb");

		auto rat01 = arcGameObject::createGameObject();
		rat01.model = rat_model;
		rat01.transform.translation = { 0.0f, 0.0f, 0.0f };
		rat01.transform.scale = glm::vec3{ 0.05f,  0.05f,  0.05f };
		rat01.transform.rotation = { -90.0f, 0.0f, 0.0f};
		//gameObj1.texture = std::make_shared<cTexture>(arc_device, path2);

		game_objects.emplace(rat01.getId(), std::move(rat01));

		auto rat02 = arcGameObject::createGameObject();
		rat02.model = rat_model;
		rat02.transform.translation = { 1.0f, 0.0f, 0.0f };
		rat02.transform.scale = glm::vec3{ 0.05f,  0.05f,  0.05f };
		rat02.transform.rotation = { -90.0f, 0.0f, 0.0f };

		//gameObj1.texture = std::make_shared<cTexture>(arc_device, path2);

		game_objects.emplace(rat02.getId(), std::move(rat02));


		//std::shared_ptr<arcModel> rat_racer = arcModel::createGLTFModelFromFile(arc_device, "ArcEngine\\Models\\RatRacer.glb");

		//auto ratr = arcGameObject::createGameObject();
		//ratr.model = rat_racer;
		//ratr.transform.translation = { -1.0f, 0.0f, 0.0f };
		//ratr.transform.scale = glm::vec3{ 0.05f,  0.05f,  0.05f };
		//ratr.transform.rotation = { 0.0f, 0.0f, 0.0f };

		////gameObj1.texture = std::make_shared<cTexture>(arc_device, path2);

		//game_objects.emplace(ratr.getId(), std::move(ratr));


		std::shared_ptr<arcModel> quad_model = arcModel::createOBJModelFromFile(arc_device, "ArcEngine/Models/quad.obj");

		auto quad = arcGameObject::createGameObject();
		quad.model = quad_model;
		quad.transform.translation = { 0.0f, 0.2f, 0.0f };
		quad.transform.scale = glm::vec3{ 3.0f,  1.0f,  3.0f };
		//gameObj1.texture = std::make_shared<cTexture>(arc_device, path2);

		game_objects.emplace(quad.getId(), std::move(quad));
		
	}
}