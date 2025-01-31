#pragma once

#include "Window/device.h"
#include "Window/window.h"

#include "Rendering/renderer.h"
#include "game_object.h"

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
		std::vector<arcGameObject> arc_game_objects;
	};
}