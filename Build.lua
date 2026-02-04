VULKAN_SDK = os.getenv("VULKAN_SDK")

workspace "Arcanum"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "Standalone"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "EntryPoints"
	include "Source/ArcanumEditor/Build-ArcanumEditor.lua"
	include "Source/Standalone/Build-Standalone.lua"
group ""

group "Game"
	include "Source/Game/Build-Game.lua"
group ""

group "Framework"
	include "Source/Framework/Build-Framework.lua"
group ""

group "ArcEngine"
	include "Source/ArcEngine/Build-ArcEngine.lua"
group ""

group "ThirdParty"
	include "Source/ThirdParty/Build-JoltPhysics.lua"
	include "Source/ThirdParty/Build-DearImGUI.lua"
group ""