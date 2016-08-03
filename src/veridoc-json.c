/*!
@file veridoc-json.c
@brief Provides declarations of utility functions for writing JSON data to
a file stream.
*/

#include "veridoc-json.h"

/*!
@brief Creates a new json file handle.
@param[in] path - File path of the file to create / overwrite.
@param[in] varName - The name of the first variable (var) in the file, which
will hold the forthcoming JSON data structure. Iff NULL, no `var X=` will be
emitted.
@returns a json_file construct if the output path can be opened, else NULL.
*/
json_file * json_new_file(
    char * path
){
    json_file * tr = calloc(1,sizeof(json_file));

    tr -> file_path = path;
    tr -> fh        = fopen(path, "w");

    if(!tr->fh)
    {
        free(tr);
        return NULL;
    }
    else
    {
        return tr;
    }
}

//! Frees the memory allocated to a json_file construct.
void json_close_file(json_file * tofree)
{
    if(tofree -> fh)
    {
        fclose(tofree -> fh);
    }

    free(tofree);
}

//! Creates and returns a new KVP object with the supplied key and datatype.
json_kvp * json_new_kvp(
    char * key,
    json_kvp_type type
){
    json_kvp * tr = calloc(1,sizeof(json_kvp));

    tr -> key = key;
    tr -> type = type;

    return tr;
}

//! Creates and returns a pointer to a new json object.
json_object * json_new_object()
{
    json_object * tr = calloc(1,sizeof(json_object));
    tr -> kvps = ast_list_new();
    return tr;
}


//! Adds a string and associated key to the supplied object.
void json_object_add_string(
    json_object * obj,
    char * key,
    char * value
){
    json_kvp * toadd = json_new_kvp(key, JSON_KVP_STR);
    toadd -> string = value;
    ast_list_append(obj -> kvps, toadd);
}

//! Adds an integer and associated key to the supplied object.
void json_object_add_int(
    json_object * obj,
    char * key,
    int    value
){
    json_kvp * toadd = json_new_kvp(key, JSON_KVP_INT);
    toadd -> integer = value;
    ast_list_append(obj -> kvps, toadd);
}

//! Adds a list and associated key to the supplied object.
void json_object_add_list(  
    json_object * obj,
    char * key,
    json_object * value 
){
    json_kvp * toadd = json_new_kvp(key, JSON_KVP_LIST);
    toadd -> list = value;
    ast_list_append(obj -> kvps, toadd);
}

//! Adds a object and associated key to the supplied object.
void json_object_add_object(
    json_object * obj,
    char * key,
    json_object * value
){
    json_kvp * toadd = json_new_kvp(key, JSON_KVP_OBJ);
    toadd -> object = value;
    ast_list_append(obj -> kvps, toadd);
}

/*!
@brief emits the supplied object into the supplied file, with an optional
variable name.
@param[in] varName - The name of the first variable (var) in the file, which
will hold the forthcoming JSON data structure. Iff NULL, no `var X=` will be
emitted.
@param[in] as_list - Emit using the syntax for a list ([...]) rather than an
object ({...}).
*/
void json_emit_object(
    json_file   * fh,
    json_object * toemit,
    char        * varName,
    unsigned char as_list
){
    assert(fh -> fh); // Make sure the file is open!
    
    if(varName){
        // Should we emit a variable to go with the JSON?
        fprintf(fh->fh,"var %s = \n", varName);
    }

    // Now do the actual object emission
    fprintf(fh->fh, as_list ? "[\n" : "{\n");
    
    unsigned int i;
    for(i = 0; i < toemit -> kvps -> items; i++)
    {
        json_kvp * kvp = ast_list_get(toemit -> kvps, i);

        if(!as_list){
            fprintf(fh->fh, "\"%s\" : ", kvp -> key);
        }

        switch(kvp -> type)
        {
            case JSON_KVP_STR:
                fprintf(fh->fh, "\"%s\"\n", kvp -> string);
                break;
            case JSON_KVP_INT:
                fprintf(fh->fh, "%d\n", kvp -> integer);
                break;
            case JSON_KVP_LIST:
                json_emit_object(fh, kvp -> object, NULL, 1);
                break;
            case JSON_KVP_OBJ:
                json_emit_object(fh, kvp -> object, NULL, 0);
                break;
        }

        if(i < toemit -> kvps -> items - 1){
            fprintf(fh -> fh, ",");
        }
    }

    fprintf(fh->fh, as_list ? "]\n" : "}\n");


    if(varName){
        // Do we need to close off a variable declaration?
        fprintf(fh->fh,";\n");
    }
}
