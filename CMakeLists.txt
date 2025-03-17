### Project Settings ###
# Set min cmake version
cmake_minimum_required(VERSION 3.5)
set(PROJ_NAME "SDL-OpenGL-Template")

# Set executable target depending on OS...
if(WIN32)
    set(PROJ_OUT "${PROJ_NAME}-win-x86_64")
    # Show console (FALSE=show, TRUE=don't show)
    set(CMAKE_WIN32_EXECUTABLE FALSE)
endif(WIN32)
if(UNIX)
    EXECUTE_PROCESS( COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE )
    set(PROJ_OUT "${PROJ_NAME}-unix-${ARCHITECTURE}")
endif(UNIX)

# Project
project(${PROJ_OUT} VERSION 0.1.1.0 LANGUAGES C CXX)
set(CMAKE_C_FLAGS "-std=c++11 -o")

# Get list of sources and headers
file (GLOB_RECURSE PROJ_SOURCES FOLLOW_SYMLINKS CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/include/*.cpp")
file (GLOB_RECURSE PROJ_HEADERS FOLLOW_SYMLINKS CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.h" "${CMAKE_SOURCE_DIR}/include/*.h" "${CMAKE_SOURCE_DIR}/include/*.hpp")

# Build list of includes
set (PROJ_SRC_DIRS "")
foreach (_headerFile ${PROJ_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND PROJ_SRC_DIRS ${_dir})
endforeach()
list (REMOVE_DUPLICATES PROJ_SRC_DIRS)

# Add executable
add_executable(${PROJ_OUT} ${PROJ_SOURCES})
set_target_properties(${PROJ_OUT} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)

# Linking
target_link_libraries(${PROJ_OUT} PRIVATE "-lGL -lGLEW -lGLU -lSDL2 -lSDL2_ttf")
file(GLOB PROJ_LIBS "${CMAKE_SOURCE_DIR}/lib/lib*.so")
target_link_libraries(${PROJ_OUT} PRIVATE ${PROJ_LIBS})

# Add include directories (everywhere there is a header file: libraries and src dirs)
target_include_directories(${PROJ_OUT} PRIVATE ${PROJ_SRC_DIRS})
target_include_directories(${PROJ_OUT} PRIVATE "include")
