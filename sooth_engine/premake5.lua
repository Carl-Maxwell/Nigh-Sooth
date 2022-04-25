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
    "%{IncludeDir.glfw}/include",
    "%{IncludeDir.glew}/include"
  }

  libdirs{
    "%{IncludeDir.glew}/lib/Release/x64",
  }

  links {
    "lapse_lib",    -- reference to vs project
    "glfw",         -- reference to vs project
    "glew32s",      -- reference to specific .lib
    "opengl32.lib"  -- reference to specific .lib
  }

  defines{
    -- "GLFW_INCLUDE_NONE"
  }

  -- prebuildcommands
  -- {
  --   ("{COPY} %{IncludeDir.glew}\\lib\\Release\\x64" .. " " .. "%{cfg.buildtarget.directory}")
  -- }
