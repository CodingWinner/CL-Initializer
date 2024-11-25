/*
Copyright 2024 Ekansh Jain
    Permission is hereby granted,
    free of charge, to any person obtaining a copy of this software and associated documentation files(the “Software”),
    to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and / or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions : The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.* /
*/

#include "cl_initializer_base.h"
#include "cl_initializer_errors.h"
#include <stdio.h>
#include <stdlib.h>

#define CHECK_BUILD_ERROR                                                                            \
    if (err != CL_SUCCESS)                                                                           \
    {                                                                                                \
        size_t log_size;                                                                             \
        err = clGetProgramBuildInfo(program, devices[0], CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);  \
        CHECK_CL_ERROR                                                                               \
        char *log = malloc(log_size);                                                                \
        MEM_ALLOC_ERR(log)                                                                           \
        err = clGetProgramBuildInfo(program, devices[0], CL_PROGRAM_BUILD_LOG, log_size, log, NULL); \
        if (err != CL_SUCCESS)                                                                       \
        {                                                                                            \
            free(log);                                                                               \
            CHECK_CL_ERROR                                                                           \
        }                                                                                            \
        printf("Build log:\n%s", log);                                                               \
        free(log);                                                                                   \
        exit(1);                                                                                     \
    }

#ifndef NO_SETUP_ERRORS
cl_int err;
#endif

cl_device_id *devices = NULL;
cl_int device_num = 0;

cl_context context = NULL;
cl_command_queue queue = NULL;

cl_program program = NULL;

cl_kernel *kernels = NULL;
cl_int num_kernels;

cl_int loadPlatforms(cl_platform_id *platforms)
{
    cl_int platform_num;

#ifndef NO_SETUP_ERRORS
    err = clGetPlatformIDs(0, NULL, &platform_num);

    if (!platform_num)
    {
        printf("No available OpenCL platforms\n");
        exit(1);
    }
    CHECK_CL_ERROR
#else
    clGetPlatformIDs(0, NULL, &platform_num);
#endif

    platforms = malloc(sizeof(cl_platform_id) * platform_num);

#ifndef NO_SETUP_ERRORS
    MEM_ALLOC_ERR(platforms)
#endif

#ifndef NO_SETUP_ERRORS
    err = clGetPlatformIDs(platform_num, platforms, NULL);
    CHECK_CL_ERROR
#else
    clGetPlatformIDs(platform_num, platforms, NULL);
#endif

    return platform_num;
}

void loadDevices(cl_int platform_num, cl_platform_id *platforms)
{
    for (int i = 0; i < platform_num; i++)
    {
#ifndef NO_SETUP_ERRORS
        err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 0, NULL, &device_num);
        CHECK_CL_ERROR
#else
        clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 0, NULL, &device_num);
#endif
        if (!device_num)
            continue;
        else
            device_num = 1;

        devices = malloc(sizeof(cl_device_id));

#ifndef NO_SETUP_ERRORS
        MEM_ALLOC_ERR(platforms)
#endif

#ifndef NO_SETUP_ERRORS
        err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 1, devices, NULL);
        CHECK_CL_ERROR
#else
        clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 1, devices, NULL);
#endif
        break;
    }

    if (!device_num)
    {
        printf("No available GPUs for OpenCL.\n");
        exit(1);
    }
}

void createQueue(cl_queue_properties properties)
{
#ifndef NO_SETUP_ERRORS
    queue = clCreateCommandQueueWithProperties(context, devices[0], properties, &err);
    CHECK_CL_ERROR
#else
    queue = clCreateCommandQueueWithProperties(context, devices[0], properties, NULL);
#endif
}

