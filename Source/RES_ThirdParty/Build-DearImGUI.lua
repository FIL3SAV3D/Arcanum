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

        "DearImGUI/misc/cpp/*.h", 
        "DearImGUI/misc/cpp/*.cpp",

		"DearImGUI/backends/imgui_impl_opengl3.h",
		"DearImGUI/backends/imgui_impl_opengl3.cpp",

		"DearImGUI/backends/imgui_impl_vulkan.h",
		"DearImGUI/backends/imgui_impl_vulkan.cpp",

        "DearImGUI/backends/imgui_impl_sdl3.h",
		"DearImGUI/backends/imgui_impl_sdl3.cpp",

    }

    includedirs
    {
        "DearImGUI",

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