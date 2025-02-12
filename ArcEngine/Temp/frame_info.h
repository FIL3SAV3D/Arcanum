#pragma once

#include "camera.h"

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
	};
}