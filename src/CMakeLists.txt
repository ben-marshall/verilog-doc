cmake_minimum_required(VERSION 2.8)

project(veridoc-src)

add_subdirectory(./verilog-parser)

set(VERIDOC_SRC main.c)

set(VERIDOC_EXE veridoc)

add_executable(${VERIDOC_EXE} ${VERIDOC_SRC})
target_link_libraries(${VERIDOC_EXE} verilogparser)
