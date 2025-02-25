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
	class cPointLightSystem
	{
	public:
		 cPointLightSystem(cDevice& _device, VkRenderPass _render_pass, VkDescriptorSetLayout _global_descriptor_set_layout);
		~cPointLightSystem();

		cPointLightSystem(const cPointLightSystem&) = delete;
		cPointLightSystem& operator=(const cPointLightSystem&) = delete;

		void update(frameInfo& _info, GlobalUBO& ubo);
		void render(frameInfo& _info);


	private:
		void createPipelineLayout(VkDescriptorSetLayout _global_descriptor_set_layout);
		void createPipeline(VkRenderPass _render_pass);

		cDevice& arc_device;

		std::unique_ptr<arcPipeline> arc_pipeline;
		VkPipelineLayout pipeline_layout;
	};
}