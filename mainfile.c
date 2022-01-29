/**
@file mainfile.c
@brief This is the mainfile of the program.

The mainfile declares MACROs to link the functions specific to the declared system. The following MACROs are used:
|MACRO|Description/function|
|-----|--------------------|
|INPUT_FILE |The relative path and name of the input file|
|KERNEL_FILE |The relative path and name of the kernel file|
|INP_PARAMS_STRUCT |A C struct/structure that holds the input parameters in its member variables|
|DATA_BUFFERS_STRUCT |A C struct.structure that holds the data buffers/arrays in its member variables|
|READ_INP_FUNCTION |The name of the function that reads the input file specific to the system into the INP_PARAMS STRUCT|
|BUFFER_INIT_FUNCTION |The function that reads the INP_PARAMS_STRUCT and initialises the necessary data buffers/arrays for the program|
|KERNEL_ITERATE_FUNCTION |The function that iterates the kernel of the declared SYSTEM |

The following table summarizes which MACRO if set to what and for which SYSTEM.
|MACRO | DIFFUSION | CAHNHILLIARD | KOBISO | KOBANISO |
|------|-----------|--------------|--------|----------|
||Diffusion|Spinodal decomposition| Isotropic dendtitic growth|Anisotropic dendtiric growth|
|INPUT_FILE|InputFiles/Diffusion.in|"InputFiles/CahnHilliard.in"|InputFiles/KobayashiIso.in|InputFiles/KobayashiAniso.in|
|KERNEL_FILE|Kernels/DiffusionKern.cl|Kernels/CahnHilliardKern.cl|Kernels/KobayashiIsoKern.cl|Kernels/KobayashiAnisoKern.cl|
|INP_PARAMS_STRUCT|DiffusionInputParams|CahnHilliardInputParams|KobIsoInputParams|KobAnisoInputParams| 
|DATA_BUFFERS_STRUCT|DiffusionDataBuffers|CahnHilliardDataBuffers|KobIsoDataBuffers|KobAnisoDataBuffers|
|READ_INP_FUNCTION|readDiffusionInParams()|readCahnHilliardInParams()| readKobIsoInParams()|readKobAnisoInParams()|
|BUFFER_INIT_FUNCTION|initDiffusionBuffers()|initCahnHilliardBuffers()|initKobayashiIsoBuffers()|initKobayashiAnisoBuffers()|
|KERNEL_ITERATE_FUNCTION|iterateDiffusionKernel()|iterateCahnHilliardKernel()|iterateKobayashiKernel()|iterateKobayashiKernel()|
*/

// Define these to ensure a smooth functioing of OpenCL
#define _CRT_SECURE_NO_WARNINGS
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
// The OpenCL target version has to be predefined if the processor and the SDK has different versions of OpenCL.
#define CL_TARGET_OPENCL_VERSION 210

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#ifdef MAC
#include <OpenCL/cl.h>
#else  
#include <CL/cl.h>
#endif

#ifdef DIFFUSION

#define INPUT_FILE "InputFiles/Diffusion.in"
#define KERNEL_FILE "Kernels/DiffusionKern.cl"
#define READ_INP_FUNCTION readDiffusionInParams
#define BUFFER_INIT_FUNCTION initDiffusionBuffers
#define KERNEL_ITERATE_FUNCTION iterateDiffusionKernel
#define INP_PARAMS_STRUCT DiffusionInputParams
#define DATA_BUFFERS_STRUCT DiffusionDataBuffers

#elif CAHNHILLIARD

#define INPUT_FILE "InputFiles/CahnHilliard.in"
#define KERNEL_FILE "Kernels/CahnHilliardKern.cl"
#define READ_INP_FUNCTION readCahnHilliardInParams
#define BUFFER_INIT_FUNCTION initCahnHilliardBuffers
#define KERNEL_ITERATE_FUNCTION iterateCahnHilliardKernel
#define INP_PARAMS_STRUCT CahnHilliardInputParams
#define DATA_BUFFERS_STRUCT CahnHilliardDataBuffers
    
#elif KOBISO

#define INPUT_FILE "InputFiles/KobayashiIso.in"
#define KERNEL_FILE "Kernels/KobayashiIsoKern.cl"
#define READ_INP_FUNCTION readKobIsoInParams
#define BUFFER_INIT_FUNCTION initKobayashiIsoBuffers
#define KERNEL_ITERATE_FUNCTION iterateKobayashiIsoKernel
#define INP_PARAMS_STRUCT KobIsoInputParams
#define DATA_BUFFERS_STRUCT KobIsoDataBuffers

#elif KOBANISO

#define INPUT_FILE "InputFiles/KobayashiAniso.in"
#define KERNEL_FILE "Kernels/KobayashiAnisoKern.cl"
#define READ_INP_FUNCTION readKobAnisoInParams
#define BUFFER_INIT_FUNCTION initKobayashiAnisoBuffers
#define KERNEL_ITERATE_FUNCTION iterateKobayashiAnisoKernel
#define INP_PARAMS_STRUCT KobAnisoInputParams
#define DATA_BUFFERS_STRUCT KobAnisoDataBuffers

#endif

#include "UtilityFunctions/global_vars.h"
#include "UtilityFunctions/error_handle.h"
#include "UtilityFunctions/read_inp_file.h"
#include "UtilityFunctions/CL_utility_funcs.h"
#include "UtilityFunctions/file_to_program.h"
#include "UtilityFunctions/data_manip_funcs.h"
#include "UtilityFunctions/init_CL_buffers.h"
#include "UtilityFunctions/iterate_kernels.h"
#include "UtilityFunctions/data_writing_funcs.h"

/** @brief The main function.

The readCommonParams() function reads the INPUT_FILE and initialises the global variables common to all systems.\n
The initCLDataStructures() function initillises the OpenCL data structures (platform to kernels).

*/
int main(int argc, char **args){
    
    readCommonParams(INPUT_FILE);
    // Initialize OpenCL data structures
    initCLDataStructures() ;
    struct INP_PARAMS_STRUCT InpParams ;
    InpParams = READ_INP_FUNCTION(INPUT_FILE);
    // Read the file into a program
    kernel = getKernelFromFile(KERNEL_FILE, InpParams);
    // Initialize data
    struct DATA_BUFFERS_STRUCT dataBuffers ;
    dataBuffers = BUFFER_INIT_FUNCTION(InpParams);
    // Iterate kernel
    KERNEL_ITERATE_FUNCTION(InpParams,dataBuffers) ;
    
    return 0 ;
}
// END OF FILE