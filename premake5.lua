workspace "nigh_sooth"
  architecture "x64"

  configurations
  {
    "debug",
    "release",
    "dist"
  }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
-- IncludeDir["glfw"] = "%{wks.location}/sooth_engine/vendor/glfw"
-- IncludeDir["glew"] = "%{wks.location}/sooth_engine/vendor/glew-2.1.0"
IncludeDir["stb_image"] = "%{wks.location}/sooth_engine/vendor/stb_image"
IncludeDir["pixel_game_engine"] = "%{wks.location}/sooth_engine/vendor/olcPixelGameEngine"

-- include "sooth_engine/vendor/glfw"
-- include "sooth_engine/vendor/glew" -- it's a pre-built release : (
include "lapse_lib"
include "lapse_test"
include "sooth_engine"

group "games"
  include "games/minesweeper"
group ""
