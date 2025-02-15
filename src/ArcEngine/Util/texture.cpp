#include "texture.h"
#include "Rendering/buffer.h"

#include "stb_image.h"
#include "stdexcept"
#include "cassert"

namespace arc
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Public Section

	cTexture::cTexture(cDevice& _device, const std::string _file_path):
		device{ _device }
	{
		printf("Trying to load texture %s\n", _file_path.c_str());
		int width;
		int height;
		int channels;
		int bytes_per_pixel;

		stbi_uc* data = stbi_load(_file_path.c_str(), &width, &height, &bytes_per_pixel, 4);

		if (data == nullptr)
		{
			printf("File or filepath is not correct! %s\n", _file_path.c_str());
			throw std::runtime_error("File or filepath is not correct!");
		}

		cBuffer staging_buffer
		{
			device,
			4,
			static_cast<uint32_t>(width * height),
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		};

		staging_buffer.map();
		staging_buffer.writeToBuffer(data);

		image_format = VK_FORMAT_R8G8B8A8_SRGB;

		VkImageCreateInfo image_info{};
		image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		image_info.imageType = VK_IMAGE_TYPE_2D;
		image_info.format = image_format;
		image_info.mipLevels = 1;
		image_info.arrayLayers = 1;
		image_info.samples = VK_SAMPLE_COUNT_1_BIT;
		image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
		image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		image_info.extent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1 };
		image_info.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		image_info.pNext = 0;

		device.createImageWithInfo(image_info, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, image, image_memory);

		transitionImageLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

		device.copyBufferToImage(staging_buffer.getBuffer(), image, static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1);

		transitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		image_layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		VkSamplerCreateInfo sampler_info{};
		sampler_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		sampler_info.magFilter = VK_FILTER_NEAREST;
		sampler_info.minFilter = VK_FILTER_NEAREST;
		sampler_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		sampler_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		sampler_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		sampler_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		sampler_info.mipLodBias = 0.0f;
		sampler_info.compareOp = VK_COMPARE_OP_NEVER;
		sampler_info.minLod = 0.0f;
		sampler_info.maxLod = static_cast<float>(image_info.mipLevels);
		sampler_info.maxAnisotropy = 1.0f;
		sampler_info.anisotropyEnable = VK_FALSE;
		sampler_info.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
		
		vkCreateSampler(device.device(), &sampler_info, nullptr, &sampler);

		VkImageViewCreateInfo image_view_info{};
		image_view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		image_view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		image_view_info.format = image_format;
		image_view_info.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
		image_view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		image_view_info.subresourceRange.baseMipLevel = 0;
		image_view_info.subresourceRange.baseArrayLayer = 0;
		image_view_info.subresourceRange.layerCount = 1;
		image_view_info.subresourceRange.levelCount = 1;
		image_view_info.image = image;

		vkCreateImageView(device.device(), &image_view_info, nullptr, &image_view);

		stbi_image_free(data);
	}

	cTexture::~cTexture()
	{
		vkDestroyImage(device.device(), image, nullptr);
		vkFreeMemory(device.device(), image_memory, nullptr);
		vkDestroyImageView(device.device(), image_view, nullptr);
		vkDestroySampler(device.device(), sampler, nullptr);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Private Section
	void cTexture::transitionImageLayout(VkImageLayout _old_layout, VkImageLayout _new_layout)
	{
		VkCommandBuffer command_buffer = device.beginSingleTimeCommands();
		
		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = _old_layout;
		barrier.newLayout = _new_layout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = image;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		VkPipelineStageFlags source_stage;
		VkPipelineStageFlags destination_stage;

		if (_old_layout == VK_IMAGE_LAYOUT_UNDEFINED && _new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
		{
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destination_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}

		else if (_old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && _new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
		{
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			source_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destination_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}

		else
		{
			throw std::runtime_error("Unsupported layout transition!");
		}

		vkCmdPipelineBarrier(command_buffer, source_stage, destination_stage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

		device.endSingleTimeCommands(command_buffer);
	}

}