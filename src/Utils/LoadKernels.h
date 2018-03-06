#ifndef LOAD_KERNELS_H_
#define LOAD_KERNELS_H_

#include "SDL_OGL_GL3W_Dev_GeneralInclude.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#include <CL/cl_gl.h>
#include <CL/cl_gl_ext.h>
#endif

#define MAX_SOURCE_SIZE (0x100000)
#define CHECK_KERNEL_ERROR if (*ret != CL_SUCCESS) { return { program, kernel }; }

typedef struct {
    const cl_context context;
    const cl_device_id* device_id;
    const char*  kernelFile;
    const char*  kernelName;
} KernelInfo;

typedef struct {
    cl_program  program;
    cl_kernel   kernel;
} KernelObjects;

KernelObjects LoadKernel(KernelInfo info, cl_int* ret);


#endif