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

    json_emit_pair(fh,"listType","file-manifest");
    json_sepp(fh);
    json_emit_pair(fh,"listTitle","List of documented files.");
    json_sepp(fh);
    json_emit_pair(fh,"listNotes","This is the list of all files specified as input to Veridoc, along with their parse status.");
    json_sepp(fh);
    json_begin_list(fh,"listData");

    int f;
    for(f = 0; f < manifest -> file_count;  f++)
    {
        fprintf(fh,"{");

        veridoc_manifest_file file = manifest -> files[f];

        json_emit_pair(fh,"path",file.path);
        json_sepp(fh);
        fprintf(fh,"\"parsed\":\"%d\",", file.parsed);
        fprintf(fh,"\"success\":\"%d\"", file.parse_success);

        fprintf(fh,"}");
        if(f < manifest -> file_count -1){
            json_sepp(fh);
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
@brief Responsible for emitting the verilog module hierarchy as JSON.
@note This is the recursive partner to @ref veridoc_pf_export_hierarchy_json
@param [in] parent_module - The parent module.
@param [in] source - The parsed source tree
@param [in] fh - The file handle to use when writing.
*/
void veridoc_pf_export_hierarchy_json_r(
    ast_module_declaration  * parent_module,
    verilog_source_tree     * source,
    FILE                    * fh
){
    ast_list * children = verilog_module_get_children(parent_module);

    fprintf(fh,"[");
    int c;

    for(c = 0; c < children -> items; c++)
    {
        ast_module_instantiation * child = ast_list_get(children,c);

        char * child_id;

        if(child -> resolved){
            child_id = child -> declaration -> identifier -> identifier;
        } else {
            child_id = child -> module_identifer -> identifier;
        }

        fprintf(fh,"{");
        fprintf(fh,"\"id\":\"%s\",", child_id);
        fprintf(fh,"\"children\":");
        if(child -> resolved)
        {
            veridoc_pf_export_hierarchy_json_r(child->declaration,source,fh);
        }
        else
        {
            fprintf(fh,"[]");
        }
        fprintf(fh,"}\n");
        if(c!=children -> items-1)
        {
            fprintf(fh,",");
        }
    }
    
    fprintf(fh,"]");
}

/*!
@brief Responsible for emitting the verilog module hierarchy as JSON.
@param [in] top_module - The top level module / root of the hierarchy.
@param [in] source - The parsed source tree
*/
void veridoc_pf_export_hierarchy_json(
    char                * top_module,
    verilog_source_tree * source,
    char                * destination
){
    FILE * fh = fopen(destination, "w");
    if(!(fh)){
        printf("ERROR: Could not open json file for writing: %s\n",
            destination);
        return;
    }

    fprintf(fh, "var veridocModuleHierarchy = {");

    fprintf(fh,"\"listType\":\"module-hierarchy\",");
    fprintf(fh,"\"listTitle\":\"Module Hierarchy\",");
    fprintf(fh,"\"listNotes\":\"This is the module hierarchy for the project.\",");
    fprintf(fh,"\"listData\":[");

    ast_identifier root_id = ast_new_identifier(top_module,0);
    ast_module_declaration * root = verilog_find_module_declaration(
        source, root_id);
    if(root == NULL)
    {
        printf("ERROR: Could not find top module: '%s'\n", top_module);
        free(root_id);
        return;
    }
    else
    {

        fprintf(fh,"{");
        fprintf(fh,"\"id\":\"%s\",", top_module);
        fprintf(fh,"\"children\":");
        veridoc_pf_export_hierarchy_json_r(root,source,fh);
        fprintf(fh,"}");
    }

    fprintf(fh, "]}");
    fclose(fh);
}

/*!
@brief Function responsible for exporting information on a module as JSON.
@param [in] config - The veridoc config being adhered to.
@param [in] module - The module to document.
*/
void veridoc_pf_export_module_json(
    veridoc_config         * config,
    ast_module_declaration * module
){


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
    printf("File Manifest:      %s\n", json_file);
    free(json_file);
    
    // Next, export the module list as a JSON document.
    char * module_file = calloc(strlen(config -> v_output) + 17, sizeof(char));
    strcat(module_file, config -> v_output);
    strcat(module_file, "/module_list.js");
    veridoc_pf_export_module_list_json(source,module_file);
    printf("Module Manifest:    %s\n", module_file);
    free(module_file);
    
    // Next, export the module hierarchy as a JSON document.
    char * module_hier = calloc(strlen(config -> v_output) + 22, sizeof(char));
    strcat(module_hier, config -> v_output);
    strcat(module_hier, "/module_hierarchy.js");
    veridoc_pf_export_hierarchy_json(config -> v_top_module,source,
        module_hier);
    printf("Module Hierarchy:   %s\n", module_hier);
    free(module_hier);


    // Next, export the individual module pages.
    printf("Exporting Module Documentation: ");
    int m;
    for(m = 0; m < source -> modules -> items; m++)
    {
        ast_module_declaration * module = ast_list_get(source->modules, m);

        printf(".");

        veridoc_pf_export_module_json(config, module);
    }
    printf("\n");
}
