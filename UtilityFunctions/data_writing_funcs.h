/**
@file data_writing_funcs.h
@brief Declaring the functions needed to write the data buffers to outfiles of various formats. 
*/

#ifndef DATA_WRITING
#define DATA_WRITING

#include <stdio.h>
#include <stdlib.h>

/**
@brief Function to write a 1D array to a file.
Two formats are supported: .csv and .vtk and the formet is chosen from
the OutDataFileType variable in the inputfile.
@param OutFileDir Name of the outputfile directory.
@param type "PHASE" or "TEMP"
@param iter The current iteration number.
@param MAT The float* data array.
*/
void Write1DMatToFile(const char OutFileDir[], const char type[], int iter, float* MAT){
    char OutFileName[80] ;
    
    // CSV file
    if(OutDataFileType==0){
        sprintf(OutFileName,"%s/%s_%d.csv",OutFileDir,type, iter);
        FILE *OutFile; 
        OutFile = fopen(OutFileName,"w");
        if(OutFile==NULL){
            perror("Error in writing to OutputFile\n");
            exit(1);   
        }
        for(int i=1;i<=SIZE;i++){
            fprintf(OutFile,"%2.6f",0.0f);
            if((i%SIZE)!=0){
                fprintf(OutFile,",");    
            }else{
                fprintf(OutFile,"\n");

            }
        }
        for(int i=1;i<(SIZE*SIZE+1);i++){
            fprintf(OutFile,"%2.6f",MAT[i-1]);
            if((i%SIZE)!=0){
                fprintf(OutFile,",");    
            }else{
                fprintf(OutFile,"\n");

            }
        }
        fclose(OutFile) ;

        }

    // VTK file
    else if (OutDataFileType==1){
        // VTK file
        sprintf(OutFileName,"%s/%s_%d.vtk",OutFileDir,type, iter);
        FILE *OutFile; 
        OutFile = fopen(OutFileName,"w");
        if(OutFile==NULL){
            perror("Error in writing to OutputFile\n");
            exit(1);   
        }
        fprintf(OutFile, "# vtk DataFile Version 3.0\n");
        fprintf(OutFile,"%s_fields\n", type);
        fprintf(OutFile,"ASCII\n");
        fprintf(OutFile,"DATASET STRUCTURED_POINTS\n");
        fprintf(OutFile,"DIMENSIONS %d %d 1\n", SIZE, SIZE);
        fprintf(OutFile,"ORIGIN 0 0 0\n");
        fprintf(OutFile,"SPACING %e %e 1.000000e+00\n", DX, DX);
        fprintf(OutFile,"POINT_DATA %d\n", SIZE*SIZE);
        fprintf(OutFile,"SCALARS FCC double 1\nLOOKUP_TABLE default\n");
        for(int i=0; i<SIZE*SIZE;i++){
            fprintf(OutFile,"%e\n",MAT[i]);
        }
        
    }
    
    
    // End msg
    printf("   : Completed writing data to file %s\n",OutFileName);
    
}

#endif
//END OF FILE