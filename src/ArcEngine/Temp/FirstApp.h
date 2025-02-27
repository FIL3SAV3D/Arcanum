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


namespace arc
{
	class cFirstApp
	{
	public:
		static constexpr int WIDTH  = 800;
		static constexpr int HEIGHT = 600;

		 cFirstApp();
		~cFirstApp();

		cFirstApp(const cFirstApp&) = delete;
		cFirstApp& operator=(const cFirstApp&) = delete;

		void run();

	private:
		void loadGameObjects();

		arcWindow arc_window{WIDTH, HEIGHT, "ARCANUM"};
		cDevice arc_device{ arc_window };
		arcRenderer arc_renderer{ arc_window, arc_device };

		std::unique_ptr<cDescriptorPool> global_pool{};
		//arcGameObject::Map game_objects;
		cTexture* tex;
		cTexture* tex_normal;
		cTexture* tex_ORM;

		Coordinator coordinator;

		std::vector<Entity> entities{};
		Entity CameraEntity;
	};
}