#pragma once

#include "Window/device.h"

// std
#include <memory>
#include <unordered_map>
#include <vector>

namespace arc {

    class cDescriptorSetLayout {
    public:
        class Builder {
        public:
            Builder(arcDevice& _device) : device{ _device } {}

            Builder& addBinding(
                uint32_t binding,
                VkDescriptorType descriptorType,
                VkShaderStageFlags stageFlags,
                uint32_t count = 1);
            std::unique_ptr<cDescriptorSetLayout> build() const;

        private:
            arcDevice& device;
            std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
        };

        cDescriptorSetLayout(
            arcDevice& arcDevice, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
        ~cDescriptorSetLayout();
        cDescriptorSetLayout(const cDescriptorSetLayout&) = delete;
        cDescriptorSetLayout& operator=(const cDescriptorSetLayout&) = delete;

        VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }

    private:
        arcDevice& device;
        VkDescriptorSetLayout descriptorSetLayout;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

        friend class cDescriptorWriter;
    };

    class cDescriptorPool {
    public:
        class Builder {
        public:
            Builder(arcDevice& _device) : device{ _device } {}

            Builder& addPoolSize(VkDescriptorType descriptorType, uint32_t count);
            Builder& setPoolFlags(VkDescriptorPoolCreateFlags flags);
            Builder& setMaxSets(uint32_t count);
            std::unique_ptr<cDescriptorPool> build() const;

        private:
            arcDevice& device;
            std::vector<VkDescriptorPoolSize> poolSizes{};
            uint32_t maxSets = 1000;
            VkDescriptorPoolCreateFlags poolFlags = 0;
        };

        cDescriptorPool(
            arcDevice& arcDevice,
            uint32_t maxSets,
            VkDescriptorPoolCreateFlags poolFlags,
            const std::vector<VkDescriptorPoolSize>& poolSizes);
        ~cDescriptorPool();
        cDescriptorPool(const cDescriptorPool&) = delete;
        cDescriptorPool& operator=(const cDescriptorPool&) = delete;

        bool allocateDescriptor(
            const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;

        void freeDescriptors(std::vector<VkDescriptorSet>& descriptors) const;

        void resetPool();

		auto& GetDescriptorPool() { return descriptorPool; }
    private:
        arcDevice& device;
        VkDescriptorPool descriptorPool;

        friend class cDescriptorWriter;
    };

    class cDescriptorWriter {
    public:
        cDescriptorWriter(cDescriptorSetLayout& setLayout, cDescriptorPool& pool);

        cDescriptorWriter& writeBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
        cDescriptorWriter& writeImage(uint32_t binding, VkDescriptorImageInfo* imageInfo);

        bool build(VkDescriptorSet& set);
        void overwrite(VkDescriptorSet& set);

    private:
        cDescriptorSetLayout& setLayout;
        cDescriptorPool& pool;
        std::vector<VkWriteDescriptorSet> writes;
    };

}  // namespace arc