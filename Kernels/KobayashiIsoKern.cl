/**
@file KobayashiIsoKern.cl
@brief The OpenCL kernel code for the Kobayashi Isotropic dendrite growth.
*/

/**
@brief A function to get the laplacian of the temperature field.
@param TEMP The pointer to the global temperature buffer.
@param x The spatial x coordinate. The global ID 0 of the work item.
@param y The spatial y coordinate. The global ID 1 of the work item.
@return The laplacian calculated using the five point stencil within the finite difference approximation.
*/
float get_temp_laplacian(__global float* TEMP, int x, int y){
    float lap = 0.0f ;
    float T = TEMP[SIZE*y+x] ;
    if(x==0){
        lap += T_L;
    }else{
        lap += TEMP[SIZE*y +x -1];
    }
    if(x==(SIZE-1)){
        lap += T_R;
    }else{
        lap += TEMP[SIZE*y +x +1];
    }
    if(y==0){
        lap += T_T;
    }else{
        lap += TEMP[SIZE*(y-1) +x];
    }
    if(y==(SIZE-1)){
        lap += T_B ;
    }else{
        lap += TEMP[SIZE*(y+1) +x];
    }
    lap -= 4.0*TEMP[SIZE*y +x] ;
    return lap/(H*H);
}

/**
@brief A function to get the laplacian of the concentration/phase field.
@param PH The pointer to the global phase buffer.
@param x The spatial x coordinate. The global ID 0 of the work item.
@param y The spatial y coordinate. The global ID 1 of the work item.
@return The laplacian calculated using the five point stencil within the finite difference approximation.
*/
float get_phase_laplacian(__global float* PH, int x, int y){
    float lap = 0.0f ;
    float p = PH[SIZE*y+x] ;
    if(x==0){
        lap += PH_L;
    }else{
        lap += PH[SIZE*y +x -1];
    }
    if(x==(SIZE-1)){
        lap += PH_R;
    }else{
        lap += PH[SIZE*y +x +1];
    }
    if(y==0){
        lap += PH_T;
    }else{
        lap += PH[SIZE*(y-1) +x];
    }
    if(y==(SIZE-1)){
        lap += PH_B;
    }else{
        lap += PH[SIZE*(y+1) +x];
    }
    lap -= 4.0*PH[SIZE*y +x] ;
    return lap/(H*H);
}


/**
@brief A function to check the neighborhood of the work item.
@param PH The pointer to the global phase buffer.
@param x The spatial x coordinate. The global ID 0 of the work item.
@param y The spatial y coordinate. The global ID 1 of the work item.
@return A true or false.
The function checks the neighborhood values for similarity. If all the values of the five point stencil are equal we skip computing laplacian and derrivatives in the phase_field_evol_kern() function as those will be 0. 
*/
bool check_neighbors(__global float* PH, int x, int y){
    bool nbh = true ;
    float C = PH[SIZE*y+x] ;
    nbh = nbh && (PH[SIZE*(y+1)+x]==C) && (PH[SIZE*(y-1)+x]==C) && (PH[SIZE*y+x+1]==C)  && (PH[SIZE*y+x-1]==C) ;
    return nbh ;
}


/**
@brief The Kobayashi Isosotropic dendrite growth evolution kernel.
@param PHASE_IN The input phase/concentration field, at time t =n .
@param PHASE_OUT The output phase/concentration field, at time t =n+1 .
@param TEMP_IN The input temperature field, at time t =n .
@param TEMP_OUT The output temperature field, at time t =n+1 .
@param PHASE_NOISE The noise amplitide for introducing randomness in the phase field. 
*/
__kernel void phase_field_evol_kern(
                                        __global float* PHASE_IN,
                                        __global float* PHASE_OUT,
                                        __global float* TEMP_IN,
                                        __global float* TEMP_OUT,
                                        float PHASE_NOISE){
    // Get global IDs
    int gx = get_global_id(0);
    int gy = get_global_id(1);

    ///////// Phase field evolution
    // get the center point
    float p1 = PHASE_IN[SIZE*gy +gx];

    // get current temperature
    float Temp = TEMP_IN[SIZE*gy +gx] ;

    // calculate m
    float m = (ALPHA/M_PI_F)*atan(GAMMA*(-T_MELT +Temp)) ;

    // calculate ther terms
    float terms =(EPS_BAR*EPS_BAR*get_phase_laplacian(PHASE_IN,gx,gy)) +(p1*(1.0-p1)*(p1-0.5+m));

    // calculate the evolution
    float noise = PHASE_NOISE*( (((float)gx/SIZE)-0.5)*(((float)gy/SIZE)-0.5) ) ;
    float p2 = p1 + (DT/TAU)*(terms +p1*(1.0-p1)*noise);

    PHASE_OUT[SIZE*gy +gx] = p2 ;

    //////// Temp field evolition 
    terms = THERM_DIFF*get_temp_laplacian(TEMP_IN,gx,gy) - LAT_H*(p2-p1)/DT;

    TEMP_OUT[SIZE*gy +gx] = Temp + DT*(terms) ;
}
