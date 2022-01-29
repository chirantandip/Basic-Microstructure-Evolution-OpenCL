/**
@file getCLINFO.c
@brief The file gets the necessary OpenCL environment info to a EnvInfo.json file.
*/

#define _CRT_SECURE_NO_WARNINGS

#define CL_TARGET_OPENCL_VERSION 210
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <math.h>
#include <string.h>

// The header file for OpenCL is different for differnet OS
#ifdef MAC
#include <OpenCL/cl.h>
#else  
#include <CL/cl.h>
#endif

/**
@brief A function to get the necessary information on an OpenCL platform.
@param platform An OpenCL platform whose details are needed.
*/
void getPlatformInfo(cl_platform_id platform){
    const int attbrCount = 5 ;
    char* attbrName[5] =  { "CL_PLATFORM_NAME", "CL_PLATFORM_VENDOR", "CL_PLATFORM_VERSION", "CL_PLATFORM_PROFILE", "CL_PLATFORM_EXTENSIONS" };
    cl_platform_info attributeTypes[5] = { CL_PLATFORM_NAME, CL_PLATFORM_VENDOR,
        CL_PLATFORM_VERSION, CL_PLATFORM_PROFILE, CL_PLATFORM_EXTENSIONS };
    
    for(int i=0;i<attbrCount;i++){
        size_t infoSize ;
        char* info ;
        clGetPlatformInfo(platform, attributeTypes[i], 0, NULL, &infoSize);
        info = (char*) malloc(infoSize);
        clGetPlatformInfo(platform, attributeTypes[i], infoSize, info, NULL);
        printf("\"%s\": \"%s\",\n",attbrName[i],info);
    }
}

/**
@brief A function to get the necessary information on an OpenCL device.
@param device An OpenCL device whose details are needed.
*/
void getDeviceInfo(cl_device_id device){
    size_t infoSize ;
    char* info ;
    
    clGetDeviceInfo(device, CL_DEVICE_NAME,0,NULL,&infoSize);
    info = (char*)malloc(infoSize);
    clGetDeviceInfo(device, CL_DEVICE_NAME,infoSize,info,NULL);
    printf("\"CL_DEVICE_NAME\": \"%s\",\n",info);
    free(info);
    
    cl_device_type dt ;
    clGetDeviceInfo(device, CL_DEVICE_TYPE,	sizeof(cl_device_type), &dt, NULL);
	printf("\"%s\": \"%s%s%s%s\",\n", "CL_DEVICE_TYPE", dt & CL_DEVICE_TYPE_CPU ? "CPU" : "", dt & CL_DEVICE_TYPE_GPU ? "GPU" : "", dt & CL_DEVICE_TYPE_ACCELERATOR ? "ACCELERATOR" : "", dt & CL_DEVICE_TYPE_DEFAULT ? "DEFAULT" : "");
    
    clGetDeviceInfo(device, CL_DEVICE_VERSION,0,NULL,&infoSize);
    info = (char*)malloc(infoSize);
    clGetDeviceInfo(device, CL_DEVICE_VERSION,infoSize,info,NULL);
    printf("\"CL_DEVICE_VERSION\" : \"%s\",\n",info);
    free(info);
    
    clGetDeviceInfo(device, CL_DEVICE_VENDOR,0,NULL,&infoSize);
    info = (char*)malloc(infoSize);
    clGetDeviceInfo(device, CL_DEVICE_VENDOR ,infoSize,info,NULL);
    printf("\"CL_DEVICE_VENDOR\" : \"%s\",\n",info);
    free(info);
    
    clGetDeviceInfo(device, CL_DEVICE_EXTENSIONS,0,NULL,&infoSize);
    info = (char*)malloc(infoSize);
    clGetDeviceInfo(device,CL_DEVICE_EXTENSIONS,infoSize,info,NULL);
    printf("\"CL_DEVICE_EXTENSIONS\": \"%s\",\n",info);
    free(info);
    
    clGetDeviceInfo(device,CL_DEVICE_OPENCL_C_VERSION,0,NULL,&infoSize);
    info = (char*)malloc(infoSize);
    clGetDeviceInfo(device,CL_DEVICE_OPENCL_C_VERSION,infoSize,info,NULL);
    printf("\"CL_DEVICE_OPENCL_C_VERSION\": \"%s\",\n",info);
    free(info);
    
    printf("\"CL_DEVICE_MEM_SIZE_UNITS\": \"Bytes\",\n");
    
    size_t dev_mem_size ;
    clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_SIZE ,sizeof(dev_mem_size),&dev_mem_size,NULL);
    printf("\"CL_DEVICE_GLOBAL_MEM_SIZE\": \"%ld\",\n",dev_mem_size);
    
    clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE ,sizeof(dev_mem_size),&dev_mem_size,NULL);
    printf("\"CL_DEVICE_GLOBAL_MEM_CACHE_SIZE\": \"%ld\",\n",dev_mem_size); 
    
    clGetDeviceInfo(device, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE ,sizeof(dev_mem_size),&dev_mem_size,NULL);
    printf("\"CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE\": \"%ld\",\n",dev_mem_size);
    
    clGetDeviceInfo(device, CL_DEVICE_LOCAL_MEM_SIZE ,sizeof(dev_mem_size),&dev_mem_size,NULL);
    printf("\"CL_DEVICE_LOCAL_MEM_SIZE\": \"%ld\",\n",dev_mem_size); 
    
    clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS ,sizeof(dev_mem_size),&dev_mem_size,NULL);
    printf("\"CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS\": \"%ld\",\n",dev_mem_size); 
    
    clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_GROUP_SIZE ,sizeof(dev_mem_size),&dev_mem_size,NULL);
    printf("\"CL_DEVICE_MAX_WORK_GROUP_SIZE\": \"%ld\",\n",dev_mem_size); 
    
}

/**
@brief The main function of the file.
It detects existing platforms and devices and extracts the required environment information and writes them to a EnvInfo.json file.
*/
int main(){
    
    cl_uint num_platforms ;
    printf("{\n");
    
    clGetPlatformIDs(1, NULL, &num_platforms);
    printf("\"NUM_PLATFORMS\": \"%d\",\n", num_platforms) ;
    cl_platform_id* platforms ;
    platforms = (cl_platform_id*)malloc(sizeof(cl_platform_id)*num_platforms);
    clGetPlatformIDs(num_platforms, platforms,NULL) ;    
    
    for(int i=0;i<num_platforms;i++){
        printf("\"PLATFORM_%d\": ",i);
        printf("{\n");
        getPlatformInfo(platforms[i]);
        
        cl_device_id* devices ;
        cl_uint num_devices;
        
        clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, 0, NULL, &num_devices);
        printf("\"NUM_DEVICES\": \"%d\",\n", num_devices);
        
        devices = (cl_device_id*)malloc(sizeof(cl_device_id)*num_devices);
        clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, num_devices, devices, NULL);
        
        for(int j=0;j<num_devices;j++){
            printf("\"DEVICE_%d\": \n",j);
            printf("{\n");
            getDeviceInfo(devices[j]);
            printf("\"COMMENTS\": \"\"\n");
            printf("}\n");
        }
        printf("}");
        if(i!=(num_platforms-1)){printf(",");}
        printf("\n");
        free(devices);
    }
    printf("}\n");
}

//END OF FILE 