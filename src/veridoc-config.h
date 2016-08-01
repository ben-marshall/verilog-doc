/*!
@file veridoc-config.h
@brief Contains data structures and functions for parsing veridoc configs.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "verilog-parser/src/verilog_ast_common.h"

#ifndef VERIDOC_CONFIG_H
#define VERIDOC_CONFIG_H

/*!
@brief Contains all of the configuration options for a run of veridoc.
*/
typedef struct veridoc_config_t{

    char * v_project;   //!< Project / Documentation Set Title.
    char * v_author;    //!< Documentation Authors
    char * v_version;   //!< Version Number
    char * v_manifest;  //!< File manifest path.
    char * v_output;    //!< Folder to put the results in.
    char * v_top_module; //!< The root module of the hierarchy.
    char * v_assets_dir; //!< Where to look for template files.
    ast_list * v_includes; //!< Include folders for header files.
} veridoc_config;


/*!
@brief Parses and returns the config file at the supplied path.
@param [in] config_file_path
@param [in] exe_path - the value of argv[0]
@returns A pointer to the supplied config structure or NULL if the file cannot
be opened or parsed.
*/
veridoc_config * veridoc_config_parse(
    char * config_file_path,
    char * exe_path
);

/*!
@brief Frees the memory allocated to the supplied config file.
*/
void veridoc_config_free(
    veridoc_config * tofree
);

#endif
