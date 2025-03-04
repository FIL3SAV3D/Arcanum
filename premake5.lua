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
            "src/Framework/**.hpp"
        }

		links { "%{vulkan_sdk}/lib/vulkan-1.lib" }
		
		links { "../Arcanum/Library/GLFW/lib-vc2022/glfw3.lib"}
		links { "ArcEngine" }
		links { "Ws2_32.lib" }


		includedirs { "$(VULKAN_SDK)/include" }
		includedirs { "../Arcanum/Library/GLFW/include" }
		includedirs { "../Arcanum/Library/GLM" }
		includedirs { "../Arcanum/src/ArcEngine" }
		includedirs { "../Arcanum/src/Jolt" }



	project "ArcEngine"
		kind "StaticLib"
		targetdir "bin/%{cfg.buildcfg}"
		dependson { "Jolt", ... }
		
		files {
            "src/ArcEngine/**.h",
            "src/ArcEngine/**.cpp",
            "src/ArcEngine/**.hpp",

			"src/ImGui/*.h",
			"src/ImGui/*.cpp",
			"src/ImGui/*.hpp",

			"src/ImGui/backends/imgui_impl_vulkan.cpp",
			"src/ImGui/backends/imgui_impl_vulkan.h",
			"src/ImGui/backends/imgui_impl_glfw.cpp",
			"src/ImGui/backends/imgui_impl_glfw.h",

			"misc/debuggers/imgui.natvis",
			"misc/debuggers/imgui.natstepfilter",
			"misc/cpp/imgui_stdlib.*"
        }
		
		links { "%{vulkan_sdk}/lib/vulkan-1.lib" }
		links { "../Arcanum/Library/GLFW/lib-vc2022/glfw3.lib"}
		links { "Jolt" }

		
		includedirs { "$(VULKAN_SDK)/include" }
		includedirs { "../Arcanum/Library/GLFW/include" }
		includedirs { "../Arcanum/Library/GLM" }
		includedirs { "../Arcanum/Library/TinyGLTF" }
		includedirs { "../Arcanum/Library/TinyObj" }
		includedirs { "../Arcanum/src/ArcEngine" }
		includedirs { "../Arcanum/src/Jolt" }
		includedirs { "../Arcanum/src/ImGui" }

	project "Jolt"
		kind "StaticLib"
		targetdir "bin/%{cfg.buildcfg}"

		files{
			"src/Jolt/**.h",
			"src/Jolt/**.cpp",
			"src/Jolt/**.hpp"
		}

		includedirs { "../Arcanum/src/Jolt" }

	project "ServerTest"
		kind "ConsoleApp"
		targetdir "bin/%{cfg.buildcfg}"

		files{
			"src/ServerTest/**.h",
			"src/ServerTest/**.cpp",
			"src/ServerTest/**.hpp"
		}
		links { "Ws2_32.lib" }
		
		includedirs { "../Arcanum/src/ServerTest" }
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