void createProgramFromSource(const char *PROGRAM_SOURCE_NAME)
{
    FILE *source_file = fopen(PROGRAM_SOURCE_NAME, "r");

#ifndef NO_SETUP_ERRORS
    if (!source_file)
    {
        printf("Unable to load program source. Name: %s. Errno: %i\n", PROGRAM_SOURCE_NAME, errno);
        exit(1);
    }

    err = fseek(source_file, 0, SEEK_END);

    if (err)
    {
        printf("Error while seeking the end of the file. Errno: %i\n", errno);
        fclose(source_file);
        exit(1);
    }

#else
    fseek(source_file, 0, SEEK_END);
#endif

    long file_size = ftell(source_file);

#ifndef NO_SETUP_ERRORS
    if (file_size == -1)
    {
        printf("Error while seeking the file position. Errno: %i\n", errno);
        fclose(source_file);
        exit(1);
    }
#endif

    rewind(source_file);
    char *source = malloc(file_size);

#ifndef NO_SETUP_ERRORS
    MEM_ALLOC_ERR(source)
#endif

    file_size = fread(source, 1, file_size, source_file);
    fclose(source_file);
    source = realloc(source, file_size + 1);
    source[file_size] = '\0';

#ifndef NO_SETUP_ERRORS
    MEM_ALLOC_ERR(source)
#endif

#ifndef NO_SETUP_ERRORS
    program = clCreateProgramWithSource(context, 1, (const char **)&source, NULL, &err);
    CHECK_CL_ERROR
    free(source);
    err = clBuildProgram(program, 1, devices, NULL, NULL, NULL);
    CHECK_BUILD_ERROR
#else
    program = clCreateProgramWithSource(context, 1, (const char **)&source, NULL, NULL);
    clBuildProgram(program, 1, devices, NULL, NULL, NULL);
#endif
}

