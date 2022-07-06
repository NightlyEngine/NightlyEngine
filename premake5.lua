ThirdPartyPath = "%{wks.location}/Engine/Source/ThirdParty"
RuntimePath = "%{wks.location}/Engine/Source/Runtime"
BuildPath = "%{wks.location}/Engine/Build/%{wks.buildcfg}"

ThirdParty = {}
ThirdParty["glad"] = 	(ThirdPartyPath .. "/glad")
ThirdParty["glfw"] = 	(ThirdPartyPath .. "/glfw")
ThirdParty["glm"] = 	(ThirdPartyPath .. "/glm")
ThirdParty["imgui"] = 	(ThirdPartyPath .. "/imgui")

workspace "NightlyEngine"
	architecture "x64"
	cppdialect "C++20"

	targetdir "Engine/Binaries/%{cfg.buildcfg}"

	configurations
	{
		"Debug",
		"Release"
	}

group "ThirdParty"
	include "Engine/Source/ThirdParty/glad"
	include "Engine/Source/ThirdParty/glfw"
	include "Engine/Source/ThirdParty/imgui"
group ""

include "Engine/Source/Runtime"
include "Engine/Source/Editor"
include "Engine/Source/Plugins"