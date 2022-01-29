/**
@file DiffusionKern.cl
@brief The OpenCL kernel code for the diffusion equation.
The time evolution expression can be expressed as : 
\f[
 \frac{\partial \phi}{\partial t} = D \nabla^2\phi  \hspace{1cm}:\left[  \nabla^2 =  \frac{\partial^2 }{\partial x^2 } + \frac{\partial^2 }{\partial y^2} \right]
\f]
where D is the diffusion coefficient.
*/ 

/**
@brief The phase-field evolution equation.
@param gMAT1 Global Matrix 1 buffer, the input buffer
@param gMAT2 Global Matrix 2 buffer, the output buffer

The computations of the kernel can be expressed as: 
\f[
 \text{gMAT2} = \text{gMAT1} + \delta t * D \nabla^2\text{gMAT1}
\f]
*/
__kernel void phase_field_evol_kern(
                        __global float* gMAT1,
                        __global float* gMAT2){

int gx = get_global_id(0);
int gy = get_global_id(1);

float M, Right, Left, Top, Bottom ;

M = gMAT1[SIZE*gy +gx];

// Apply periodic boundary conditions
if(gy==0){
    Top = gMAT1[SIZE*(SIZE-1)+gx];
}else{
    Top = gMAT1[SIZE*(gy-1) +gx];
}

if(gx==0){
    Left = gMAT1[SIZE*gy + (SIZE-1)];
}else{
    Left = gMAT1[SIZE*gy +gx -1];
}
if(gy==(SIZE-1)){
    Bottom = gMAT1[gx];
}else{
    Bottom = gMAT1[SIZE*(gy+1)+gx];
}
if(gx==(SIZE-1)){
    Right = gMAT1[SIZE*gy];
}else{
    Right = gMAT1[SIZE*gy +gx + 1];
}

float p = Top +Bottom +Right +Left - 4*M;
float out = M + DT*COEFF*p/(H*H) ;

gMAT2[gy*SIZE+gx] = out;

}
//END OF FILE