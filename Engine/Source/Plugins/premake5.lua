project "Plugins"
	kind "SharedLib"
	language "C++"
	objdir "%{BuildPath}"

	pchheader "%{RuntimePath}/Core/PCH.h"
	pchsource "%{RuntimePath}/Core/PCH.cpp"

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