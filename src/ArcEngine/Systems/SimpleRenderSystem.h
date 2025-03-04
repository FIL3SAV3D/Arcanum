#pragma once

#include "window/device.h"
#include "rendering/pipeline.h"
#include "temp/GameObject.h"
#include "temp/camera.h"

#include "temp/FrameInfo.h"

// std
#include <memory>
#include <vector>

namespace arc
{
	class simpleRenderSystem
	{
	public:
		 simpleRenderSystem(arcDevice& _device, VkRenderPass _render_pass, VkDescriptorSetLayout _global_descriptor_set_layout);
		~simpleRenderSystem();

		simpleRenderSystem(const simpleRenderSystem&) = delete;
		simpleRenderSystem& operator=(const simpleRenderSystem&) = delete;

		void renderGameObjects(frameInfo& _info);


	private:
		void createPipelineLayout(VkDescriptorSetLayout _global_descriptor_set_layout);
		void createPipeline(VkRenderPass _render_pass);

		arcDevice& arc_device;

		std::unique_ptr<arcPipeline> arc_pipeline;
		VkPipelineLayout pipeline_layout;
	};
}