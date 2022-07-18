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


	filter { "configurations:Debug", "system:windows" }
		buildoptions "/MDd"

	filter { "configurations:Release", "system:windows" }
		buildoptions "/MD"

	filter "configurations:Debug"
		defines "DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines "NDEBUG"
		optimize "on"