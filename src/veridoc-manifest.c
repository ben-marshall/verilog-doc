/*!
@file veridoc-manifest.c
@brief Contains common data structures and functions used on the
file manifests
*/

#include "veridoc-manifest.h"


/*!
@brief Parses the supplied manifest file and returns a representation of it.
@param [in] path - The manifest file to parse.
@returns A pointer to the manifest object, or NULL if the file could not be
found or parsed.
*/
veridoc_manifest * veridoc_manifest_parse(
    char * path
){
    FILE * fh = fopen(path, "r");

    if(fh == NULL){
        printf("ERROR: Could not open manifest file path: '%s'\n", path);
        return NULL;
    }
    
    veridoc_manifest * tr = calloc(1,sizeof(veridoc_manifest));

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
        else
        {
            // This is a line worth parsing!
            
            char * line = calloc(1023,sizeof(char));
            int i = 0;
            while(i < 1023 && !feof(fh) && first_on_line != '\n'){
                line[i] = first_on_line;
                first_on_line = fgetc(fh);
                i++;
            }

            if(strlen(line) <=0)
            {
                free(line);
                continue;
            }

            // Is this line a file or a folder?
            DIR * d = opendir(line);

            if(d)
            {
                // It is a directory.
                tr -> directory_count += 1;
                tr -> directories = realloc(tr -> directories, 
                    tr->directory_count * sizeof(veridoc_manifest_directory));
                tr -> directories[tr -> directory_count-1].path = line;
            } 
            else if(errno == ENOTDIR)
            {
                // The path exists, and is a file.
                tr -> file_count += 1;
                tr -> files= realloc(tr -> files, 
                    tr->file_count * sizeof(veridoc_manifest_file));

                tr -> files[tr -> file_count-1].path = line;
                tr -> files[tr -> file_count-1].parsed = BOOL_FALSE;
                tr -> files[tr -> file_count-1].parse_success =  BOOL_FALSE;
            }
            else if(errno == ENOENT)
            {
                printf("ERROR: no such manifest path:'%s'.\n", line);
            }
            else
            {
                // The specified file does not exist.
                printf("ERROR: unknown error with manifest path:'%s'.\n", line);
            }
        }
    }
    
    return tr;

}

/*!
@brief Frees a file manifest from memory.
*/
void veridoc_manifest_free(
    veridoc_manifest * tofree
){

    int d;
    for(d = 0; d < tofree -> directory_count; d++)
    {
        free(tofree -> directories[d].path);
    }

    int f;
    for(f = 0; f < tofree -> directory_count; f++)
    {
        free(tofree -> files[f].path);
    }

    free(tofree -> files);
    free(tofree -> directories);
    free(tofree);
}
