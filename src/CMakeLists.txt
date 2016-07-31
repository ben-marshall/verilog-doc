cmake_minimum_required(VERSION 2.8)

project(veridoc-src)

add_subdirectory(./verilog-parser)

set(VERIDOC_SRC main.c)
set(VERIDOC_EXE veridoc)

set(VERIDOC_CORE_SRC veridoc-config.c 
                     veridoc-parsing.c
                     veridoc-manifest.c
                     veridoc-page-factory.c
                     veridoc-json.c)

set(VERIDOC_CORE     veridoccore)

add_library(${VERIDOC_CORE} ${VERIDOC_CORE_SRC})

add_executable(${VERIDOC_EXE} ${VERIDOC_SRC})

target_link_libraries(${VERIDOC_EXE}  verilogparser)
target_link_libraries(${VERIDOC_CORE} verilogparser)

target_link_libraries(${VERIDOC_EXE}  ${VERIDOC_CORE})


# Copy asset files.

set(BIN_DIR_ASSETS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets)
configure_file(assets/module.html ${BIN_DIR_ASSETS}/module.html)
configure_file(assets/list.html   ${BIN_DIR_ASSETS}/list.html  )
configure_file(assets/script.js   ${BIN_DIR_ASSETS}/script.js  )
configure_file(assets/style.css   ${BIN_DIR_ASSETS}/style.css  )
configure_file(assets/veridoc.cfg ${BIN_DIR_ASSETS}/veridoc.cfg  )
configure_file(assets/test_manifest.txt ${BIN_DIR_ASSETS}/test_manifest.txt )
