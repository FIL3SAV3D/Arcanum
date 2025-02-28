#include "FirstApp.h"

#include "Temp/Camera.h"
#include "Temp/KeyboardMovementController.h"
#include "Rendering/Buffer.h"

#include "systems/SimpleRenderSystem.h"
#include "systems/InfiniteGridSystem.h"
#include "systems/PointLightSystem.h"

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

#include "Temp/JoltHelper.h"

#define JPH_USE_SSE4_2

namespace arc
{
	struct Texture
	{
		VkSampler sampler;
	};

	cFirstApp::cFirstApp()
	{
		printf("FirstApp Construct\n");


		global_pool = cDescriptorPool::Builder(arc_device)
			.setMaxSets(arcSwapChain::MAX_FRAMES_IN_FLIGHT)
			.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, arcSwapChain::MAX_FRAMES_IN_FLIGHT)
			.build();

		coordinator.Init();

		/// Jolt Testing Ground
		JPH::RegisterDefaultAllocator();

		JPH::Factory::sInstance = new JPH::Factory();

		JPH::RegisterTypes();

		

		const JPH::uint cMaxBodies = 8000000;

		const JPH::uint cNumBodyMutexes = 0;

		const JPH::uint cMaxBodyPairs = 4500000;

		const JPH::uint cMaxContactConstraints = 4500000;

		physics_system.Init(cMaxBodies, cNumBodyMutexes, cMaxBodyPairs, cMaxContactConstraints, broad_phase_layer_interface, object_vs_broadphase_layer_filter, object_vs_object_layer_filter);

		physics_system.SetBodyActivationListener(&body_activation_listener);

		physics_system.SetContactListener(&contact_listener);

		BodyInterface& BodyInterface = physics_system.GetBodyInterface();

		BoxShapeSettings floor_shape_settings(Vec3(100.0f, 1.0f, 100.0f));
		floor_shape_settings.SetEmbedded();

		ShapeSettings::ShapeResult floor_shape_result = floor_shape_settings.Create();
		ShapeRefC floor_shape = floor_shape_result.Get();

		BodyCreationSettings floor_settings(floor_shape, RVec3(0.0, 0.0, 0.0), Quat::sIdentity(), EMotionType::Static, Layers::NON_MOVING);

		BodyInterface.CreateAndAddBody(floor_settings, EActivation::DontActivate);

		physics_system.SetGravity({0.0f, 1.0f, 0.0f});
		/// Jolt Testing Ground

