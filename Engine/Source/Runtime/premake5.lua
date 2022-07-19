project "NightlyEngine"
	kind "StaticLib"
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
		"."
	}

	sysincludedirs
	{
		"%{ThirdParty.glfw}/include",
		"%{ThirdParty.glad}/include",
		"%{ThirdParty.imgui}",
		"%{ThirdParty.glm}"
	}

	links
	{
		"glad",
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

	filter "configurations:Debug"
		defines "DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines "NDEBUG"
		optimize "on"