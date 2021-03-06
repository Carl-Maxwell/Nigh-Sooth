project "lapse_lib"
  kind "StaticLib"
  language "C++"
  cppdialect "C++20"

  targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
  objdir ("%{wks.location}/bin-intermediate/" .. outputdir .. "/%{prj.name}")

  files
  {
    "src/**.h",
    "src/**.cpp",
  }
