/*
    Copyright 2024 Ekansh Jain
    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”),
    to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*\

#ifndef CL_INITIALIZER_H
#define CL_INITIALIZER_H

#include "cl_initializer_base.h"
#include "cl_initializer_errors.h"

/*!
    @file cl_initializer.h
    @author Ekansh Jain
    @date 11/24/2024
*/

/*!
    @brief Initializes OpenCL for use in code
    @param PROGRAM_SOURCE_NAME A string that contains the full path (if necessary) or the name of the file that contains the program if CREATE_BINARY is 1. Else, this does nothing.
    @param BINARY_NAME If CREATE_BINARIES is set to 1, then this is the name of the binary file created. If CREATE_BINARY is set to 0, this is the name of the binary to load the program from.
    @param PROFILE If PROFILE is 1, this creates a queue with profiling enabled
    @param OUT_OF_ORDER If OUT_OF_ORDER is 1, this creates a queue with out of order execution enabled
    @attention This functions exits the program if an error is found and NO_SETUP_ERRORS is not defined
*/
void initCL(const char *PROGRAM_SOURCE_NAME, const char *BINARY_NAME, const int CREATE_BINARIES, const int PROFILE, const int OUT_OF_ORDER);

#endif
