add_executable(demo1 demo1.cpp
        )

message(STATUS CMAKE_SOURCE_DIR : ${CMAKE_SOURCE_DIR})
target_include_directories(
        demo1
        PRIVATE
        $<BUILD_INTERFACE:${FETCHCONTENT_SOURCE_DIR_BOOST}>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>

)

target_link_libraries(
        demo1
        PUBLIC
        coroutine
)