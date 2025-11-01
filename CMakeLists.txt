cmake_minimum_required(VERSION 3.10.0)
project(tileadv VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(COPY "${CMAKE_SOURCE_DIR}/assets" DESTINATION "${CMAKE_BINARY_DIR}")
file(COPY "${CMAKE_SOURCE_DIR}/guiassets" DESTINATION "${CMAKE_BINARY_DIR}")
file(COPY "${CMAKE_SOURCE_DIR}/world" DESTINATION "${CMAKE_BINARY_DIR}")

find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE SOURCES
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

file(GLOB_RECURSE HEADERS
    "${PROJECT_SOURCE_DIR}/include/*.hpp"
)

add_executable(tileadv ${SOURCES})

target_link_libraries(tileadv PRIVATE SDL3::SDL3 SDL3_image::SDL3_image)

if(MSVC)
  target_compile_options(tileadv PRIVATE /W4 /WX)
else()
  target_compile_options(tileadv PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

file(GLOB_RECURSE GAME_SOURCES
     "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

list(REMOVE_ITEM GAME_SOURCES "${CMAKE_SOURCE_DIR}/src/main.cpp")

file(GLOB TEST_SOURCES "${PROJECT_SOURCE_DIR}/tests/*.cpp")

foreach(test_src ${TEST_SOURCES})
    get_filename_component(test_name ${test_src} NAME_WE)

    add_executable(${test_name}_tests ${PROJECT_SOURCE_DIR}/testsrc/catch_amalgamated.cpp ${test_src} ${GAME_SOURCES})
    target_include_directories(${test_name}_tests PRIVATE include include/thirdparty)
    target_link_libraries(${test_name}_tests PRIVATE stdc++fs SDL3::SDL3 SDL3_image::SDL3_image)

    add_test(NAME ${test_name} COMMAND ${test_name}_tests)
endforeach()

# add_executable(run_tests
#    ${TEST_SOURCES}
#    ${GAME_SOURCES}
#)
#target_include_directories(run_tests PRIVATE include include/thirdparty)
#target_link_libraries(run_tests PRIVATE stdc++fs SDL3::SDL3 SDL3_image::SDL3_image)

include(CTest)
enable_testing()

# add_test(NAME RunECSTests COMMAND run_tests)
