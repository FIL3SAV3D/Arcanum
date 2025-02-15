vulkan_sdk = os.getenv("VULKAN_SDK")

cppdialect "c++20"
language "C++"
staticruntime "off"

filter "configurations:Debug"
	    runtime "Debug"
	    symbols "on"
	    defines { "ARC_DEBUG" }

filter "configurations:Release"
	runtime "Release"
	optimize "speed"
	symbols "off"
	defines { "ARC_RELEASE" }

workspace "Arcanum"
	architecture "x86_64"
    configurations { "Debug","Release" }
    startproject "Framework"
	
	project "Framework"
		kind "ConsoleApp"
		targetdir "bin/%{cfg.buildcfg}"
		dependson { "ArcEngine", "Shaders", ... }

		files {
            "src/Framework/**.h",
            "src/Framework/**.cpp",
            "src/Framework/**.hpp",
        }

		links { "%{vulkan_sdk}/lib/vulkan-1.lib" }
		
		links { "../Arcanum/Library/GLFW/lib-vc2022/glfw3.lib"}
		links { "ArcEngine" }

		includedirs { "$(VULKAN_SDK)/include" }
		includedirs { "../Arcanum/Library/GLFW/include" }
		includedirs { "../Arcanum/Library/GLM" }
		includedirs { "../Arcanum/src/ArcEngine" }

	project "ArcEngine"
		kind "StaticLib"
		targetdir "bin/%{cfg.buildcfg}"
		
		files {
            "src/ArcEngine/**.h",
            "src/ArcEngine/**.cpp",
            "src/ArcEngine/**.hpp",
        }
		
		links { "%{vulkan_sdk}/lib/vulkan-1.lib" }
		
		links { "../Arcanum/Library/GLFW/lib-vc2022/glfw3.lib"}
		
		includedirs { "$(VULKAN_SDK)/include" }
		includedirs { "../Arcanum/Library/GLFW/include" }
		includedirs { "../Arcanum/Library/GLM" }
		includedirs { "../Arcanum/Library/TinyGLTF" }
		includedirs { "../Arcanum/Library/TinyObj" }
		includedirs { "../Arcanum/src/ArcEngine" }

	project "Shaders"
		kind "Utility"
		targetdir "bin/%{cfg.buildcfg}"

		files{
			    "src/Shaders/**.frag",
			    "src/Shaders/**.vert"
		}

		filter 'files:**.frag'
            buildmessage 'Compiling %{file.relpath}'

            buildcommands{ "%{vulkan_sdk}/Bin/glslc.exe %{file.abspath} -o %{wks.location}src/shaders/compiled_shaders/%{file.basename}.frag.spv" }

            buildoutputs { "ArcEngine/shaders/compiled_shaders/%{file.basename}.frag.spv" }

		filter 'files:**.vert'
            buildmessage 'Compiling %{file.relpath}'

            buildcommands{ "%{vulkan_sdk}/Bin/glslc.exe %{file.abspath} -o %{wks.location}src/shaders/compiled_shaders/%{file.basename}.vert.spv" }

            buildoutputs { "ArcEngine/shaders/compiled_shaders/%{file.basename}.vert.spv" }