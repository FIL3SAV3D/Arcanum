#pragma once

#include "window/device.h"
#include "rendering/pipeline.h"
#include "game_object.h"
#include "camera.h"

// std
#include <memory>
#include <vector>

namespace arc
{
	class simpleRenderSystem
	{
	public:
		 simpleRenderSystem(cDevice& _device, VkRenderPass _render_pass);
		~simpleRenderSystem();

		simpleRenderSystem(const simpleRenderSystem&) = delete;
		simpleRenderSystem& operator=(const simpleRenderSystem&) = delete;

		void renderGameObjects(VkCommandBuffer _command_buffer, std::vector<arcGameObject>& _game_objects, const arcCamera& _camera);


	private:
		void createPipelineLayout();
		void createPipeline(VkRenderPass _render_pass);

		cDevice& arc_device;

		std::unique_ptr<arcPipeline> arc_pipeline;
		VkPipelineLayout pipeline_layout;
	};
}