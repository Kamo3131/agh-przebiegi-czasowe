project(Oscillator CXX)

set(INCLUDES_OSCILLATOR ${PROJECT_SOURCE_DIR}/include)
include_directories(${INCLUDES_OSCILLATOR})

file(GLOB SOURCES_OSCILLATOR "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_executable(${PROJECT_NAME} Main.cpp ${SOURCES_OSCILLATOR})

target_include_directories(Oscillator PRIVATE 
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(${PROJECT_NAME} 
    PRIVATE
      # ImGUI_SFML
      sfml-graphics
      sfml-window
      sfml-system
      imgui
      imgui-sfml
      implot
)