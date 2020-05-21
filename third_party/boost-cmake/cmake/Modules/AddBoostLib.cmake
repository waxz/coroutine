function(_add_boost_lib)
  set(options )
  set(oneValueArgs NAME)
  set(multiValueArgs SOURCES LINK DEFINE DEFINE_PRIVATE CXXFLAGS_PRIVATE INCLUDE_PRIVATE)
  cmake_parse_arguments(BOOSTLIB "${options}" "${oneValueArgs}"
                        "${multiValueArgs}" ${ARGN})
  if (SHARED_LIBS)
  message(STATUS BUILD SHARED_LIBS FOR BOOST)
    add_library(Boost_${BOOSTLIB_NAME} SHARED ${BOOSTLIB_SOURCES})
    add_library(Boost::${BOOSTLIB_NAME} ALIAS Boost_${BOOSTLIB_NAME})
    install(TARGETS Boost_${BOOSTLIB_NAME}
            EXPORT Boost_${BOOSTLIB_NAME}Export

            ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
            LIBRARY DESTINATION ${INSTALL_LIB_DIR}
            RUNTIME DESTINATION ${INSTALL_BIN_DIR})
  else()
  message(STATUS BUILD STATIC_LIBS FOR BOOST)

    add_library(Boost_${BOOSTLIB_NAME} STATIC ${BOOSTLIB_SOURCES})
    add_library(Boost::${BOOSTLIB_NAME} ALIAS Boost_${BOOSTLIB_NAME})
    install(TARGETS Boost_${BOOSTLIB_NAME}
            EXPORT Boost_${BOOSTLIB_NAME}Export

            ARCHIVE DESTINATION ${INSTALL_LIB_DIR})
  endif(SHARED_LIBS)

#  install(EXPORT ${PROJECT_EXPORT}  DESTINATION ${INSTALL_CMAKE_DIR} NAMESPACE ${PROJECT_NAME}:: )
  # This "exports" all targets which have been put into the export set
  message("--INSTALL_CMAKE_DIR: ${INSTALL_CMAKE_DIR}")
  install(EXPORT Boost_${BOOSTLIB_NAME}Export
          DESTINATION ${INSTALL_CMAKE_DIR}
          FILE Boost_${BOOSTLIB_NAME}Targets.cmake)
  set(PROJECT_LIBS ${PROJECT_LIBS} "Boost_${BOOSTLIB_NAME}" PARENT_SCOPE)

  set_target_properties(Boost_${BOOSTLIB_NAME} PROPERTIES
    OUTPUT_NAME "boost_${BOOSTLIB_NAME}"
    FOLDER "Boost"
  )
  if(NOT BOOST_STANDALONE)
    set_target_properties(Boost_${BOOSTLIB_NAME} PROPERTIES EXCLUDE_FROM_ALL 1)
  endif()
  #todo: fix Boost::boost cannot import
  # target_link_libraries(Boost_${BOOSTLIB_NAME} PUBLIC Boost::boost)
  target_link_libraries(Boost_${BOOSTLIB_NAME} PUBLIC
  $<BUILD_INTERFACE:Boost::boost>
   
   )

  if(MSVC)
    target_compile_options(Boost_${BOOSTLIB_NAME} PRIVATE /W0)
  else()
    target_compile_options(Boost_${BOOSTLIB_NAME} PRIVATE -w)
  endif()
  if(BOOSTLIB_LINK)
    target_link_libraries(Boost_${BOOSTLIB_NAME} PUBLIC ${BOOSTLIB_LINK})
  endif()
  if(BOOSTLIB_DEFINE)
    target_compile_definitions(Boost_${BOOSTLIB_NAME} PUBLIC ${BOOSTLIB_DEFINE})
  endif()
  if(BOOSTLIB_DEFINE_PRIVATE)
    target_compile_definitions(Boost_${BOOSTLIB_NAME} PRIVATE ${BOOSTLIB_DEFINE_PRIVATE})
  endif()
  if(BOOSTLIB_CXXFLAGS_PRIVATE)
    target_compile_options(Boost_${BOOSTLIB_NAME} PRIVATE ${BOOSTLIB_CXXFLAGS_PRIVATE})
  endif()
  if(BOOSTLIB_INCLUDE_PRIVATE)
    target_include_directories(Boost_${BOOSTLIB_NAME} PRIVATE ${BOOSTLIB_INCLUDE_PRIVATE})
  endif()
endfunction()
