#include "pipeline.h"

#include "temp/model.h"

// std
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <cassert>

namespace arc
{
	arcPipeline::arcPipeline(
		cDevice& _device,
		const pipelineConfigInfo& _config,
		const std::string& _vert_file_path,
		const std::string& _frag_file_path) : arc_device{ _device }
	{
		createGraphicsPipeline( _config,  _vert_file_path, _frag_file_path );
	}

	arcPipeline::~arcPipeline()
	{
		vkDestroyShaderModule(arc_device.device(), vert_shader_module, nullptr);
		vkDestroyShaderModule(arc_device.device(), frag_shader_module, nullptr);

		vkDestroyPipeline(arc_device.device(), graphics_pipeline, nullptr);
	}

	void arcPipeline::bind(VkCommandBuffer command_buffer)
	{
		vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline);
	}

	void arcPipeline::defaultPipelineConfigInfo(pipelineConfigInfo& _config_info)
	{
		_config_info.input_assembly_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		_config_info.input_assembly_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		_config_info.input_assembly_info.primitiveRestartEnable = VK_FALSE;

		_config_info.viewport_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		_config_info.viewport_info.viewportCount = 1;
		_config_info.viewport_info.pViewports = nullptr;
		_config_info.viewport_info.scissorCount = 1;
		_config_info.viewport_info.pScissors = nullptr;

		_config_info.rasterization_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		_config_info.rasterization_info.depthClampEnable = VK_FALSE;
		_config_info.rasterization_info.rasterizerDiscardEnable = VK_FALSE;
		_config_info.rasterization_info.polygonMode = VK_POLYGON_MODE_FILL;
		_config_info.rasterization_info.lineWidth = 1.0f;
		_config_info.rasterization_info.cullMode = VK_CULL_MODE_FRONT_BIT;
		_config_info.rasterization_info.frontFace = VK_FRONT_FACE_CLOCKWISE;
		_config_info.rasterization_info.depthBiasEnable = VK_FALSE;
		_config_info.rasterization_info.depthBiasConstantFactor = 0.0f;  // Optional
		_config_info.rasterization_info.depthBiasClamp = 0.0f;           // Optional
		_config_info.rasterization_info.depthBiasSlopeFactor = 0.0f;     // Optional

		_config_info.multisample_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		_config_info.multisample_info.sampleShadingEnable = VK_FALSE;
		_config_info.multisample_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		_config_info.multisample_info.minSampleShading = 1.0f;           // Optional
		_config_info.multisample_info.pSampleMask = nullptr;             // Optional
		_config_info.multisample_info.alphaToCoverageEnable = VK_FALSE;  // Optional
		_config_info.multisample_info.alphaToOneEnable = VK_FALSE;       // Optional

		_config_info.color_blend_attachment.colorWriteMask =
			VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
			VK_COLOR_COMPONENT_A_BIT;
		_config_info.color_blend_attachment.blendEnable = VK_FALSE;
		_config_info.color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
		_config_info.color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
		_config_info.color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD;              // Optional
		_config_info.color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
		_config_info.color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
		_config_info.color_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD;              // Optional

		_config_info.color_blend_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		_config_info.color_blend_info.logicOpEnable = VK_FALSE;
		_config_info.color_blend_info.logicOp = VK_LOGIC_OP_COPY;  // Optional
		_config_info.color_blend_info.attachmentCount = 1;
		_config_info.color_blend_info.pAttachments = &_config_info.color_blend_attachment;
		_config_info.color_blend_info.blendConstants[0] = 0.0f;  // Optional
		_config_info.color_blend_info.blendConstants[1] = 0.0f;  // Optional
		_config_info.color_blend_info.blendConstants[2] = 0.0f;  // Optional
		_config_info.color_blend_info.blendConstants[3] = 0.0f;  // Optional

		_config_info.depth_stencil_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		_config_info.depth_stencil_info.depthTestEnable = VK_TRUE;
		_config_info.depth_stencil_info.depthWriteEnable = VK_TRUE;
		_config_info.depth_stencil_info.depthCompareOp = VK_COMPARE_OP_LESS;
		_config_info.depth_stencil_info.depthBoundsTestEnable = VK_FALSE;
		_config_info.depth_stencil_info.minDepthBounds = 0.0f;  // Optional
		_config_info.depth_stencil_info.maxDepthBounds = 1.0f;  // Optional
		_config_info.depth_stencil_info.stencilTestEnable = VK_FALSE;
		_config_info.depth_stencil_info.front = {};  // Optional
		_config_info.depth_stencil_info.back = {};   // Optional

		_config_info.dynamic_state_enables = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
		_config_info.dynamic_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		_config_info.dynamic_state_info.pDynamicStates = _config_info.dynamic_state_enables.data();
		_config_info.dynamic_state_info.dynamicStateCount = static_cast<uint32_t>(_config_info.dynamic_state_enables.size());
		_config_info.dynamic_state_info.flags = 0;

		_config_info.bindingDescriptions   = arcModel::sVertex::getBindingDescriptions();
		_config_info.attributeDescriptions = arcModel::sVertex::getAttributeDescriptions();
	}

	void arcPipeline::enableAlphaBlending(pipelineConfigInfo& _config_info)
	{
		_config_info.color_blend_attachment.blendEnable = VK_TRUE;

		_config_info.color_blend_attachment.colorWriteMask =
			VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
			VK_COLOR_COMPONENT_A_BIT;
		_config_info.color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		_config_info.color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		_config_info.color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD;
		_config_info.color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		_config_info.color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		_config_info.color_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD;
	}

	VkSampleCountFlagBits arcPipeline::GetMaxUsableSampleCount()
	{
		VkPhysicalDeviceProperties physicalDeviceProperties = arc_device.properties;
		VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;
		if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
		if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
		if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
		if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
		if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
		if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

		return VK_SAMPLE_COUNT_1_BIT;
	}

	std::vector<char> arcPipeline::readFile(const std::string& _file_path)
	{
		std::ifstream file{ _file_path, std::ios::ate | std::ios::binary };

		if (!file.is_open())
		{
			throw std::runtime_error("Failed to open file: " + _file_path + " in arc_pipeline.cpp");
		}

		size_t file_size = static_cast<size_t>( file.tellg() );

		std::vector<char> buffer( file_size );

		file.seekg(0);
		file.read(buffer.data(), file_size);

		file.close();

		return buffer;
	}

	void arcPipeline::createGraphicsPipeline( const pipelineConfigInfo& _config, const std::string& _vert_file_path, const std::string& _frag_file_path )
	{
		assert(_config.pipeline_layout != VK_NULL_HANDLE && "Cannot create graphics pipeline:: no pipeline layout provided in config info");
		assert(_config.render_pass != VK_NULL_HANDLE && "Cannot create graphics pipeline:: no render pass provided in config info");

		auto vert_code = readFile(_vert_file_path);
		auto frag_code = readFile(_frag_file_path);

		createShaderModule(vert_code, &vert_shader_module);
		createShaderModule(frag_code, &frag_shader_module);

		VkPipelineShaderStageCreateInfo shader_stages[2];
		shader_stages[0].sType               = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shader_stages[0].stage               = VK_SHADER_STAGE_VERTEX_BIT;
		shader_stages[0].module              = vert_shader_module;
		shader_stages[0].pName               = "main";
		shader_stages[0].flags               = 0;
		shader_stages[0].pNext               = nullptr;
		shader_stages[0].pSpecializationInfo = nullptr;

		shader_stages[1].sType               = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shader_stages[1].stage               = VK_SHADER_STAGE_FRAGMENT_BIT;
		shader_stages[1].module              = frag_shader_module;
		shader_stages[1].pName               = "main";
		shader_stages[1].flags               = 0;
		shader_stages[1].pNext               = nullptr;
		shader_stages[1].pSpecializationInfo = nullptr;

		auto& binding_descriptions = _config.bindingDescriptions;
		auto& attribute_descriptions = _config.attributeDescriptions;
		VkPipelineVertexInputStateCreateInfo vertex_input_info{};
		vertex_input_info.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertex_input_info.vertexAttributeDescriptionCount = static_cast<uint32_t>(attribute_descriptions.size());
		vertex_input_info.vertexBindingDescriptionCount   = static_cast<uint32_t>(binding_descriptions.size());
		vertex_input_info.pVertexAttributeDescriptions    = attribute_descriptions.data();
		vertex_input_info.pVertexBindingDescriptions      = binding_descriptions.data();

		VkGraphicsPipelineCreateInfo pipeline_info{};
		pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipeline_info.stageCount = 2;
		pipeline_info.pStages = shader_stages;
		pipeline_info.pVertexInputState = &vertex_input_info;
		pipeline_info.pInputAssemblyState = &_config.input_assembly_info;
		pipeline_info.pViewportState = &_config.viewport_info;
		pipeline_info.pRasterizationState = &_config.rasterization_info;
		pipeline_info.pMultisampleState = &_config.multisample_info;
		pipeline_info.pColorBlendState = &_config.color_blend_info;
		pipeline_info.pDepthStencilState = &_config.depth_stencil_info;
		pipeline_info.pDynamicState = &_config.dynamic_state_info;

		pipeline_info.layout        = _config.pipeline_layout;
		pipeline_info.renderPass    = _config.render_pass;
		pipeline_info.subpass       = _config.subpass;

		pipeline_info.basePipelineIndex  = -1;
		pipeline_info.basePipelineHandle = VK_NULL_HANDLE;

		if (vkCreateGraphicsPipelines(arc_device.device(), VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &graphics_pipeline) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create graphics pipeline");
		}
	}

	void arcPipeline::createShaderModule(const std::vector<char>& _code, VkShaderModule* _shader_module)
	{
		VkShaderModuleCreateInfo create_info{};
		create_info.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		create_info.codeSize = _code.size();
		create_info.pCode    = reinterpret_cast<const uint32_t*>(_code.data());

		if (vkCreateShaderModule(arc_device.device(), &create_info, nullptr, _shader_module) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create shader module");
		}
	}
}