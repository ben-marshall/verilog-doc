cmake_minimum_required(VERSION 2.8)

project(veridoc-src)

set(DISABLE_VERILOG_PARSER_TESTS ON) # disable subdirectory tests.
add_subdirectory(./verilog-parser)

set(VERIDOC_SRC main.c)
set(VERIDOC_EXE veridoc)

set(VERIDOC_CORE_SRC veridoc-config.c 
                     veridoc-parsing.c
                     veridoc-manifest.c
                     veridoc-page-factory.c
                     veridoc-json.c)

set(VERIDOC_CORE     veridoccore)

# ---------------------- Add build options -------------------------------

option(VERIDOC_COVERAGE "If YES, build the debug executable with coverage." NO)

SET(COV_FLAGS_C     "-fprofile-arcs -ftest-coverage")
SET(COV_FLAGS_LINK  "-fprofile-arcs -ftest-coverage")

# ---------------------- Set build flags ---------------------------------
    
SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -Wall -W")
SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_C_FLAGS_RELEASE}")

if( ${VERIDOC_COVERAGE} )

message(STATUS "Building with coverage flags set.")
    SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${COV_FLAGS_C} -DVERIDOC_COVERAGE_ON")
    SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${COV_FLAGS_LINK}")
else()
    message(STATUS "NOT building with coverage.")
endif()


SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")

message(STATUS "Parser Build Flags Debug:   ${CMAKE_C_FLAGS_DEBUG}")
message(STATUS "Parser Link Flags Debug:   ${CMAKE_EXE_LINKER_FLAGS_DEBUG}")
message(STATUS "Parser Build Flags Release: ${CMAKE_C_FLAGS_RELEASE}")
message(STATUS "Parser Link Flags Release: ${CMAKE_EXE_LINKER_FLAGS_RELEASE}")

# ---------------------- Add output libraries and files ------------------

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

# ---------------------- Add Tests ---------------------------------------

if( ${VERIDOC_COVERAGE} )

    set(CMAKE_C_OUTPUT_EXTENSION_REPLACE 1)

    add_test(NAME veridoc_coverage
             COMMAND veridoc -c ./build/debug/bin/assets/veridoc.cfg -F ./build/debug/bin/assets/test_manifest.txt  -v
             WORKING_DIRECTORY ../
    )

else ()

    add_test(NAME veridoc_0
             COMMAND veridoc -c ./build/debug/bin/assets/veridoc.cfg -F ./build/debug/bin/assets/test_manifest.txt  -v
             WORKING_DIRECTORY ../
    )

endif()

