/*!
@file veridoc-dot.h
@brief Contains common data structures and functions used by the program
*/

#include <stdio.h>

#include "veridoc-types.h"
#include "veridoc-config.h"
#include "veridoc-manifest.h"
#include "veridoc-parsing.h"
#include "veridoc-page-factory.h"

#ifndef VERIDOC_H
#define VERIDOC_H

/*!
@brief Stores all of the command line arguments to the program
@see parse_args
*/
typedef struct shell_args_t{
    boolean     verbose;     //!< Be verbose with status messages?
    char      * config_path; //!< Path to config file.
    char      * manifest_path; //!< Path to input file list.
} shell_args;

/*!
@brief Responsible for parsing all of the command line arguments.
@returns A shell_args pointer
*/
shell_args * parse_args(int argc, char ** argv);


#endif

