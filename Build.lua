VULKAN_SDK = os.getenv("VULKAN_SDK")

StaticLinking = "off";

workspace "Arcanum"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "Standalone"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}"
BuildDir = "../../Build/"

group "00_EntryPoints"
	include "Source/00_Arcanum/Build-Arcanum.lua"
	include "Source/00_StandaloneGame/Build-StandaloneGame.lua"
group ""

group "01_Editor"
	include "Source/01_Editor/Build-Editor.lua"
group ""

group "01_Game"
	include "Source/01_Game/Build-Game.lua"
group ""

group "02_Framework"
	include "Source/02_Framework/Build-Framework.lua"
group ""

group "03_ArcEngine"
	include "Source/03_ArcEngine/Build-ArcEngine.lua"
group ""

group "RES_Shaders"
	include "Source/RES_Shaders/ShadersOpenGL/Build-ShadersOpenGL.lua"
	include "Source/RES_Shaders/ShadersVulkan/Build-ShadersVulkan.lua"
group ""

group "RES_ThirdParty"
	include "Source/RES_ThirdParty/Build-JoltPhysics.lua"
	include "Source/RES_ThirdParty/Build-DearImGUI.lua"
group ""

