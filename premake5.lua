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
		dependson { "ArcEngine", "Shaders", "ArcNetCommon", ... }

		files {
            "source/Framework/**.h",
            "source/Framework/**.cpp",
            "source/Framework/**.hpp"
        }

		links { "%{vulkan_sdk}/lib/vulkan-1.lib" }
		
		links { "../Arcanum/library/GLFW/lib-vc2022/glfw3.lib"}
		links { "ArcEngine" }
		links { "ArcNetCommon" }


		includedirs { "$(VULKAN_SDK)/include" }
		includedirs { "../Arcanum/library/GLFW/include" }
		includedirs { "../Arcanum/library/GLM" }
		includedirs { "../Arcanum/source/ArcEngine" }
		includedirs { "../Arcanum/source/Jolt" }
		includedirs { "../Arcanum/source/ArcNetCommon" }
		includedirs { "../Arcanum/library/asio-1.34.2/include" }
		includedirs { "../Arcanum/library/GLAD/include" }
		includedirs { "../Arcanum/library/std_image" }


	project "ArcEngine"
		kind "StaticLib"
		targetdir "bin/%{cfg.buildcfg}"
		dependson { "Jolt", ... }
		
		files {
            "source/ArcEngine/**.h",
            "source/ArcEngine/**.cpp",
            "source/ArcEngine/**.hpp",

			"source/ArcEngine/**.c",

			"submodules/imgui/*.h",
			"submodules/imgui/*.cpp",
			"submodules/imgui/*.hpp",

			"submodules/imgui/backends/imgui_impl_vulkan.cpp",
			"submodules/imgui/backends/imgui_impl_vulkan.h",
			"submodules/imgui/backends/imgui_impl_glfw.cpp",
			"submodules/imgui/backends/imgui_impl_glfw.h",

			"misc/debuggers/imgui.natvis",
			"misc/debuggers/imgui.natstepfilter",
			"misc/cpp/imgui_stdlib.*"
        }
		
		links { "%{vulkan_sdk}/lib/vulkan-1.lib" }
		links { "../Arcanum/library/GLFW/lib-vc2022/glfw3.lib"}
		links { "Jolt" }
		links { "ArcNetCommon" }
		
		
		includedirs { "$(VULKAN_SDK)/include" }
		includedirs { "../Arcanum/library/GLFW/include" }
		includedirs { "../Arcanum/library/GLM" }
		includedirs { "../Arcanum/library/TinyGLTF" }
		includedirs { "../Arcanum/library/TinyObj" }
		includedirs { "../Arcanum/source/ArcEngine" }
		includedirs { "../Arcanum/source/Jolt" }
		includedirs { "../Arcanum/submodules/imgui" }
		includedirs { "../Arcanum/source/ArcNetCommon" }
		includedirs { "../Arcanum/library/asio-1.34.2/include" }
		includedirs { "../Arcanum/library/GLAD/include" }
		includedirs { "../Arcanum/library/std_image" }
		
	project "Jolt"
		kind "StaticLib"
		targetdir "bin/%{cfg.buildcfg}"

		files{
			"source/Jolt/**.h",
			"source/Jolt/**.cpp",
			"source/Jolt/**.hpp"
		}

		includedirs { "../Arcanum/source/Jolt" }

	project "ArcNetCommon"
		kind "StaticLib"
		targetdir "bin/%{cfg.buildcfg}"

		files{
			"source/ArcNetCommon/**.h",
			"source/ArcNetCommon/**.cpp",
			"source/ArcNetCommon/**.hpp"
		}
		
		includedirs { "../Arcanum/source/ArcNetCommon" }
		includedirs { "../Arcanum/library/asio-1.34.2/include" }
		
	project "Shaders"
		kind "Utility"
		targetdir "bin/%{cfg.buildcfg}"

		files{
			    "source/Shaders/**.frag",
			    "source/Shaders/**.vert"
		}

		filter 'files:**.frag'
            buildmessage 'Compiling %{file.relpath}'

            buildcommands{ "%{vulkan_sdk}/Bin/glslc.exe %{file.abspath} -o %{wks.location}source/shaders/compiled_shaders/%{file.basename}.frag.spv" }

            buildoutputs { "source/shaders/compiled_shaders/%{file.basename}.frag.spv" }

		filter 'files:**.vert'
            buildmessage 'Compiling %{file.relpath}'

            buildcommands{ "%{vulkan_sdk}/Bin/glslc.exe %{file.abspath} -o %{wks.location}source/shaders/compiled_shaders/%{file.basename}.vert.spv" }

            buildoutputs { "source/shaders/compiled_shaders/%{file.basename}.vert.spv" }