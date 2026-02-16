#pragma once

#include "ArcEngine/Graphics/Interface/IGraphics.h"

#include "ArcEngine/Graphics/Vulkan-1.4/VulkanSwapchain.h"
#include <ArcEngine/Platform/Window.h>

#include "vk_descriptors.h"

namespace ArcEngine
{
	constexpr unsigned int FRAME_OVERLAP = 2;

	struct DeletionQueue
	{
		std::deque<std::function<void()>> deletors;

		void push_function(std::function<void()>&& function) {
			deletors.push_back(function);
		}

		void flush() {
			// reverse iterate the deletion queue to execute all the functions
			for (auto it = deletors.rbegin(); it != deletors.rend(); it++) {
				(*it)(); //call functors
			}

			deletors.clear();
		}
	};

	struct FrameData
	{
		VkSemaphore _swapchainSemaphore;
		VkSemaphore _renderSemaphore;

		VkFence _renderFence;

		VkCommandPool _commandPool;
		VkCommandBuffer _mainCommandBuffer;

		DeletionQueue _deletionQueue;
	};

	struct AllocatedImage 
	{
		VkImage image;
		VkImageView imageView;
		VmaAllocation allocation;
		VkExtent3D imageExtent;
		VkFormat imageFormat;
	};

    class VulkanGraphics : public IGraphics
    {
    public:
        void Create(std::shared_ptr<Window> _window) override;
        void Destroy() override;
        void Blit() override;
        void RenderMesh() override;
		void draw_background(VkCommandBuffer cmd);
        void RenderMeshInstanced() override;
        void RenderMeshIndirect() override;

		void immediate_submit(std::function<void(VkCommandBuffer cmd)>&& function);

	private:
		void init_descriptors();
		void init_pipelines();
		void init_background_pipelines();

		void init_imgui(std::shared_ptr<Window> _window);

		void draw_imgui(VkCommandBuffer cmd, VkImageView targetImageView);

    private:
		int _frameNumber{ 0 };
		bool stop_rendering{ false };

		DescriptorAllocator globalDescriptorAllocator;

		VkDescriptorSet _drawImageDescriptors;
		VkDescriptorSetLayout _drawImageDescriptorLayout;

		FrameData& get_current_frame() { return _frames[_frameNumber % FRAME_OVERLAP]; }

		FrameData _frames[FRAME_OVERLAP];

        VulkanSwapchain swapchain;

        AllocatedImage _drawImage;
        VkExtent2D _drawExtent;

        VmaAllocator _allocator;

        VkInstance _instance;// Vulkan library handle
        VkDebugUtilsMessengerEXT _debug_messenger;// Vulkan debug output handle
        VkPhysicalDevice _chosenGPU;// GPU chosen as the default device
        VkDevice _device; // Vulkan device for commands
        VkSurfaceKHR _surface;// Vulkan window surface

		VkQueue _graphicsQueue;
		uint32_t _graphicsQueueFamily;

		DeletionQueue _mainDeletionQueue;

		uint32_t swapchainImageIndex;


		// immediate submit structures
		VkFence _immFence;
		VkCommandBuffer _immCommandBuffer;
		VkCommandPool _immCommandPool;


		VkPipeline _gradientPipeline;
		VkPipelineLayout _gradientPipelineLayout;
    };
}