/**
@file CahnHilliardKern.cl
@brief The OpenCL kernel code for the Cahn-Hilliard (spinodal decomposition) evolution.
The time evolution equation is expressed as: 
\f[
\frac{\partial \phi}{\partial t}=\mu \nabla^2 \left[ f - \kappa \nabla^2\phi \right] \hspace{1cm}:\left[  \nabla^2 =  \frac{\partial^2 }{\partial x^2 } + \frac{\partial^2 }{\partial y^2} \right]
\f]
The kernel is divided into two functions because of the complex evolution equation. The CHInnerEvol() function computes the inner evolution i.e. inside the squar brackets and the CHOuterEvol() function computes the outer evolution.
*/


/**
@brief The Cahn-Hilliard Inner Evolution Kernel.
@param IN The input concentration field.
@param OUT The output data field which is the global InBracM buffer.

The kernel computes the following : 
\f[
\text{OUT} =  f - \kappa \nabla^2\phi 
\f]
*/
void CHInnerEvol(
                        __global float* IN,
                        __global float* OUT){
    int gx = get_global_id(0);
    int gy = get_global_id(1);

    float M, Right, Left, Top, Bottom ;
    
    M = IN[SIZE*gy +gx];
    // Calculate g(C)
    float g = (float)2*M*(0.9-M)*(1-2*M);

    // Calculate the laplacian
    if(gy==0){
        Top = IN[SIZE*(SIZE-1)+gx];
    }else{
        Top = IN[SIZE*(gy-1) +gx];
    }

    if(gx==0){
        Left = IN[SIZE*gy + (SIZE-1)];
    }else{
        Left = IN[SIZE*gy +gx -1];
    }
    if(gy==(SIZE-1)){
        Bottom = IN[gx];
    }else{
        Bottom = IN[SIZE*(gy+1)+gx];
    }
    if(gx==(SIZE-1)){
        Right = IN[SIZE*gy];
    }else{
        Right = IN[SIZE*gy +gx + 1];
    }

    float del2C = Top +Bottom +Right +Left -4*M;
    OUT[gy*SIZE+gx] =  g - KAPPA*2.0*(float)del2C;
}


/**
@brief The Cahn-Hilliard Outer evolution kernel.
@param CONC The concentration field taken as input.
@param InBracM The inner bracket matrix, which is the output of the CHInnerEvol(), taken as input.
@param OUT The output buffer where the integrated concentration field values are written.

The kernel computes the following : 
\f[
\text{OUT}= \text{CONC} + \delta t * \left[ \mu \nabla^2 \text{InBracM} \right]
\f]

*/
void CHOuterEvol(
                          __global float* InBracM,
                          __global float* CONC,
                          __global float* OUT){
int gx = get_global_id(0);
int gy = get_global_id(1);

float C, M, Right, Left, Top, Bottom ;
M = InBracM[SIZE*gy +gx];
C = CONC[SIZE*gy +gx];

if(gy==0){
    Top = InBracM[SIZE*(SIZE-1)+gx];
}else{
    Top = InBracM[SIZE*(gy-1) +gx];
}

if(gx==0){
    Left = InBracM[SIZE*gy + (SIZE-1)];
}else{
    Left = InBracM[SIZE*gy +gx -1];
}
if(gy==(SIZE-1)){
    Bottom = InBracM[gx];
}else{
    Bottom = InBracM[SIZE*(gy+1)+gx];
}
if(gx==(SIZE-1)){
    Right = InBracM[SIZE*gy];
}else{
    Right = InBracM[SIZE*gy +gx + 1];
}

float del2M = Top +Bottom +Right +Left -4*M;
del2M = (1.0f/(H*H))*del2M ;

float out = C + DT*MOBILITY*del2M;

if (out > 1.0){
     out = 1.0 ;
}
else if (out < 0.0){
    out = 0.0 ;
}

OUT[gy*SIZE+gx] = out ;

}

/**
@brief The Cahn-Hilliard evolutin kernel. 
@param InBracM A global float array to temporarily hold the inner evolution calculations.
@param PHASE1 The input concentration field at time t=n .
@param PHASE2 The output concentration field at time t=n+1 .

The kernel combines the inner and outer evolution function and the computations can be expressed as : 
\f[

\text{PHASE2} = \text{PHASE1} +  \delta t * \mu \nabla^2 \left[ f - \kappa \nabla^2\text{PHASE1} \right]

\f]
*/
__kernel void phase_field_evol_kern(
                                    __global float* InBracM,
                                    __global float* PHASE1,
                                    __global float* PHASE2){
    CHInnerEvol(PHASE1,InBracM);
    barrier(CLK_GLOBAL_MEM_FENCE);
    CHOuterEvol(InBracM, PHASE1, PHASE2);
}
//END OF FILE