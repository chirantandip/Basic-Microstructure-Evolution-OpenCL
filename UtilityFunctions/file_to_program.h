/**
@file file_to_program.h
@brief Defines the function(s) that reads a .cl file and converts them into a cl_kernel.
It is essential that a file contains only one kernel of one name. The .cl files are found in the Kernels directory.
*/


#ifndef FILE_TO_PROG
#define FILE_TO_PROG

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef MAC
#include <OpenCL/cl.h>
#else  
#include <CL/cl.h>
#endif

#include "global_vars.h"
#include "error_handle.h"

/**
@brief The function reads a program file, complies it and returns a kernel.

@param ProgFileName defines the name of the program file with path. 
@param InpParams is the input parameters struct. Here INP_PARAMS_STRUCT is a MACRO that takes a specific struct name as per the declared SYSTEM.
@return A compiled cl_kernel.

It reads the raw program file into a file handle of type FILE*, creates and char* buffer and reads the entire file into it. Then it compiles the program buffer with the inbuilt clCreateProgramWithSource() function. The clCreateProgramWithSource() functions takes an arugument called Build Program Options in which we pass the constants from the INP_PARAMS_STRUCT as MACROs. If there is any error in compiling, the function will print (in runtime log) the the program build log and exit. If successfull it will return the OpenCL kernel in the file named phase_field_evol_kern

NOTE AGAIN : The kernel has to be named phase_field_evol_kern .

*/
cl_kernel getKernelFromFile(const char ProgFileName[], struct INP_PARAMS_STRUCT InpParams){
    cl_int err;
    FILE *program_handle ;
    char *program_buffer ;
    size_t program_size ;
    program_handle = fopen(ProgFileName, "r");
    if(program_handle == NULL){
        perror("Error in reading program file. \n");
        exit(1);
    }
    fseek(program_handle, 0, SEEK_END);
    program_size = ftell(program_handle);
    rewind(program_handle);
    program_buffer = (char*)malloc(sizeof(char)*(program_size+1));
    program_buffer[program_size] = '\0';
    fread(program_buffer, sizeof(char), program_size, program_handle);
    fclose(program_handle);

    cl_program program ;
    program = clCreateProgramWithSource(context, 1, (const char**)&program_buffer, &program_size, &err);
    ErrorHandle(err, "clCreateProgramWithSource");
    free(program_buffer);
    
#ifdef DIFFUSION
    char BuildProgOptions[65];
    err = sprintf(BuildProgOptions,"-DSIZE=%d -DH=%f -DDT=%f -DCOEFF=%f", SIZE, DX,DT, InpParams.DIFF_COEFF);
#endif
    
#ifdef CAHNHILLIARD
    char BuildProgOptions[80];
    err = sprintf(BuildProgOptions,"-DSIZE=%d -DH=%f -DDT=%f -DMOBILITY=%f -DKAPPA=%f",SIZE, DX,DT, InpParams.MOBILITY, InpParams.KAPPA);
#endif


#ifdef KOBISO
    char BuildProgOptions[900];
    err = sprintf(BuildProgOptions,"-DSIZE=%d -DH=%f -DDT=%f  -DEPS_BAR=%f -DALPHA=%f -DGAMMA=%f -DTAU=%f -DPH_L=%f -DPH_R=%f -DPH_T=%f -DPH_B=%f -DTHERM_DIFF=%f -DLAT_H=%f -DT_MELT=%f -DT_L=%f -DT_R=%f -DT_T=%f -DT_B=%f", SIZE, DX, DT, InpParams.EPS_BAR, InpParams.ALPHA, InpParams.GAMMA, InpParams.TAU, InpParams.PHASE_L,InpParams.PHASE_R, InpParams.PHASE_T, InpParams.PHASE_B ,InpParams.TH_DIFF, InpParams.L_HEAT, InpParams.T_MELT, InpParams.TEMP_L,InpParams.TEMP_R,InpParams.TEMP_T,InpParams.TEMP_B);
    
#endif

#ifdef KOBANISO
    char BuildProgOptions[300];
    err = sprintf(BuildProgOptions,"-DSIZE=%d -DH=%f -DEPS_BAR=%f -DALPHA=%f -DGAMMA=%f -DDELTA=%f -DTAU=%f -DTHETA0=%f -DJ=%f -DDT=%f -DTHERM_DIFF=%f -DLAT_H=%f -DT_MELT=%f ", SIZE, DX, InpParams.EPS_BAR, InpParams.ALPHA, InpParams.GAMMA, InpParams.DELTA,InpParams.TAU, InpParams.THETA0, InpParams.J, DT, InpParams.TH_DIFF, InpParams.L_HEAT, InpParams.T_MELT);
#endif
    

    err = clBuildProgram(program, 0, NULL, BuildProgOptions, NULL, NULL);

    if (err<0){        
        size_t log_size ;
        clGetProgramBuildInfo(program, devices[devID], CL_PROGRAM_BUILD_LOG, 0, NULL,&log_size);
        char *program_log;
        program_log = (char*)malloc(log_size+1);
        program_log[log_size] = '\0';
        clGetProgramBuildInfo(program, devices[devID], CL_PROGRAM_BUILD_LOG, log_size+1, program_log, NULL);
        printf("%s\n", program_log);
        exit(1);
    }
    else{
        ErrorHandle(0, "clBuildProgram" );
    }
    kernel = clCreateKernel(program, "phase_field_evol_kern", &err);
    ErrorHandle(err, "clCreateKernel");

    return kernel ;
}

#endif
// END OF FILE