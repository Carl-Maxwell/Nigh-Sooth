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
IncludeDir["glfw"] = "%{wks.location}/sooth_engine/vendor/glfw"
IncludeDir["glew"] = "%{wks.location}/sooth_engine/vendor/glew-2.1.0"

include "sooth_engine/vendor/glfw"
-- include "sooth_engine/vendor/glew" -- it's a pre-built release : (
include "lapse_lib"
include "lapse_test"
include "sooth_engine"
