/*!
@file veridoc-config.h
@brief Contains data structures and functions for parsing veridoc configs.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef VERIDOC_CONFIG_H
#define VERIDOC_CONFIG_H

/*!
Stores the names of the various configuration options.
*/
typedef enum veridoc_config_key_e{
    CFGKEY_PROJECT,
    CFGKEY_AUTHOR,
    CFGKEY_VERSION,
    CFGKEY_MANIFEST,
} veridoc_config_key;

/*!
@brief Contains all of the configuration options for a run of veridoc.
*/
typedef struct veridoc_config_t{

    char * v_project;   //!< Project / Documentation Set Title.
    char * v_author;    //!< Documentation Authors
    char * v_version;   //!< Version Number
    char * v_manifest;  //!< File manifest path.

} veridoc_config;


/*!
@brief Parses and returns the config file at the supplied path.
@param [in] config_file_path
@returns A pointer to the supplied config structure or NULL if the file cannot
be opened or parsed.
*/
veridoc_config * veridoc_config_parse(
    char * config_file_path
);

/*!
@brief Frees the memory allocated to the supplied config file.
*/
void * veridoc_config_free(
    veridoc_config * tofree
);

#endif