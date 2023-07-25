add_executable(Blazing-Mandelbrot
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

# Yep, that's it!
target_link_libraries(Blazing-Mandelbrot
  PUBLIC
    ImGui-SFML::ImGui-SFML
)

include(Install.cmake)
