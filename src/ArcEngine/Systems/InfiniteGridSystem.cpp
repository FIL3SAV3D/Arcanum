#include "InfiniteGridSystem.h"

// libs
#define GLM_FORCE_RADIANS
#define FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <stdexcept>
#include <array>

namespace arc
{
	//struct sSimplePushConstantData
	//{
	//	glm::mat4 mdoelMatrix  { 1.0f };
	//	glm::mat4 normalMatrix{ 1.0f };
	//	
	//};

	cInfiniteGridRenderSystem::cInfiniteGridRenderSystem(cDevice& _device, VkRenderPass _render_pass, VkDescriptorSetLayout _global_descriptor_set_layout):
		arc_device{_device}
	{
		createPipelineLayout(_global_descriptor_set_layout);
		createPipeline(_render_pass);
	}

	cInfiniteGridRenderSystem::~cInfiniteGridRenderSystem()
	{
		vkDestroyPipelineLayout(arc_device.device(), pipeline_layout, nullptr);
	}

	void cInfiniteGridRenderSystem::createPipelineLayout(VkDescriptorSetLayout _global_descriptor_set_layout)
	{

		//VkPushConstantRange push_constant_range{};
		//push_constant_range.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		//push_constant_range.offset = 0;
		//push_constant_range.size = sizeof(sSimplePushConstantData);

		std::vector<VkDescriptorSetLayout> descriptorSetLayouts{ _global_descriptor_set_layout };

		VkPipelineLayoutCreateInfo pipeline_layout_info{};
		pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipeline_layout_info.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
		pipeline_layout_info.pSetLayouts = descriptorSetLayouts.data();
		pipeline_layout_info.pushConstantRangeCount = 0;
		pipeline_layout_info.pPushConstantRanges = nullptr;
		if (vkCreatePipelineLayout(arc_device.device(), &pipeline_layout_info, nullptr, &pipeline_layout) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create pipeline layout");
		}
	}

	void cInfiniteGridRenderSystem::createPipeline(VkRenderPass _render_pass)
	{
		assert(pipeline_layout != nullptr && "Cannot create pipeline before pipeline layout");

		pipelineConfigInfo pipeline_config{};
		arcPipeline::defaultPipelineConfigInfo(pipeline_config);
		arcPipeline::enableAlphaBlending(pipeline_config);
		pipeline_config.bindingDescriptions.clear();
		pipeline_config.attributeDescriptions.clear();
		pipeline_config.render_pass = _render_pass;
		pipeline_config.pipeline_layout = pipeline_layout;
		arc_pipeline = std::make_unique<arcPipeline>(
			arc_device,
			pipeline_config,
			"E:/Arcanum/src/Shaders/compiled_shaders/InfGridShader.vert.spv",
			"E:/Arcanum/src/Shaders/compiled_shaders/InfGridShader.frag.spv"
		);
	}


	void cInfiniteGridRenderSystem::renderGrid(frameInfo& _info)
	{
		VkCommandBuffer command_buffer = _info.command_buffer;

		arc_pipeline->bind(command_buffer);

		vkCmdBindDescriptorSets(
			command_buffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			pipeline_layout,
			0, 
			1,
			&_info.global_descriptor_set,
			0, 
			nullptr
		);

		vkCmdDraw(_info.command_buffer, 6, 1, 0, 0);
	}
}