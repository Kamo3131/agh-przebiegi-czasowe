cmake_minimum_required(VERSION 4.0.0)
project(MySFMLProjects CXX)
cmake_policy(SET CMP0169 OLD)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall")
set(CMAKE_CXX_FLAGS_RELEASE_NIT "-Wall")

include(FetchContent)

# --------------------------------------------------------------------------
# Save all static libs to Lib/
# --------------------------------------------------------------------------
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Lib)

#============================================================================
# SFML 3.0.2 (STATIC)
#============================================================================
set(SFML_STATIC_LIBRARIES ON)
set(BUILD_SHARED_LIBS OFF)
set(SFML_BUILD_EXAMPLES OFF)
set(SFML_BUILD_TESTS OFF)
set(SFML_BUILD_DOC OFF)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
)
FetchContent_MakeAvailable(SFML)

# Required for static SFML
add_compile_definitions(SFML_STATIC)

#============================================================================
# ImGui (STATIC)
#============================================================================
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.1
)
FetchContent_MakeAvailable(imgui)

file(GLOB IMGUI_SRC
    ${imgui_SOURCE_DIR}/*.cpp
)

add_library(imgui STATIC ${IMGUI_SRC})
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})

#============================================================================
# ImGui-SFML (STATIC) — manual build, bypass CMakeLists.txt
#============================================================================
FetchContent_Declare(
    imgui_sfml
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
    GIT_TAG v3.0
)
FetchContent_Populate(imgui_sfml)

# Build ImGui-SFML manually
add_library(imgui-sfml STATIC
    ${imgui_sfml_SOURCE_DIR}/imgui-SFML.cpp
)

target_include_directories(imgui-sfml PUBLIC
    ${imgui_sfml_SOURCE_DIR}
    ${imgui_SOURCE_DIR}
    ${SFML_SOURCE_DIR}/include
)

target_link_libraries(imgui-sfml PUBLIC
    imgui
    sfml-graphics
    sfml-window
    sfml-system
)

#============================================================================
# Implot (STATIC)
#============================================================================
FetchContent_Declare(
    implot
    GIT_REPOSITORY https://github.com/epezent/implot.git
    GIT_TAG v0.16     # or latest stable
)
FetchContent_MakeAvailable(implot)

file(GLOB IMPLOT_SRC
    ${implot_SOURCE_DIR}/*.cpp
)

add_library(implot STATIC ${IMPLOT_SRC})

target_include_directories(implot PUBLIC
    ${implot_SOURCE_DIR}
    ${imgui_SOURCE_DIR}     # ImPlot uses imgui headers
)

target_link_libraries(implot PUBLIC imgui)

#============================================================================
add_subdirectory(Generator)
add_subdirectory(Oscillator)