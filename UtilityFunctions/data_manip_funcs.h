/**
@file data_manip_funcs.h
@brief Declares a few data manipulation functions.
*/

#ifndef DATA_MANIPULATION_FUNCTIONS
#define DATA_MANIPULATION_FUNCTIONS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef MAC
#include <OpenCL/cl.h>
#else  
#include <CL/cl.h>
#endif


/**
@brief Initialize a 1D float matrix.
@param SIZE The size of the matrix. The generated array will be of size SIZE*SIZE.
@param initVal The value to which the float array should be initialized.
@return A pointer to the generated float array.
*/
float *Init1DFloatMatrix(cl_int SIZE, cl_float initVal){
    float *MAT;
    MAT = (float *)malloc(sizeof(float)*SIZE*SIZE);
    for(int i=0;i<SIZE*SIZE;i++){
        MAT[i] = initVal;
    }
    return MAT;
}

/**
@brief Initialize a 1D float matrix.
@param SIZE The size of the matrix. The generated array will be of size SIZE*SIZE.
@param initVal The value to which the float array should be initialized.
@return A pointer to the generated float array.
*/
float *Init1DFloatMatrixWithBoundary(cl_int SIZE, cl_float initVal, cl_float left, cl_float right, cl_float top, cl_float bottom){
    float *MAT;
    MAT = (float *)malloc(sizeof(float)*SIZE*SIZE);
    for(int i=0;i<SIZE;i++){
        for(int j=0; j<SIZE;j++){
            if(i==0){
                MAT[SIZE*j+i]=top;
            }else if(i==(SIZE-1)){
                MAT[SIZE*j+i]=bottom;
            }else if(j==0){
                MAT[SIZE*j+i]=left;
            }else if(j==(SIZE-1)){
                MAT[SIZE*j+i]=right;
            }else{
                MAT[SIZE*j+i]=initVal;
            }
        }
    }
    return MAT;
}

/**
@brief Random initialization of a 1D float matrix.
@param SIZE The size of the created matrix.
@param mean The mean of the random distribution.
@param noiseAmp The amplitude of the distribution. How much the maximum and the minimum value deviates from the mean.
@return Pointer to a random flat array.

Random floats are generted using the srand() and rand() C functions.
*/
float *RandomInit1DFloatMatrix(cl_int SIZE, cl_float mean, cl_float noiseAmp){
    srand((unsigned int)time(NULL));
    float *MAT ; 
    MAT = (float*)malloc(sizeof(float)*SIZE*SIZE);
    for(int i=0; i<SIZE*SIZE; i++){
        if (i==1){
            printf("mean=%f\n noise=%f\n", mean, noiseAmp);
        }
        MAT[i] = mean + 1.0*noiseAmp*( 0.5 - ((float)rand()/(float)RAND_MAX));
    }
    return MAT ;
}

/**
@brief Initialize a 2D array to a square in the center.
@param MAT The pointer to the array on which a square is to be initialised.
@param SIZE The size of the MAT array.
@param s The size of the square.
@param val The value to which to initialize.
*/
void InitCenterSquare(cl_float* MAT, cl_int SIZE, cl_int s, cl_float val){
    int S = SIZE/2 ;
    for(int i=0; i<SIZE;i++){
        for(int j=0; j<SIZE;j++){
            cl_bool cond =((i-S)<s)&&((S-i)<s)&&((j-S)<s)&&((S-j)<s);
            if(cond){
                MAT[j*SIZE+i] = val ;
            }
        }
    }
}

/**
@brief Initialize a 2D array to a circle in the center.
@param MAT The pointer to the array on which a square is to be initialised.
@param SIZE The size of the MAT array.
@param radius The radius of the circle.
@param val The value to which to initialize.
*/
void InitCenterCircle(cl_float* MAT, cl_int SIZE, cl_int radius, cl_float val){
    int S = SIZE/2 ;
    for(int i=0; i<SIZE;i++){
        for(int j=0; j<SIZE;j++){
            cl_bool cond = ((i-S)*(i-S) + (j-S)*(j-S)) <= (radius*radius) ; 
            if(cond){
                MAT[j*SIZE+i] = val ;
            }
        }
    }
}


#endif
//END OF FILE