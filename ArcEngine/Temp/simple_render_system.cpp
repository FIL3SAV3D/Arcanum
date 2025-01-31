#include "simple_render_system.h"

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
	struct sSimplePushConstantData
	{
		glm::mat4 transform  { 1.0f };
		glm::mat4 normalMatrix{ 1.0f };
		
	};

	simpleRenderSystem::simpleRenderSystem(cDevice& _device, VkRenderPass _render_pass):
		arc_device{_device}
	{
		createPipelineLayout();
		createPipeline(_render_pass);
	}

	simpleRenderSystem::~simpleRenderSystem()
	{
		vkDestroyPipelineLayout(arc_device.device(), pipeline_layout, nullptr);
	}

	void simpleRenderSystem::createPipelineLayout()
	{

		VkPushConstantRange push_constant_range{};
		push_constant_range.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		push_constant_range.offset = 0;
		push_constant_range.size = sizeof(sSimplePushConstantData);

		VkPipelineLayoutCreateInfo pipeline_layout_info{};
		pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipeline_layout_info.setLayoutCount = 0;
		pipeline_layout_info.pSetLayouts = nullptr;
		pipeline_layout_info.pushConstantRangeCount = 1;
		pipeline_layout_info.pPushConstantRanges = &push_constant_range;
		if (vkCreatePipelineLayout(arc_device.device(), &pipeline_layout_info, nullptr, &pipeline_layout) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create pipeline layout");
		}
	}

	void simpleRenderSystem::createPipeline(VkRenderPass _render_pass)
	{
		assert(pipeline_layout != nullptr && "Cannot create pipeline before pipeline layout");

		pipelineConfigInfo pipeline_config{};
		arcPipeline::defaultPipelineConfigInfo(pipeline_config);

		pipeline_config.render_pass = _render_pass;
		pipeline_config.pipeline_layout = pipeline_layout;
		arc_pipeline = std::make_unique<arcPipeline>(arc_device, pipeline_config, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv");
	}


	void simpleRenderSystem::renderGameObjects(VkCommandBuffer _command_buffer, std::vector<arcGameObject>& _game_objects, const arcCamera& _camera)
	{
		arc_pipeline->bind(_command_buffer);

		auto projection_view = _camera.getProjectionMatrix() * _camera.getViewMatrix();

		for (auto& obj : _game_objects)
		{
			sSimplePushConstantData push{};
			auto modelMatrix = obj.transform.mat4();
			push.transform   = projection_view * modelMatrix;
			push.normalMatrix = obj.transform.normalMatrix();

			vkCmdPushConstants(
				_command_buffer,
				pipeline_layout,
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
				0,
				sizeof(sSimplePushConstantData),
				&push);

			obj.model->bind(_command_buffer);
			obj.model->draw(_command_buffer);
		}
	}
}