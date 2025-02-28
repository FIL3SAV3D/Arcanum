#pragma once
#include "Window/device.h"
#include "Rendering/buffer.h"

#include "Jolt/Jolt.h"
#include "Jolt/Math/Vec3.h"

// libs
#define GLM_FORCE_RADIANS
#define FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

// std
#include <memory>
#include <vector>

namespace arc
{
	class arcModel
	{
	public:
		struct sVertex
		{
			glm::vec3 position{};
			glm::vec3 color{};
			glm::vec3 normal{};
			glm::vec2 uv{};

			static std::vector<VkVertexInputBindingDescription>   getBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

			bool operator==(const sVertex& other) const
			{
				return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
			}
		};

		struct sBuilder
		{
			std::vector<sVertex> vertices{};
			std::vector<uint32_t> indices{};

			JPH::Array<JPH::Vec3> JPHVertArray{};

			void loadOBJModel(const std::string& _filepath);
			void loadGLTFModel(const std::string& _filepath);
		};

		arcModel(cDevice& _device, const arcModel::sBuilder& _builder);
		~arcModel();

		arcModel(const arcModel&) = delete;
		arcModel& operator=(const arcModel&) = delete;

		static std::unique_ptr<arcModel> createOBJModelFromFile(cDevice& device, const std::string& _filepath);
		static std::unique_ptr<arcModel> createGLTFModelFromFile(cDevice& device, const std::string& _filepath);

		void bind(VkCommandBuffer _command_buffer);
		void draw(VkCommandBuffer _command_buffer);

		JPH::Array<JPH::Vec3> JPHVertArray{};
	private:
		void createVertexBuffers(const std::vector<sVertex>& _vertices);
		void createIndexBuffers(const std::vector<uint32_t>& _indices);

		cDevice& device;
		std::unique_ptr<cBuffer> vertex_buffer = nullptr;
		uint32_t vertex_count;

		bool has_index_buffer = false;
		std::unique_ptr<cBuffer> index_buffer = nullptr;
		uint32_t index_count;
	};
}