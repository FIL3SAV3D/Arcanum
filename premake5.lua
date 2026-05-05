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
            "source/Framework/**.h",
            "source/Framework/**.cpp",
            "source/Framework/**.hpp",

			"submodules/imgui/*.h",
			"submodules/imgui/*.cpp",

			"submodules/imgui/backends/imgui_impl_opengl3.h",
			"submodules/imgui/backends/imgui_impl_opengl3.cpp",

			"submodules/imgui/backends/imgui_impl_glfw.h",
			"submodules/imgui/backends/imgui_impl_glfw.cpp",
        }

		filter {"configurations:Debug"}
			links { "%{cwd}/library/Assimp/lib/assimp-vc143-mtd.lib"}

		filter {"configurations:Release"}
			links { "%{cwd}/library/Assimp/lib/assimp-vc143-mt.lib"}
		
		filter{}

		links { "%{vulkan_sdk}/lib/vulkan-1.lib" }
		
		links { "%{cwd}/library/GLFW/lib-vc2022/glfw3.lib"}
		links { "ArcEngine" }


		includedirs { "$(VULKAN_SDK)/include" }
		includedirs { "%{cwd}/library/GLFW/include" }
		includedirs { "%{cwd}/library/GLM" }
		includedirs { "%{cwd}/source/ArcEngine" }
		includedirs { "%{cwd}/source/Jolt" }
		includedirs { "%{cwd}/library/asio-1.34.2/include" }
		includedirs { "%{cwd}/library/GLAD/include" }
		includedirs { "%{cwd}/library/std_image" }
		includedirs { "%{cwd}/library/Assimp/include" }

		-- Submodules
		includedirs { "%{cwd}/submodules/imgui" }


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

			"submodules/imgui/backends/imgui_impl_opengl3.h",
			"submodules/imgui/backends/imgui_impl_opengl3.cpp",

			"submodules/imgui/backends/imgui_impl_glfw.h",
			"submodules/imgui/backends/imgui_impl_glfw.cpp",
        }

		filter {"configurations:Debug"}
			links { "%{cwd}/library/Assimp/lib/assimp-vc143-mtd.lib"}

		filter {"configurations:Release"}
			links { "%{cwd}/library/Assimp/lib/assimp-vc143-mt.lib"}
		
		filter{}

		links { "%{vulkan_sdk}/lib/vulkan-1.lib" }
		links { "%{cwd}/library/GLFW/lib-vc2022/glfw3.lib"}
		links { "Jolt" }
		
		
		includedirs { "$(VULKAN_SDK)/include" }
		includedirs { "%{cwd}/library/GLFW/include" }
		includedirs { "%{cwd}/library/GLM" }
		includedirs { "%{cwd}/library/TinyGLTF" }
		includedirs { "%{cwd}/library/TinyObj" }
		includedirs { "%{cwd}/source/ArcEngine" }
		includedirs { "%{cwd}/source/Jolt" }
		includedirs { "%{cwd}/library/asio-1.34.2/include" }
		includedirs { "%{cwd}/library/GLAD/include" }
		includedirs { "%{cwd}/library/std_image" }
		includedirs { "%{cwd}/library/Assimp/include" }

		-- Submodules
		includedirs { "%{cwd}/submodules/imgui" }
		
	project "Jolt"
		kind "StaticLib"
		targetdir "bin/%{cfg.buildcfg}"

		files{
			"source/Jolt/**.h",
			"source/Jolt/**.cpp",
			"source/Jolt/**.hpp"
		}

		includedirs { "%{cwd}/source/Jolt" }
		includedirs { "%{cwd}/source/Jolt/Jolt" }

	project "ArcNetCommon"
		kind "StaticLib"
		targetdir "bin/%{cfg.buildcfg}"

		files{
			"source/ArcNetCommon/**.h",
			"source/ArcNetCommon/**.cpp",
			"source/ArcNetCommon/**.hpp"
		}
		
		includedirs { "%{cwd}/source/ArcNetCommon" }
		includedirs { "%{cwd}/library/asio-1.34.2/include" }
		
	project "Shaders"
		kind "Utility"
		targetdir "bin/%{cfg.buildcfg}"

		files{
			    "source/Shaders/**.frag",
			    "source/Shaders/**.vert"
		}

		-- filter 'files:**.frag'
        --     buildmessage 'Compiling %{file.relpath}'

        --     buildcommands{ "%{vulkan_sdk}/Bin/glslc.exe %{file.abspath} -o -c %{wks.location}source/shaders/compiled_shaders/%{file.basename}.frag.spv" }

        --     buildoutputs { "source/shaders/compiled_shaders/%{file.basename}.frag.spv" }

		-- filter 'files:**.vert'
        --     buildmessage 'Compiling %{file.relpath}'

        --     buildcommands{ "%{vulkan_sdk}/Bin/glslc.exe %{file.abspath} -o -c %{wks.location}source/shaders/compiled_shaders/%{file.basename}.vert.spv" }

        --     buildoutputs { "source/shaders/compiled_shaders/%{file.basename}.vert.spv" }