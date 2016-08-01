/*!
@file veridoc-parsing.h
@brief Contains data structures and functions for parsing verilog files.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "veridoc-types.h"
#include "veridoc-manifest.h"
#include "veridoc-config.h"

#ifndef VERIDOC_PARSING_H
#define VERIDOC_PARSING_H

/*!
@brief Responsible for parsing each file in the manifest, and building the
internal source tree.
@param [in] manifest - The set of files to parse and directories to search.
@returns The parsed source tree object. 
*/
verilog_source_tree * veridoc_parse_input_source(
    veridoc_manifest * manifest,
    veridoc_config   * config
);


#endif
