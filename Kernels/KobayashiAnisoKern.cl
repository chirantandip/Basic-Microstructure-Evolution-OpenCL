/**
@file KobayashiAnisoKern.cl
@brief The OpenCL kernel code for the Kobayashi anisotropic dendrite growth.
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

lap += TEMP[SIZE*y +x -1];
lap += TEMP[SIZE*y +x +1];
lap += TEMP[SIZE*(y-1) +x];
lap += TEMP[SIZE*(y+1) +x];
lap -= 4.0*T ;

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

lap += PH[SIZE*y +x -1];
lap += PH[SIZE*y +x +1];
lap += PH[SIZE*(y-1) +x];
lap += PH[SIZE*(y+1) +x];
lap -= 4.0*p ;

return lap/(H*H);
}

/**
@brief A function to calculate the partial derivative of the concentration field along the Y axis.
@param PH The pointer to the global phase buffer.
@param x The spatial x coordinate. The global ID 0 of the work item.
@param y The spatial y coordinate. The global ID 1 of the work item.
@return The partial derivative calculated within the finite difference approximation.
*/
 float get_dPdY(
               __global float* PH,
               int x,
               int y){
float Top, Bottom ;
Top = PH[SIZE*(y-1) +x];
Bottom = PH[SIZE*(y+1)+x];

return (Bottom-Top)/(2.0*(float)H) ;
}

/**
@brief A function to calculate the partial derivative of the concentration field along the X axis.
@param PH The pointer to the global phase buffer.
@param x The spatial x coordinate. The global ID 0 of the work item.
@param y The spatial y coordinate. The global ID 1 of the work item.
@return The partial derivative calculated within the finite difference approximation.
*/
 float get_dPdX(
               __global float* PH,
               int x,
               int y){
float Right, Left ;
Left = PH[SIZE*y +x -1];
Right = PH[SIZE*y +x + 1];

return (Right - Left)/(2.0*(float)H) ;
}

/**
@brief A function to calculate the value of theta.
@param PH The pointer to the global phase buffer.
@param x The spatial x coordinate. The global ID 0 of the work item.
@param y The spatial y coordinate. The global ID 1 of the work item.
@return The decimal value of theta.

Theta is calculated using the following equation :
\f[
\theta = \arctan{ \left(\frac{\partial \phi / \partial y}{\partial \phi/ \partial x} \right)}
\f]
*/
float get_theta(
                __global float* PH,
                int x,
                int y){

float dPdY = get_dPdY(PH,x,y) ;
float dPdX = get_dPdX(PH,x,y) ;

if ((dPdX==0)||(dPdY==0)){
    return 0 ;
} 
else{
    return atan(dPdY/dPdX) ;
}
}

/**
@brief A function to calculate the value of epsilon.
@param theta The value of theta.


Epsilon is calculated using the following equation :
\f[
\epsilon = \bar{\epsilon}(1 + \delta \cos{j(\theta - \theta_0)})
\f]
*/
float get_epsilon(float theta){
    return (float)EPS_BAR*(1 +DELTA*cos(J*(theta -THETA0))) ;
}

/**
@brief A function to calculate the partial derivative of epsilon wrt. theta.
@param theta The value of theta.

The derivative is calculated using the following equation :
\f[
\frac{\partial \epsilon}{\partial \theta}= -\bar{\epsilon}*\delta *j* \sin{j(\theta - \theta_0)}
\f]
*/
float get_DepsDtheta(float theta){
    return -(float)EPS_BAR*DELTA*J*sin(J*(theta -THETA0));
}


/**
@brief A function to calculate the value of e*de/dtheta.
@param PH The pointer to the global phase buffer.
@param x The spatial x coordinate. The global ID 0 of the work item.
@param y The spatial y coordinate. The global ID 1 of the work item.
@return The decimal value of expression.

Calculated using the following equation :
\f[
\text{return} = \epsilon*\frac{\partial \epsilon}{\partial \theta}
\f]
Here both epsilon and the partial derivative is calculated using the declared get_DepsDtheta() and get_epsilon() functions.
*/
float get_epsDepsDtheta(__global float* PH, int x, int y){
    float theta  = get_theta(PH,x,y) ;
    return get_epsilon(theta)*get_DepsDtheta(theta) ;
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
@brief The Kobayashi Anisotropic dendrite growth evolution kernel.
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

    bool condition = (gx<2)||(gx>(SIZE-3))||(gy<2)||(gy>(SIZE-3)) || (check_neighbors(PHASE_IN,gx,gy));

    // basic vars to be used in both situations
    float Temp, mm, term3, p1, p2 ;
    p1 = PHASE_IN[SIZE*gy +gx];
    Temp = TEMP_IN[SIZE*gy +gx] ;
    mm = ((float)ALPHA/3.14152557)*atan((float)GAMMA*(-T_MELT +Temp)) ;

    if(condition){

        // because everything else is zero!
        term3 = p1*(1.0-p1)*(p1-0.5+mm) ;
        p2 = p1+ ((float)DT/(float)TAU)*(term3 +p1*(1.0 -p1)*PHASE_NOISE*sin((float)gx)*cos((float)gy)); ;

        PHASE_OUT[SIZE*gy +gx] = p2 ;
        TEMP_OUT[SIZE*gy +gx] = Temp - LAT_H*(p2-p1) ;

    }else{

        // printf("[[%d,%d]]",gx,gy);
        // get the center point
        float tmp1, tmp2, term2, term1, eps ; 

        // get term1
        tmp1=get_dPdX(PHASE_IN,gx,gy+1)*get_epsDepsDtheta(PHASE_IN,gx,gy+1) ; 
        tmp2=get_dPdX(PHASE_IN,gx,gy-1)*get_epsDepsDtheta(PHASE_IN,gx,gy-1) ; 
        term1 = (tmp1-tmp2)/(2.0*(float)H) ;

        // get term2
        tmp1 = get_dPdY(PHASE_IN,gx+1,gy)*get_epsDepsDtheta(PHASE_IN,gx+1,gy) ;
        tmp2 = get_dPdY(PHASE_IN,gx-1,gy)*get_epsDepsDtheta(PHASE_IN,gx-1,gy) ;
        term2 = (tmp1-tmp2)/(2.0*(float)H) ;

        // get epsilon
        eps  = get_epsilon( get_theta(PHASE_IN, gx, gy)) ;

        term3 = eps*eps*get_phase_laplacian(PHASE_IN,gx,gy) + p1*(1.0-p1)*(p1-0.5+mm) ;
        // calculate the evolution
        p2= p1+ ((float)DT/(float)TAU)*(term1-term2+term3 +p1*(1.0 -p1)*PHASE_NOISE*sin((float)gx)*cos((float)gy));

        PHASE_OUT[SIZE*gy +gx] = p2;

        //////// Temp field evolution 
        term1 = THERM_DIFF*get_temp_laplacian(TEMP_IN,gx,gy);
        term2 = LAT_H*(p2-p1);
        TEMP_OUT[SIZE*gy +gx] = Temp + DT*term1 -term2 ;
    }

}

// END OF FILE