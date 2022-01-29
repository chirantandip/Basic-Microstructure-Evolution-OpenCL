/**
@file iterate_kernels.h
@brief Declares the functions to iterate one and multiple timesteps of evolution for each SYSTEM. 

*/

#ifndef ITERATE_KERN
#define ITERATE_KERN

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#ifdef MAC
#include <OpenCL/cl.h>
#else  
#include <CL/cl.h>
#endif

#include "global_vars.h"
#include "error_handle.h"
#include "CL_utility_funcs.h"
#include "data_writing_funcs.h"

/**
@brief One step of evolution in the diffusion system.
@param globalWS An array with the 2D global work size.
@param localWS An array with the 2D local work size.
@param PHASE1buff The input phase buffer.
@param PHASE2buff The output phase buffer.
@param events The cl_event s associated with each iteration to profile kernel execution.
@param iter The iteration index. Whether its the 1st, 2nd or 56th iteration.

Because the function is to be called as many times as the number of iterations, it is declared to be an inline function, which though increases the compiling time but reduces the running time for large number of iterations.
*/
static inline void DiffusionEvolutionStep(size_t globalWS[2], size_t localWS[2],  cl_mem PHASE1buff, cl_mem PHASE2buff, cl_event *events, cl_int iter){
    // Error variable
    cl_int err;       
    // Set inner kernel arguments;
    err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &PHASE1buff);
    KernErrorHandle(err,"SetKernelArg 0");
    err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &PHASE2buff);
    KernErrorHandle(err,"SetKernelArg 1");
    
    // Enqueue the kernel
    err = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, globalWS, localWS, 0, NULL, &events[iter]);
    KernErrorHandle(err, "clEnqueueNDRangeKernel EvolKern");
    
}

/**
@brief A function to fully iterate the diffusion kernel.
@param inpparams A DiffusionInputParams structure.
@param databuffers A DiffusionDataBuffers structure.
Along with executing the kernel, the function also writes output to output files and profiles the kernel to compute the execution time.
*/
static inline void iterateDiffusionKernel(struct DiffusionInputParams inpparams, struct DiffusionDataBuffers databuffers){
    cl_int err ;
    // Iterate kernel with a random float
    // WG parameters
    size_t globalWS[2] = {SIZE, SIZE};
    
    if(WGsize < 8){
        WGsize = GetOptimumWGSize(kernel, devices[devID]) ;
    }
    size_t localWS[2] = {WGsize,WGsize};
    printf("   : Work group size: %d\n", WGsize);
    
    // Profiling events
    cl_event* timing_events ; 
    timing_events = (cl_event*)malloc(sizeof(cl_event)*2);
    cl_float tot_exec_time = 0.0f;
    
    // Read the buffers and profile the reading time.
    char OutFileDir[50] ;
    err = sprintf(OutFileDir, "./OutDataFiles/DIFUSION_%dS_%dITERS", SIZE, ITERS);
    mkdir(OutFileDir,0777);
    printf("   : Enqueuing kernels:\n   : Compute size is %u\n", SIZE*SIZE*ITERS);
    
    for(int iter = 0 ; iter < ITERS ; iter++){
        
        DiffusionEvolutionStep(globalWS, localWS, databuffers.PHASE1buff, databuffers.PHASE2buff, timing_events,0);
        DiffusionEvolutionStep(globalWS, localWS, databuffers.PHASE2buff, databuffers.PHASE1buff, timing_events,1);
        
        clFinish(queue);
        
        tot_exec_time += GetEventExecTime(timing_events[0]) ;
        tot_exec_time += GetEventExecTime(timing_events[1]) ;
        
        if(iter %((int)(ITERS/NSAVE)) == 0){
            printf("%2d%s: complete in time %5.2f seconds\n",100*iter/ITERS,"%", tot_exec_time);
            
            err = clEnqueueReadBuffer(queue, databuffers.PHASE1buff, CL_TRUE, 0, sizeof(float)*SIZE*SIZE, databuffers.PHASE1, 0, NULL,NULL);
            KernErrorHandle(err, "clEnqueueReadBuffer");
            
            // Write data to file
            Write1DMatToFile(OutFileDir, "PHASE",iter,databuffers.PHASE1);
            
        }
        
    }
    
    printf("%2d%s: complete in time %5.2f mins\n",100,"%", tot_exec_time/60.0);
    err = clEnqueueReadBuffer(queue, databuffers.PHASE1buff, CL_TRUE, 0, sizeof(float)*SIZE*SIZE, databuffers.PHASE1, 0, NULL,NULL);
    KernErrorHandle(err, "clEnqueueReadBuffer");
    
    Write1DMatToFile(OutFileDir, "PHASE",ITERS,databuffers.PHASE1);

    
    
}


