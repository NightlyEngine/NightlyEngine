project "NightlyEngine"
	kind "SharedLib"
	language "C++"
	objdir "%{BuildPath}"

	pchheader "PCH.h"
	pchsource "PCH.cpp"

	files
	{
		"**.h",
		"**.cpp"
	}

	defines
	{
		"NL_EXPORT_DLL"
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

		filter "configurations:Debug"
			buildoptions "/MDd"

		filter "configurations:Release"
			buildoptions "/MD"

	filter "configurations:Debug"
		defines "DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines "NDEBUG"
		optimize "on"