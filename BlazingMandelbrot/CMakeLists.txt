# This is an example of how to use FetchContent to easily
# manage 3rd-party dependencies
# First, check out src/CMakeLists.txt - it's really simple!
# Then, check out dependencies directory, it get a bit more complex
# there, but you'll see how it goes
#
# Options:
# * USE_SYSTEM_DEPS - try to find dependencies using find_package (OFF by default)
# * LINK_DEPS_STATIC - link to dependencies statically (ON by default)
#
cmake_minimum_required(VERSION 3.12)

project(Blazing-Mandelbrot
  VERSION 1.2
  LANGUAGES CXX
)

add_subdirectory(dependencies)
add_subdirectory(src)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/src/)

file(COPY src/frag.glsl DESTINATION ${CMAKE_BINARY_DIR}/src)
file(COPY src/mainicon.png DESTINATION ${CMAKE_BINARY_DIR}/src)
