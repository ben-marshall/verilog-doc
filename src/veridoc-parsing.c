/*!
@file veridoc-parsing.c
@brief Contains data structures and functions for parsing verilog files.
*/

#include "veridoc-parsing.h"

/*!
@brief Responsible for setting up the verilog-parser preprocessor
@param [in] manifest - The set of files to parse and directories to search.
@details This function uses the yy_preproc global variable (exposed by the
verilog-parser library) and adds to its internal list of directories to
search for include files.
*/
void veridoc_parse_init_include_paths(
    veridoc_manifest * manifest,
    veridoc_config   * config
){
    int d;
    // iterate over the directories
    for(d = 0; d < manifest -> directory_count; d++)
    {
        // add to the search paths.
        ast_list_append(yy_preproc -> search_dirs,
            manifest -> directories[d].path);
        printf("Added '%s' to search path.\n", 
            manifest -> directories[d].path);
    }
    
    // iterate over the paths in the config 
    for(d = 0; d < config -> v_includes -> items ; d++)
    {
        // add to the search paths.
        ast_list_append(yy_preproc -> search_dirs,
            ast_list_get(config -> v_includes,d));
        printf("Added '%s' to search path.\n", 
            (char*)ast_list_get(config -> v_includes,d));
    }
}

/*!
@brief Responsible for iterating over the files in the manifest and adding them
to the source tree.
@param [in] manifest - The set of files to parse and directories to search.
*/
void veridoc_parse_manifest_files(
    veridoc_manifest * manifest
){
    int f;
    for(f = 0; f < manifest -> file_count; f++)
    {
        veridoc_manifest_file * record = &manifest -> files[f];

        FILE * fh = fopen(record -> path,"r");

        if(fh)
        {
            verilog_preprocessor_set_file(yy_preproc, record -> path);
            int result = verilog_parse_file(fh);
            
            record -> parsed        = BOOL_TRUE;
            if(result == 0) {
                record -> parse_success = BOOL_TRUE;
            } else {
                record -> parse_success = BOOL_FALSE;
            }

            fclose(fh);
        }
        else
        {
            record -> parsed        = BOOL_FALSE;
            record -> parse_success = BOOL_FALSE;
        }

        printf("%d / %d - %d %d - %s\n", f, manifest -> file_count,
            record -> parsed, record -> parse_success, record -> path);
        
    }
}

/*!
@brief Responsible for parsing each file in the manifest, and building the
internal source tree.
@param [in] manifest - The set of files to parse and directories to search.
@returns The parsed source tree object. 
*/
verilog_source_tree * veridoc_parse_input_source(
    veridoc_manifest * manifest,
    veridoc_config   * config
){
    // First, we must initialise the parser library
    verilog_parser_init();

    // Next, we need to set up the include paths for the pre-processor.
    veridoc_parse_init_include_paths(manifest,config);

    // Now we can do the parsing!
    veridoc_parse_manifest_files(manifest);

    // The value to be returned is a global variable exposed by the
    // parser library.
    verilog_source_tree * tr = yy_verilog_source_tree;

    // Resolve all of the module names.
    printf("Resolving Modules...\n");
    verilog_resolve_modules(tr);

    return tr;
}
