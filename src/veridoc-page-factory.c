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
@brief Responsible for copying all template asset files to the output folder.
*/
void veridoc_pf_copy_assets(
    veridoc_config      * config
){
    char * to_copy[4];
    to_copy[0] = "module.html";
    to_copy[1] = "list.html";
    to_copy[2] = "script.js";
    to_copy[3] = "style.css";

    size_t src_len = strlen(config -> v_assets_dir);
    size_t tgt_len = strlen(config -> v_output);

    int i = 0;
    for(i = 0; i < 4; i++)
    {
        char * src_path = calloc(src_len + strlen(to_copy[i])+1,sizeof(char));
        strcat(src_path, config -> v_assets_dir);
        strcat(src_path, to_copy[i]);
        char * tgt_path = calloc(tgt_len + strlen(to_copy[i])+1,sizeof(char));
        strcat(tgt_path, config -> v_output);
        strcat(tgt_path, to_copy[i]);

        FILE * source = fopen(src_path,"r");
        FILE * target = fopen(tgt_path,"w");

        if(!source)
        {
            printf("ERROR: Could not source template file: '%s'\n", src_path);
        }
        else if(!target)
        {
            printf("ERROR: Could not open target file: '%s'\n", tgt_path);
        }
        else
        {
            printf("Copying '%s' -> '%s'\n", src_path, tgt_path);
            char ch;
            while((ch = fgetc(source)) != EOF){
                fputc(ch,target);
            }
        }

        free(src_path);
        free(tgt_path);
        fclose(source);
        fclose(target);
    }
}

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
        
        if(result == 0){
            return BOOL_TRUE;
        }
        else{
            return BOOL_FALSE;
        }
    }
    return BOOL_TRUE;
}


/*!
@brief Responsible for exporting the list of parsed files to a json data file.
@param [in] manifest - The list of files.
@param [in] destination - The JSON file to write to.
@returns Void
*/
void veridoc_pf_export_file_list_json(
    veridoc_manifest * manifest,
    json_file        * fh
){
    json_object * top = json_new_object();
    json_object_add_string(top,"listType","file-manifest");
    json_object_add_string(top,"listTitle","List of documented files.");
    json_object_add_string(top,"listNotes","This is the list of all files specified as input to Veridoc, along with their parse status.");

    json_object * list = json_new_object();

    unsigned int i;
    for(i = 0; i < manifest -> file_count; i ++){
        veridoc_manifest_file f = manifest -> files[i];
        json_object * toadd = json_new_object();

        json_object_add_string(toadd, "path", f.path);
        json_object_add_int(toadd, "parsed", f.parsed);
        json_object_add_int(toadd, "success", f.parse_success);

        json_object_add_object(list, "", toadd);
    }

    json_object_add_list(top,"listData", list);
    json_emit_object(fh, top, "veridocFileList", 0);
}


/*!
@brief Responsible for emitting the list of modules for the project.
*/
void veridoc_pf_export_module_list_json(
    verilog_source_tree * source,
    json_file        * fh
){
    
    json_object * top = json_new_object();
    json_object_add_string(top,"listType","module-manifest");
    json_object_add_string(top,"listTitle","List of documented modules.");
    json_object_add_string(top,"listNotes","This is the list of all hardware modules declared in the source code.");

    json_object * list = json_new_object();
    unsigned int i;

    for(i = 0; i < source -> modules -> items; i ++)
    {
        ast_module_declaration * module = ast_list_get(source -> modules,i);
        char * identifier = ast_identifier_tostring(module -> identifier);
        
        json_object * toadd = json_new_object();

        json_object_add_string(toadd, "id", identifier);
        json_object_add_string(toadd, "brief", "None");
        json_object_add_string(toadd, "file", module -> meta.file);
        json_object_add_int   (toadd, "line", module -> meta.line);

        json_object_add_object(list, "", toadd);
    }
    
    json_object_add_list(top,"listData", list);
    json_emit_object(fh, top, "veridocModuleList", 0);

}


