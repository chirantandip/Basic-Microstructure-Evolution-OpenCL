/**
@file CL_utility_funcs.h
@brief Defines some utility functions to make life easier.
*/

#ifndef CL_UTILITY_FUNCTIONS
#define CL_UTILITY_FUNCTIONS

#include <stdio.h>
#include <stdlib.h>
#ifdef MAC
#include <OpenCL/cl.h>
#else  
#include <CL/cl.h>
#endif

#include "global_vars.h"
#include "error_handle.h"

/** The function initialises all the necessary OpenCL data structures. 
@return It does not return anything but initialises the globally declared OpenCL structures.
@param It does not take any input parameter but reads directly from the global variables.
*/
static inline void initCLDataStructures(){
    // Error variable
    cl_int err ;
    
    // Get platforms
    cl_uint num_platforms ;
    err = clGetPlatformIDs(1,NULL, &num_platforms);
    cl_platform_id *platforms;
    platforms = (cl_platform_id*)malloc(sizeof(cl_platform_id)*num_platforms);
    err = clGetPlatformIDs(num_platforms, platforms, NULL);
    ErrorHandle(err, "clGetPlatformIDs");
    platform = platforms[platID];
    free(platforms);
    
    // get the number of devices availabe on that platform
    cl_uint numDevices;
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 0, NULL, &numDevices);
    ErrorHandle(err, "clGetDeviceIDs numDevices");
    
    // get the devices into an array
    devices = (cl_device_id*)malloc(sizeof(cl_device_id)*numDevices);
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL,numDevices, devices, NULL);
    ErrorHandle(err, "clGetDeviceIDs devices");
    
    // Create a context for all availabe devices
    context = (cl_context)malloc(sizeof(cl_context));
    context = clCreateContext(NULL, numDevices, devices, NULL, NULL, &err);
    ErrorHandle( err, "clCreateContext");
    
    queue = clCreateCommandQueue(context,devices[devID], CL_QUEUE_PROFILING_ENABLE, &err);
    ErrorHandle(err, "clCreateCommandQueue");
    
}


/**
@brief The function computes the optimum work group (WG) size for an OpenCL kernel and a device.
@param kernel The cl_kernel for which the optimum WG size is to be found.
@param device The cl_device_id device on which the optimum WG size is to be found.
@return It returns the optimum WG size in size_t type.

This value is used when the work group size is set to 0 in the input file.
It uses clGetKernelWorkGroupInfo() function and CL_KERNEL_WORK_GROUP_SIZE flag.
*/
size_t GetOptimumWGSize(cl_kernel kernel, cl_device_id device){
    cl_int err;
    size_t kernWGsize;
    err = clGetKernelWorkGroupInfo( kernel, device, CL_KERNEL_WORK_GROUP_SIZE, sizeof(kernWGsize), &kernWGsize, NULL);
    ErrorHandle(err,"clGetKernelWorkGroupInfo");
    int localSize = 2;
    for(localSize = 2; localSize*localSize*4 < kernWGsize; localSize*=2) ;
    return localSize;
}


/**
@brief The functions calculates the total execution time of an event.
@param event A cl_event whose execution time is to be calculated. 
@return The function returns the time in decimal seconds.

The function does not calculate how much time the event spent waiting in the command queue.
*/
cl_float GetEventExecTime(cl_event event){
    cl_int err ;
    cl_ulong start, end;
    err = clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(start), &start, NULL);
    err = clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(end), &end, NULL);
    KernErrorHandle(err, "clGetEventProfilingInfo");
    return (end-start)/1.0e9 ; 
}

#endif
// END OF FILE