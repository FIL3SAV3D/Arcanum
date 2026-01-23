
project "Framework"
		kind "StaticLib"
		targetdir "bin/%{cfg.buildcfg}"
		dependson { "ArcEngine", ... }

		files {
            "source/Framework/**.h",
            "source/Framework/**.cpp",
            "source/Framework/**.hpp",
        }

		filter {"configurations:Debug"}
			links { "../Arcanum/library/Assimp/lib/assimp-vc143-mtd.lib"}

		filter {"configurations:DebugAsan"}
			links { "../Arcanum/library/Assimp/lib/assimp-vc143-mtd.lib"}

		filter {"configurations:Release"}
			links { "../Arcanum/library/Assimp/lib/assimp-vc143-mt.lib"}
		
		filter{}

		links { "%{vulkan_sdk}/lib/vulkan-1.lib" }
		
		links { "../Arcanum/library/GLFW/lib-vc2022/glfw3.lib"}
		links { "ArcEngine" }
		links { "ArcNetCommon" }

		includedirs { "$(VULKAN_SDK)/include" }
		includedirs { "../Arcanum/library/GLFW/include" }
		includedirs { "../Arcanum/library/GLM" }
		includedirs { "../Arcanum/source/ArcEngine" }
		includedirs { "../Arcanum/source/Framework" }
		includedirs { "../Arcanum/source/Jolt" }
		includedirs { "../Arcanum/source/ArcNetCommon" }
		includedirs { "../Arcanum/library/asio-1.34.2/include" }
		includedirs { "../Arcanum/library/GLAD/include" }
		includedirs { "../Arcanum/library/std_image" }
		includedirs { "../Arcanum/library/Assimp/include" }

		-- Submodules
		includedirs { "../Arcanum/submodules/imgui" }