/**
@brief One step of evolution in the kobayashi dendrite growth system.
@param globalWS An array with the 2D global work size.
@param localWS An array with the 2D local work size.
@param PHASE1buff The input phase buffer.
@param PHASE2buff The output phase buffer.
@param TEMP1buff The input temperature buffer.
@param TEMP2buff The output temperature buffer.
@param noise The noise amplitide needed to generate randomness in the phase field.
@param events The cl_event s associated with each iteration to profile kernel execution.
@param iter The iteration index. Whether its the 1st, 2nd or 56th iteration.

Because the function is to be called as many times as the number of iterations, it is declared to be an inline function, which though increases the compiling time but reduces the running time for large number of iterations.
*/
static inline void KobayashiEvolutionStep(size_t globalWS[2], size_t localWS[2],  cl_mem PHASE1buff, cl_mem PHASE2buff, cl_mem TEMP1buff, cl_mem TEMP2buff, cl_float noise, cl_event *events, cl_int iter){
    // Error variable
    cl_int err;       
    // Set inner kernel arguments;
    err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &PHASE1buff);
    KernErrorHandle(err,"SetKernelArg 0");
    err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &PHASE2buff);
    KernErrorHandle(err,"SetKernelArg 1");
    err = clSetKernelArg(kernel, 2, sizeof(cl_mem), &TEMP1buff);
    KernErrorHandle(err,"SetKernelArg 2");
    err = clSetKernelArg(kernel, 3, sizeof(cl_mem), &TEMP2buff);
    KernErrorHandle(err,"SetKernelArg 3");
    err = clSetKernelArg(kernel, 4, sizeof(cl_float), &noise);
    KernErrorHandle(err,"SetKernelArg 4");
    
    // Enqueue the kernel
    err = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, globalWS, localWS, 0, NULL, &events[iter]);
    KernErrorHandle(err, "clEnqueueNDRangeKernel EvolKern");
}

