project "ArcEngine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
	targetdir "Binaries/%{cfg.buildcfg}"
	staticruntime "off"

    files 
    { 
        "Source/**.h", 
        "Source/**.cpp", 
        "Source/**.c"
    }

    includedirs
    {
        "Source",
        "../Thirdparty/JoltPhysics-Arcanum",
        "../Thirdparty/DearImGUI",

        "../../Library/GLM",
        "../../Library/Assimp/include",
        "../../Library/asio-1.34.2/include",
        "../../Library/GLAD/include",
        "../../Library/GLFW/include",
        "../../Library/std_image",
        
        "../../Library/fmt/include",
        "../../Library/vma",
        "../../Library/volk",
        "../../Library/SDL/include",
        "../../Library/SLANG-2026.2.2/include",
        "../../Library/zpp_bits-4.6",

        "$(VULKAN_SDK)/include",
    }

    links
    {
        "JoltPhysics-Arcanum",
        "DearImGUI",
        "%{VULKAN_SDK}/Lib/vulkan-1.lib",

        "../../Library/GLFW/lib-vc2022/glfw3.lib",

        "../../Library/SDL/lib/SDL3.lib",
        "../../Library/SLANG-2026.2.2/lib/slang.lib",
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

       links 
       { 
        "../../Library/fmt/lib/fmtd.lib",
        "../../Library/Assimp/lib/assimp-vc143-mtd.lib",
        "../../Library/Assimp/lib/zlibstaticd.lib"
       }

    filter "configurations:Release"
       defines { "RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "On"

       links 
       { 
        "../../Library/fmt/lib/fmt.lib",
        "../../Library/Assimp/lib/assimp-vc143-mt.lib",
        "../../Library/Assimp/lib/zlibstatic.lib"
        }
    filter "configurations:Dist"
       defines { "DIST" }
       runtime "Release"
       optimize "On"
       symbols "Off"