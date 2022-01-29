# The makefile to compile the program
# Define the name of the program
PROG:=mainfile

# Define the system to run
# The default is Kobayashi Anisotropic Evolution evolution
SYSTEM:=KOBANISO
ifeq ($(SYSTEM), DIFFUSION)
running="System: Diffusion equation."
else ifeq ($(SYSTEM),CAHNHILLIARD)
running="System: Cahn-Hilliard evolution."
else ifeq ($(SYSTEM),KOBISO)
running="System: Kobayashi isotropic dendrite growth."
else ifeq ($(SYSTEM),KOBANISO)
running="System: Kobayashi anisotropic dendrite growth."
else
running="Undefined system name.\nRunning: default system[Kobayashi Anisotropic dendritic growth]"
SYSTEM=KOBANISO
endif

# Define what compiler to be used
CC:=gcc

# Check Hostname and set flags
HOST_NAME:= $(shell hostname)

ifeq ($(HOST_NAME),sironton)
	HOST_FLAGS=
endif
ifeq ($(HOST_NAME),gphaniserver)
	HOST_FLAGS=
endif
ifeq ($(HOST_NAME),aqua)
	HOST_FLAGS=
endif

# Define the Running Directory of the program
RUN_DIR:=.

# Check for 32-bit or 64-bit OS system
PROC_TYPE = $(strip $(shell uname -m | grep 64))
 
# Check Operating System
OS = $(shell uname -s | tr [:lower:] [:upper:])

# Set flags according to the compiler used
# the following flags are the same for either gcc or icc compiler
CFLAGS=-std=c99 -Wall -g -DDEBUG

# Check for DARWIN
ifeq ($(OS),DARWIN)
	# Define a macro MAC
	CFLAGS += -DMAC
	# the MAC library for OpenCL
	LIBS=-framework OpenCL
	# PROC_TYPE is null means 32 bit architecture
	ifeq ($(PROC_TYPE),)
		CFLAGS+=-arch i386
	else
		CFLAGS+=-arch x86_64
	endif
endif

# Check for LINUX
ifeq ($(OS),LINUX)
	LIBS=-lOpenCL
	# Processor type
	ifeq ($(PROC_TYPE),)
		CFLAGS+=-m32
	else
		CFLAGS+=-m64
	endif
endif

# Check for AMD GPU : AMDAPPSDKROOT 
ifdef AMDAPPSDKROOT
INCLUDE_DIRS=. $(AMDAPPSDKROOT)/include
ifeq ($(PROC_TYPE),)
LIB_DIRS=$(AMDAPPSDKROOT)/lib/x86
else
LIB_DIRS=$(AMDAPPSDKROOT)/lib/x86_64 /usr/local/lib
endif
endif

# Check for NVIDIA GPU ROOT : NVSDKCOMPUTE_ROOT
ifdef AVSDKCOMPUTE_ROOT
        INCLUDE_DIRS=. $(NVSDKCOMPUTE_ROOT)/OpenCL/common/inc
endif

####
# All make targets written here : 

build: $(RUN_DIR)/$(PROG).c
	@echo $(running) ;
	$(CC) $(CFLAGS) -D$(SYSTEM) -o $(RUN_DIR)/$(PROG) $(RUN_DIR)/$^ $(LIBS) ;
	@echo "Compiling $(PROG).c successful" ;

run: build $(RUN_DIR)/$(PROG)
	$(RUN_DIR)/$(PROG) ;
	@echo "Running $(PROG) successful" ;

check: $(RUN_DIR)/configure.sh
	bash $(RUN_DIR)/configure.sh ;

getinfo: $(RUN_DIR)/EnvInfoFuncs/getCLINFO.c
	@$(CC) $(CFLAGS) -o $(RUN_DIR)/getCLINFO $(RUN_DIR)/EnvInfoFuncs/getCLINFO.c $(LIBS) ;
	@$(RUN_DIR)/getCLINFO > $(RUN_DIR)/OpenCLenvInfo.json ;
	@echo "CL environment info written to OpenCLenvInfo.json" ;
	@mv $(RUN_DIR)/getCLINFO $(RUN_DIR)/EnvInfoFuncs/ ;

advice: $(RUN_DIR)/EnvInfoFuncs/getCLadvice.py
	python $(RUN_DIR)/EnvInfoFuncs/getCLadvice.py ;

doc: $(RUN_DIR)/Documentation/configdoxygen
	doxygen $(RUN_DIR)/Documentation/configdoxygen

.PHONY: clean

clean: 
	rm $(RUN_DIR)/$(PROG) $(RUN_DIR)/OpenCLenvInfo.json ; 
