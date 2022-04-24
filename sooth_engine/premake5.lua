project "sooth_engine"
  kind "ConsoleApp"
  language "C++"
  cppdialect "C++20"

  targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
  objdir ("%{wks.location}/bin-intermediate/" .. outputdir .. "/%{prj.name}")

  files
  {
    "entry_point.cpp",
    "src/**.h",
    "src/**.cpp",
  }

  includedirs{
    "%{wks.location}/lapse_lib/src",
    "%{IncludeDir.glfw}/include"
  }

  links {
    "lapse_lib",
    "GLFW",
    "opengl32.lib"
  }

  defines{
    -- "GLFW_INCLUDE_NONE"
  }
