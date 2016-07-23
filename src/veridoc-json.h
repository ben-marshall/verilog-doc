/*!
@file veridoc-json.h
@brief Provides declarations of utility functions for writing JSON data to
a file stream.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifndef VERIDOC_JSON_H
#define VERIDOC_JSON_H

/*!
@brief Emits a single JSON encoded key/value pair to the supplied file handle.
*/
void json_emit_str(
    FILE * fh,
    char * key,
    char * value
);

/*!
@brief Emits a single JSON encoded key/value pair to the supplied file handle.
*/
void json_emit_int(
    FILE * fh,
    char * key,
    int    value
);


/*!
@brief Opens a JSON list element with the supplied key.
*/
void json_begin_list(
    FILE * fh,
    char * key
);

/*!
@brief Closes the most recently opened JSON list element.
*/
void json_end_list(
    FILE * fh
);

/*!
@brief Emits a sepparator (comma) to delimit a list of json elements.
*/
void json_sepp(
    FILE * fh
);


#endif
