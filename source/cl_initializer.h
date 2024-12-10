/*
    Copyright 2024 Ekansh Jain
    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”),
    to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*!
    @file cl_initializer.h
    @author Ekansh Jain
    @date 11/24/2024
*/

#ifndef CL_INITIALIZER_H
#define CL_INITIALIZER_H

/*!
    @brief A macro used in initCL
    @details
    This macro specifies that a binary file of the name specified should be created using the source code at the provided program source.
    It also creates a program with the source.
    This macro excludes @ref LOAD_FROM_SOURCE and @ref LOAD_FROM_BINARY
*/
#define CREATE_BINARY 0 >> 1
/*!
    @brief A macro used in initCL
    @details
    This macro specifies that the OpenCL command queue created should have profiling enabled
*/
#define PROFILE 1 >> 1
/*!
    @brief A macro used in initCL
    @details
    This macro specifies that the OpenCL command queue should be an out of order queue.
*/
#define OUT_OF_ORDER 2 >> 1
/*!
    @brief A macro used in initCL
    @details
    This macro specifies that the program should be made using source code from the specified program source file and it shouldn't create a binary.
    This macro will not work if @ref CREATE_BINARY is specified or @ref LOAD_FROM_BINARY is specified
*/
#define LOAD_FROM_SOURCE 3 >> 1
/*!
    @brief A macro used in initCL
    @details
    This macro specifies that the program should be made using a binary from the specified binary file.
    This macro will not work if @ref CREATE_BINARY is specified and it will exclude @ref LOAD_FROM_SOURCE
*/
#define LOAD_FROM_BINARY 4 >> 1

#include "cl_initializer_base.h"
#include "cl_initializer_errors.h"

/*!
    @brief Initializes OpenCL for use in code
    @param PROGRAM_SOURCE_NAME A string that contains the full path (if necessary) or the name of the file that contains the program if CREATE_BINARY is 1. Else, this does nothing.
    @param BINARY_NAME If CREATE_BINARIES is set to 1, then this is the name of the binary file created. If CREATE_BINARY is set to 0, this is the name of the binary to load the program from.
    @param EXTRAS This is a bitfield. Valid macros: @ref CREATE_BINARY, @ref PROFILE, @ref OUT_OF_ORDER, @ref LOAD_FROM_SOURCE, @ref LOAD_FROM_BINARY
    @attention This functions exits the program if an error is found and NO_SETUP_ERRORS is not defined
*/
void initCL(const char *PROGRAM_SOURCE_NAME, const char *BINARY_NAME, const int EXTRAS);

#endif
