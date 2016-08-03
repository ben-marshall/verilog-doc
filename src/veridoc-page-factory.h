/*!
@file veridoc-page-factory.h
@brief Declares functions and structures responsible for building the
documentation pages.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "veridoc-json.h"
#include "veridoc-types.h"
#include "veridoc-config.h"
#include "veridoc-manifest.h"

#ifndef VERIDOC_PAGE_FACTORY_H
#define VERIDOC_PAGE_FACTORY_H

/*!
@brief Responsible for exporting the list of parsed files to a json data file.
@param [in] manifest - The list of files.
@param [in] destination - The file path to write to.
@returns Void
*/
void veridoc_pf_export_file_list_json(
    veridoc_manifest * manifest,
    json_file        * fh
);


/*!
@brief Responsible for emitting the list of modules for the project.
*/
void veridoc_pf_export_module_list_json(
    verilog_source_tree * source,
    json_file        * fh
);


/*!
@brief Responsible for emitting the verilog module hierarchy as JSON.
@param [in] top_module - The top level module / root of the hierarchy.
@param [in] source - The parsed source tree
*/
json_object * veridoc_pf_export_hierarchy_json(
    ast_module_declaration  * top_module,
    json_file               * destination,
    unsigned int              depth
);


/*!
@brief Top level function for exporting the whole parsed data set to html.
@param [in] manifest - The list of files parsed.
@param [in] config - Configuration options for the output.
@param [in] source - The parsed source tree
*/
void veridoc_pf_build(
    veridoc_manifest    * manifest,
    veridoc_config      * config,
    verilog_source_tree * source
);

/*!
@brief Responsible for copying all template asset files to the output folder.
*/
void veridoc_pf_copy_assets(
    veridoc_config      * config
);

#endif

