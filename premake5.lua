include "Dependencies.lua"

workspace "Cubeland"
	architecture "x64"
	startproject "Cubeland"

	configurations 
	{ 
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "vendor/GLFW"
	include "vendor/Glad"
	include "vendor/ImGui"
group ""

include "Cubeland"