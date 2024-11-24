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
    @param PROGRAM_SOURCE_NAME A string that contains the full path (if necessary) or the name of the file that contains the program if CREATE_BINARY is defined. Else, this does nothing.
    @param BINARY_NAME If CREATE_BINARY is defined, then this is the name of the binary file created. If CREATE_BINARY is not defined, this is the name of the binary to load the program from.
    @attention This functions exits the program if an error is found and NO_SETUP_ERRORS is not defined
*/
void initCL(const char *PROGRAM_SOURCE_NAME, const char *BINARY_NAME);

#endif