/*!
@brief Responsible for emitting the verilog module hierarchy as JSON.
@param [in] top_module - The top level module / root of the hierarchy.
@param [in] source - The parsed source tree
*/
json_object * veridoc_pf_export_hierarchy_json(
    ast_module_declaration  * top_module,
    json_file               * destination,
    unsigned int              depth
){
    
    if(depth == 0)
    {
        json_object * top = json_new_object();
        json_object_add_string(top,"listType","module-hierarchy");
        json_object_add_string(top,"listTitle","Module Hierarchy.");
        json_object_add_string(top,"listNotes","This is the module inheritance hierarchy for the project.");

        json_object * listdata = json_new_object();
        json_object * module   = json_new_object();
        
        char * top_id = ast_identifier_tostring(top_module->identifier);

        json_object_add_string(module, "id",   top_id);
        json_object_add_string(module, "file", top_module -> meta.file);
        json_object_add_int   (module, "line", top_module -> meta.line);
        json_object_add_int   (module, "depth", depth);

        // Recurse down the list of children.
        json_object * children = veridoc_pf_export_hierarchy_json(
            top_module, destination, depth+1);
        json_object_add_list(module,"children", children);

        json_object_add_object(listdata, "", module);
        json_object_add_list(top,"listData",listdata);
        json_emit_object(destination, top, "veridocModuleHierarchy", 0);
        return;
    }
    else
    {

        ast_list    * children = verilog_module_get_children(top_module);
        json_object * tr       = json_new_object();

        unsigned int m;
        for(m = 0; m < children -> items; m++)
        {
            ast_module_instantiation * child = ast_list_get(children, m);
            char * child_id;
            json_object * child_list;
            json_object * child_obj = json_new_object();;

            if(child -> resolved){
                 child_id = ast_identifier_tostring(
                    child->declaration->identifier);
            } else {
                 child_id = ast_identifier_tostring(child->module_identifer);
            }

            json_object_add_string(child_obj, "id", child_id);
            json_object_add_string(child_obj, "file", child -> meta.file);
            json_object_add_int   (child_obj, "line", child -> meta.line);
            json_object_add_int   (child_obj, "depth", depth);
            
            if(child -> resolved){
                child_list = veridoc_pf_export_hierarchy_json(
                child -> declaration, destination, depth + 1);
            } else {
                child_list = json_new_object();
            }

            json_object_add_list(child_obj,"children", child_list);
            json_object_add_object(tr,"",child_obj);
        }

        return tr;
    }
}


//! Creates a uniform filename for a module's JSON data structure.
char * veridoc_pf_module_filename(
    veridoc_config         * config,
    ast_module_declaration * module
){
    char * file_name;
    char * module_id = ast_identifier_tostring(module -> identifier);
    
    size_t len = strlen(module_id) + strlen(config -> v_output) + 14;

    file_name = calloc(len, sizeof(char));
    strcat(file_name, config -> v_output);
    strcat(file_name, "/module_");
    strcat(file_name, module_id);
    strcat(file_name, ".json");

    free(module_id);
    return file_name;
}

/*!
@brief Responsible for creating a json_object node which describes all of the
IO ports for the supplied module.
*/
json_object * veridoc_pf_export_module_ports_json(
    ast_module_declaration * module
){
    unsigned int i_p;
    json_object * m_ports = json_new_object();

    for(i_p = 0; i_p < module -> module_ports -> items; i_p ++){
        json_object * toadd = json_new_object();
        ast_port_declaration * port =ast_list_get(module -> module_ports,i_p);

        char * port_id = ast_identifier_tostring(
            ast_list_get(port -> port_names,0));
        char * p_type = "?";
        char * port_range = "?:?";
        char * p_dir;

        if(port -> is_reg) p_type = "reg";
        else if(port -> is_variable) p_type = "var";
        else p_type = "wire";

        switch(port -> direction){
            case PORT_INPUT:
                p_dir = "Input"; break;
            case PORT_INOUT:
                p_dir = "Inout"; break;
            case PORT_OUTPUT:
                p_dir = "Output"; break;
            default:
                p_dir = "Unknown"; break;
        }
        
        json_object_add_string(toadd,"name", port_id);
        json_object_add_string(toadd,"type", p_type);
        json_object_add_string(toadd,"range", port_range);
        json_object_add_string(toadd,"direction", p_dir);
        
        json_object_add_object(m_ports,"",toadd);
    }
    return m_ports;
}

