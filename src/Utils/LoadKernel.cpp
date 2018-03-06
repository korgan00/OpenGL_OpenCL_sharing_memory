
#include "LoadKernels.h"

KernelObjects LoadKernel(KernelInfo info, cl_int* ret) {
    FILE *fp;
    char *source_str;
    size_t source_size;

    //fp = fopen("../Kernels/kernel.cl", "r");
    fp = fopen(info.kernelFile, "r");
    if (!fp) {
        fprintf(stderr, "Failed to load kernel.\n");
        exit(1);
    }
    source_str = (char*)malloc(MAX_SOURCE_SIZE);
    source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
    fclose(fp);

    cl_kernel kernel = 0;
    cl_program program = clCreateProgramWithSource(info.context, 1,
        (const char **)&source_str, (const size_t *)&source_size, ret);
    CHECK_KERNEL_ERROR;

    *ret = clBuildProgram(program, 1, info.device_id, NULL, NULL, NULL);
    CHECK_KERNEL_ERROR;

    kernel = clCreateKernel(program, info.kernelName, ret);
    CHECK_KERNEL_ERROR;

    return { program, kernel };
}