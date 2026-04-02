#pragma once

#include "Framework/ResourceManagement/ResourceManager.h"

class Texture : public Resource
{
private:
    // Core Vulkan GPU resources for texture representation
    vk::Image image;              // GPU image object containing pixel data
    vk::DeviceMemory memory;      // GPU memory allocation backing the image
    vk::DeviceSize offset;        // Offset within the memory allocation for this texture
    vk::ImageView imageView;      // Shader-accessible view into the image
    vk::Sampler sampler;          // Sampling configuration (filtering, wrapping, etc.)

    // Texture metadata for validation and debugging
    int width = 0;                // Image width in pixels
    int height = 0;               // Image height in pixels
    int channels = 0;             // Number of color channels (RGB=3, RGBA=4, etc.)

public:
    explicit Texture(const std::string& id) : Resource(id) {}

    ~Texture() override {
        Unload();                 // Ensure proper cleanup when object is destroyed
    }
};