/**
@brief A function to fully iterate the kobayashi kernels.
@param inpparams A KobAnisoInputParams structure.
@param databuffers A KobAnisoDataBuffers structure.
Along with executing the kernel, the function also writes output to output files and profiles the kernel to compute the execution time.
*/
static inline void iterateKobayashiAnisoKernel(struct KobAnisoInputParams inpparams, struct KobAnisoDataBuffers databuffers){
    cl_int err ;
    // Iterate kernel with a random float
    // WG parameters
    size_t globalWS[2] = {SIZE, SIZE};
    
    if(WGsize < 8){
        WGsize = GetOptimumWGSize(kernel, devices[devID]) ;
    }
    size_t localWS[2] = {WGsize,WGsize};
    printf("   : Work group size: %d\n", WGsize);
    
    // Profiling events
    cl_event* timing_events ; 
    timing_events = (cl_event*)malloc(sizeof(cl_event)*2);
    cl_float tot_exec_time = 0.0f;
    
    // random number generation
    srand((unsigned int)time(NULL));
    cl_float noise;
    
    // Read the buffers and profile the reading time.
    char OutFileDir[50] ;
    err = sprintf(OutFileDir,"./OutDataFiles/KOB_ANISO_%dS_%dITERS",SIZE,ITERS);
    mkdir(OutFileDir,0777);
    printf("   : Enqueuing kernels:\n   : Compute size is %u\n", SIZE*SIZE*ITERS);
    cl_float noiseAMP = inpparams.NOISE_AMP ;
    
    for(int iter = 0 ; iter < ITERS ; iter++){
        if((iter%50)==0){
            noise=noiseAMP*(((float)rand()/(float)RAND_MAX)-0.5);
        }else{
            noise=0.0;
        }
        
        KobayashiEvolutionStep(globalWS, localWS, databuffers.PHASE1buff, databuffers.PHASE2buff, databuffers.TEMP1buff, databuffers.TEMP2buff, noise, timing_events,0);
        KobayashiEvolutionStep(globalWS, localWS, databuffers.PHASE2buff, databuffers.PHASE1buff, databuffers.TEMP2buff, databuffers.TEMP1buff, noise, timing_events,1);
        
        clFinish(queue);
        
        tot_exec_time += GetEventExecTime(timing_events[0]) ;
        tot_exec_time += GetEventExecTime(timing_events[1]) ;
        
        if(iter %((int)(ITERS/NSAVE)) == 0){
            printf("%2d%s: complete in time %5.2f seconds\n",100*iter/ITERS,"%", tot_exec_time);
            
            err = clEnqueueReadBuffer(queue, databuffers.PHASE1buff, CL_FALSE, 0, sizeof(float)*SIZE*SIZE, databuffers.PHASE1, 0, NULL,NULL);
            err = clEnqueueReadBuffer(queue, databuffers.TEMP1buff, CL_FALSE, 0, sizeof(float)*SIZE*SIZE, databuffers.TEMP1, 0, NULL,NULL);
            KernErrorHandle(err, "clEnqueueReadBuffer");
            clFinish(queue);
            
 
            Write1DMatToFile(OutFileDir, "PHASE",iter,databuffers.PHASE1);
            Write1DMatToFile(OutFileDir,"TEMP", iter, databuffers.TEMP1);
        }
    }
    
    printf("%2d%s: complete in time %5.2f mins\n",100,"%", tot_exec_time/60.0);
    err = clEnqueueReadBuffer(queue, databuffers.PHASE1buff, CL_FALSE, 0, sizeof(float)*SIZE*SIZE, databuffers.PHASE1, 0, NULL,NULL);
    err = clEnqueueReadBuffer(queue, databuffers.TEMP1buff, CL_FALSE, 0, sizeof(float)*SIZE*SIZE, databuffers.TEMP1, 0, NULL,NULL);
    KernErrorHandle(err, "clEnqueueReadBuffer");
    clFinish(queue);
    
    Write1DMatToFile(OutFileDir, "PHASE",ITERS,databuffers.PHASE1);
    Write1DMatToFile(OutFileDir,"TEMP",ITERS, databuffers.TEMP1);

}


