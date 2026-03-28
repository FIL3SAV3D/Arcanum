project "ShadersVulkan"
    kind "Utility"
	targetdir "Binaries/%{cfg.buildcfg}"

    files 
    { 
        "Source/**.frag",
        "Source/**.vert",
        "Source/**.comp",
        "Source/**.slang"
    }

    includedirs
    {
        "Source",
    }

    targetdir (BuildDir .. OutputDir )
    objdir (BuildDir .. "Intermediates/" .. OutputDir .. "/%{prj.name}")

    -- filter 'files:**.frag'
    --         buildmessage 'Compiling %{file.relpath}'
    --         buildmessage 'location %{file.abspath}'

    --         buildcommands{ '%{VULKAN_SDK}/Bin/glslc.exe %{file.abspath} -o %{wks.location}\\Source\\Shaders\\ShadersVulkan\\Source\\VulkanShadersCompiled\\%{file.basename}.frag.spv' }

    --         buildmessage 'Build Output: %{wks.location}\\Source\\Shaders\\ShadersVulkan\\Source\\VulkanShadersCompiled\\%{file.basename}.comp.spv'
    --         buildoutputs { "%{wks.location}\\Source\\Shaders\\ShadersVulkan\\Source\\VulkanShadersCompiled\\%{file.basename}.frag.spv" }

	-- filter 'files:**.vert'
    --         buildmessage 'Compiling %{file.relpath}'

    --         buildcommands{ "%{VULKAN_SDK}/Bin/glslc.exe %{file.abspath} -o %{wks.location}\\Source\\Shaders\\ShadersVulkan\\Source\\VulkanShadersCompiled\\%{file.basename}.vert.spv" }

    --         buildmessage 'Build Output: %{wks.location}\\Source\\Shaders\\ShadersVulkan\\Source\\VulkanShadersCompiled\\%{file.basename}.comp.spv'
    --         buildoutputs { "%{wks.location}\\Source\\Shaders\\ShadersVulkan\\Source\\VulkanShadersCompiled\\%{file.basename}.vert.spv" }

    -- filter 'files:**.comp'
    --         buildmessage 'Compiling %{file.relpath}'

    --         buildcommands{ "%{VULKAN_SDK}/Bin/glslc.exe %{file.abspath} -o %{wks.location}\\Source\\Shaders\\ShadersVulkan\\Source\\VulkanShadersCompiled\\%{file.basename}.comp.spv" }

    --         buildmessage 'Build Output: %{wks.location}\\Source\\Shaders\\ShadersVulkan\\Source\\VulkanShadersCompiled\\%{file.basename}.comp.spv'
    --         buildoutputs { "%{wks.location}\\Source\\Shaders\\ShadersVulkan\\Source\\VulkanShadersCompiled\\%{file.basename}.comp.spv" }

    -- filter 'files:**.slang'
    --         buildmessage 'Compiling %{file.relpath}'

    --         buildcommands{ '%{VULKAN_SDK}/Bin/slangc.exe %{file.abspath} -profile glsl_460 -target glsl -o %{wks.location}\\Source\\Shaders\\ShadersVulkan\\Source\\VulkanShadersCompiled\\%{file.basename}.comp -entry main' }

    --         buildmessage 'Build Output: %{wks.location}\\Source\\Shaders\\ShadersVulkan\\Source\\VulkanShadersCompiled\\%{file.basename}.comp'
    --         buildoutputs { "%{wks.location}\\Source\\Shaders\\ShadersVulkan\\Source\\VulkanShadersCompiled\\%{file.basename}.comp" }