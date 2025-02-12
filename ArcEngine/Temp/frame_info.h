#pragma once

#include "camera.h"
#include "game_object.h"

// lib
#include "vulkan/vulkan.h"

namespace arc
{
	struct frameInfo
	{
		int frame_index;
		float frame_time;
		VkCommandBuffer command_buffer;
		arcCamera& camera;
		VkDescriptorSet global_descriptor_set;
		arcGameObject::Map& game_objects;
	};
}