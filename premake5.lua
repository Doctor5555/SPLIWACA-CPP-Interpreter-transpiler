workspace "SPLIWACA"
	architecture "x64"
	
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["spdlog"] = "Spliwaca/vendor/spdlog/include"
IncludeDir["timing"] = "Spliwaca/vendor/timing"

startproject "Spliwaca"

--group "Dependencies"
--	include "Supernova/vendor/GLFW"
--	include "Supernova/vendor/Glad"
--	include "Supernova/vendor/imgui"

group ""

project "Spliwaca"
	location "Spliwaca"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	--pchheader "splwpch.h"
	--pchsource "Spliwaca/src/splwpch.cpp"
	
	files
	{
		"c:/dev/EPQ Spliwaca/Spliwaca/Spliwaca/src/**.h",
		"c:/dev/EPQ Spliwaca/Spliwaca/Spliwaca/src/**.cpp",
		"c:/dev/EPQ Spliwaca/Spliwaca/vendor/timing/**.h",
		"c:/dev/EPQ Spliwaca/Spliwaca/vendor/spdlog/include/**.h"
	}
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}
	
	includedirs
	{
		"C:/dev/EPQ Spliwaca/Spliwaca/Spliwaca/src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.timing}"
	}
	
--[[	links
	{
		"GLFW",
		"GLAD",
		"imgui",
		"opengl32.lib"
	}
	
	filter "system:windows"
		systemversion "latest"
		
		defines
		{
			"SN_PLATFORM_WINDOWS",
			"SN_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}
		
		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
		}]]
		
	filter "configurations:Debug"
		defines "SPLW_DEBUG"
		runtime "Debug"
		symbols "on"
		
	filter "configurations:Release"
		defines "SPLW_RELEASE"
		runtime "Release"
		optimize "on"
	
	filter "configurations:Dist"
		defines "SPLW_DIST"
		runtime "Release"
		optimize "on"
		
--[[
project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	
	includedirs
	{
		"Supernova/src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.glm}",
		"Supernova/vendor"
	}
	
	links
	{
		"Supernova"
	}
	
	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"
		
		defines
		{
			"SN_PLATFORM_WINDOWS",
		}
		
	filter "configurations:Debug"
		defines "SN_DEBUG"
		symbols "on"
		
	filter "configurations:Release"
		defines "SN_RELEASE"
		optimize "on"
	
	filter "configurations:Dist"
		defines "SN_DIST"
		optimize "on"
]]