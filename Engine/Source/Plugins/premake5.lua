project "Plugins"
	kind "SharedLib"
	language "C++"
	objdir "%{BuildPath}"

	pchheader "PCH.h"
	pchsource "%{RuntimePath}/PCH.cpp"

	files
	{
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