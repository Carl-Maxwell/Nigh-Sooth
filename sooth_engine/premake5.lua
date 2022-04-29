project "sooth_engine"
  kind "StaticLib"
  language "C++"
  cppdialect "C++20"

  targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
  objdir ("%{wks.location}/bin-intermediate/" .. outputdir .. "/%{prj.name}")

  files
  {
    "sooth.cpp",
    "sooth.h",
    "src/**.h",
    "src/**.cpp",
    "%{IncludeDir.stb_image}/**.cpp",
  }

  includedirs{
    "src/",
    "%{wks.location}/lapse_lib/src",
    -- "%{IncludeDir.glfw}/include",
    -- "%{IncludeDir.glew}/include",
    "%{IncludeDir.stb_image}/",
    "%{IncludeDir.pixel_game_engine}/"
  }

  -- libdirs{
  --   "%{IncludeDir.glew}/lib/Release/x64",
  -- }

  links {
    "lapse_lib",    -- reference to vs project
    -- "glfw",         -- reference to vs project
    -- "glew32s",      -- reference to specific .lib
    "opengl32.lib"  -- reference to specific .lib
  }

  defines{
    -- "GLFW_INCLUDE_NONE"
  }

  -- prebuildcommands
  -- {
  --   ("{COPY} %{IncludeDir.glew}\\lib\\Release\\x64" .. " " .. "%{cfg.buildtarget.directory}")
  -- }
