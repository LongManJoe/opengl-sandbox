cmake_minimum_required(VERSION 3.21)
project(opengl-sandbox LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Put all executables in one place so it is easy to find them.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

option(LEARN_OPENGL_WITH_ASSIMP "Fetch Assimp (needed only for the Model Loading chapters; slow first build)" OFF)

# ---------------------------------------------------------------------------
# Third-party dependencies
# ---------------------------------------------------------------------------
include(FetchContent)

# GLFW -- windowing, context, input.
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
    GIT_SHALLOW    TRUE
)

# GLM -- header-only maths.
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.1
    GIT_SHALLOW    TRUE
)

FetchContent_MakeAvailable(glfw glm)

# GLAD -- OpenGL function loader (vendored, pre-generated for GL 3.3 core).
add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)
target_link_libraries(glad PRIVATE ${CMAKE_DL_LIBS})

# Assimp -- optional, only fetched when model loading is needed.
if(LEARN_OPENGL_WITH_ASSIMP)
    set(ASSIMP_BUILD_TESTS         OFF CACHE BOOL "" FORCE)
    set(ASSIMP_INSTALL             OFF CACHE BOOL "" FORCE)
    set(ASSIMP_BUILD_ASSIMP_TOOLS  OFF CACHE BOOL "" FORCE)
    set(ASSIMP_WARNINGS_AS_ERRORS  OFF CACHE BOOL "" FORCE)
    set(ASSIMP_BUILD_ZLIB          ON  CACHE BOOL "" FORCE)
    FetchContent_Declare(
        assimp
        GIT_REPOSITORY https://github.com/assimp/assimp.git
        GIT_TAG        v5.4.3
        GIT_SHALLOW    TRUE
    )
    FetchContent_MakeAvailable(assimp)
endif()

# ---------------------------------------------------------------------------
# The application: ONE executable built from every .cpp under src/.
#
# Add a new pair like src/mesh.h + src/mesh.cpp, #include "mesh.h" from any
# other file, and it is compiled and linked into the same binary. CLion
# reloads CMake when you add a file (that is what CONFIGURE_DEPENDS enables),
# so the new source is picked up automatically. GLOB_RECURSE means you can
# also organise sources into subfolders of src/ if you like.
# ---------------------------------------------------------------------------
file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")

if(NOT APP_SOURCES)
    message(FATAL_ERROR "No sources found in src/.")
endif()

add_executable(app ${APP_SOURCES})

# Header search paths: include/ (provided helpers) and src/ (your own headers),
# so #include "shader.h" or #include "mesh.h" works from anywhere in the app.
target_include_directories(app PRIVATE
    "${CMAKE_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/src")

target_link_libraries(app PRIVATE glad glfw glm::glm)

# LOGL_ROOT lets FileSystem::getPath() resolve assets no matter the CWD.
target_compile_definitions(app PRIVATE LOGL_ROOT="${CMAKE_SOURCE_DIR}")

if(LEARN_OPENGL_WITH_ASSIMP)
    target_link_libraries(app PRIVATE assimp)
    target_compile_definitions(app PRIVATE WITH_ASSIMP)
endif()
