/**
@file read_inp_file.h
@brief Declares functions to read input files to data structures.
*/

#ifndef READ_INP_FILE
#define READ_INP_FILE

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
@brief A function to read the parameters common in all input files to global variables.
@param InputFileName The Input File name as defined by the INPUT_FILE macro in the mainfile.c .
*/
void readCommonParams(const char InputFileName[]){
    FILE *FileHandle = fopen(InputFileName, "rt");
    if(FileHandle==NULL){
        printf("File %s ot found\n",InputFileName);
        perror("ERROR!");
    }
    
    char tmpbuff[1500];
    char tmpstr1[100];
    char tmpstr2[100];
    
    while(fgets(tmpbuff,1000,FileHandle)){
        sscanf(tmpbuff,"%100s = %100[^;];",tmpstr1,tmpstr2);
        if(tmpstr1[0] != '#'){
            // Simulation matrix size parameters
            if(strcmp(tmpstr1,"platformID")==0){
                platID = atoi(tmpstr2);
            }else if(strcmp(tmpstr1,"deviceID")==0){
                devID = atoi(tmpstr2);
            }else if(strcmp(tmpstr1,"WGsize")==0){
                WGsize = atoi(tmpstr2);
            }else if(strcmp(tmpstr1,"SIZE")==0){
                SIZE = atoi(tmpstr2);
            }else if(strcmp(tmpstr1,"DX")==0){
                DX = atof(tmpstr2);
            }else if(strcmp(tmpstr1,"ITERS")==0){
                ITERS = atoi(tmpstr2);
            }else if(strcmp(tmpstr1,"DT")==0){
                DT = atof(tmpstr2);
            }else if(strcmp(tmpstr1,"NSave")==0){
                NSAVE = atoi(tmpstr2);
            }else if(strcmp(tmpstr1,"OutDataFileType")==0){
                OutDataFileType = atoi(tmpstr2);
            }
        }
    }
    
}

/**
@brief A function to read the parameters specific to the diffusion system input file.
@param InputFileName The Input File name as defined by the INPUT_FILE macro in the mainfile.c .
@return A DiffusionInputParams struct.
*/
struct DiffusionInputParams readDiffusionInParams(const char InputFileName[]){
    FILE *FileHandle = fopen(InputFileName, "rt");
    if(FileHandle==NULL){
        printf("File %s ot found\n",InputFileName);
        perror("ERROR!");
    }
    struct DiffusionInputParams Params;
    char tmpbuff[1500];
    char tmpstr1[100];
    char tmpstr2[100];
    
    while(fgets(tmpbuff,1000,FileHandle)){
        
        sscanf(tmpbuff,"%100s = %100[^;];",tmpstr1,tmpstr2);
        
        if(tmpstr1[0] != '#'){
            // Simulation matrix size parameters
            if(strcmp(tmpstr1,"DIFFUSION_COEFFICIENT")==0){
                Params.DIFF_COEFF = atof(tmpstr2);
            }
        }
    }
    return Params ;
}

/**
@brief A function to read the parameters specific to the cahn-hilliard (spinodal decomposition) system input file.
@param InputFileName The Input File name as defined by the INPUT_FILE macro in the mainfile.c .
@return A CahnHilliardInputParams struct.
*/
struct CahnHilliardInputParams readCahnHilliardInParams(const char InputFileName[]){
    FILE *FileHandle = fopen(InputFileName, "rt");
    if(FileHandle==NULL){
        printf("File %s ot found\n",InputFileName);
        perror("ERROR!");
    }
    struct CahnHilliardInputParams Params;
    char tmpbuff[1000];
    char tmpstr1[100];
    char tmpstr2[100];
    
    while(fgets(tmpbuff,1000,FileHandle)){
        
        sscanf(tmpbuff,"%100s = %100[^;];",tmpstr1,tmpstr2);
        
        if(tmpstr1[0] != '#'){
            // Simulation matrix size parameters
            if(strcmp(tmpstr1,"MEAN_CONCENTRATION")==0){
                Params.MEAN_C = atof(tmpstr2);
            }else if(strcmp(tmpstr1,"NOISE_AMP")==0){
                Params.NOISE_AMP = atof(tmpstr2);
            }else if(strcmp(tmpstr1,"KAPPA")==0){
                Params.KAPPA = atof(tmpstr2);
            }else if(strcmp(tmpstr1,"MOBILITY")==0){
                Params.MOBILITY = atof(tmpstr2);
            }
        }
    }
    return Params ;
}

/**
@brief A function to read the parameters specific to the Kobayashi isotropic dendritic growth system input file.
@param InputFileName The Input File name as defined by the INPUT_FILE macro in the mainfile.c .
@return A KobIsoInputParams struct.
*/
struct KobIsoInputParams readKobIsoInParams(const char InputFileName[]){
    FILE *FileHandle = fopen(InputFileName, "rt");
    if(FileHandle==NULL){
        printf("File %s ot found\n",InputFileName);
        perror("ERROR!");
    }
    struct KobIsoInputParams Params;
    char tmpbuff[1500];
    char tmpstr1[100];
    char tmpstr2[100];
    
