#include "point_light_system.h"

// libs
#define GLM_FORCE_RADIANS
#define FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <stdexcept>
#include <array>
#include <map>

namespace arc
{
	struct PointLightPushConstants
	{
		glm::vec4 position{ 0.0f }; // Ignore w
		glm::vec4 color{ 1.0f }; // w is intensity
		float radius = 1.0f;
	};

	cPointLightSystem::cPointLightSystem(cDevice& _device, VkRenderPass _render_pass, VkDescriptorSetLayout _global_descriptor_set_layout):
		arc_device{_device}
	{
		createPipelineLayout(_global_descriptor_set_layout);
		createPipeline(_render_pass);
	}

	cPointLightSystem::~cPointLightSystem()
	{
		vkDestroyPipelineLayout(arc_device.device(), pipeline_layout, nullptr);
	}

	void cPointLightSystem::createPipelineLayout(VkDescriptorSetLayout _global_descriptor_set_layout)
	{

		VkPushConstantRange push_constant_range{};
		push_constant_range.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		push_constant_range.offset = 0;
		push_constant_range.size = sizeof(PointLightPushConstants);

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

	void cPointLightSystem::createPipeline(VkRenderPass _render_pass)
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
			"F:/Arcanum/src/Shaders/compiled_shaders/point_light_shader.vert.spv", 
			"F:/Arcanum/src/Shaders/compiled_shaders/point_light_shader.frag.spv");
	}


	void cPointLightSystem::update(frameInfo& _info, GlobalUBO& ubo)
	{
		auto rotateLight = glm::rotate(glm::mat4(1.0f), _info.frame_time, { 0.0f, -1.0f, 0.0f });
		int lightIndex = 0;
		for (auto& kv : _info.game_objects)
		{
			auto& obj = kv.second;

			if (obj.point_light == nullptr)
				continue;

			assert(lightIndex < MAX_LIGHTS && "Exceeded max lights limit");

			// update light position
			obj.transform.translation = glm::vec3(rotateLight * glm::vec4(obj.transform.translation, 1.0f));

			// Copy Light to ubo
			ubo.pointLights[lightIndex].position = glm::vec4(obj.transform.translation, 1.0f);
			ubo.pointLights[lightIndex].color = glm::vec4(obj.color, obj.point_light->light_intensity);
			lightIndex += 1;
		}
		ubo.numLights = lightIndex;
	}

	void cPointLightSystem::render(frameInfo& _info)
	{
		// Sort Lights
		std::map<float, arcGameObject::id_t> sorted;
		for (auto& kv : _info.game_objects)
		{
			auto& obj = kv.second;
			if (obj.point_light == nullptr)
				continue;

			auto offset = _info.camera.getCameraPos() - obj.transform.translation;
			float distSquared = glm::dot(offset, offset);
			sorted[distSquared] = obj.getId();
		}

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

		for (auto it = sorted.rbegin(); it != sorted.rend(); ++it)
		{
			auto& obj = _info.game_objects.at(it->second);

			PointLightPushConstants push{};
			push.position = glm::vec4(obj.transform.translation, 1.0f);
			push.color = glm::vec4(obj.color, obj.point_light->light_intensity);
			push.radius = obj.transform.scale.x;

			vkCmdPushConstants(command_buffer, pipeline_layout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PointLightPushConstants), &push);

			vkCmdDraw(_info.command_buffer, 6, 1, 0, 0);
		}

	}
}