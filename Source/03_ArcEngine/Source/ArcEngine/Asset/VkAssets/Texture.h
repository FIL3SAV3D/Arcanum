#pragma once

#include <vulkan/vulkan.h>
#include "ArcEngine/Asset/VkAssets/Resource.h"

class Texture : public Resource
{
private:
    // Core Vulkan GPU resources for texture representation
    VkImage image;              // GPU image object containing pixel data
    VkDeviceMemory memory;      // GPU memory allocation backing the image
    VkDeviceSize offset;        // Offset within the memory allocation for this texture
    VkImageView imageView;      // Shader-accessible view into the image
    VkSampler sampler;          // Sampling configuration (filtering, wrapping, etc.)

    // Texture metadata for validation and debugging
    int width = 0;                // Image width in pixels
    int height = 0;               // Image height in pixels
    int channels = 0;             // Number of color channels (RGB=3, RGBA=4, etc.)

public:
    explicit Texture(const std::string& id) : Resource(id) {}

    ~Texture() override {
        Unload();                 // Ensure proper cleanup when object is destroyed
    }

    bool Load() override {
        // Step 2a: Construct file path using resource ID and expected format
        std::string filePath = "textures/" + GetId() + ".ktx";

        // Step 2b: Load raw image data from disk with format detection
        unsigned char* data = LoadImageData(filePath, &width, &height, &channels);
        if (!data) {
            return false;           // Failed to load - return failure without partial state
        }

        // Step 2c: Transform raw pixel data into Vulkan GPU resources
        CreateVulkanImage(data, width, height, channels);

        // Step 2d: Clean up temporary CPU memory to prevent leaks
        FreeImageData(data);

        return Resource::Load();    // Mark resource as successfully loaded
    }

    void Unload() override {
        // Only perform cleanup if resource is currently loaded
        if (IsLoaded()) 
        {
            // Step 3a: Obtain device handle for resource destruction
            VkDevice device = GetDevice();

            // Step 3b: Destroy GPU objects in reverse creation order
            // This ordering prevents use-after-free errors in GPU drivers
            vkDestroySampler(device, sampler, nullptr);;    // Destroy sampling configuration
            vkDestroyImageView(device, imageView, nullptr); // Destroy shader view
            vkDestroyImage(device, image, nullptr);         // Destroy image object
            vkFreeMemory(device, memory, nullptr);          // Release GPU memory allocation

            // Step 3c: Update base class state to reflect unloaded status
            Resource::Unload();
        }
    }

    // Public interface for accessing Vulkan resources safely
    VkImage GetImage() const { return image; }
    VkImageView GetImageView() const { return imageView; }
    VkSampler GetSampler() const { return sampler; }

private:
    unsigned char* LoadImageData(const std::string& filePath, int* width, int* height, int* channels) {
        // Implementation using stb_image or ktx library
        // This method abstracts the details of different image format support
        // and provides a consistent interface for pixel data loading
        // ...
        return nullptr; // Placeholder
    }

    void FreeImageData(unsigned char* data) {
        // Implementation using stb_image or ktx library
        // Ensures proper cleanup of image loader specific memory allocations
        // Different libraries may require different cleanup approaches
        // ...
    }

    void CreateVulkanImage(unsigned char* data, int width, int height, int channels) {
        // Implementation to create Vulkan image, allocate memory, and upload data
        // This involves complex Vulkan operations including:
        // - Format selection based on channel count and data type
        // - Memory allocation with appropriate usage flags
        // - Image creation with optimal tiling and layout
        // - Data upload via staging buffers for efficiency
        // - Image view creation for shader access
        // - Sampler creation with appropriate filtering settings
        // ...
    }

    VkDevice GetDevice() {
        // Get device from somewhere (e.g., singleton or parameter)
        // Production code would use dependency injection or service location
        // to provide the Vulkan device handle without tight coupling
        // ...
        return VkDevice(); // Placeholder
    }
};