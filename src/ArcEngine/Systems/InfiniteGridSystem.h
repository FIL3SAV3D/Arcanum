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
	class cInfiniteGridRenderSystem
	{
	public:
		 cInfiniteGridRenderSystem(cDevice& _device, VkRenderPass _render_pass, VkDescriptorSetLayout _global_descriptor_set_layout);
		~cInfiniteGridRenderSystem();

		cInfiniteGridRenderSystem(const cInfiniteGridRenderSystem&) = delete;
		cInfiniteGridRenderSystem& operator=(const cInfiniteGridRenderSystem&) = delete;

		void renderGrid(frameInfo& _info);


	private:
		void createPipelineLayout(VkDescriptorSetLayout _global_descriptor_set_layout);
		void createPipeline(VkRenderPass _render_pass);

		cDevice& arc_device;

		std::unique_ptr<arcPipeline> arc_pipeline;
		VkPipelineLayout pipeline_layout;
	};
}