		loadGameObjects();
	}

	cFirstApp::~cFirstApp()
	{
		delete tex;
		delete tex_normal;
		delete tex_ORM;
	}

	void arc::cFirstApp::run()
	{
		

		printf("FirstApp Run\n");

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
			.addBinding(3, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
			.build();

		VkDescriptorImageInfo info;
		info.imageLayout = tex->getImageLayout();
		info.imageView = tex->getImageView();
		info.sampler = tex->getSampler();

		VkDescriptorImageInfo info_normal;
		info_normal.imageLayout = tex_normal->getImageLayout();
		info_normal.imageView = tex_normal->getImageView();
		info_normal.sampler = tex_normal->getSampler();

		VkDescriptorImageInfo info_ORM;
		info_ORM.imageLayout = tex_ORM->getImageLayout();
		info_ORM.imageView = tex_ORM->getImageView();
		info_ORM.sampler = tex_ORM->getSampler();

		std::vector<VkDescriptorSet> globalDescriptorSets(arcSwapChain::MAX_FRAMES_IN_FLIGHT);
		for (int i = 0; i < globalDescriptorSets.size(); i++)
		{
			auto bufferInfo = uboBuffers[i]->descriptorInfo();
			cDescriptorWriter(*global_set_layout, *global_pool)
				.writeBuffer(0, &bufferInfo)
				.writeImage(1, &info)
				.writeImage(2, &info_normal)
				.writeImage(3, &info_ORM)
				.build(globalDescriptorSets[i]);
		}

		simpleRenderSystem simple_render_system{ arc_device, arc_renderer.getSwapChainRenderPass(), global_set_layout->getDescriptorSetLayout() };
		cInfiniteGridRenderSystem inf_grid_system{ arc_device, arc_renderer.getSwapChainRenderPass(), global_set_layout->getDescriptorSetLayout() };
		cPointLightSystem point_light_system   { arc_device, arc_renderer.getSwapChainRenderPass(), global_set_layout->getDescriptorSetLayout()};
		arcCamera camera{};

		auto viewObject = arcGameObject::createGameObject();
		KeyboardMovementController cameraController{};

		auto current_time = std::chrono::high_resolution_clock::now();

		JPH::TempAllocatorImpl temp_allocator( 4048 * size_t(1024 * 1024));
		JPH::JobSystemThreadPool job_system(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, JPH::thread::hardware_concurrency() - 1);
		physics_system.OptimizeBroadPhase();

		while (!arc_window.shouldClose())
		{
			glfwPollEvents();

			if (glfwGetKey(arc_window.getGLFWWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
				break;

			auto new_time = std::chrono::high_resolution_clock::now();
			float frame_time = std::chrono::duration<float, std::chrono::seconds::period>(new_time - current_time).count();
			current_time = new_time;

			physics_system.Update(frame_time, 1, &temp_allocator, &job_system);

			cameraController.moveInPlaneXZ(arc_window.getGLFWWindow(), frame_time, coordinator.GetComponent<TransformComponent>(CameraEntity));

			camera.setViewYXZ(coordinator.GetComponent<TransformComponent>(CameraEntity).position, coordinator.GetComponent<TransformComponent>(CameraEntity).rotation);

			float aspect = arc_renderer.getAspectRatio();
			//camera.setOrthographicProjection(-aspect, aspect, -1, 1, -10, 1000.0f);
			camera.setPerspectiveProjection(glm::radians(45.0f), aspect, 0.1f, 1000.0f);
			/*game_objects.at(0).transform.rotation.y += 1.0f * frame_time;*/

			if (auto command_buffer = arc_renderer.beginFrame())
			{
				int frame_index = arc_renderer.getFrameIndex();

				frameInfo frame_info{
					frame_index,
					frame_time,
					command_buffer,
					camera,
					globalDescriptorSets[frame_index],
					entities,
					coordinator
				};
				
				auto& BInter = physics_system.GetBodyInterface();
				for (auto& entity : entities)
				{
					if (!coordinator.HasComponent<PhysicsComponent>(entity))
						continue;
					auto& Pcomp = coordinator.GetComponent<PhysicsComponent>(entity);
					auto& Tcomp = coordinator.GetComponent<TransformComponent>(entity);
					JPH::RVec3 pos{};
					JPH::Quat quat{};
					JPH::RMat44 Transform = BInter.GetCenterOfMassTransform(Pcomp.CollisionID);
					JPH::RMat44 rotation = Transform.GetRotation();
					
					JPH::RVec3 rot = rotation.GetQuaternion().GetEulerAngles();
					Tcomp.position = { Transform.GetTranslation().GetX(), Transform.GetTranslation().GetY(), Transform.GetTranslation().GetZ() };
					Tcomp.rotation = { rot.GetX(), rot.GetY(), rot.GetZ() };
				}
				
				// Update
				GlobalUBO ubo{};
				ubo.projection = camera.getProjectionMatrix();
				ubo.view = camera.getViewMatrix();
				ubo.inverseView = camera.getInverseViewMatrix();
				ubo.camPos = glm::vec4(camera.getCameraPos(), 1.0f);
				point_light_system.update(frame_info, ubo);
				uboBuffers[frame_index]->writeToBuffer(&ubo);
				uboBuffers[frame_index]->flush();

				Texture texture{};
				uboImageBuffers[frame_index]->writeToBuffer(&texture);
				uboImageBuffers[frame_index]->flush();

				// Render
				arc_renderer.beginSwapChainRenderPass(command_buffer);

				// order matters
				simple_render_system.renderGameObjects(frame_info);

				// VERY simple way of doing this
				/// TODO: make a proper alpha sorting system
				if (camera.getCameraPos().y > 0.0)
				{
					point_light_system.render(frame_info);
					inf_grid_system.renderGrid(frame_info);
				}
				else
				{
					inf_grid_system.renderGrid(frame_info);
					point_light_system.render(frame_info);
				}

				arc_renderer.endSwapChainRenderPass(command_buffer);
				arc_renderer.endFrame();
			}
		}

		vkDeviceWaitIdle(arc_device.device());
	}

	void cFirstApp::loadGameObjects()
	{
		printf("FirstApp loadGameObjects\n");

		entities.insert(entities.begin(), MAX_ENTITIES - 1, 0);

		std::shared_ptr<arcModel> rat_model = arcModel::createGLTFModelFromFile(arc_device, "E:\\Arcanum\\src\\ArcEngine\\Models\\glb_models\\Rat.glb");

		std::default_random_engine generator;
		std::uniform_real_distribution<float> randPosition(-50.0f, 50.0f);
		std::uniform_real_distribution<float> randHeight(5.0f, 20.0f);
		std::uniform_real_distribution<float> randRotation(0.0f, 3.0f);
		std::uniform_real_distribution<float> randScale(0.1f, 0.5f);

		//{randScale(generator), randScale(generator), randScale(generator)}
		for (auto& entity : entities)
		{
			entity = coordinator.CreateEntity();

			TransformComponent TComp{
				{randPosition(generator), -randHeight(generator), randPosition(generator)},
				{randRotation(generator), randRotation(generator), randRotation(generator)},
				{0.1f, 0.1f, 0.1f}
			};
			coordinator.AddComponent(
				entity, 
				TComp
			);

			coordinator.AddComponent(entity, ModelComponent{ rat_model });

			PhysicsComponent pComp{};
			pComp.CreateCollision(physics_system, rat_model->JPHVertArray);
			coordinator.AddComponent(entity, pComp);
			
			JPH::Quat quat{};
			quat = quat.sEulerAngles(RVec3{ TComp.rotation.x, TComp.rotation.y, TComp.rotation.z });
			quat = quat.Normalized();
			physics_system.GetBodyInterface().SetPositionAndRotation(
				pComp.CollisionID, 
				{ TComp.position.x, TComp.position.y, TComp.position.z }, 
				quat,
				EActivation::Activate);
			//physics_system.GetBodyInterface().SetLinearVelocity(pComp.CollisionID, Vec3(0.0f, 5.0f, 0.0f));
		}

		CameraEntity = coordinator.CreateEntity();
		coordinator.AddComponent(
			CameraEntity,
			TransformComponent{}
		);

		coordinator.AddComponent(
			CameraEntity,
			CameraComponent{}
		);

		auto test2 = std::filesystem::current_path().string();
		auto path2 = "E:\\Arcanum\\src\\ArcEngine\\Models\\src_images\\T_Rat_BC.png";
		auto path_normal = "E:\\Arcanum\\src/ArcEngine\\Models\\src_images\\T_Rat_N.png";
		auto path_orm = "E:\\Arcanum\\src/ArcEngine\\Models\\src_images\\T_Rat_ORM.png";

		tex = new cTexture{ arc_device, path2 };
		tex_normal = new cTexture{ arc_device, path_normal };
		tex_ORM = new cTexture{ arc_device, path_orm };
	}
}