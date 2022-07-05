project "NightlyEngine"
	kind "SharedLib"
	language "C++"
	objdir "%{BuildPath}"

	pchheader "Core/PCH.h"
	pchsource "Core/PCH.cpp"

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

	filter "configurations:Debug"
		defines "DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines "NDEBUG"
		optimize "on"