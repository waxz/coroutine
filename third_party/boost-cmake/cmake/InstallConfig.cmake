
set(${PROJECT_NAME}_INCLUDE_DIRS ${HEADER_PATH} ${PROJECT_DEPEND_INCLUDE_DIRS})


# central location for specifying the Open3D version
set(VersionFile "${CMAKE_CURRENT_SOURCE_DIR}/version.txt")
if (EXISTS ${VersionFile})
    file(STRINGS ${VersionFile} PROJECT_VERSION_READ)
    foreach (ver ${PROJECT_VERSION_READ})
        if (ver MATCHES "VERSION_(MAJOR|MINOR|PATCH|TWEAK) +([^ ]+)$")
            set(PROJECT_VERSION_${CMAKE_MATCH_1} "${CMAKE_MATCH_2}" CACHE INTERNAL "")
        endif ()
    endforeach ()


else ()
    message(STATUS "${VersionFile} NOT FOUND")

    set(PROJECT_VERSION_MAJOR 0)
    set(PROJECT_VERSION_MINOR 0)
    set(PROJECT_VERSION_PATCH 0)

endif ()

string(CONCAT PROJECT_VERSION "${PROJECT_VERSION_MAJOR}"
        ".${PROJECT_VERSION_MINOR}"
        ".${PROJECT_VERSION_PATCH}")
message(STATUS "PROJECT_VERSION:${PROJECT_VERSION}")


# Install headers
# Create 'version.h'
#configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.h.in"
#        "${HEADER_PATH}/version.h" @ONLY)
# version.h + library header

message(STATUS "INSTALL_INCLUDE_DIR: " ${INSTALL_INCLUDE_DIR})
message(STATUS "LIBRARY_FOLDER: " ${PROJECT_NAME})

#install(DIRECTORY ${HEADER_PATH}/
#        DESTINATION "${INSTALL_INCLUDE_DIR}/${PROJECT_NAME}"
#        FILES_MATCHING PATTERN "*.h"
#        )

# Create the <package>Config.cmake.in

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in
        "${PROJECT_CMAKE_FILES}/${PROJECT_NAME}Config.cmake" @ONLY)

# Create the <package>ConfigVersion.cmake.in
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/ConfigVersion.cmake.in
        "${PROJECT_CMAKE_FILES}/${PROJECT_NAME}ConfigVersion.cmake" @ONLY)

# Install <package>Config.cmake and <package>ConfigVersion.cmake files
install(FILES
        "${PROJECT_CMAKE_FILES}/${PROJECT_NAME}Config.cmake"
        "${PROJECT_CMAKE_FILES}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)

# Uninstall targets
if (NOT TARGET uninstall)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/Uninstall.cmake.in"
            "${PROJECT_CMAKE_FILES}/Uninstall.cmake"
            IMMEDIATE @ONLY)
#    add_custom_target(uninstall  COMMAND ${CMAKE_COMMAND} -P ${PROJECT_CMAKE_FILES}/Uninstall.cmake)

endif ()