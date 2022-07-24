ThirdPartyPath = "%{wks.location}/Engine/Source/ThirdParty"
RuntimePath = "%{wks.location}/Engine/Source/Runtime"
BuildPath = "%{wks.location}/Engine/Build/%{wks.buildcfg}"

--Imgui neeeds to include glfw, which is one level above
GlfwInclude = "../glfw/include"

ThirdParty = {}
ThirdParty["glad"] = 	(ThirdPartyPath .. "/glad")
ThirdParty["glfw"] = 	(ThirdPartyPath .. "/glfw")
ThirdParty["glm"] = 	(ThirdPartyPath .. "/glm")
ThirdParty["imgui"] = 	(ThirdPartyPath .. "/imgui")

newoption
{
	trigger = "platform",
	value = "platform",
	description = "Choose an architecture for building the project",
	allowed = 
	{
		{ "x64", "64-bit" },
		{ "ARM64", "Arm 64-bit" }
	},
	default = "x64"
}

workspace "NightlyEngine"
	architecture (_OPTIONS["platform"])
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