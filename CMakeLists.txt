cmake_minimum_required(VERSION 3.0.0)
project(Mesh2D)

# c++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
#add_compile_options(-Wall -Wextra -pedantic -Werror)
add_compile_options(-DOUTPUTEXP)

# header and src files
include_directories(include)
file(GLOB CADSOURCE "CAD2D/*.cpp")

ADD_CUSTOM_TARGET(debug
  COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Debug ${CMAKE_SOURCE_DIR}
  COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
  COMMENT "Switch CMAKE_BUILD_TYPE to Debug"
  )

ADD_CUSTOM_TARGET(release
  COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Release ${CMAKE_SOURCE_DIR}
  COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
  COMMENT "Switch CMAKE_BUILD_TYPE to Release"
  )

# build shared library
add_library(LIBPROCESSING SHARED ${CADSOURCE})

# process executable
add_executable(meshIn meshIn.cpp)
target_link_libraries(meshIn LIBPROCESSING)

# process executable
add_executable(meshOut meshOut.cpp)
target_link_libraries(meshOut LIBPROCESSING)

# process executable
add_executable(stretchgrid stretchgrid.cpp)
target_link_libraries(stretchgrid LIBPROCESSING)

# process executable
add_executable(cargeo OutputGeo.cpp)
target_link_libraries(cargeo LIBPROCESSING)