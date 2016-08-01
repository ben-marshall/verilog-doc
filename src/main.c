/*!
@file main.c
@brief Contains the main entry point of the program.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "veridoc.h"

/*!
@brief Prints the help text and exists, depending on the parameters.
*/
void print_help(boolean and_exit)
{
printf("Usage: ./veridoc <options> <input files>\n"); 
printf("Options:\n");
printf("-v, --verbose              - Be verbose with status information.\n");
printf("-c <file>, --config <file> - Set the configuration file path.   \n");
printf("-F <file>, --files  <file> - Input file manifest list.          \n");
printf("\n");

    if(and_exit){
        exit(0);
    }
}

/*!
@brief Responsible for parsing all of the command line arguments.
@returns A shell_args pointer
*/
shell_args * parse_args(int argc, char ** argv)
{
    if(argc == 1)
    {
        print_help(BOOL_TRUE);
    }

    shell_args * tr = calloc(1,sizeof(shell_args));

    int i;  // Iterate over argv
    for(i = 1; i < argc; i++)
    {
        if(strcmp("-v", argv[i]) == 0 ||
           strcmp("--verbose", argv[i]) == 0)
        {
            tr -> verbose = BOOL_TRUE;
        }
        else if((strcmp("-c", argv[i]) == 0        ||
                 strcmp("--config", argv[i]) == 0) &&
                 i <= argc-1)
        {
            i++;
            tr -> config_path = argv[i];
        }
        else if((strcmp("-F", argv[i]) == 0        ||
                 strcmp("--files", argv[i]) == 0) &&
                 i <= argc-1)
        {
            i++;
            tr -> manifest_path= argv[i];
        }
    }

    return tr;
}

int main(int argc, char ** argv)
{
    // Parse any command line arguments.
    shell_args     * args   = parse_args(argc,argv);

    if(!args -> config_path){
        print_help(BOOL_TRUE);
    }
    
    veridoc_config * config = veridoc_config_parse(
        args -> config_path,
        argv[0]
    );
    
    // Check the config file was laoded correctly.
    if(!config){
        print_help(BOOL_TRUE);
    }
    
    // Has the manifest path been specified?
    if(args -> manifest_path)
    {
        if(config -> v_manifest){
            free(config -> v_manifest);
        }
        config -> v_manifest = args -> manifest_path;
    }
    else if(!config -> v_manifest)
    {
        // If not, set it to a default value.
        config -> v_manifest = "Veridoc.cfg\0";
    }

    veridoc_manifest * manifest = veridoc_manifest_parse(config -> v_manifest);

    // Check that the manifest was loaded correctly.
    if(!manifest)
    {
        printf("ERROR: Could not load or parse the manifest file: %s\n",
            config -> v_manifest);
        veridoc_config_free(config);
        return 0;
    }
    
    // Print out some status information.
    if(args -> verbose){
        printf("Exe Path:      %s\n", argv[0]);
        printf("Config Path:   %s\n", args -> config_path);
        printf("File Manifest: %s\n", config -> v_manifest);
        printf("Project:       %s\n", config -> v_project);
        printf("Author:        %s\n", config -> v_author );
        printf("Version:       %s\n", config -> v_version);
        printf("Assets Path:   %s\n", config -> v_assets_dir);
        printf("Input Files:   %d\n", manifest -> file_count);
        printf("Output To:     %s\n", config-> v_output);
        printf("Include Paths:\n");

        unsigned int i;
        for(i = 0; i < config -> v_includes -> items; i++)
        {
            printf("\t'%s'\n", (char*)ast_list_get(config -> v_includes, i));
        }

        printf("\n");
        printf("Starting parsing...\n");
    }
    
    // Read in all of the source files.
    verilog_source_tree * source = veridoc_parse_input_source(manifest,config);
    
    // Build the output documentation.
    veridoc_pf_build(manifest, config, source);

    // Copy the asset files from their source locations to the output dir.
    veridoc_pf_copy_assets(config);
    
    // Free everything
    veridoc_config_free(config);
    veridoc_manifest_free(manifest);
    
    printf("Veridoc Completed Successfully.\n");
    return 0;
}
