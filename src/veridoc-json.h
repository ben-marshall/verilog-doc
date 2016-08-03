/*!
@file veridoc-json.h
@brief Provides declarations of utility functions for writing JSON data to
a file stream.
@details Provides Funcions for:
- Creating new output files
- Creating the main variables for the files.
- Adding new key,value pairs for
 - strings
 - numbers
 - lists
- Maintaining and properly emitting lists.
- Properly closing all of the constructs.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// For common data structures like linked lists etc.
#include "verilog-parser/src/verilog_ast_common.h"

#ifndef VERIDOC_JSON_H
#define VERIDOC_JSON_H

typedef struct json_object_t json_object;

/*!
@brief A simple structure for helping to write out JSON encoded data.
*/
typedef struct json_file_t{
    char * file_path;   //!< Output file path.
    FILE * fh;          //!< The open file handle.
} json_file;


/*!
@brief Creates a new json file handle.
@param[in] path - File path of the file to create / overwrite.
@returns a json_file construct if the output path can be opened, else NULL.
*/
json_file * json_new_file(
    char * path
);

//! Frees the memory allocated to a json_file construct and closes the file.
void json_close_file(json_file * tofree);

// ---------------------------------------------------------------------

/*!
@brief A holder for a new json object or collection of properties.
*/
struct json_object_t{
    ast_list * kvps; //!< Key value pairs. Each value is a json_kvp
};


//! Creates and returns a pointer to a new json object.
json_object * json_new_object();

//! Adds a string and associated key to the supplied object.
void json_object_add_string(
    json_object * obj,
    char * key,
    char * value
);

//! Adds an integer and associated key to the supplied object.
void json_object_add_int(
    json_object * obj,
    char * key,
    int    value
);

//! Adds a list and associated key to the supplied object.
void json_object_add_list(  
    json_object * obj,
    char * key,
    json_object * list
);

//! Adds a object and associated key to the supplied object.
void json_object_add_object(
    json_object * obj,
    char * key,
    json_object * value
);

/*!
@brief emits the supplied object into the supplied file, with an optional
variable name.
@param[in] varName - The name of the first variable (var) in the file, which
will hold the forthcoming JSON data structure. Iff NULL, no `var X=` will be
emitted.
*/
void json_emit_object(
    json_file   * fh,
    json_object * toemit,
    char        * varName,
    unsigned char as_list
);

// ---------------------------------------------------------------------

//! Describes the type of object stored in a json_kvp.
typedef enum json_kvp_type_e{
    JSON_KVP_INT,   //!< Value is an integer (int)
    JSON_KVP_STR,   //!< Value is a string (char*)
    JSON_KVP_LIST,  //!< Value is a json_list
    JSON_KVP_OBJ,   //!< Value is a json_object
} json_kvp_type;

//! Stores a single <key,value> pair.
typedef struct json_kvp_t{
    json_kvp_type type; //! What sort of data type does the key point at?
    char * key; //<! The key to the value.
    union{
        int         integer;    //!< IFF type == INT
        char *      string;     //!< IFF type == STR
        json_object * list;       //!< IFF type == LIST
        json_object * object;     //!< IFF type == OBJ
    };
} json_kvp;

//! Creates and returns a new KVP object with the supplied key and datatype.
json_kvp * json_new_kvp(
    char * key,
    json_kvp_type type
);


// ---------------------------------------------------------------------

#endif