/**
@brief A function to fully iterate the kobayashi kernels.
@param inpparams A KobIsoInputParams structure.
@param databuffers A KobIsoDataBuffers structure.
Along with executing the kernel, the function also writes output to output files and profiles the kernel to compute the execution time.
*/
static inline void iterateKobayashiIsoKernel(struct KobIsoInputParams inpparams, struct KobIsoDataBuffers databuffers){
    cl_int err ;
    // Iterate kernel with a random float
    // WG parameters
    size_t globalWS[2] = {SIZE, SIZE};
    
    if(WGsize < 8){
        WGsize = GetOptimumWGSize(kernel, devices[devID]) ;
    }
    size_t localWS[2] = {WGsize,WGsize};
    printf("   : Work group size: %d\n", WGsize);
    
    // Profiling events
    cl_event* timing_events ; 
    timing_events = (cl_event*)malloc(sizeof(cl_event)*2);
    cl_float tot_exec_time = 0.0f;
    
    // random number generation
    srand((unsigned int)time(NULL));
    cl_float noise;
    
    // Read the buffers and profile the reading time.
    char OutFileDir[50] ;
    err = sprintf(OutFileDir,"./OutDataFiles/KOB_ISO_%dS_%dITERS",SIZE,ITERS);
    mkdir(OutFileDir,0777);
    printf("   : Enqueuing kernels:\n   : Compute size is %u\n", SIZE*SIZE*ITERS);
    cl_float noiseAMP = inpparams.NOISE_AMP ;
    
    for(int iter = 0 ; iter < ITERS ; iter++){
        if((iter%50)==0){
            noise=noiseAMP*(((float)rand()/(float)RAND_MAX)-0.5);
        }else{
            noise=0.0;
        }
        
        KobayashiEvolutionStep(globalWS, localWS, databuffers.PHASE1buff, databuffers.PHASE2buff, databuffers.TEMP1buff, databuffers.TEMP2buff, noise, timing_events,0);
        KobayashiEvolutionStep(globalWS, localWS, databuffers.PHASE2buff, databuffers.PHASE1buff, databuffers.TEMP2buff, databuffers.TEMP1buff, noise, timing_events,1);
        
        clFinish(queue);
        
        tot_exec_time += GetEventExecTime(timing_events[0]) ;
        tot_exec_time += GetEventExecTime(timing_events[1]) ;
        
        if(iter %((int)(ITERS/NSAVE)) == 0){
            printf("%2d%s: complete in time %5.2f seconds\n",100*iter/ITERS,"%", tot_exec_time);
            
            err = clEnqueueReadBuffer(queue, databuffers.PHASE1buff, CL_FALSE, 0, sizeof(float)*SIZE*SIZE, databuffers.PHASE1, 0, NULL,NULL);
            err = clEnqueueReadBuffer(queue, databuffers.TEMP1buff, CL_FALSE, 0, sizeof(float)*SIZE*SIZE, databuffers.TEMP1, 0, NULL,NULL);
            KernErrorHandle(err, "clEnqueueReadBuffer");
            clFinish(queue);
            
            Write1DMatToFile(OutFileDir, "PHASE",iter,databuffers.PHASE1);
            Write1DMatToFile(OutFileDir,"TEMP",iter, databuffers.TEMP1);

        }
    }
    
    printf("%2d%s: complete in time %5.2f mins\n",100,"%", tot_exec_time/60.0);
    err = clEnqueueReadBuffer(queue, databuffers.PHASE1buff, CL_FALSE, 0, sizeof(float)*SIZE*SIZE, databuffers.PHASE1, 0, NULL,NULL);
    err = clEnqueueReadBuffer(queue, databuffers.TEMP1buff, CL_FALSE, 0, sizeof(float)*SIZE*SIZE, databuffers.TEMP1, 0, NULL,NULL);
    KernErrorHandle(err, "clEnqueueReadBuffer");
    clFinish(queue);
    
    Write1DMatToFile(OutFileDir, "PHASE",ITERS,databuffers.PHASE1);
    Write1DMatToFile(OutFileDir,"TEMP",ITERS, databuffers.TEMP1);


}


/**
@brief One step of evolution in the cahn-hilliard (spinodal decomposition) system.
@param globalWS An array with the 2D global work size.
@param localWS An array with the 2D local work size.
@param PHASE1buff The input phase buffer.
@param PHASE2buff The output phase buffer.
@param InBracMbuff The temporary value holder, inner brackets matrix buffer.
@param events The cl_event s associated with each iteration to profile kernel execution.
@param iter The iteration index. Whether its the 1st, 2nd or 56th iteration.

Because the function is to be called as many times as the number of iterations, it is declared to be an inline function, which though increases the compiling time but reduces the running time for large number of iterations.
*/
static inline void CahnHilliardEvolutinStep(size_t globalWS[2], size_t localWS[2],  cl_mem PHASE1buff, cl_mem PHASE2buff, cl_mem InBracMbuff, cl_event *events, cl_int iter){
    // Error variable
    cl_int err;       
    // Set inner kernel arguments;
    err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &InBracMbuff);
    KernErrorHandle(err,"SetKernelArg 0");
    err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &PHASE1buff);
    KernErrorHandle(err,"SetKernelArg 1");
    err = clSetKernelArg(kernel, 2, sizeof(cl_mem), &PHASE2buff);
    KernErrorHandle(err,"SetKernelArg 2");
    // Enqueue the kernel
    err = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, globalWS, localWS, 0, NULL, &events[iter]);
    KernErrorHandle(err, "clEnqueueNDRangeKernel EvolKern");
    
}

