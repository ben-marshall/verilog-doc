cmake_minimum_required(VERSION 2.8)

project(veridoc-src)

add_subdirectory(./verilog-parser)

set(VERIDOC_SRC main.c)
set(VERIDOC_EXE veridoc)

set(VERIDOC_CORE_SRC veridoc-config.c 
                     veridoc-manifest.c
                     veridoc-parsing.c)

set(VERIDOC_CORE     veridoccore)

add_library(${VERIDOC_CORE} ${VERIDOC_CORE_SRC})

add_executable(${VERIDOC_EXE} ${VERIDOC_SRC})

target_link_libraries(${VERIDOC_EXE}  verilogparser)
target_link_libraries(${VERIDOC_CORE} verilogparser)

target_link_libraries(${VERIDOC_EXE}  ${VERIDOC_CORE})
