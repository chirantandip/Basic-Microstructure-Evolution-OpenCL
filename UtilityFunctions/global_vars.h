/**
@file global_vars.h

@brief The file declares all the global variables and structures.
*/

#ifndef GLOBAL_VARS
#define GLOBAL_VARS

#include <stdio.h>
#include <stdlib.h>
#ifdef MAC
#include <OpenCL/cl.h>
#else  
#include <CL/cl.h>
#endif

/// The Platform ID variable. Read from the INPUT_FILE and holds the ID of the platform where the program will run.
cl_int platID ;
/// The Device ID variable. Read from the INPUT_FILE and holds the ID of the device where the program will run.
cl_int devID ;

/// The platform data structure. Holds the platform specidfied by the platID variable.
cl_platform_id platform;
/// Number of devices. Holds the count of available devices in the speficied platform.
cl_int numDevices;
/// Device array. Holds the device data structres available in the platform.
cl_device_id *devices;
/// The context. The context is a group of devices on which command queues are created.
cl_context context;
/// The command queue structure. It is the queue where kernels line up and wait for excution.
cl_command_queue queue;
/// The kernel structure. It represents a kernel, read from a .cl file using the getKernelFromFile() function.
cl_kernel kernel ;
// Work group size.
cl_int WGsize ;

/// Matrix/mesh/grid size of the system.
cl_int SIZE ; 
/// The delta x variable. dx=dy=H
cl_float DX ;
/// Number of iterations to run.
cl_int ITERS ;
/// The delta t variable. 
cl_float DT ;
/// Total number of iterations to save. Declares the total number of outfiles to write. if set to (say) 10, the program will write 19 ecenly spaced output files througout the iteration process.
cl_int NSAVE ; 
/// Defines the type of outputfile. The following table states the values and output file types :
/// |Value|Output file type|
/// |-----|----------------|
/// |0|.csv|
/// |1|.vtk|
cl_int OutDataFileType ;

/// Diffusion system input parameters.
struct DiffusionInputParams{
    /// The Diffusion coefficient.
    cl_float DIFF_COEFF ; 
};

/// Diffusion system data buffers
struct DiffusionDataBuffers{
    /// ARRAYS to hold the phase field phi.
    cl_float *PHASE1, *PHASE2 ;
    /// The OpenCL buffers for the phase fields arrays.
    cl_mem PHASE1buff, PHASE2buff ;
};

/// Cahn-Hilliard system input parameters. The input parameters for the spinodal decomposition phenomenon.
struct CahnHilliardInputParams{
    /// The mean concentration value. The MEAN_C variable determines the mean of the random initialisaton of the phase field array.
    cl_float MEAN_C ;
    /// The Noise Amplitude. The NOISE_AMP defines how wide spread the noise/randomness is in the initialisation of the phase field.
    cl_float NOISE_AMP ;
    /// KAPPA controls the free energy cost of variation in concentration.
    cl_float KAPPA ;
    /// The mobility of the material.
    cl_float MOBILITY ;
};

/// Cahn-Hilliard system data buffers. 
struct CahnHilliardDataBuffers{
    /// Arrays to hold the phase field values. 
    cl_float *PHASE1, *PHASE2, *InBracM ;
    /// the OpenCL buffers for the phase field arrays.
    cl_mem PHASE1buff, PHASE2buff, InBracMbuff ;
}; 
    
/// Kobayashi Anisotropic system input parameters.
struct KobAnisoInputParams{
    /// Epsilon bar is the mean value of epsilon. Epsilon is the variable that controls the anisotropy.
    cl_float EPS_BAR ;
    /// alpha controls the degree of supercooling.
    cl_float ALPHA ;
    /// gamma is a constant used in calculating m(T).
    cl_float GAMMA ;
    /// Strength of anisotropy.
    cl_float DELTA ;
    /// The symmetry coefficient. It determines the symmetry of the system. J=6 implies hexagonal symmetry, J=2 implies bilateral symmetry and so on.
    cl_float J ;
    /// Tau. It is a small potitive constant in the LHS of the evolution equation. 
    cl_float TAU ;
    /// Theta0 defines the initial offset of theta in radians.
    cl_float THETA0 ;
    /// The noise amplitude.
    cl_float NOISE_AMP ;
    /// Thermal Diffusivity
    cl_float TH_DIFF ;
    /// Latent heat of solidification.
    cl_float L_HEAT ;
    /// Initial temperature of the supercooled liquid phase.
    cl_float T_INIT ;
    /// Melting point of the material.
    cl_float T_MELT ;
    /// The temperature of the nuclueated solid.
    cl_float T_BOUND ;
};

/// Kobayashi anisotropic data buffers.
struct KobAnisoDataBuffers{
    /// The C arrays for phase field and temperature field.
    cl_float *PHASE1, *PHASE2, *TEMP1, *TEMP2;
    /// The OpenCL buffers for the declared arrays.
    cl_mem PHASE1buff, PHASE2buff, TEMP1buff, TEMP2buff;
};

/// Kobayashi Anisotropic system input parameters.
struct KobIsoInputParams{    
    /// Epsilon bar is the mean value of epsilon. Epsilon is the variable that controls the anisotropy.
    cl_float EPS_BAR ;
    /// alpha controls the degree of supercooling.
    cl_float ALPHA ;
    /// gamma is a constant used in calculating m(T).
    cl_float GAMMA ;
    /// Tau. It is a small potitive constant in the LHS of the evolution equation. 
    cl_float TAU ;
    /// The noise amplitude.
    cl_float NOISE_AMP ;
    /// Phase field boundary values
    cl_float PHASE_L ;
    cl_float PHASE_R ;
    cl_float PHASE_T ;
    cl_float PHASE_B ;
    /// Thermal Diffusivity
    cl_float TH_DIFF ;
    /// Latent heat of solidification.
    cl_float L_HEAT ;
    /// Initial temperature of the supercooled liquid phase.
    cl_float T_INIT ;
    /// Melting point of the material.
    cl_float T_MELT ;
    /// The temperature field boundary values
    cl_float TEMP_L ;
    cl_float TEMP_R ;
    cl_float TEMP_T ;
    cl_float TEMP_B ;
};

/// Kobayashi Isotropic data buffers.
struct KobIsoDataBuffers{
    /// The C arrays for phase field and temperature field.
    cl_float *PHASE1, *PHASE2, *TEMP1, *TEMP2;
    /// The OpenCL buffers for the declared arrays.
    cl_mem PHASE1buff, PHASE2buff, TEMP1buff, TEMP2buff;
};

#endif
// END OF FILE