project(coroutine)
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



set(SHARED_LIBS ON  CACHE INTERNAL "")

# boost
set(BUILD_BOOST ON  CACHE INTERNAL "")

set(FETCHCONTENT_SOURCE_DIR_BOOST ""  CACHE PATH "")
set(BOOST_BUILD_PATH ""  CACHE PATH "")
set(BOOST_LIBS_OPTIONAL context CACHE INTERNAL "")

if(NOT FETCHCONTENT_SOURCE_DIR_BOOST)

    message(FATAL_ERROR "FETCHCONTENT_SOURCE_DIR_BOOST NOT DEFINED")
endif()
message(STATUS FETCHCONTENT_SOURCE_DIR_BOOST:${FETCHCONTENT_SOURCE_DIR_BOOST})

if(NOT BOOST_BUILD_PATH)

    message(FATAL_ERROR "BOOST_BUILD_PATH NOT DEFINED")
endif()


message(STATUS BOOST_BUILD_PATH:${BOOST_BUILD_PATH})


if(NOT BOOST_LIBS_OPTIONAL)

    message(FATAL_ERROR "BOOST_LIBS_OPTIONAL NOT DEFINED")
endif()



if(BUILD_BOOST)


#    set(FETCHCONTENT_SOURCE_DIR_BOOST ${CMAKE_SOURCE_DIR}/../../../ThirdParty/boost)
    add_subdirectory(${BOOST_BUILD_PATH} boost-cmake)
    set(Boost_LIBRARIES
            $<BUILD_INTERFACE:Boost::context>

            )
    add_library(Boost_LIB INTERFACE)
    add_dependencies(Boost_LIB ${Boost_LIBRARIES})
endif()


if(SHARED_LIBS)
add_library(coroutine SHARED ${CMAKE_SOURCE_DIR}/coroutine/coroutine.cpp
        ${CMAKE_SOURCE_DIR}/coroutine/clock_time.cpp)
else()
add_library(coroutine STATIC ${CMAKE_SOURCE_DIR}/coroutine/coroutine.cpp
        ${CMAKE_SOURCE_DIR}/coroutine/clock_time.cpp)

endif()

target_include_directories(
        coroutine
        PRIVATE
        $<BUILD_INTERFACE:${FETCHCONTENT_SOURCE_DIR_BOOST}>
)

target_link_libraries(
        coroutine
        PRIVATE
        $<BUILD_INTERFACE:Boost::context>
        $<INSTALL_INTERFACE:Boost::context>
)
add_dependencies(coroutine  Boost::context)

#install and export
#lib
install(TARGETS coroutine
        EXPORT coroutineExport

        # In order to export target, uncomment next line
        #            RUNTIME DESTINATION "${INSTALL_LIB_DIR}/${PROJECT_NAME}" COMPONENT bin
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT shlib
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}" COMPONENT stlib
        )
install(EXPORT coroutineExport
        DESTINATION ${INSTALL_CMAKE_DIR}
        FILE coroutineTargets.cmake)

list(APPEND PROJECT_LIBS Boost_context)
list(APPEND PROJECT_LIBS coroutine)
#---------------------





#header
install(DIRECTORY ${CMAKE_SOURCE_DIR}/coroutine/
       DESTINATION "${INSTALL_INCLUDE_DIR}/${PROJECT_NAME}"
       FILES_MATCHING PATTERN "*.h"
       )


get_filename_component(header_install_path "${INSTALL_INCLUDE_DIR}/${PROJECT_NAME}" ABSOLUTE)

list(APPEND PROJECT_DEPEND_INCLUDE_DIRS ${CMAKE_INSTALL_PREFIX}/${INSTALL_INCLUDE_DIR} )

list(APPEND PROJECT_DEPEND_INCLUDE_DIRS ${FETCHCONTENT_SOURCE_DIR_BOOST} )
message(STATUS 233PROJECT_DEPEND_INCLUDE_DIRS = ${PROJECT_DEPEND_INCLUDE_DIRS})
include(cmake/InstallConfig.cmake)
include(cmake/CpackConfig.cmake)




if(BUILD_TEST)
add_subdirectory(test)
endif()