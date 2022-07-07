Welcome to the README.md file. It provides information on how to compile, run, use and maintain the program.

## How to run a simulation?

**CAUTION ! :**
> Before you run the program check whether all the dependencies are installed on your system with `make check`. See "How to use the Makefile?" for detailed info.

The program has four inbuillt simulations, there are four phase filed models written in C  + OpenCL framework. They are :
1. Classical diffusion equation : **DIFFUSION**
2. Spinodal decomposition using the Cahn-Hilliard equation : **CAHNHILLIARD**
3. Kobayashi dendrite growth.
	1. Isotropic directional solidification : **KOBISO**
	2. Anisotropic dendrite growth : **KOBANISO**

The bold words are the names of the systems in the makefile.
**Simulations are run by setting the `SYSTEM` variable of the makefile to the name of the system you want to run .**

The following code illustrates the four commands to run the four systems. 
```
make run SYSTEM=DIFFUSION
make run SYSTEM=CAHNHILLIARD
make run SYSTEM=KOBANISO
make run SYSTEM=KOBISO
```

Each SYSTEM has its own reading, initializing, processing, iterating and writing functions. All such functions are located in the header files of the `UtilityFunctions` directory.

Here is a brief summary of the header files:
|File name| Info|
|---------|------------|
|read_inp_file.h| Functions to read the input files specific to the system|
|global_vars.h|	All global variables/structs are declared here. |
|CL_utility_funcs.h|	Some OpenCL utility functions. |
|error_handle.h|	Error handling functions |
|file_to_program.h|	Functions to read kernel files and compile them to kernel|
|data_manip_funcs.h| Data initialization and manipulation functions.|
|init_CL_buffers.h|	Functions to initialize OpenCL data buffers |
|iterate_kernels.h| Functions to iterate the time evolution kernel |
|data_writing_funcs.h|	Data writing functions.|

***
## How to use the Makefile?
The Makefile is run by the command `make` . That without specifying any targets will just execute the first target which is 'build'.

The Makefile has the following targets :
#### build
```
make build
```
The command `make build` compiles the program and creates an executible file in the running directory. The running directory is defined by the Makefile's `RUN_DIR` variable. The name of the executible is `mainfile`. 
#### run
```
make run
```
The command `make run` compiles and runs the program, irrespective of any preexisting executable files.  You can alter the Makefile variables by passing them along with the run target. For example: `make run SYSTEM=CAHNHILLIARD CC=icc` to compile the Spinodal decomposition system with the Intel icc compiler and run it.
#### check
```
make check
```
The command `make check` will run the bash script `configure.sh`. You can also run it independently with the command `./configure.sh`.  What the configre file does can be found under the heading "What the configure.sh file does?" in this document.
#### getinfo
```
make getinfo
```
The command `make getinfo` uses the functions in the `EnvInfoFuncs` directory. It generates an `OpenCLenvInfo.json` file in the running directory, which contains the OpenCL environment information in the .json format. It can be used to optimize the code w.r.t the host computer's computing capabilities.
#### advice
```
make advice
```
The command `make advice` generates some programming advice using the python script `getAdvice.py` in the  `EnvInfoFuncs` directory. This utility is still under development.
#### doc
```
make doc
```
The command `make doc` generates HTML and LaTeX documentation using **doxygen**. The HTML documentation will be generated in the `Documentaion/html/` directory. (It will have multiple files and sub-directories and will look like a mess) The documentation can be viewed by opening the `index.html` file in the `Documentaion/html/` directory. 

The configuration script for doxygen is located in the `Documentation` directory. It also has an extra .css stylesheet that enlarges the fonts because doxygen's default fontsize is 10pt, very small on a laptop screen. 
#### clean
The `make clean` command will remove/delete the `mainfile` executable and the `OpenCLenvInfo.json` file.
***
The makefile also has the following variables that could be set from the command line.
#### SYSTEM
As discussed in the above section, the `SECTION `variable holds the name of the system/simulation to be compiled or run. It is explained under th heading "How to run a simulation?"
#### CC
The compiler, `CC` variavle holds the name of the compiler. The default compiler is `gcc`. To compile with any other compiler set the variable to the compiler name or name with path. For example: `make run CC=icc` or `make run CC=clang` or `make run cc=/usr/bin/intel/icc` etc.
#### CFLAGS
The compiler flags, `CFLAGS` variale is only to be changed if you are using a compiler other than `gcc` and that has other flags. It is used as `make run CC=icc CFLAGS=-Wall -m64 -someFlag`.
#### RUN_DIR
The running directory, `RUN_DIR` variable holds the name/path of the directory where the program is being compiled or is being run. The default value is `.` which is the current directory.

***
## What the configure.sh file does?
The configure file checks your host system for the following things:
1. **clinfo** : clinfo is an OpenCL package(?) that extracts the the OpenCL SDK information on your computer. For more information type `man clinfo`. If its not installed, you should install it using the command `sudo apt-get install clinfo`. If that does not work, please look up online
2. **devices** : the configure file checks for te available devices on your system. Your CPUs, GPUs etc will be detected. The file uses the bash utility function `lshw` and it **requires root privilege**. It will list th detected devices along with the vendor name on the command log/terminal screen.
3. **OpenCL SDK** SDK (Software Development Kit). Each vendor, NVIDIA, Intel etc. has their own SDKs which serves as the platfrom on which OpenCL codes run. The configure file will detect the existing OpenCL SDKs and will let you know your devices for which SDKs are available. 
4. **doxygen** : Doxygen is used to generate HTML documentation for the program with the command `make doc`. 
5. **python** : The configure file checks is python is available as its used to collect envioronment information and generate advice. That utility is still under development.
