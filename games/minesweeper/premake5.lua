project "minesweeper"
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
    "src/",
    "%{wks.location}/lapse_lib/src",
    "%{wks.location}/sooth_engine",
    "%{wks.location}/sooth_engine/src"
  }

  links {
    "lapse_lib",
    "sooth_engine"
  }
