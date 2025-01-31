#include "model.h"

#include "Utils.h"

// libs
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <json.hpp>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tiny_gltf.h"

// std
#include <cassert>
#include <cstring>
#include <unordered_map>
#include <fstream>

#include <filesystem>
#include <iostream>

namespace std
{
	template<>
	struct hash<arc::arcModel::sVertex>
	{
		size_t operator()(arc::arcModel::sVertex const& _vertex) const
		{
			size_t seed = 0;
			arc::hashCombine(seed, _vertex.position, _vertex.color, _vertex.normal, _vertex.uv);
			return seed;
		}
	};
}

namespace arc
{
	arcModel::arcModel(cDevice& _device, const arcModel::sBuilder& _builder) : device{ _device }
	{
		createVertexBuffers(_builder.vertices);
		createIndexBuffers(_builder.indices);
	}

	arcModel::~arcModel()
	{
	}

	std::unique_ptr<arcModel> arcModel::createOBJModelFromFile(cDevice& device, const std::string& _filepath)
	{
		sBuilder builder{};
		builder.loadOBJModel(_filepath);

		return std::make_unique<arcModel>(device, builder);
	}

	std::unique_ptr<arcModel> arcModel::createGLTFModelFromFile(cDevice& device, const std::string& _filepath)
	{
		sBuilder builder{};
		builder.loadGLTFModel(_filepath);

		return std::make_unique<arcModel>(device, builder);
	}

	void arcModel::bind(VkCommandBuffer _command_buffer)
	{
		VkBuffer buffers[] = { vertex_buffer->getBuffer() };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(_command_buffer, 0, 1, buffers, offsets);

		if (has_index_buffer)
		{
			vkCmdBindIndexBuffer(_command_buffer, index_buffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
		}
	}

	void arcModel::draw(VkCommandBuffer _command_buffer)
	{
		if (has_index_buffer)
		{
			vkCmdDrawIndexed(_command_buffer, index_count, 1, 0, 0, 0);
		}
		else
		{
			vkCmdDraw(_command_buffer, vertex_count, 1, 0, 0);
		}

	}

	void arcModel::createVertexBuffers(const std::vector<sVertex>& _vertices)
	{
		vertex_count = static_cast<uint32_t>(_vertices.size());
		assert(vertex_count >= 3 && "Vertex count must at least be 3!");
		VkDeviceSize buffer_size = sizeof(_vertices[0]) * vertex_count;

		uint32_t vertex_size = sizeof(_vertices[0]);

		cBuffer staging_buffer{
			device,
			vertex_size,
			vertex_count,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		};

		staging_buffer.map();
		staging_buffer.writeToBuffer((void*)_vertices.data());

		vertex_buffer = std::make_unique<cBuffer>(
			device,
			vertex_size,
			vertex_count,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
			);

		device.copyBuffer(staging_buffer.getBuffer(), vertex_buffer->getBuffer(), buffer_size);
	}


	void arcModel::createIndexBuffers(const std::vector<uint32_t>& _indices)
	{
		index_count = static_cast<uint32_t>(_indices.size());
		has_index_buffer = index_count > 0;

		if (!has_index_buffer)
		{
			return;
		}
		
		VkDeviceSize buffer_size = sizeof(_indices[0]) * index_count;

		uint32_t index_size = sizeof(_indices[0]);

		cBuffer staging_buffer{
			device,
			index_size,
			index_count,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		};

		staging_buffer.map();
		staging_buffer.writeToBuffer((void*)_indices.data());

		index_buffer = std::make_unique<cBuffer>(
			device,
			index_size,
			index_count,
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		);

		device.copyBuffer(staging_buffer.getBuffer(), index_buffer->getBuffer(), buffer_size);
	}

	std::vector<VkVertexInputBindingDescription> arcModel::sVertex::getBindingDescriptions()
	{
		std::vector<VkVertexInputBindingDescription> binding_descriptions(1);
		binding_descriptions[0].binding = 0;
		binding_descriptions[0].stride = sizeof(sVertex);
		binding_descriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return binding_descriptions;
	}
	std::vector<VkVertexInputAttributeDescription> arcModel::sVertex::getAttributeDescriptions()
	{
		std::vector<VkVertexInputAttributeDescription> attribute_descriptions{};

		attribute_descriptions.push_back({ 0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(sVertex, position) });
		attribute_descriptions.push_back({ 1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(sVertex, color   ) });
		attribute_descriptions.push_back({ 2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(sVertex, normal  ) });
		attribute_descriptions.push_back({ 3, 0, VK_FORMAT_R32G32_SFLOAT   , offsetof(sVertex, uv      ) });

		return attribute_descriptions;
	}

	void arcModel::sBuilder::loadOBJModel(const std::string& _filepath)
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warn, err;

		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, _filepath.c_str()))
		{
			throw std::runtime_error(warn + err);
		}

		vertices.clear();
		indices.clear();

		std::unordered_map<sVertex, uint32_t> uniqueVertices{};
		for (const auto& shape : shapes)
		{
			for (const auto& index : shape.mesh.indices)
			{
				sVertex vertex{};

				if (index.vertex_index >= 0)
				{
					vertex.position = 
					{
						attrib.vertices[ 3 * index.vertex_index + 0 ],
						attrib.vertices[ 3 * index.vertex_index + 1 ],
						attrib.vertices[ 3 * index.vertex_index + 2 ]
					};

					vertex.color =
					{
						attrib.colors[3 * index.vertex_index + 0],
						attrib.colors[3 * index.vertex_index + 1],
						attrib.colors[3 * index.vertex_index + 2]
					};
				}

				if (index.normal_index >= 0)
				{
					vertex.normal = 
					{
						attrib.normals[ 3 * index.normal_index + 0 ],
						attrib.normals[ 3 * index.normal_index + 1 ],
						attrib.normals[ 3 * index.normal_index + 2 ]
					};
				}

				if (index.texcoord_index >= 0)
				{
					vertex.uv =
					{
						attrib.texcoords[ 2 * index.texcoord_index + 0 ],
						attrib.texcoords[ 2 * index.texcoord_index + 1 ]
					};
				}

				if (uniqueVertices.count(vertex) == 0)
				{
					uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
					vertices.push_back(vertex);
				}
				indices.push_back(uniqueVertices[vertex]);
			}
		}
	}

	void arcModel::sBuilder::loadGLTFModel(const std::string& _filepath)
	{
		auto test = std::filesystem::current_path().string();
		auto path = test + "\\" + _filepath;

		tinygltf::TinyGLTF loader;
		tinygltf::Model model;

		std::string err, warn;

		loader.LoadASCIIFromFile(&model, &err, &warn, path);

		if (!warn.empty())
			std::cout << "Warning: " << warn << std::endl;

		if (!err.empty())
			std::cout << "Error: " << err << std::endl;

		vertices.clear();
		indices.clear();

		//for (auto& bf : model.bufferViews)
		//{
		//	if (bf.target == 0)
		//		continue;

		//	const auto buffer = model.buffers[bf.buffer];

		//	buffer
		//}

		for (auto& mesh : model.meshes)
		{
			for (auto& primitive : mesh.primitives)
			{
				auto indexacc = model.accessors[primitive.indices];


				for (auto& attrib : primitive.attributes)
				{
					auto acc = model.accessors[attrib.second];
					int bytestride = acc.ByteStride(model.bufferViews[acc.bufferView]);

					auto buff = model.buffers[model.bufferViews[acc.bufferView].buffer];

					std::vector<uint8_t> out{};
					std::string mime;


				}

			}
		}
	}
}