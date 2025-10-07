#pragma once

#include "camera.h"
#include "GameObject.h"
#include "EntityComponentSystem/Coordinator.h"

// lib
#include "vulkan/vulkan.h"

namespace arc
{
#define MAX_LIGHTS 10

	struct PointLight
	{
		glm::vec4 position{ 0.0f }; // Ignore w
		glm::vec4 color{ 1.0f }; // w is intensity
	};

	struct GlobalUBO
	{
		glm::mat4 projection{ 1.0f };
		glm::mat4 view{ 1.0f };
		glm::mat4 inverseView{ 1.0f };
		glm::vec4 camPos{ 1.0 };
		glm::vec4 ambientLightColor{ 1.0f, 1.0f, 1.0f, 0.12f }; // w is light intensity
		PointLight pointLights[MAX_LIGHTS];
		int numLights;
	};

	struct frameInfo
	{
		int frame_index;
		float frame_time;
		VkCommandBuffer command_buffer;
		arcCamera& camera;
		VkDescriptorSet global_descriptor_set;
		std::vector<Entity> entities{};
		Coordinator& coordinator;
	};
}