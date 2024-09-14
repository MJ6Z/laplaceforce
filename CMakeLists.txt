cmake_minimum_required(VERSION 3.1)

# name of project
project(emvectors)

# From CMAKE_SYSTEM work out which of __OSX__, __GLN__, __NIX__ are required
if(CMAKE_SYSTEM MATCHES Linux.*)
  set(OS_FLAG "-D__GLN__")
else()
  message(ERROR "Operating system not supported: " ${CMAKE_SYSTEM})
endif()

# morphologica uses c++-17 language features
set(CMAKE_CXX_STANDARD 17)
# Add the host definition to CXXFLAGS along with other switches, depending on OS/Compiler
if (APPLE)
  set(CMAKE_CXX_FLAGS "${OS_FLAG} -Wall -Wfatal-errors -g -O3")
else()
  # This assumes a gcc compiler (or a gcc mimic like Clang)
  set(CMAKE_CXX_FLAGS "${OS_FLAG} -Wall -Wfatal-errors -g -O3 -Wno-unused-result -Wno-unknown-pragmas -march=native")
endif()

# Tell clang to be quiet about brace initialisers:
if(CMAKE_CXX_COMPILER_ID MATCHES Clang)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-braces")
endif()

# This assumes a gcc compiler
set(CMAKE_CXX_FLAGS "${OS_FLAG} -Wall -Wfatal-errors -g -O3 -Wno-unused-result -Wno-unknown-pragmas -march=native")

# Add OpenMP flags here, if necessary
find_package(OpenMP)
if(OpenMP_FOUND)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

# Additional GL compiler flags.
set(OpenGL_GL_PREFERENCE "GLVND") # Following `cmake --help-policy CMP0072`
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGL3_PROTOTYPES -DGL_GLEXT_PROTOTYPES")


# Tell the program where the morph fonts are, to compile them into the binary
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMORPH_FONTS_DIR=\"\\\"${PROJECT_SOURCE_DIR}/morphologica/fonts\\\"\"")

# Find the libraries which will be needed
find_package(HDF5 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Armadillo REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(Freetype REQUIRED)

set(MORPH_INC_CORE ${ARMADILLO_INCLUDE_DIR} ${ARMADILLO_INCLUDE_DIRS} ${HDF5_INCLUDE_DIR})
set(MORPH_INC_GL ${OPENGL_INCLUDE_DIR} ${GLFW3_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIRS})
include_directories(${MORPH_INC_CORE} ${MORPH_INC_GL})

# Assuming morphologica is installed in-tree.
set(MORPH_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/morphologica" CACHE PATH "The path to morphologica")
include_directories(BEFORE ${MORPH_INCLUDE_PATH}/include) # Allows GL3/gl3.h to be found
include_directories(BEFORE ${MORPH_INCLUDE_PATH})         # Allows morph/Header.h to be found

#
# All the library finding is now done. It's time to compile the program.
#
add_executable(model quiver.cpp)

set(MORPH_LIBS_CORE ${ARMADILLO_LIBRARY} ${ARMADILLO_LIBRARIES} ${HDF5_C_LIBRARIES})
set(MORPH_LIBS_GL OpenGL::GL Freetype::Freetype glfw)
target_link_libraries(model ${MORPH_LIBS_CORE} ${MORPH_LIBS_GL})

if(OpenMP_FOUND)
  target_link_libraries(model OpenMP::OpenMP_CXX)
endif()

# For debugging of variables:
option(DEBUG_VARIABLES OFF)
if(DEBUG_VARIABLES)
  get_cmake_property(_variableNames VARIABLES)
  foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
  endforeach()
endif(DEBUG_VARIABLES)


