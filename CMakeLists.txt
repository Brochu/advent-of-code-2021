cmake_minimum_required (VERSION 3.10)

project (AoC2023)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_CXX_STANDARD 20)

set (DEBUG_OPTIONS -g -O0)
set (RELEASE_OPTIONS -O3)

message(" -=- Creating build folder for Day : ${DAY} -=- ")

set (headers
    ${CMAKE_CURRENT_LIST_DIR}/include/day.h
    ${CMAKE_CURRENT_LIST_DIR}/include/parsing.h
)
set (source
    ${CMAKE_CURRENT_LIST_DIR}/src/day${DAY}.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/parsing.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
)

message(" -=- Headers generated: ${headers} -=- ")
message(" -=- Source generated: ${source} -=- ")

add_executable (AoC2023 ${source} ${headers})

target_compile_options(AoC2023 PUBLIC "$<$<CONFIG:DEBUG>:${DEBUG_OPTIONS}>")
target_compile_options(AoC2023 PUBLIC "$<$<CONFIG:RELEASE>:${RELEASE_OPTIONS}>")

target_include_directories(AoC2023
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include
)
set_target_properties(AoC2023
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY       "${CMAKE_CURRENT_LIST_DIR}/"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_LIST_DIR}/"
)
target_compile_definitions(AoC2023
    PRIVATE
    DAY=${DAY}
)
