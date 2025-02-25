#pragma once

#include "vulkan/vulkan_core.h"
#include "Window/Device.h"
#include "string"



namespace arc
{
	class cTexture
	{
	public:
		cTexture( cDevice& _device,  const std::string _file_path);
		~cTexture();

		cTexture(const cTexture&) = delete;
		cTexture& operator=(const cTexture&) = delete;
		cTexture(cTexture&&) = delete;
		cTexture& operator=(cTexture&&) = delete;

		VkSampler getSampler() { return sampler; }
		VkImageView getImageView() { return image_view; }
		VkImageLayout getImageLayout() { return image_layout; }
	private:
		void transitionImageLayout(VkImageLayout _old_layout, VkImageLayout _new_layout);

		cDevice& device;
		VkImage image;
		VkDeviceMemory image_memory;
		VkImageView image_view;
		VkSampler sampler;
		VkFormat image_format;
		VkImageLayout image_layout;
	};

}