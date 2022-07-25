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

	filter "system:macosx"
		pchheader "%{RuntimePath}/PCH.h"

	filter "system:windows"
		buildoptions
		{
			"/FI\"PCH.h\""
		}

		staticruntime "off"

	filter { "configurations:Debug", "system:windows" }
		runtime "Debug"

	filter { "configurations:Release", "system:windows" }
		runtime "Release"

	filter "system:linux"
		pic "on"