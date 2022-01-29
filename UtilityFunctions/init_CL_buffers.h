/**
@file init_CL_buffers.h
@brief Declares functions to initialize OpenCL data buffers.
*/


#ifndef INIT_CL_BUFF
#define INIT_CL_BUFF

#include <stdio.h>
#include <stdlib.h>

#ifdef MAC
#include <OpenCL/cl.h>
#else  
#include <CL/cl.h>
#endif

#include "global_vars.h"
#include "error_handle.h"
#include "data_manip_funcs.h"

/**
@brief Initialize Kobayashi Anisotropic Data Buffers. 
@param InpParams The KobAnisoInputParams struct.
@return A KobAnisoDataBuffers struct.

*/
struct KobAnisoDataBuffers initKobayashiAnisoBuffers(struct KobAnisoInputParams InpParams){
    cl_int err ;
    struct KobAnisoDataBuffers dataBuffers ;
    // Initialize data
    dataBuffers.PHASE1 = Init1DFloatMatrix(SIZE,1.0) ;
    InitCenterCircle(dataBuffers.PHASE1, SIZE, SIZE/32, 0.0);
    dataBuffers.PHASE2 = Init1DFloatMatrix(SIZE,1.0) ;
    dataBuffers.TEMP1 = Init1DFloatMatrix(SIZE,InpParams.T_INIT) ;
    InitCenterCircle(dataBuffers.TEMP1, SIZE, SIZE/32, InpParams.T_BOUND);
    dataBuffers.TEMP2 = Init1DFloatMatrix(SIZE,InpParams.T_INIT) ;
    
    // Create buffers from matrix
    dataBuffers.PHASE1buff = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float)*SIZE*SIZE, dataBuffers.PHASE1, &err);
    ErrorHandle(err, "clCreateBuffer PHASE1");
    
    dataBuffers.PHASE2buff = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float)*SIZE*SIZE, dataBuffers.PHASE2, &err);
    ErrorHandle(err, "clCreateBuffer PHASE2");
    
    dataBuffers.TEMP1buff = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float)*SIZE*SIZE, dataBuffers.TEMP1, &err);
    ErrorHandle(err, "clCreateBuffer TEMP1");
    
    dataBuffers.TEMP2buff = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float)*SIZE*SIZE, dataBuffers.TEMP2, &err);
    ErrorHandle(err, "clCreateBuffer TEMP2");
    
    return dataBuffers ;
}

/**
@brief Initialize Kobayashi Isotropic Data Buffers. 
@param InpParams The KobIsoInputParams struct.
@return A KobIsoDataBuffers struct.

*/
struct KobIsoDataBuffers initKobayashiIsoBuffers(struct KobIsoInputParams InpParams){
    cl_int err ;
    struct KobIsoDataBuffers dataBuffers ;
    // Initialize data
    dataBuffers.PHASE1 = Init1DFloatMatrix(SIZE,1.0 ) ;
    dataBuffers.PHASE2 = Init1DFloatMatrix(SIZE,1.0 ) ;
    dataBuffers.TEMP1 = Init1DFloatMatrix(SIZE,InpParams.T_INIT) ;
    dataBuffers.TEMP2 = Init1DFloatMatrix(SIZE,InpParams.T_INIT) ;
    
    // Create buffers from matrix
    dataBuffers.PHASE1buff = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float)*SIZE*SIZE, dataBuffers.PHASE1, &err);
    ErrorHandle(err, "clCreateBuffer PHASE1");
    
    dataBuffers.PHASE2buff = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float)*SIZE*SIZE, dataBuffers.PHASE2, &err);
    ErrorHandle(err, "clCreateBuffer PHASE2");
    
    dataBuffers.TEMP1buff = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float)*SIZE*SIZE, dataBuffers.TEMP1, &err);
    ErrorHandle(err, "clCreateBuffer TEMP1");
    
    dataBuffers.TEMP2buff = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float)*SIZE*SIZE, dataBuffers.TEMP2, &err);
    ErrorHandle(err, "clCreateBuffer TEMP2");
    
    return dataBuffers ;
}

/**
@brief Initialize Diffusion system Data Buffers. 
@param InpParams The DiffusionInputParams struct.
@return A DiffusionDataBuffers struct.
*/
struct DiffusionDataBuffers initDiffusionBuffers(struct DiffusionInputParams InpParams){
    cl_int err ;
    struct DiffusionDataBuffers dataBuffers ;
    // Initialize data
    dataBuffers.PHASE1 = Init1DFloatMatrix(SIZE,0) ;
    InitCenterCircle(dataBuffers.PHASE1, SIZE, SIZE/8, 1);
    dataBuffers.PHASE2 = Init1DFloatMatrix(SIZE,0) ;
    
    // Create buffers from matrix
    dataBuffers.PHASE1buff = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float)*SIZE*SIZE, dataBuffers.PHASE1, &err);
    ErrorHandle(err, "clCreateBuffer PHASE1");
    
    dataBuffers.PHASE2buff = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float)*SIZE*SIZE, dataBuffers.PHASE2, &err);
    ErrorHandle(err, "clCreateBuffer PHASE2");
    
    return dataBuffers ;
}

/**
@brief Initialize Cahn-hilliard system (Spinodal decomposition problem) Data Buffers. 
@param InpParams The CahnHilliardInputParams struct.
@return A CahnHilliardDataBuffers struct.
*/
struct CahnHilliardDataBuffers initCahnHilliardBuffers(struct CahnHilliardInputParams InpParams){
    cl_int err ;
    struct CahnHilliardDataBuffers dataBuffers;
    //Initialize data
    dataBuffers.PHASE1 = RandomInit1DFloatMatrix( SIZE, InpParams.MEAN_C, InpParams.NOISE_AMP) ;
    dataBuffers.PHASE2 = Init1DFloatMatrix(SIZE,0.0) ;
    dataBuffers.InBracM = Init1DFloatMatrix(SIZE,0.0);
    
    // Create buffers from matrix
    dataBuffers.PHASE1buff = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float)*SIZE*SIZE, dataBuffers.PHASE1, &err);
    ErrorHandle(err, "clCreateBuffer PHASE1");
    
    dataBuffers.PHASE2buff = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float)*SIZE*SIZE, dataBuffers.PHASE2, &err);
    ErrorHandle(err, "clCreateBuffer PHASE2");
    
    dataBuffers.InBracMbuff = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float)*SIZE*SIZE, dataBuffers.InBracM, &err);
    ErrorHandle(err, "clCreateBuffer InBracM");
    
    return dataBuffers ;
    
}

#endif
//END OF FILE