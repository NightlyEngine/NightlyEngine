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

		staticruntime "off"

	filter { "configurations:Debug", "system:windows" }
		runtime "Debug"

	filter { "configurations:Release", "system:windows" }
		runtime "Release"

	filter "system:linux"
		links
		{
			"dl",
			"pthread"
		}