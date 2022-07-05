project "glad"
	kind "StaticLib"
	language "C"
	objdir "%{BuildPath}"

	files
	{
		"include/glad/glad.h",
		"include/KHR/khrplatform.h",
		"src/glad.c"
	}

	sysincludedirs "include"