    while(fgets(tmpbuff,1000,FileHandle)){
        
        sscanf(tmpbuff,"%100s = %100[^;];",tmpstr1,tmpstr2);
        
        if(tmpstr1[0] != '#'){
            // Simulation matrix size parameters
            if(strcmp(tmpstr1,"EPS_BAR")==0){
                Params.EPS_BAR = atof(tmpstr2);
            }else if(strcmp(tmpstr1,"ALPHA")==0){
                Params.ALPHA = atof(tmpstr2);
            }else if(strcmp(tmpstr1,"GAMMA")==0){
                Params.GAMMA = atof(tmpstr2);
            }else if(strcmp(tmpstr1,"TAU")==0){
                Params.TAU = atof(tmpstr2);
            }else if(strcmp(tmpstr1,"NOISE_AMP")==0){
                Params.NOISE_AMP = atof(tmpstr2);
            }else if(strcmp(tmpstr1,"PHASE_BOUND_LEFT")==0){
                Params.PHASE_L = atof(tmpstr2);
            }else if(strcmp(tmpstr1,"PHASE_BOUND_RIGHT")==0){
                Params.PHASE_R = atof(tmpstr2);
            }else if(strcmp(tmpstr1,"PHASE_BOUND_TOP")==0){
                Params.PHASE_T = atof(tmpstr2);
            }else if(strcmp(tmpstr1,"PHASE_BOUND_BOTTOM")==0){
                Params.PHASE_B = atof(tmpstr2);
            }else if(strcmp(tmpstr1,"THERMAL_DIFFUSIVITY")==0){
                Params.TH_DIFF = atof(tmpstr2);
            }else if(strcmp(tmpstr1,"LATENT_HEAT_SLD")==0){
                Params.L_HEAT = atof(tmpstr2);
            }else if(strcmp(tmpstr1,"TEMP_INIT")==0){
                Params.T_INIT = atof(tmpstr2);
            }else if(strcmp(tmpstr1,"TEMP_MELT")==0){
                Params.T_MELT = atof(tmpstr2);
            }else if(strcmp(tmpstr1,"TEMP_BOUND_LEFT")==0){
                Params.TEMP_L = atof(tmpstr2);
            }else if(strcmp(tmpstr1,"TEMP_BOUND_RIGHT")==0){
                Params.TEMP_R = atof(tmpstr2);
            }else if(strcmp(tmpstr1,"TEMP_BOUND_TOP")==0){
                Params.TEMP_T = atof(tmpstr2);
            }else if(strcmp(tmpstr1,"TEMP_BOUND_BOTTOM")==0){
                Params.TEMP_B = atof(tmpstr2);
            }
        }
    }
    return Params ;
}



/**
@brief A function to read the parameters specific to the Kobayashi anisotropic dendritic growth system input file.
@param InputFileName The Input File name as defined by the INPUT_FILE macro in the mainfile.c .
@return A KobAnisoInputParams struct.
*/
struct KobAnisoInputParams readKobAnisoInParams(const char InputFileName[]){
    FILE *FileHandle = fopen(InputFileName, "rt");
    if(FileHandle==NULL){
        printf("File %s ot found\n",InputFileName);
        perror("ERROR!");
    }
    struct KobAnisoInputParams Params;
    char tmpbuff[1500];
    char tmpstr1[100];
    char tmpstr2[100];
    
    while(fgets(tmpbuff,1000,FileHandle)){
        
        sscanf(tmpbuff,"%100s = %100[^;];",tmpstr1,tmpstr2);
        
        if(tmpstr1[0] != '#'){
            // Simulation matrix size parameters
            if(strcmp(tmpstr1,"EPS_BAR")==0){
                Params.EPS_BAR = atof(tmpstr2);
            }else if(strcmp(tmpstr1,"ALPHA")==0){
                Params.ALPHA = atof(tmpstr2);
            }else if(strcmp(tmpstr1,"GAMMA")==0){
                Params.GAMMA = atof(tmpstr2);
            }else if(strcmp(tmpstr1,"DELTA")==0){
                Params.DELTA = atof(tmpstr2);
            }else if(strcmp(tmpstr1,"J")==0){
                Params.J = atof(tmpstr2);
            }else if(strcmp(tmpstr1,"TAU")==0){
                Params.TAU = atof(tmpstr2);
            }else if(strcmp(tmpstr1,"THETA0")==0){
                Params.THETA0 = atof(tmpstr2);
            }else if(strcmp(tmpstr1,"NOISE_AMP")==0){
                Params.NOISE_AMP = atof(tmpstr2);
            }else if(strcmp(tmpstr1,"THERMAL_DIFFUSIVITY")==0){
                Params.TH_DIFF = atof(tmpstr2);
            }else if(strcmp(tmpstr1,"LATENT_HEAT_SLD")==0){
                Params.L_HEAT = atof(tmpstr2);
            }else if(strcmp(tmpstr1,"TEMP_INIT")==0){
                Params.T_INIT = atof(tmpstr2);
            }else if(strcmp(tmpstr1,"TEMP_MELT")==0){
                Params.T_MELT = atof(tmpstr2);
            }else if(strcmp(tmpstr1,"TEMP_BOUND")==0){
                Params.T_BOUND = atof(tmpstr2);
            }
        }
    }
    return Params ;
}


#endif
// END OF FILE