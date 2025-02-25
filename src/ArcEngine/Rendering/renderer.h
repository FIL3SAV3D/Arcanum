#pragma once

#include "Window/device.h"
#include "Window/Window.h"

#include "SwapChain.h"

// std
#include <cassert>
#include <memory>
#include <vector>

namespace arc
{
	class arcRenderer
	{
	public:

		arcRenderer(arcWindow& _arc_window, cDevice& _arc_device);
		~arcRenderer();

		arcRenderer(const arcRenderer&) = delete;
		arcRenderer& operator=(const arcRenderer&) = delete;

		VkRenderPass getSwapChainRenderPass() const { return arc_swap_chain->getRenderPass(); }

		float getAspectRatio() const { return arc_swap_chain->extentAspectRatio(); }

		bool isFrameInProgress() const { return is_frame_started; }

		VkCommandBuffer getCurrentCommandBuffer() const
		{
			assert(is_frame_started && "Cannot get command buffer when frame not in progress");
			return command_buffers[current_frame_index];
		};

		int getFrameIndex() const 
		{
			assert(is_frame_started && "Cannot get frame index when frame not in progress");
			return current_frame_index;
		};

		VkCommandBuffer beginFrame();
		void endFrame();
		void beginSwapChainRenderPass(VkCommandBuffer _command_buffer);
		void endSwapChainRenderPass  (VkCommandBuffer _command_buffer);


	private:
		void createCommandBuffers();
		void freeCommandBuffers();
		void recreateSwapChain();

		arcWindow& arc_window;
		cDevice& arc_device;
		std::unique_ptr<arcSwapChain> arc_swap_chain;
		std::vector<VkCommandBuffer> command_buffers;

		uint32_t current_image_index = 0;
		int current_frame_index = 0;
		bool is_frame_started = false;
	};
}