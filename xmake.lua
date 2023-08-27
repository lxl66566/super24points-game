set_project("super24points")
set_version("2.1.1")
set_languages("cxx20")
set_encodings("utf-8")
add_rules("mode.debug", "mode.release", "mode.minsizerel")
add_requires("fmt")

toolchain("myqt")
  set_kind("standalone")
  if is_os("linux") then
    set_sdkdir("~/program/qt/6.6.0/gcc_64/")
    set_toolset("cxx", "clang")
    add_linkdirs("/usr/lib/qt6/")
  else
    set_toolset("cxx", "msvc")
  end

target("main")
  set_toolchains("myqt")
  add_rules("qt.widgetapp")
  add_packages("fmt")
  add_files("src/*.cpp")
  add_files("src/*.h")
  add_files("src/*.ui")
  add_files("static/static.qrc")
  add_files("lib/dark/darkstyle.qrc")
  add_packages("lib/expected.hpp")

target("test")
  set_default(false)
  add_files("test/*.cpp")
  add_packages("lib/expected.hpp")
