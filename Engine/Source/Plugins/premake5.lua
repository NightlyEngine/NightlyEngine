project "Plugins"
	kind "SharedLib"
	language "C++"
	objdir "%{BuildPath}"

	pchheader "PCH.h"
	pchsource "PCH.cpp"

	files
	{
		"PCH.cpp",
		"TestPlugin/**.h",
		"TestPlugin/**.cpp"
	}

	includedirs
	{
		"%{RuntimePath}"
	}

	links
	{
		"NightlyEngine"
	}

	filter "system:windows"
		buildoptions
		{
			"/FI\"PCH.h\""
		}