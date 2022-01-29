## The file gets the host device's environment information and writes it on a .json file.
import json
from types import SimpleNamespace
import os
import platform
import math

"""A function to get host's local memory information."""
def getLocalMemInfo(CLdict):
    for PL in CLdict :
        if not PL == "NUM_PLATFORMS":
            PLT = CLdict[PL]
            numdevs = int(PLT.NUM_DEVICES)
            print("In "+PL+": "+PLT.CL_PLATFORM_NAME)
            PLAT = PLT.__dict__
            devID = "DEVICE_"
            for i in range(numdevs):
                devID = devID + str(i)
                dev = PLAT[devID]
                print(devID+": "+dev.CL_DEVICE_NAME)
                locMemSize = int(dev.CL_DEVICE_LOCAL_MEM_SIZE)
                print(" Device local memory size:                "+ str(locMemSize) +" bytes = "
                      + str(locMemSize//1024)+"kB")
                print(" Max floating data points it can hold:    " + str(locMemSize//4) )
                sq_mat_size = math.floor(math.sqrt(locMemSize//4))
                print(" Max SIZE of a square matrix it can hold: " + str(sq_mat_size)+"x"
                      +str(sq_mat_size)+" = "+str(sq_mat_size*sq_mat_size) )
            print("")
    return 0

# Check if EnvInfo.json exists. 
# If it does not exist compile getCLINFO anf generate one
env_info_file = "EnvInfo.json"
if not os.path.isfile(env_info_file):
    print("%s file does not exist" % (env_info_file))
    compilegetCLINFO = "gcc -std=c99 -Wall -g -DDEBUG"
    OSname = str(platform.system()).upper()
    if OSname == "LINUX" :
        oclLIB = " -lOpenCL"
    elif OSname == "DARWIN" :
        oclLIB = " -framework OpenCL"
    else :
        print("Cannot run this program on your operating system.")
        exit(1)
    compilegetCLINFO += " EnvInfoFuncs/getCLINFO.c -o getCLINFO "
    compilegetCLINFO += oclLIB
    print(compilegetCLINFO)
    if not os.system(compilegetCLINFO) == 0 :
        print("Unknown error in compiling EnvInfoFuncs/getCLINFO.c")
    else :
        print("Succesfully compiled EnvInfoFuncs/getCLINFO.c ")
    rungetCLINFO = "./getCLINFO > "
    rungetCLINFO += env_info_file
    if not os.system(rungetCLINFO) == 0 :
        print("Unknown error in running getCLINFO")
    else :
        print("Succesfully generated EnvInfo.json")
        os.system("rm getCLINFO")
# Now that the file exists read it and generate advoce
if os.path.isfile(env_info_file):
    CLINFO = open(env_info_file,"r")
    CLdata = json.load(CLINFO,object_hook=lambda d: SimpleNamespace(**d))
    CLdict = CLdata.__dict__
    getLocalMemInfo(CLdict)