/*!
@brief Responsible for creating a json_object node which describes all of the
module parameters for the supplied module.
*/
json_object * veridoc_pf_export_module_params_json(
    ast_module_declaration * module
){
    unsigned int i;
    json_object * m_params = json_new_object();
    
    for(i=0; i<module -> module_parameters->items;i++)
    {
        ast_parameter_declarations * param =
                                ast_list_get(module -> module_parameters,i);
        
        unsigned int j;
        for(j=0; j < param -> assignments -> items; j ++)
        {
            json_object * toadd = json_new_object();
            ast_single_assignment*assign = ast_list_get(param->assignments,j);
            ast_lvalue * lv = assign -> lval;
            
            if(lv -> type == PARAM_ID){
                char * pname = ast_identifier_tostring(lv->data.identifier);
                char * pval  = ast_expression_tostring(assign -> expression);

                json_object_add_string(toadd, "name",pname);
                json_object_add_string(toadd, "default",pval);
                json_object_add_string(toadd, "brief","None");
                
                json_object_add_object(m_params,"",toadd);
            }
        }
    }

    return m_params;
}

/*! @brief Iterates over a list of ast_var_declaration and adds a json 
 representation to the supplied add_to list object.
@bug Sometimes td=ast_list_get(to_itter,i) returns NULL.
*/
json_object * veridoc_pf_itter_vardecs(
    char        * decType, //!< Value for the JSON "type" property
    ast_list    * to_itter, //!< A list of ast_var_declaration* objects.
    json_object ** add_to    //!< Something to add the json objects to.
){
    unsigned int i;
    unsigned int j;
    for(i=0; i< to_itter -> items;i++)
    {
        ast_var_declaration * td = ast_list_get(to_itter,i);
        if(td == NULL){continue;}

        json_object * toadd = json_new_object();
                
        char * name      = ast_identifier_tostring(td -> identifier);

        json_object_add_string(toadd,"name",name);
        json_object_add_string(toadd,"type",decType);
        json_object_add_string(toadd,"file", td -> meta.file);
        json_object_add_int   (toadd,"line", td -> meta.line);
        json_object_add_string(toadd,"brief", "None");

        json_object_add_object(*add_to, "", toadd);
    }
    
    return *add_to;
}

/*! @brief Iterates over a list of ast_var_declaration and adds a json 
representation to the supplied add_to list object.
@bug Sometimes td=ast_list_get(to_itter,i) returns NULL.
*/
json_object * veridoc_pf_itter_netdecs(
    ast_list    * to_itter, //!< A list of ast_net_declaration* objects.
    json_object ** add_to    //!< Something to add the json objects to.
){
    unsigned int i;
    for(i=0; i< to_itter -> items;i++)
    {
        ast_net_declaration * td = ast_list_get(to_itter,i);
        if(td == NULL){continue;}

        json_object * toadd = json_new_object();
                
        char * name      = ast_identifier_tostring(td -> identifier);
        char * range     = "?";
        char * nettype   = "?";

        json_object_add_string(toadd,"name",name);
        json_object_add_string(toadd,"type","net");
        json_object_add_string(toadd,"nettype",nettype);
        json_object_add_string(toadd,"range",range);
        json_object_add_int   (toadd,"vectored",td -> vectored);
        json_object_add_int   (toadd,"scalared",td -> scalared);
        json_object_add_int   (toadd,"signed",td -> is_signed);
        json_object_add_string(toadd,"file", td -> meta.file);
        json_object_add_int   (toadd,"line", td -> meta.line);
        json_object_add_string(toadd,"brief", "None");

        json_object_add_object(*add_to, "", toadd);
    }
    
    return *add_to;
}

