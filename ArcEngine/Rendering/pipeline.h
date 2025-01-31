#pragma once

#include "Window/device.h"

// std
#include <string>
#include <vector>

namespace arc
{
	struct pipelineConfigInfo
	{
		pipelineConfigInfo(const pipelineConfigInfo&) = delete;
		pipelineConfigInfo& operator=(const pipelineConfigInfo&) = delete;

		VkPipelineViewportStateCreateInfo      viewport_info;
		VkPipelineInputAssemblyStateCreateInfo input_assembly_info;
		VkPipelineRasterizationStateCreateInfo rasterization_info;
		VkPipelineMultisampleStateCreateInfo   multisample_info;
		VkPipelineColorBlendAttachmentState    color_blend_attachment;
		VkPipelineColorBlendStateCreateInfo    color_blend_info;
		VkPipelineDepthStencilStateCreateInfo  depth_stencil_info;
		std::vector<VkDynamicState>            dynamic_state_enables;
		VkPipelineDynamicStateCreateInfo       dynamic_state_info;
		VkPipelineLayout                       pipeline_layout = nullptr;
		VkRenderPass                           render_pass = nullptr;
		uint32_t                               subpass = 0;
	};

	class arcPipeline
	{
	public:
		arcPipeline(
			cDevice&          _device,
			const pipelineConfigInfo& _config,
			const std::string&        _vert_file_path,
			const std::string&        _frag_file_path);
		~arcPipeline();

		arcPipeline(const arcPipeline&) = delete;
		arcPipeline& operator=(const arcPipeline&) = delete;

		void bind(VkCommandBuffer command_buffer);

		static void defaultPipelineConfigInfo(pipelineConfigInfo& _config_info);

	private:
		static std::vector<char> readFile(const std::string& _file_path);

		void createGraphicsPipeline(
			const pipelineConfigInfo& _config, 
			const std::string& _vert_file_path, 
			const std::string& _frag_file_path);

		void createShaderModule( const std::vector<char>& _code, VkShaderModule* _shader_module );

		cDevice& arc_device;

		VkPipeline graphics_pipeline;

		VkShaderModule vert_shader_module;
		VkShaderModule frag_shader_module;
	};
}