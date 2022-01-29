#/bin/bash

# This configure script is intended to guide the user through
# the installation process. This checks for all environmental
# conditions required to run an OpenCL program on the host
# device and tells the user to install what's not found.

# Some aliases set for the purpose of this script
echo "Running configure script. Some commands require root privilege.
In that case please type in password." ;
echo "";

# Check for clinfo
CL_INFO_EXISTS=$(which clinfo) ;
CL_INFO_ERR="CLINFO_EXISTS_CHECK : Failed
clinfo not found on your device.
Install it from : https://github.com/Oblomov/clinfo" ;
CL_INFO_SUC="CL_INFO_EXISTS_CHECK : Successful
clinfo found in $CL_INFO_EXISTS" ;
if [[ -z $CL_INFO_EXISTS ]]; then
	echo "$CL_INFO_ERR" ;
	exit 1 ;
else
	echo "$CL_INFO_SUC" 
fi
echo ""

# Check for the operating system. MAC or LINUX
OS=$(uname -s | tr [:lower:] [:upper:] ;) ;
echo "Operating system detected : $OS" ;
if [[ "$OS" == "LINUX" ]]; then
	OCL_LIB="-lOpenCL";
elif [[ "$OS" == "DARWIN" ]]; then
	OCL_LIB="framework OpenCL";
else
	echo "The program cannot run in your operating system" ;
	exit 1 ;
fi
# Check for CPUs available
CPUS_VENDOR=$(sudo lshw -class processor | grep 'vendor' ;);
CPUS_NAMES=$(sudo lshw -class processor | grep 'product' ;);
NUM_CPUS=$(echo "$CPUS_NAMES" | wc -l );
echo "CPUs detected: $NUM_CPUS ";
while IFS= read -r line; do
    echo "${line#*product:}"
done <<< "$CPUS_NAMES"
# Check for GPUs available
GPUS_VENDOR=$(sudo lshw -numeric -C display | grep 'vendor' ;);
GPUS_NAMES=$(sudo lshw -numeric -C display | grep 'product' ;);
NUM_GPUS=$(echo "$GPUS_NAMES" | wc -l);
echo "GPUs detected: $NUM_GPUS ";
while IFS= read -r line; do
    echo "${line#*product:}"
done <<< "$GPUS_NAMES"
echo ""

# Check for OpenCL drivers available
echo "OpenCL drivers detected for the following devices: "
OCL_DRIVERS=$(clinfo -l | grep 'Device');
while IFS= read -r line; do
    echo "${line# \`-- Device #[0-9]:}"
done <<< "$OCL_DRIVERS"
NUM_DRIVERS=$(echo "$OCL_DRIVERS" | wc -l );
NUM_DEVICES=$(( $NUM_CPUS + $NUM_GPUS ));
echo "" ;
NO_DRIVER_HELP_TEXT="device(s) do(es) not have OpenCL driver(s) installed.
Install from vendor's website. Check the Readme file for more info.";
if [[ "$NUM_DRIVERS" -eq "0" ]]; then
	echo "No $NO_DRIVER_HELP_TEXT" ;
    exit 1 ;
elif [[ "$NUM_DRIVERS" -eq "$NUM_DEVICES" ]]; then
	echo "OPENCL_SDK_CHECK : Successful
 All devices have OpenCL drivers installed." ;
else
	echo "$(($NUM_DEVICES - $NUM_DRIVERS)) $NO_DRIVER_HELP_TEXT";
fi

echo "" ;

#
# Check for the C header files needed to be included
# to run this program
#HEADER_COMPILE_CHECK=$(gcc -Wall -g -DDEBUG ./EnvInfoFuncs/check_C_headers.c -o check_C_headers $OCL_LIB;)
#HEADER_CHECK_ERR="HEADER_FILES_COMPILE_CHECK : Failed
#Check the Readme file for information on how to install each header file required.
#ERROR: " ;
#HEADER_CHECK_SUC="HEADER_FILES_COMPILE_CHECK : Successful" ;
#if [[ -z $HEADER_COMPILE_CHECK ]]; then
#	echo "$HEADER_CHECK_SUC";
#	echo "Running ./check_headers to check further.";
#	HEADER_RUN_CHECK=$(./check_headers) ;
#	if [[ -z $HEADER_RUN_CHECK ]]; then
#		echo "HEADER_FILES_RUN_CHECK : Successful" ;
#		echo "Removing ./check_headers file" ;
#		rm ./check_headers ;
#	else
#		echo "HEADER_FILES_RUN_CHECK : Failed : ERROR:" ;
#		echo "$HEADER_RUN_CHECK" ;
#		exit 1 ;
#	fi
#else
#	echo "$HEADER_CHECK_ERR";
#	echo "$HEADER_COMPILED_CHECK" ;
#	exit 1 ;
#fi
#echo ""
#

# Check for doxygen
DOXYGEN_EXISTS=$(which doxygen) ;
DOXYGEN_ERR="DOXYGEN_EXISTS_CHECK : Failed
doxygen not found on your device. It is essential to build html/LaTeX documentation of this program.
Kindly install it from : https://github.com/doxygen/doxygen" ;
DOXYGEN_SUC="DOXYGEN_EXISTS_CHECK : Successful
clinfo found in $DOXYGEN_EXISTS" ;
if [[ -z $DOXYGEN_EXISTS ]]; then
	echo "$DOXYGEN_ERR" ;
	exit 1 ;
else
	echo "$DOXYGEN_SUC" 
fi
echo ""



# Check for python and related libraries
PYTHON_EXISTS=$(which python) ;
PYTHON_ERR="PYTHON_EXISTS_CHECK : Failed
python not found on your device.
You can install it using: sudo apt-get install python
For more information on the python requirement check
the Readme file file.";
PYTHON_SUC="PYTHON_EXISTS_CHECK : Successful
python found in $PYTHON_EXISTS" ;
if [[ -z $PYTHON_EXISTS ]]; then
	echo "$PYTHON_ERR" ;
else
	echo "$PYTHON_SUC" ;
fi

#END_OF_FILE