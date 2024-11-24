#ifndef CL_INITIALIZER_BASE_H
#define CL_INITIALIZER_BASE_H

#define CL_TARGET_OPENCL_VERSION 300
#include <CL/cl.h>

extern cl_context context;
extern cl_command_queue queue;
extern cl_kernel *kernels;

#endif