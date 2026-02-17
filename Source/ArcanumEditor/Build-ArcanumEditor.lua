project "ArcanumEditor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
	targetdir "Binaries/%{cfg.buildcfg}"
	staticruntime "off"

    files { "Source/**.h", "Source/**.cpp" }

    includedirs
    {
        "Source",
        "../Game/Source",
        "../Framework/Source",
        "../ArcEngine/Source",
        "../Thirdparty/JoltPhysics-Arcanum",
        "../Thirdparty/DearImGUI",

        "../../Library/GLM",
        "../../Library/Assimp/include",
        "../../Library/asio-1.34.2/include",
        "../../Library/GLAD/include",
        "../../Library/GLFW/include",
        "../../Library/std_image",
        "../../Library/SDL/include",

        "../../Library/fmt/include",
        "../../Library/vma",
        "../../Library/volk",

        "../../Library/SLANG-2026.2.2/include",

        "$(VULKAN_SDK)/include",
    }

    links
    {
        "Game",
        "Framework",
        "ArcEngine",
        "JoltPhysics-Arcanum",
        "DearImGUI",
        "%{VULKAN_SDK}/Lib/vulkan-1.lib",
        "../../Library/GLFW/lib-vc2022/glfw3.lib",
        "../../Library/SDL/lib/SDL3.lib",
        "../../Library/SLANG-2026.2.2/lib/slang.lib",
    }

    targetdir ("../../Binaries/" .. OutputDir .. "/%{prj.name}")
    objdir ("../../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

    filter "system:windows"
       systemversion "latest"
       defines { }

    filter "configurations:Debug"
       defines { "DEBUG" }
       runtime "Debug"
       symbols "On"

       links 
       { 
        "../../Library/Assimp/dll/assimp-vc143-mtd.dll",
        "../../Library/fmt/lib/fmtd.lib",
       }

    filter "configurations:Release"
       defines { "RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "On"

       links 
       { 
        "../../Library/Assimp/dll/assimp-vc143-mt.dll",
        "../../Library/fmt/lib/fmt.lib",
        }

    filter "configurations:Dist"
       defines { "DIST" }
       runtime "Release"
       optimize "On"
       symbols "Off"

       links 
       { 
        "../../Library/Assimp/dll/assimp-vc143-mt.dll",
        }