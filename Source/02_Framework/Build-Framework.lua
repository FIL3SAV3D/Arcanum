project "Framework"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
	targetdir "Binaries/%{cfg.buildcfg}"
	staticruntime (StaticLinking)

    files { "Source/**.h", "Source/**.cpp" }

    includedirs
    {
        -- Engine hierarchy includes from top to bottom (Both)
        "Source",
        "../03_ArcEngine/Source",

        -- Library includes
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
        "../../Library/zpp_bits-4.6",
        "../../Library/SLANG-2026.2.2/include",

        -- Resource includes
        "../RES_ThirdParty/JoltPhysics-Arcanum",
        "../RES_ThirdParty/DearImGUI",

        -- OS includes
        "$(VULKAN_SDK)/include",
    }

    links
    {
        -- Engine hierarchy links from top to bottom (Both)
        "ArcEngine",

        -- Library links
        "../../Library/GLFW/lib-vc2022/glfw3.lib",
        "../../Library/SDL/lib/SDL3.lib",
        "../../Library/SLANG-2026.2.2/lib/slang.lib",

        -- Resource links
        "JoltPhysics-Arcanum",
        "DearImGUI",

        -- OS links
        "%{VULKAN_SDK}/Lib/vulkan-1.lib",
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