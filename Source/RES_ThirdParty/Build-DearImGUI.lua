project "DearImGUI"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
	targetdir "Binaries/%{cfg.buildcfg}"
	staticruntime (StaticLinking)

    files 
    {
        "DearImGUI/*.h", 
        "DearImGUI/*.cpp",

		"DearImGUI/backends/imgui_impl_opengl3.h",
		"DearImGUI/backends/imgui_impl_opengl3.cpp",

		"DearImGUI/backends/imgui_impl_vulkan.h",
		"DearImGUI/backends/imgui_impl_vulkan.cpp",

		"DearImGUI/backends/imgui_impl_glfw.h",
		"DearImGUI/backends/imgui_impl_glfw.cpp",

        "DearImGUI/backends/imgui_impl_sdl3.h",
		"DearImGUI/backends/imgui_impl_sdl3.cpp",

    }

    includedirs
    {
        "DearImGUI",

        "../../Library/GLM",
        "../../Library/Assimp/include",
        "../../Library/asio-1.34.2/include",
        "../../Library/GLAD/include",
        "../../Library/GLFW/include",
        "../../Library/std_image",

        "../../Library/SDL/include",


        "$(VULKAN_SDK)/include",
    }

    targetdir (BuildDir .. OutputDir )
    objdir (BuildDir .. "Intermediates/" .. OutputDir .. "/%{prj.name}")

    filter "system:windows"
       systemversion "latest"
       defines { }

    filter "configurations:Debug"
       defines { "DEBUG" }
       runtime "Debug"
       symbols "On"

    filter "configurations:Release"
       defines { "RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "On"

    filter "configurations:Dist"
       defines { "DIST" }
       runtime "Release"
       optimize "On"
       symbols "Off"