/*! @brief Iterates over a list of ast_reg_declaration and adds a json 
representation to the supplied add_to list object.
@bug Sometimes td=ast_list_get(to_itter,i) returns NULL.
*/
json_object * veridoc_pf_itter_regdecs(
    ast_list    * to_itter, //!< A list of ast_reg_declaration* objects.
    json_object ** add_to    //!< Something to add the json objects to.
){
    unsigned int i;
    for(i=0; i< to_itter -> items;i++)
    {
        ast_reg_declaration * td = ast_list_get(to_itter,i);
        if(td == NULL){printf("\tskip\n");continue;}

        json_object * toadd = json_new_object();
                
        char * name      = ast_identifier_tostring(td -> identifier);
        char * range     = "?";
        char * nettype   = "?";

        json_object_add_string(toadd,"name",name);
        json_object_add_string(toadd,"type","reg");
        json_object_add_string(toadd,"range",range);
        json_object_add_int   (toadd,"signed",td -> is_signed);
        json_object_add_string(toadd,"file", td -> meta.file);
        json_object_add_int   (toadd,"line", td -> meta.line);

        json_object_add_object(*add_to, "", toadd);
    }
    
    return *add_to;
}

/*!
@brief Responsible for creating a json_object node which describes all of the
module nets and variables for the supplied module.
@details Looks through:
module -> genvar_declarations
module -> integer_declarations
module -> net_declarations
module -> real_declarations
module -> realtime_declarations
module -> reg_declarations
module -> time_declarations
*/
json_object * veridoc_pf_export_module_vars_json(
    ast_module_declaration * module
){
    json_object * m_tr= json_new_object();
    
    json_object * nets = json_new_object();
    json_object * regs = json_new_object();
    json_object * vars = json_new_object();
    
    veridoc_pf_itter_netdecs(module -> net_declarations,&nets);
    veridoc_pf_itter_regdecs(module -> reg_declarations,&regs);
    veridoc_pf_itter_vardecs("genvar",module -> genvar_declarations,&vars);
    veridoc_pf_itter_vardecs("integer",module->integer_declarations,&vars);
    veridoc_pf_itter_vardecs("real",module->real_declarations,&vars);
    veridoc_pf_itter_vardecs("realtime",module->realtime_declarations,&vars);
    veridoc_pf_itter_vardecs("time",module -> time_declarations,&vars);

    json_object_add_list(m_tr, "nets", nets);
    json_object_add_list(m_tr, "regs", regs);
    json_object_add_list(m_tr, "vars", vars);

    return m_tr;
}

/*!
@brief Responsible for creating a json_object node which describes all of the
instanced child modules for the supplied module.
*/
json_object * veridoc_pf_export_module_children_json(
    ast_module_declaration * module
){
    unsigned int i_m;
    json_object * m_children = json_new_object();

    for(i_m = 0; i_m < module -> module_instantiations -> items; i_m++)
    {
        json_object * toadd = json_new_object();
        ast_module_instantiation * inst = ast_list_get(
            module -> module_instantiations, i_m);
        
        char * child_id;
        if(inst -> resolved){
            child_id = ast_identifier_tostring(inst->declaration->identifier);
        } else {
            child_id = ast_identifier_tostring(inst->module_identifer);
        }

        json_object_add_string(toadd, "moduleName", child_id);
        
        json_object_add_object(m_children, "", toadd);
    }
    return m_children;
}

