project "ShadersOpenGL"
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

    -- filter 'files:**.slang'
    --         buildmessage 'Compiling %{file.relpath}'

    --         buildcommands{ '%{VULKAN_SDK}/Bin/slangc.exe %{file.abspath} -profile glsl_460 -target glsl -o %{wks.location}\\Source\\Shaders\\ShadersOpenGL\\Source\\CompiledShaders\\%{file.basename}.comp -entry main' }

    --         buildmessage 'Build Output: %{wks.location}\\Source\\Shaders\\ShadersOpenGL\\Source\\CompiledShaders\\%{file.basename}.comp'
    --         buildoutputs { "%{wks.location}\\Source\\Shaders\\ShadersOpenGL\\Source\\CompiledShaders\\%{file.basename}.comp" }