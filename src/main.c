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
printf("-C <file>, --config <file> - Set the configuration file path.\n");
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
    }

    return tr;
}

int main(int argc, char ** argv)
{
    shell_args * args = parse_args(argc,argv);

    free(args);
    return 0;
}
