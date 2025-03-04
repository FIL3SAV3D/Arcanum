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

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"

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
			.setMaxSets(arcSwapChain::MAX_FRAMES_IN_FLIGHT * 2)
			.setPoolFlags(VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT)
			.addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, arcSwapChain::MAX_FRAMES_IN_FLIGHT * 2)
			.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, arcSwapChain::MAX_FRAMES_IN_FLIGHT * 2)
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

		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	void setupImGuiStyle()
	{
		ImGuiStyle& style = ImGui::GetStyle();

		style.Alpha = 1.0f;
		style.DisabledAlpha = 0.6000000238418579f;
		style.WindowPadding = ImVec2(8.0f, 8.0f);
		style.WindowRounding = 0.0f;
		style.WindowBorderSize = 1.0f;
		style.WindowMinSize = ImVec2(32.0f, 32.0f);
		style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
		style.WindowMenuButtonPosition = ImGuiDir_Left;
		style.ChildRounding = 0.0f;
		style.ChildBorderSize = 1.0f;
		style.PopupRounding = 0.0f;
		style.PopupBorderSize = 1.0f;
		style.FramePadding = ImVec2(4.0f, 3.0f);
		style.FrameRounding = 0.0f;
		style.FrameBorderSize = 1.0f;
		style.ItemSpacing = ImVec2(8.0f, 4.0f);
		style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
		style.CellPadding = ImVec2(4.0f, 2.0f);
		style.IndentSpacing = 21.0f;
		style.ColumnsMinSpacing = 6.0f;
		style.ScrollbarSize = 14.0f;
		style.ScrollbarRounding = 0.0f;
		style.GrabMinSize = 10.0f;
		style.GrabRounding = 0.0f;
		style.TabRounding = 0.0f;
		style.TabBorderSize = 1.0f;
		style.TabCloseButtonMinWidthSelected = 0.0f;
		style.TabCloseButtonMinWidthUnselected = 0.0f;
		style.ColorButtonPosition = ImGuiDir_Right;
		style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
		style.SelectableTextAlign = ImVec2(0.0f, 0.699999988079071f);

		style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 0.9999899864196777f, 0.9999899864196777f, 1.0f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.6000000238418579f, 0.6000000238418579f, 0.6000000238418579f, 1.0f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.2980392277240753f, 0.3450980484485626f, 0.2666666805744171f, 1.0f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.2431372553110123f, 0.2745098173618317f, 0.2156862765550613f, 1.0f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.2980392277240753f, 0.3450980484485626f, 0.2666666805744171f, 1.0f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.501960813999176f, 0.501960813999176f, 0.501960813999176f, 1.0f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.1568627506494522f, 0.1803921610116959f, 0.1333333402872086f, 1.0f);

		style.Colors[ImGuiCol_DockingPreview] = ImVec4(0.5882353186607361f, 0.5372549295425415f, 0.1882352977991104f, 1.0f);
		style.Colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.5882353186607361f, 0.5372549295425415f, 0.1882352977991104f, 1.0f);

		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.2431372553110123f, 0.2745098173618317f, 0.2156862765550613f, 1.0f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.5882353186607361f, 0.5372549295425415f, 0.1882352977991104f, 1.0f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.2470588237047195f, 0.2745098173618317f, 0.2196078449487686f, 1.0f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.2980392277240753f, 0.3450980484485626f, 0.2666666805744171f, 1.0f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.2980392277240753f, 0.3450980484485626f, 0.2666666805744171f, 1.0f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.0f, 1.0f, 1.0f, 0.5099999904632568f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.2980392277240753f, 0.3450980484485626f, 0.2666666805744171f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.2980392277240753f, 0.3450980484485626f, 0.2666666805744171f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.686274528503418f, 0.686274528503418f, 0.686274528503418f, 0.800000011920929f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.4862745106220245f, 0.4862745106220245f, 0.4862745106220245f, 0.800000011920929f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.4862745106220245f, 0.4862745106220245f, 0.4862745106220245f, 1.0f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.9999899864196777f, 0.9999945759773254f, 1.0f, 1.0f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.772549033164978f, 0.7137255072593689f, 0.3137255012989044f, 1.0f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.772549033164978f, 0.7137255072593689f, 0.3137255012989044f, 1.0f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.2980392277240753f, 0.3450980484485626f, 0.2666666805744171f, 1.0f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.5882353186607361f, 0.5372549295425415f, 0.1882352977991104f, 1.0f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.2431372553110123f, 0.2745098173618317f, 0.2156862765550613f, 1.0f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.2980392277240753f, 0.3450980484485626f, 0.2666666805744171f, 1.0f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.5882353186607361f, 0.5372549295425415f, 0.1882352977991104f, 1.0f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.2431372553110123f, 0.2745098173618317f, 0.2156862765550613f, 1.0f);
		style.Colors[ImGuiCol_Separator] = ImVec4(1.0f, 0.0f, 0.0f, 0.6200000047683716f);
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.5882353186607361f, 0.5372549295425415f, 0.1882352977991104f, 1.0f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.2431372553110123f, 0.2745098173618317f, 0.2156862765550613f, 1.0f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.529411792755127f, 0.5647059082984924f, 0.4980392158031464f, 1.0f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.5882353186607361f, 0.5372549295425415f, 0.1882352977991104f, 1.0f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.2431372553110123f, 0.2745098173618317f, 0.2156862765550613f, 1.0f);
		style.Colors[ImGuiCol_Tab] = ImVec4(0.2980392277240753f, 0.3450980484485626f, 0.2666666805744171f, 1.0f);
		style.Colors[ImGuiCol_TabHovered] = ImVec4(0.5882353186607361f, 0.5372549295425415f, 0.1882352977991104f, 1.0f);
		style.Colors[ImGuiCol_TabActive] = ImVec4(0.2431372553110123f, 0.2745098173618317f, 0.2156862765550613f, 1.0f);
		style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.2980392277240753f, 0.3450980484485626f, 0.2666666805744171f, 1.0f);
		style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.2431372553110123f, 0.2745098173618317f, 0.2156862765550613f, 1.0f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.5803921818733215f, 0.5372549295425415f, 0.196078434586525f, 1.0f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.7686274647712708f, 0.7098039388656616f, 0.3137255012989044f, 1.0f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.5803921818733215f, 0.5372549295425415f, 0.196078434586525f, 1.0f);
		style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.2431372553110123f, 0.2745098173618317f, 0.2156862765550613f, 1.0f);
		style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.1568627506494522f, 0.1803921610116959f, 0.1333333402872086f, 1.0f);
		style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.1568627506494522f, 0.1803921610116959f, 0.1333333402872086f, 1.0f);
		style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.2431372553110123f, 0.2745098173618317f, 0.2156862765550613f, 0.3476395010948181f);
		style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 0.9999899864196777f, 0.9999899864196777f, 0.07725322246551514f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.7686274647712708f, 0.7098039388656616f, 0.3137255012989044f, 1.0f);
		style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.949999988079071f);
		style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.800000011920929f);
		style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.6980392336845398f, 0.6980392336845398f, 0.6980392336845398f, 0.699999988079071f);
		style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2000000029802322f, 0.2000000029802322f);
		style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2000000029802322f, 0.3499999940395355f);
	}
	void cFirstApp::InitImGui()
	{
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

		setupImGuiStyle();

		ImGui_ImplGlfw_InitForVulkan(arc_window.getGLFWWindow(), true);
		ImGui_ImplVulkan_InitInfo ImGuiInfo{};
		ImGuiInfo.DescriptorPool = global_pool->GetDescriptorPool();
		ImGuiInfo.RenderPass = ArcRenderer.getSwapChainRenderPass();
		ImGuiInfo.Device = arc_device.device();
		ImGuiInfo.PhysicalDevice = arc_device.GetPhysicalDevice();
		ImGuiInfo.Instance = arc_device.GetInstance();
		ImGuiInfo.Queue = arc_device.graphicsQueue();
		ImGuiInfo.QueueFamily = arc_device.findPhysicalQueueFamilies().graphicsFamily;
		ImGuiInfo.ImageCount = arcSwapChain::MAX_FRAMES_IN_FLIGHT;
		ImGuiInfo.MinImageCount = 2;
		ImGuiInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

		ImGui_ImplVulkan_Init(&ImGuiInfo);
		ImGui_ImplVulkan_CreateFontsTexture();
		ImGui_ImplVulkan_DestroyFontsTexture();
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

		simpleRenderSystem simple_render_system{ arc_device, ArcRenderer.getSwapChainRenderPass(), global_set_layout->getDescriptorSetLayout() };
		cInfiniteGridRenderSystem inf_grid_system{ arc_device, ArcRenderer.getSwapChainRenderPass(), global_set_layout->getDescriptorSetLayout() };
		cPointLightSystem point_light_system   { arc_device, ArcRenderer.getSwapChainRenderPass(), global_set_layout->getDescriptorSetLayout()};
		arcCamera camera{};

		auto viewObject = arcGameObject::createGameObject();
		KeyboardMovementController cameraController{};

		auto current_time = std::chrono::high_resolution_clock::now();

		JPH::TempAllocatorImpl temp_allocator( 4048 * size_t(1024 * 1024));
		JPH::JobSystemThreadPool job_system(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, JPH::thread::hardware_concurrency() - 1);
		physics_system.OptimizeBroadPhase();
		
		InitImGui();

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

			float aspect = ArcRenderer.getAspectRatio();
			//camera.setOrthographicProjection(-aspect, aspect, -1, 1, -10, 1000.0f);
			camera.setPerspectiveProjection(glm::radians(45.0f), aspect, 0.1f, 1000.0f);
			/*game_objects.at(0).transform.rotation.y += 1.0f * frame_time;*/

			if (auto command_buffer = ArcRenderer.beginFrame())
			{
				int frame_index = ArcRenderer.getFrameIndex();

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
				ArcRenderer.beginSwapChainRenderPass(command_buffer);

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

				ImGui_ImplVulkan_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();

				ImGui::ShowDemoWindow();

				ImGui::Render();
				ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), command_buffer);

				ArcRenderer.endSwapChainRenderPass(command_buffer);
				ArcRenderer.endFrame();
			}
		}

		vkDeviceWaitIdle(arc_device.device());
	}

	void cFirstApp::loadGameObjects()
	{
		printf("FirstApp loadGameObjects\n");

		entities.insert(entities.begin(), MAX_ENTITIES - 1, 0);

		auto path = std::filesystem::current_path().string();
		//std::replace(path.begin(), path.end(), '\\', '/');

		std::shared_ptr<arcModel> rat_model = arcModel::createGLTFModelFromFile(arc_device, path + "\\src\\ArcEngine\\Models\\glb_models\\Rat.glb");

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
		auto path2 = test2 + "\\src\\ArcEngine\\Models\\src_images\\T_Rat_BC.png";
		auto path_normal = test2 + "\\src\\ArcEngine\\Models\\src_images\\T_Rat_N.png";
		auto path_orm = test2 + "\\src\\ArcEngine\\Models\\src_images\\T_Rat_ORM.png";

		tex = new cTexture{ arc_device, path2 };
		tex_normal = new cTexture{ arc_device, path_normal };
		tex_ORM = new cTexture{ arc_device, path_orm };
	}
}