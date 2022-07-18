project "NightlyEditor"
	kind "ConsoleApp"
	language "C++"
	objdir "%{BuildPath}"

	pchheader "PCH.h"
	pchsource "PCH.cpp"

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
		"glfw"
	}

	filter "system:windows"
		buildoptions
		{
			"/FI\"PCH.h\""
		}

	filter { "configurations:Debug", "system:windows" }
		buildoptions "/MDd"

	filter { "configurations:Release", "system:windows" }
		buildoptions "/MD"

	filter "system:linux"
		links
		{
			"dl",
			"pthread"
		}