/**
@brief A function to fully iterate the cahn-Hilliard kernel.
@param inpparams A CahnHilliardInputParams structure.
@param databuffers A CahnHilliardDataBuffers structure.
Along with executing the kernel, the function also writes output to output files and profiles the kernel to compute the execution time.
*/
static inline void iterateCahnHilliardKernel(struct CahnHilliardInputParams inpparams, struct CahnHilliardDataBuffers databuffers){
    
    cl_int err ;
    // Iterate kernel with a random float
    // WG parameters
    size_t globalWS[2] = {SIZE, SIZE};
    
    if(WGsize < 8){
        WGsize = GetOptimumWGSize(kernel, devices[devID]) ;
    }
    size_t localWS[2] = {WGsize,WGsize};
    printf("   : Work group size: %d\n", WGsize);
    
    // Profiling events
    cl_event* timing_events ; 
    timing_events = (cl_event*)malloc(sizeof(cl_event)*2);
    cl_float tot_exec_time = 0.0f;
       
    // Read the buffers and profile the reading time.
    char OutFileDir[50] ;
    err = sprintf(OutFileDir,"./OutDataFiles/CAHN_HILLIARD_%dS_%dITERS",SIZE,ITERS);
    mkdir(OutFileDir,0777);
    printf("   : Enqueuing kernels:\n   : Compute size is %u\n", SIZE*SIZE*ITERS);
    
    for(int iter = 0 ; iter < ITERS ; iter++){
        
        CahnHilliardEvolutinStep(globalWS, localWS, databuffers.PHASE1buff, databuffers.PHASE2buff, databuffers.InBracMbuff, timing_events,0);
        CahnHilliardEvolutinStep(globalWS, localWS, databuffers.PHASE2buff, databuffers.PHASE1buff, databuffers.InBracMbuff, timing_events,1);
        
        clFinish(queue);
        
        tot_exec_time += GetEventExecTime(timing_events[0]) ;
        tot_exec_time += GetEventExecTime(timing_events[1]) ;
        
        if(iter %((int)(ITERS/NSAVE)) == 0){
            printf("%2d%s: complete in time %5.2f mins\n",100*iter/ITERS,"%", tot_exec_time/60.0);
            
            err = clEnqueueReadBuffer(queue, databuffers.PHASE1buff, CL_FALSE, 0, sizeof(float)*SIZE*SIZE, databuffers.PHASE1, 0, NULL,NULL);
            KernErrorHandle(err, "clEnqueueReadBuffer");
            clFinish(queue);
            
            Write1DMatToFile(OutFileDir, "PHASE",iter,databuffers.PHASE1);
        }
    }
    
    printf("%2d%s: complete in time %5.2f mins\n",100,"%", tot_exec_time/60.0);
    err = clEnqueueReadBuffer(queue, databuffers.PHASE1buff, CL_FALSE, 0, sizeof(float)*SIZE*SIZE, databuffers.PHASE1, 0, NULL,NULL);
    KernErrorHandle(err, "clEnqueueReadBuffer");
    clFinish(queue);
    
    Write1DMatToFile(OutFileDir, "PHASE",ITERS,databuffers.PHASE1);
}


#endif
//END OF FILE