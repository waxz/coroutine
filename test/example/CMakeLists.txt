project(coroutine_demo)
# the minimum version of CMake required
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 14)


# INSTALL_LIB_DIR
set(INSTALL_LIB_DIR lib
        CACHE PATH "Relative instalation path for libraries")

# INSTALL_BIN_DIR
set(INSTALL_BIN_DIR bin
        CACHE PATH "Relative instalation path for executables")

# INSTALL_INCLUDE_DIR
set(INSTALL_INCLUDE_DIR include
        CACHE PATH "Relative instalation path for header files")

# INSTALL_SHARE_DIR
set(INSTALL_SHARE_DIR share
        CACHE PATH "Relative instalation path for share files")


# INSTALL_CMAKE_DIR
if (WIN32 AND NOT CYGWIN)
    set(DEF_INSTALL_CMAKE_DIR cmake)
else ()
    set(DEF_INSTALL_CMAKE_DIR lib/cmake/${PROJECT_NAME})
endif ()
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR}
        CACHE PATH "Relative instalation path for CMake files")

message(STATUS PROJECT_NAME : ${PROJECT_NAME})
message(STATUS INSTALL_CMAKE_DIR : ${INSTALL_CMAKE_DIR})
message(STATUS DEF_INSTALL_CMAKE_DIR : ${DEF_INSTALL_CMAKE_DIR})
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR}
        CACHE PATH "Relative instalation path for CMake files")
message(STATUS INSTALL_CMAKE_DIR : ${INSTALL_CMAKE_DIR})

# The export set for all the targets
set(PROJECT_EXPORT ${PROJECT_NAME}EXPORT)

# Path of the CNake files generated
set(PROJECT_CMAKE_FILES ${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY})

# The RPATH to be used when installing
set(CMAKE_INSTALL_RPATH ${INSTALL_LIB_DIR})


# required modules for our task
include(CheckIncludeFile)
include(CheckIncludeFileCXX)
include(CheckIncludeFiles)
include(ExternalProject)

#REQUIRED
find_package(coroutine  HINTS /home/waxz/dev/dynamic_localization/robot_suit/upload/coroutine/install )
message(STATUS coroutine_LIBRARIES ${coroutine_LIBRARIES})
message(STATUS coroutine_INCLUDE_DIRS ${coroutine_INCLUDE_DIRS})





add_executable(demo1 demo1.cpp
        )

message(STATUS CMAKE_SOURCE_DIR : ${CMAKE_SOURCE_DIR})
target_link_libraries(
        demo1
        PRIVATE
        # /home/waxz/dev/dynamic_localization/robot_suit/upload/coroutine/install/lib/libboost_context.a
        # /home/waxz/dev/dynamic_localization/robot_suit/upload/coroutine/install/lib/libcoroutine.a #coroutine
         ${coroutine_LIBRARIES}

)

 target_include_directories(
        demo1
        PUBLIC
        ${coroutine_INCLUDE_DIRS}
)