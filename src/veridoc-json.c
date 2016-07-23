/*!
@file veridoc-json.c
@brief Provides declarations of utility functions for writing JSON data to
a file stream.
*/

#include "veridoc-json.h"

/*!
@brief Emits a single JSON encoded key/value pair to the supplied file handle.
*/
void json_emit_str(
    FILE * fh,
    char * key,
    char * value
){
    fprintf(fh,"\"%s\":\"%s\"\n",key,value);
}

/*!
@brief Emits a single JSON encoded key/value pair to the supplied file handle.
*/
void json_emit_int(
    FILE * fh,
    char * key,
    int    value
){
    fprintf(fh,"\"%s\":\"%d\"\n",key,value);
}


/*!
@brief Opens a JSON list element with the supplied key.
*/
void json_begin_list(
    FILE * fh,
    char * key
){
    fprintf(fh,"\"%s\":[\n",key);
}

/*!
@brief Closes the most recently opened JSON list element.
*/
void json_end_list(
    FILE * fh
){
    fprintf(fh,"]\n");
}

/*!
@brief Emits a sepparator (comma) to delimit a list of json elements.
*/
void json_sepp(
    FILE * fh
){
    fprintf(fh,",");
}
