cmake_minimum_required(VERSION 3.24...3.30)
project(platformer2D)

include(FetchContent)

# Dependencies 
set(RAYLIB_VERSION 5.5)
set(RAYGUI_VERSION 4.0)

# Fetch Raylib
FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    FIND_PACKAGE_ARGS
)
FetchContent_MakeAvailable(raylib)

# Fetch Raygui
FetchContent_Declare(
    raygui
    GIT_REPOSITORY https://github.com/raysan5/raygui.git
    GIT_TAG ${RAYGUI_VERSION}
)
FetchContent_MakeAvailable(raygui)

FetchContent_Declare(
    cereal
    URL https://github.com/USCiLab/cereal/archive/refs/tags/v1.3.2.tar.gz
    FIND_PACKAGE_ARGS
)
set(JUST_INSTALL_CEREAL ON CACHE BOOL "Only install cereal" FORCE)
FetchContent_MakeAvailable(cereal)

# Add executable
add_executable(${PROJECT_NAME})
add_subdirectory(src)

# Include raygui header
target_include_directories(${PROJECT_NAME} PRIVATE ${raygui_SOURCE_DIR}/src)

# Link raylib
target_link_libraries(${PROJECT_NAME} raylib cereal)

# Web Configurations
if ("${PLATFORM}" STREQUAL "Web")
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
    target_link_options(${PROJECT_NAME} PUBLIC -sUSE_GLFW=3)
endif()

# MacOS Framework Linking
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()