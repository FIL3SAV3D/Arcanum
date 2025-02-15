#include "renderer.h"

// std
#include <stdexcept>
#include <array>

namespace arc
{
	arcRenderer::arcRenderer(arcWindow& _arc_window, cDevice& _arc_device):
		arc_window{_arc_window},
		arc_device{_arc_device}
	{
		recreateSwapChain();
		createCommandBuffers();
	}

	arcRenderer::~arcRenderer()
	{
		freeCommandBuffers();
	}

	void arcRenderer::recreateSwapChain()
	{
		auto extent = arc_window.getExtent();

		while (extent.width == 0 || extent.height == 0)
		{
			extent = arc_window.getExtent();
			glfwWaitEvents();
		}
		vkDeviceWaitIdle(arc_device.device());

		if (arc_swap_chain == nullptr)
		{
			arc_swap_chain = std::make_unique<arcSwapChain>(arc_device, extent);
		}
		else
		{
			std::shared_ptr<arcSwapChain> old_swap_chain = std::move(arc_swap_chain);
			arc_swap_chain = std::make_unique<arcSwapChain>(arc_device, extent, old_swap_chain);

			if (!old_swap_chain->compareSwapFormats(*arc_swap_chain.get()))
			{
				throw std::runtime_error("Swap chain image format or depth has changed!");
			}
		}

		// TODO
	}

	VkCommandBuffer arcRenderer::beginFrame()
	{
		assert(!is_frame_started && "Can't call beginFrame while already in progress");

		auto result = arc_swap_chain->acquireNextImage(&current_image_index);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			recreateSwapChain();
			return nullptr;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			throw std::runtime_error("Failed to aquire swap chain image");
		}

		is_frame_started = true;

		auto command_buffer = getCurrentCommandBuffer();

		VkCommandBufferBeginInfo begin_info{};
		begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		if (vkBeginCommandBuffer(command_buffer, &begin_info) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to begin recording command buffer!");
		}

		return command_buffer;
	}

	void arcRenderer::endFrame()
	{
		assert(is_frame_started && "Can't call endFrame while frame is in progress");
		auto command_buffer = getCurrentCommandBuffer();

		if (vkEndCommandBuffer(command_buffer) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to record command buffer!");
		}

		auto result = arc_swap_chain->submitCommandBuffers(&command_buffer, &current_image_index);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || arc_window.wasWindowResized())
		{
			arc_window.resetWindowResizedFlag();
			recreateSwapChain();
		}

		else if (result != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to present swap chain image");
		}

		is_frame_started = false;
		current_frame_index = (current_frame_index + 1) % arcSwapChain::MAX_FRAMES_IN_FLIGHT;
	}

	void arcRenderer::beginSwapChainRenderPass(VkCommandBuffer _command_buffer)
	{
		assert(is_frame_started && "Can't call beginSwapChainRenderPass if frame is not in progress");
		assert(_command_buffer == getCurrentCommandBuffer() && "Can't begin render pass on command buffer from a different frame");

		VkRenderPassBeginInfo render_pass_info{};
		render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		render_pass_info.renderPass = arc_swap_chain->getRenderPass();
		render_pass_info.framebuffer = arc_swap_chain->getFrameBuffer(current_image_index);

		render_pass_info.renderArea.offset = { 0, 0 };
		render_pass_info.renderArea.extent = arc_swap_chain->getSwapChainExtent();

		std::array<VkClearValue, 2> clear_values{};
		clear_values[0].color = { 0.01f, 0.01f, 0.01f, 1.0f };
		clear_values[1].depthStencil = { 1.0f, 0 };
		render_pass_info.clearValueCount = static_cast<uint32_t>(clear_values.size());
		render_pass_info.pClearValues = clear_values.data();

		vkCmdBeginRenderPass(_command_buffer, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(arc_swap_chain->getSwapChainExtent().width);
		viewport.height = static_cast<float>(arc_swap_chain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{ {0,0}, arc_swap_chain->getSwapChainExtent() };
		vkCmdSetViewport(_command_buffer, 0, 1, &viewport);
		vkCmdSetScissor(_command_buffer, 0, 1, &scissor);
	}

	void arcRenderer::endSwapChainRenderPass(VkCommandBuffer _command_buffer)
	{
		assert(is_frame_started && "Can't call endSwapChainRenderPass if frame is not in progress");
		assert(_command_buffer == getCurrentCommandBuffer() && "Can't end render pass on command buffer from a different frame");

		vkCmdEndRenderPass(_command_buffer);
	}

	void arcRenderer::createCommandBuffers()
	{
		command_buffers.resize(arcSwapChain::MAX_FRAMES_IN_FLIGHT);

		VkCommandBufferAllocateInfo alloc_info{};
		alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		alloc_info.commandPool = arc_device.getCommandPool();
		alloc_info.commandBufferCount = static_cast<uint32_t>(command_buffers.size());

		if (vkAllocateCommandBuffers(arc_device.device(), &alloc_info, command_buffers.data()) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create command buffers!");
		}
	}

	void arcRenderer::freeCommandBuffers()
	{
		vkFreeCommandBuffers(arc_device.device(), arc_device.getCommandPool(), static_cast<uint32_t>(command_buffers.size()), command_buffers.data());
		command_buffers.clear();
	}
}