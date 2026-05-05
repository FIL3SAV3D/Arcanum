vulkan_sdk = os.getenv("VULKAN_SDK")
cwd = os.getcwd()

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
		
		links { "%{cwd}/Library/GLFW/lib-vc2022/glfw3.lib"}
		links { "ArcEngine" }


		includedirs { "$(VULKAN_SDK)/include" }
		includedirs { "%{cwd}/Library/GLFW/include" }
		includedirs { "%{cwd}/Library/GLM" }
		includedirs { "%{cwd}/src/ArcEngine" }
		includedirs { "%{cwd}/src/Jolt" }


	project "ArcEngine"
		kind "StaticLib"
		targetdir "bin/%{cfg.buildcfg}"
		dependson { "Jolt", ... }
		
		files {
            "src/ArcEngine/**.h",
            "src/ArcEngine/**.cpp",
            "src/ArcEngine/**.hpp",
        }
		
		links { "%{vulkan_sdk}/lib/vulkan-1.lib" }
		
		links { "%{cwd}/Library/GLFW/lib-vc2022/glfw3.lib"}
		links { "Jolt" }

		
		includedirs { "$(VULKAN_SDK)/include" }
		includedirs { "%{cwd}/Library/GLFW/include" }
		includedirs { "%{cwd}/Library/GLM" }
		includedirs { "%{cwd}/Library/TinyGLTF" }
		includedirs { "%{cwd}/Library/TinyObj" }
		includedirs { "%{cwd}/src/ArcEngine" }
		includedirs { "%{cwd}/src/Jolt" }

	project "Jolt"
		kind "StaticLib"
		targetdir "bin/%{cfg.buildcfg}"

		files{
			"src/Jolt/**.h",
			"src/Jolt/**.cpp",
			"src/Jolt/**.hpp"
		}

		includedirs { "%{cwd}/src/Jolt" }

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

            buildoutputs { "src/shaders/compiled_shaders/%{file.basename}.frag.spv" }

		filter 'files:**.vert'
            buildmessage 'Compiling %{file.relpath}'

            buildcommands{ "%{vulkan_sdk}/Bin/glslc.exe %{file.abspath} -o %{wks.location}src/shaders/compiled_shaders/%{file.basename}.vert.spv" }

            buildoutputs { "src/shaders/compiled_shaders/%{file.basename}.vert.spv" }