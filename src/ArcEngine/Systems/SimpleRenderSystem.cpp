#include "SimpleRenderSystem.h"

// libs
#define GLM_FORCE_RADIANS
#define FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <stdexcept>
#include <array>
#include <filesystem>

namespace arc
{
	struct sSimplePushConstantData
	{
		glm::mat4 modelMatrix { 1.0f };
		glm::mat4 normalMatrix{ 1.0f };
		
	};

	simpleRenderSystem::simpleRenderSystem(cDevice& _device, VkRenderPass _render_pass, VkDescriptorSetLayout _global_descriptor_set_layout):
		arc_device{_device}
	{
		createPipelineLayout(_global_descriptor_set_layout);
		createPipeline(_render_pass);
	}

	simpleRenderSystem::~simpleRenderSystem()
	{
		vkDestroyPipelineLayout(arc_device.device(), pipeline_layout, nullptr);
	}

	void simpleRenderSystem::createPipelineLayout(VkDescriptorSetLayout _global_descriptor_set_layout)
	{

		VkPushConstantRange push_constant_range{};
		push_constant_range.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		push_constant_range.offset = 0;
		push_constant_range.size = sizeof(sSimplePushConstantData);

		std::vector<VkDescriptorSetLayout> descriptorSetLayouts{ _global_descriptor_set_layout };

		VkPipelineLayoutCreateInfo pipeline_layout_info{};
		pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipeline_layout_info.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
		pipeline_layout_info.pSetLayouts = descriptorSetLayouts.data();
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

		auto path = std::filesystem::current_path().string();
		std::replace(path.begin(), path.end(), '\\', '/');

		pipelineConfigInfo pipeline_config{};
		arcPipeline::defaultPipelineConfigInfo(pipeline_config);
		arcPipeline::enableAlphaBlending(pipeline_config);

		pipeline_config.render_pass = _render_pass;
		pipeline_config.pipeline_layout = pipeline_layout;
		arc_pipeline = std::make_unique<arcPipeline>(arc_device, pipeline_config, 
			path + "/src/Shaders/compiled_shaders/SimpleShader.vert.spv",
			path + "/src/Shaders/compiled_shaders/SimpleShader.frag.spv");
	}


	void simpleRenderSystem::renderGameObjects(frameInfo& _info)
	{
		arcCamera& camera = _info.camera;
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

		for (auto& entity : _info.entities)
		{
			if (_info.coordinator.HasComponent<PointLightComponent>(entity))
				continue;

			auto& transformComponent = _info.coordinator.GetComponent<TransformComponent>(entity);

			auto& modelComponent     = _info.coordinator.GetComponent<ModelComponent>(entity);

			sSimplePushConstantData push{};
			push.modelMatrix  = transformComponent.GetMat4x4();
			push.normalMatrix = transformComponent.GetNormalMat4x4();

			vkCmdPushConstants(
				command_buffer,
				pipeline_layout,
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
				0,
				sizeof(sSimplePushConstantData),
				&push);

			modelComponent.model->bind(command_buffer);
			modelComponent.model->draw(command_buffer);
		}
	}
}