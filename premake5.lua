os.execute[[shader_compile.bat]]

cppdialect "c++17"

workspace "Arcanum"  
configurations { "Debug", "Release" } 
platforms { "x64" }


project "ArcEngine"
kind "ConsoleApp"   
language "C++"   
targetdir "bin/%{cfg.buildcfg}" 
files { "**.h", "**.cpp" } 


links { "$(VULKAN_SDK)/lib/vulkan-1.lib"}

links { "../Arcanum/Library/GLFW/lib-vc2022/glfw3.lib"}

includedirs { "$(VULKAN_SDK)/include" }
includedirs { "../Arcanum/Library/GLFW/include" }
includedirs { "../Arcanum/Library/GLM" }
includedirs { "../Arcanum/Library/TinyGLTF" }
includedirs { "../Arcanum/Library/TinyObj" }
includedirs { "../Arcanum/ArcEngine" }

filter "configurations:Debug"
defines { "DEBUG" }  
symbols "On" 

filter "configurations:Release"  
defines { "NDEBUG" }    
optimize "On" 

