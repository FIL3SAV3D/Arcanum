#pragma once

#include "Window/device.h"
#include "Window/Window.h"

#include "Rendering/renderer.h"
//#include "temp/GameObject.h"

#include "temp/descriptors.h"
#include "Util/texture.h"

#include "EntityComponentSystem/Coordinator.h"

// std
#include <memory>
#include <vector>

#include "LiveCollabTemp/User.h"
#include <map>
#include <Server/Server.h>


namespace arc
{
	class cFirstApp
	{
	public:
		static constexpr int WIDTH  = 1280;
		static constexpr int HEIGHT = 720;

		 cFirstApp();
		~cFirstApp();

		void InitImGui();

		cFirstApp(const cFirstApp&) = delete;
		cFirstApp& operator=(const cFirstApp&) = delete;

		void run();

		void operator() ()
		{
			run();
		}

	private:
		void loadGameObjects();

		bool InFocus = true;
		bool firstpress = false;
		float cooldown = 1.0f;
		float count = 0.0f;

		arcWindow arc_window{WIDTH, HEIGHT, "ARCANUM"};
		arcDevice arc_device{ arc_window };
		arcRenderer ArcRenderer{ arc_window, arc_device };

		std::unique_ptr<cDescriptorPool> global_pool{};
		
		cTexture* tex;
		cTexture* tex_normal;
		cTexture* tex_ORM;

		Coordinator coordinator;

		JPH::PhysicsSystem physics_system;
		BPLayerInterfaceImpl broad_phase_layer_interface;
		ObjectVsBroadPhaseLayerFilterImpl object_vs_broadphase_layer_filter;
		ObjectLayerPairFilterImpl object_vs_object_layer_filter;
		MyContactListener contact_listener;
		MyBodyActivationListener body_activation_listener;
		JPH::TempAllocatorImpl* temp_allocator;

		std::vector<Entity> entities{};
		std::map<uint32_t, User*> users{};
		Entity CameraEntity;

		std::shared_ptr<ServerInstance> ServerInst = nullptr;

		std::shared_ptr<arcModel> rat_model = nullptr;
	};
}