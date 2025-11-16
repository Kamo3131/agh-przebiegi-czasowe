project(Generator CXX)

set(INCLUDES_GENERATOR ${PROJECT_SOURCE_DIR}/include)
include_directories(${INCLUDES_GENERATOR})

file(GLOB SOURCES_GENERATOR "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_executable(${PROJECT_NAME} Main.cpp ${SOURCES_GENERATOR})

target_include_directories(Generator PRIVATE 
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