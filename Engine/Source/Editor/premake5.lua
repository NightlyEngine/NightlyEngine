project "NightlyEditor"
	kind "ConsoleApp"
	language "C++"
	objdir "%{BuildPath}"

	pchheader "PCH.h"
	pchsource "%{RuntimePath}/PCH.cpp"

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