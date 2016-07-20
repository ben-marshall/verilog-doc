/*!
@file veridoc-config.c
@brief Contains data structures and functions for parsing veridoc configs.
*/

#include "veridoc-config.h"

/*!
@brief Parses and returns the config file at the supplied path.
@param [in] config_file_path
@returns A pointer to the supplied config structure.
*/
veridoc_config * veridoc_config_parse(
    char * config_file_path
){

    veridoc_config * tr = calloc(1,sizeof(veridoc_config));
    FILE * fh = fopen(config_file_path, "r");

    if(fh == NULL){
        printf("ERROR: Could not open config file path: %s\n", 
            config_file_path);
        return NULL;
    }

    char * key = calloc(255,sizeof(char));
    char * value = calloc(1023,sizeof(char));
    
    while(feof(fh) == 0)
    {
        char first_on_line = fgetc(fh);
        if(first_on_line == '#' || first_on_line == ' '){
            // Read until the end of the line and then continue.
            while(fgetc(fh)!='\n' && !feof(fh)){
                // DO Nothing
            }
        }
        else if(first_on_line == '\n'){
            // DO nothing, this is an empty line.
        }
        else{
            // This is a line worth parsing!
            ungetc(first_on_line, fh);

            // Read upto the equals to get the key:
            memset(key,'\0',255);
            fscanf(fh, "%s= ", key);

            // Now read to the end of the line.
            int i = 0;
            char toadd = fgetc(fh);
            while(toadd == ' ' || toadd == '='){
                toadd = fgetc(fh);
            }

            while(i < 1023 && toadd != '\n' && toadd != '\0' && !feof(fh)){
                value[i] = toadd;
                toadd = fgetc(fh);
                i++;
            }

            if(strcmp(key,"project") == 0 && !tr -> v_project)
            {
                tr -> v_project = value;
                value = calloc(1023,sizeof(char));
            }
            else if(strcmp(key,"author") == 0 && !tr -> v_author)
            {
                tr -> v_author = value;
                value = calloc(1023,sizeof(char));
            }
            else if(strcmp(key,"output") == 0 && !tr -> v_author)
            {
                tr -> v_output= value;
                value = calloc(1023,sizeof(char));
            }
            else if(strcmp(key,"version") == 0 && !tr -> v_version)
            {
                tr -> v_version= value;
                value = calloc(1023,sizeof(char));
            }
            else if(strcmp(key,"manifest") == 0 && !tr -> v_manifest)
            {
                tr -> v_manifest = value;
                value = calloc(1023,sizeof(char));
            }
            else
            {
                if(strlen(key) > 0){
                printf("Error parsing config : %s\n", config_file_path);
                printf("Unknown key '%s' ignored\n", key);
                break;
                }
            }
        }
    }

    if(tr -> v_output == NULL)
    {
        strcat(tr -> v_output, "./veridoc-out");
    }

    free(key);
    free(value);
    
    return tr;
}


/*!
@brief Frees the memory allocated to the supplied config file.
*/
void * veridoc_config_free(
    veridoc_config * tofree
){
    if(tofree -> v_project)
        free(tofree -> v_project ); //!< Project / Documentation Set Title.
    if(tofree -> v_author)
        free(tofree -> v_author  ); //!< Documentation Authors
    if(tofree -> v_version)
        free(tofree -> v_version ); //!< Version Number
    if(tofree -> v_manifest)
        free(tofree -> v_manifest); //!< File manifest path.
    
    if(tofree)
        free(tofree);
}