/*!
@brief Iterates over all process blocks (initial, always, triggered) and
returns a JSON data structure representing them.
@todo Complete this after re-factoring the always block code in the parser
library.
*/
json_object * veridoc_pf_export_module_blocks_json(
    ast_module_declaration * module
){
    json_object * tr = json_new_object();

    // We must loop over both module -> always_blocks and 
    // module -> initial_blocks

    unsigned int i;
    for(i = 0; i < module -> always_blocks; i++)
    {
        ast_statement * block = ast_list_get(module -> always_blocks, i);
        if(block -> type != STM_BLOCK){ continue; }

        json_object * toadd = json_new_object();
        
        // Do nothing here while the code for expressing statement blocks is
        // refactored.

        json_object_add_object(tr,"",toadd);
    }
    
    return tr;
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
    // Where are we going to put all the JSON data?
    char * output_file_name = veridoc_pf_module_filename(config,module);
    json_file * fh          = json_new_file(output_file_name);
    json_object * top = json_new_object();

    char * module_name = ast_identifier_tostring(module->identifier);

    // Set the standard module properties.
    json_object_add_string(top, "moduleName",  module_name);
    json_object_add_string(top, "moduleFile",  module -> meta.file);
    json_object_add_int   (top, "moduleLine",  module -> meta.line);
    json_object_add_string(top, "moduleBrief", "None");

    // Add the list of ports
    json_object * module_ports = veridoc_pf_export_module_ports_json(module);
    json_object_add_list(top, "ports", module_ports);

    // Add the list of child modules.
    json_object * module_kids = veridoc_pf_export_module_children_json(module);
    json_object_add_list(top, "children", module_kids);

    // Add the list of module parameters
    json_object * module_params = veridoc_pf_export_module_params_json(module);
    json_object_add_list(top, "parameters", module_params);

    // Add the list of internal variable & net declarations
    json_object * module_vars  = veridoc_pf_export_module_vars_json(module);
    json_object_add_object(top, "declarations", module_vars);

    // Add the list of internal tasks & functions
    json_object * module_funcs  = NULL;

    // Add the list of internal process blocks (initial & others)
    json_object * module_blocks = NULL;

    // Emit and finish up.
    json_emit_object(fh,top,"veridocModuleInformation",0);
    fprintf(fh->fh,"\n\nveridoc_render_module();\n");
    json_close_file(fh);
}


/*!
@brief concatenates the config->v_output and supplied filename into a single
file path and returns it.
*/
char * veridoc_pf_jsonfilename(
    veridoc_config          * config,
    char                    * filename
){
    size_t len = strlen(config -> v_output) + strlen(filename) + 1;
    char * json_file = calloc(len, sizeof(char));
    strcat(json_file, config -> v_output);
    strcat(json_file, filename);
    return json_file;
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
    // Make sure we can find the module supposedly at the hierarchy root.
    ast_module_declaration * top_module = verilog_find_module_declaration(
        source,
        ast_new_identifier(config -> v_top_module, 0)
    );


    // First, we need to setup the output directory.
    veridoc_pf_setup_output_folder(config);
    
    char * json_file_p =veridoc_pf_jsonfilename(config, "file_list.js");
    char * module_file =veridoc_pf_jsonfilename(config, "module_list.js");
    char * module_hier =veridoc_pf_jsonfilename(config,"module_hierarchy.js");
    
    json_file * json_file_h   = json_new_file(json_file_p);
    json_file * module_file_h = json_new_file(module_file);
    json_file * module_hier_h = json_new_file(module_hier);

    veridoc_pf_export_file_list_json(manifest,json_file_h);
    veridoc_pf_export_module_list_json(source,module_file_h);
    
    if(!top_module){
        printf("ERROR: Could not find the top module '%s' in the source.\n",
            config -> v_top_module);
        printf("\tThe module hierarchy page will not work.\n");
    } else {
        veridoc_pf_export_hierarchy_json(top_module, module_hier_h, 0);
    }
    
    free(json_file_p);
    free(module_file);
    free(module_hier);

    json_close_file(json_file_h);
    json_close_file(module_file_h);
    json_close_file(module_hier_h);

    // Next, export the individual module pages.
    printf("Exporting Module Documentation: ");
    int m;
    for(m = 0; m < source -> modules -> items; m++)
    {
        ast_module_declaration * module = ast_list_get(source->modules, m);
        veridoc_pf_export_module_json(config, module);
    }
    printf("\n");
}
