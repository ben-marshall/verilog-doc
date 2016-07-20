/*!
@file veridoc-page-factory.c
@brief Defines functions and structures responsible for building the
documentation pages.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "veridoc-page-factory.h"

/*!
@brief Sets up the output folder.
@returns True if all goes well, false otherwise.
*/
boolean veridoc_pf_setup_output_folder(
    veridoc_config * config
){
    struct stat status = {0};

    if (stat(config -> v_output, &status) == -1) {
        int result = mkdir(config -> v_output, 0700);
        
        if(result == 0)
        {
            return BOOL_TRUE;
        }
        else
        {
            return BOOL_FALSE;
        }
    }
    return BOOL_TRUE;
}


/*!
@brief Responsible for exporting the list of parsed files to a json data file.
@param [in] manifest - The list of files.
@param [in] destination - The file path to write to.
@returns Void
*/
void veridoc_pf_export_file_list_json(
    veridoc_manifest * manifest,
    char             * destination
){
    FILE * fh = fopen(destination, "w");
    if(!(fh)){
        printf("ERROR: Could not open json file for writing: %s\n",
            destination);
        return;
    }

    fprintf(fh, "var veridocFileList = {");

    fprintf(fh,"\"listType\":\"file-manifest\",");
    fprintf(fh,"\"listTitle\":\"List Of Documented Files\",");
    fprintf(fh,"\"listNotes\":\"This is the list of all files specified as input to Veridoc, along with their parse status.\",");
    fprintf(fh,"\"listData\":[");

    int f;
    for(f = 0; f < manifest -> file_count;  f++)
    {
        fprintf(fh,"{");

        veridoc_manifest_file file = manifest -> files[f];

        fprintf(fh,"\"path\":\"%s\",", file.path);
        fprintf(fh,"\"parsed\":\"%d\",", file.parsed);
        fprintf(fh,"\"success\":\"%d\"", file.parse_success);

        fprintf(fh,"}");
        if(f < manifest -> file_count -1){
            fprintf(fh,",");
        }
    }

    fprintf(fh, "]}");
    fclose(fh);
}


/*!
@brief Responsible for emitting the list of modules for the project.
*/
void veridoc_pf_export_module_list_json(
    verilog_source_tree * source,
    char                * destination
){
    FILE * fh = fopen(destination, "w");
    if(!(fh)){
        printf("ERROR: Could not open json file for writing: %s\n",
            destination);
        return;
    }

    fprintf(fh, "var veridocModuleList = {");

    fprintf(fh,"\"listType\":\"module-manifest\",");
    fprintf(fh,"\"listTitle\":\"List Of Documented Modules\",");
    fprintf(fh,"\"listNotes\":\"This is a list of all hardware modules documented.\",");
    fprintf(fh,"\"listData\":[");

    int m;
    for(m = 0; m < source -> modules -> items; m++)
    {
        fprintf(fh,"{");
        
        ast_module_declaration * module = ast_list_get(source -> modules,m);

        char * identifier = ast_identifier_tostring(module -> identifier);
        
        fprintf(fh,"\"id\":\"%s\",",identifier);
        free(identifier);

        fprintf(fh,"\"brief\":\"None\"");


        fprintf(fh,"}");
        if(m < source -> modules -> items -1){
            fprintf(fh,",");
        }
    }

    fprintf(fh, "]}");
    fclose(fh);
}


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
){
    // First, we need to setup the output directory.
    veridoc_pf_setup_output_folder(config);

    // Next, export the file list as a JSON document.
    char * json_file = calloc(strlen(config -> v_output) + 16, sizeof(char));
    strcat(json_file, config -> v_output);
    strcat(json_file, "/file_list.js");
    veridoc_pf_export_file_list_json(manifest,json_file);
    free(json_file);
    
    // Next, export the module list as a JSON document.
    char * module_file = calloc(strlen(config -> v_output) + 17, sizeof(char));
    strcat(module_file, config -> v_output);
    strcat(module_file, "/module_list.js");
    veridoc_pf_export_module_list_json(source,module_file);
    free(module_file);
}
