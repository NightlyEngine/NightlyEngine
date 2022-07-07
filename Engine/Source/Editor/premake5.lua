project "NightlyEditor"
	kind "ConsoleApp"
	language "C++"
	objdir "%{BuildPath}"

	pchheader "%{RuntimePath}/Core/PCH.h"
	pchsource "%{RuntimePath}/Core/PCH.cpp"

	files
	{
		"**.h",
		"**.cpp"
	}

	includedirs
	{
		".",
		"%{RuntimePath}"
	}

	sysincludedirs
	{
		"%{ThirdParty.imgui}",
		"%{ThirdParty.glm}",
		"%{ThirdParty.glfw}/include"
	}

	links
	{
		"NightlyEngine",
		"imgui",
		"glfw",
		"dl",
		"pthread"
	}