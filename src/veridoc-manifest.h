/*!
@file veridoc-manifest.h
@brief Contains common data structures and functions used on the
file manifests
*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <dirent.h>

#include "veridoc-types.h"

#ifndef VERIDOC_MANIFEST_H
#define VERIDOC_MANIFEST_H

/*!
@brief A description of a directory to be included in the search path for
header files.
*/
typedef struct veridoc_manifest_directory_t{
    char    * path;             //!< The directory path
} veridoc_manifest_directory;

/*!
@brief A description of a single file to be included in the parsed source
tree.
*/
typedef struct veridoc_manifest_file_t{
    char    * path;             //!< The file path
    boolean   parsed;           //!< Was the file parsed?
    boolean   parse_success;    //!< Did the file parse without errors?
} veridoc_manifest_file;

/*!
@brief Contains a list of files to parse and document, and folders to search
for header files in.
*/
typedef struct veridoc_manifest_t{
    veridoc_manifest_directory * directories; //!< The folders to search.
    veridoc_manifest_file      * files;       //!< The files to parse.
    int file_count;         //!< How many files are specified.
    int directory_count;    //!< How many directories are specified.
} veridoc_manifest;

/*!
@brief Parses the supplied manifest file and returns a representation of it.
@param [in] path - The manifest file to parse.
@returns A pointer to the manifest object, or NULL if the file could not be
found or parsed.
*/
veridoc_manifest * veridoc_manifest_parse(
    char * path
);

/*!
@brief Frees a file manifest from memory.
*/
void veridoc_manifest_free(
    veridoc_manifest * tofree
);

#endif

