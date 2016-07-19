/*!
@file veridoc-types.h
@brief Contains type definitions used globally by the program.
*/

#include "verilog-parser/src/verilog_parser.h"

#ifndef VERIDOC_TYPES_H
#define VERIDOC_TYPES_H

//! A simple boolean type.
typedef enum boolean_e{
    BOOL_TRUE  = 1,
    BOOL_FALSE = 0
} boolean;

#endif