void initCL(const char *PROGRAM_SOURCE_NAME, const char *BINARY_NAME, const int CREATE_BINARIES, const int PROFILE, const int OUT_OF_ORDER, const int LOAD_FROM_SOURCE)
{

    cl_platform_id *platforms;
    cl_int platform_num = loadPlatforms(platforms);
    loadDevices(platform_num, platforms);

#ifndef NO_SETUP_ERRORS
    context = clCreateContext(NULL, 1, devices, NULL, NULL, &err);
    CHECK_CL_ERROR

#else
    context = clCreateContext(NULL, 1, devices, NULL, NULL, NULL);
#endif

    if (PROFILE && OUT_OF_ORDER)
    {
        cl_queue_properties properties[4] = {CL_QUEUE_PROPERTIES, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, CL_QUEUE_PROFILING_ENABLE, 0};
        createQueue(properties);
    }
    else if (PROFILE)
    {
        cl_queue_properties properties[3] = {CL_QUEUE_PROPERTIES, CL_QUEUE_PROFILING_ENABLE, 0};
        createQueue(properties);
    }
    else if (OUT_OF_ORDER)
    {
        cl_queue_properties properties[3] = {CL_QUEUE_PROPERTIES, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, 0};
        createQueue(properties);
    }
    else
    {
        cl_queue_properties properties[2] = {CL_QUEUE_PROPERTIES, 0};
        createQueue(properties);
    } // PROFILE

    // Create a binary
    if (CREATE_BINARIES)
    {

        createProgramFromSource(PROGRAM_SOURCE_NAME);

        size_t binary_size;
        unsigned char *binary;

#ifndef NO_SETUP_ERRORS
        err = clGetProgramInfo(program, CL_PROGRAM_BINARY_SIZES, sizeof(size_t), (const unsigned char **)&binary_size, NULL);
        CHECK_CL_ERROR
        binary = malloc(binary_size);
        MEM_ALLOC_ERR(binary)
        err = clGetProgramInfo(program, CL_PROGRAM_BINARIES, sizeof(char *), (const unsigned char **)&binary, NULL);
        CHECK_CL_ERROR
        FILE *binary_file = fopen(BINARY_NAME, "wb");
        if (!binary_file)
        {
            printf("Unable to open %s. Errno: %i", BINARY_NAME, errno);
            exit(1);
        }
#else
        clGetProgramInfo(program, CL_PROGRAM_BINARY_SIZES, sizeof(size_t), &binary_size, NULL);
        binary = malloc(binary_size);
        clGetProgramInfo(program, CL_PROGRAM_BINARIES, sizeof(char *), (const unsigned char **)&binary, NULL);
        FILE *binary_file = fopen(BINARY_NAME, "wb");
#endif

        fwrite(binary, binary_size, 1, binary_file);
        fclose(binary_file);
        free(binary);
    }
    else if (!LOAD_FROM_SOURCE)
    {

        FILE *binary_file = fopen(BINARY_NAME, "rb");

#ifndef NO_SETUP_ERRORS
        if (!binary_file)
        {
            printf("Unable to open %s. Errno: %i", BINARY_NAME, errno);
            exit(1);
        }
        err = fseek(binary_file, 0, SEEK_END);
        if (err)
        {
            printf("Unable to seek the end of the binary file. Errno: %i", errno);
            fclose(binary_file);
            exit(1);
        }
#else
        fseek(binary_file, 0, SEEK_END);
#endif

        size_t binary_size = ftell(binary_file);

#ifndef NO_SETUP_ERRORS
        if (binary_size == -1)
        {
            printf("Unable to get the position of the binary file. Errno: %i", errno);
            fclose(binary_file);
            exit(1);
        }
#endif

        rewind(binary_file);

        unsigned char *binary = malloc(binary_size);

#ifndef NO_SETUP_ERRORS
        MEM_ALLOC_ERR(binary)
#endif

        binary_size = fread(binary, 1, binary_size, binary_file);
        binary = realloc(binary, binary_size + 1);
        binary[binary_size] = '\0';

        fclose(binary_file);

#ifndef NO_SETUP_ERRORS
        MEM_ALLOC_ERR(binary)

        cl_int binary_status;
        program = clCreateProgramWithBinary(context, 1, devices, &binary_size, (const unsigned char **)&binary, &binary_status,
                                            &err);
        free(binary);
        CHECK_CL_ERROR
        if (binary_status != CL_SUCCESS)
        {
            printf("Unable to load program binary. Error code: %i", binary_status);
            exit(1);
        }
        err = clBuildProgram(program, 1, devices, NULL, NULL, NULL);
        CHECK_BUILD_ERROR
#else
        program = clCreateProgramWithBinary(context, 1, devices, (const size_t *)&binary_size, (const unsigned char **)&binary,
                                            NULL, NULL);
        clBuildProgram(program, 1, devices, NULL, NULL, NULL);
#endif
    }
    else if (LOAD_FROM_SOURCE)
    {
        createProgramFromSource(PROGRAM_SOURCE_NAME);
    } // CREATE_BINARIES

#ifndef NO_SETUP_ERRORS
    err = clCreateKernelsInProgram(program, 0, NULL, &num_kernels);
    CHECK_CL_ERROR
    kernels = malloc(sizeof(cl_kernel) * num_kernels);
    MEM_ALLOC_ERR(kernels);
    err = clCreateKernelsInProgram(program, num_kernels, kernels, NULL);
    CHECK_CL_ERROR

#else
    clCreateKernelsInProgram(program, 0, NULL, &num_kernels);
    kernels = malloc(sizeof(cl_kernel) * num_kernels);
    clCreateKernelsInProgram(program, num_kernels, kernels, NULL);
#endif

    clReleaseDevices(devices[0]);
    free(devices);
    clReleaseProgram(program);
}

void __attribute__((destructor, used)) end()
{
    if (devices)
    {
        clReleaseDevice(devices[0]);
        free(devices);
    }
    if (context)
    {
        clReleaseContext(context);
    }
    if (queue)
    {
        clReleaseCommandQueue(queue);
    }
    if (program)
    {
        clReleaseProgram(program);
    }
    if (kernels)
    {
        for (int i = 0; i < num_kernels; i++)
        {
            clReleaseKernel(kernels[i]);
        }
        free(kernels);
    }
}
