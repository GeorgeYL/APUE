#!/bin/bash
#
################################################################################
# Date: 2019/08/08
# Desc: The script just shows the usage of import files into a LMe build relase.
# Author:
################################################################################

# Please make sure the params input sequece is right.
GROUP=$1;   # Group name
APP=$2;     # Application name
REL_B=$3;   # Build release name
VERSION=$4; # BuildRelease version

USR=$5      # LMe user used to signon ald command
PWD=$6      # LMe user password

SCRIPT_LOCATION=/tmp/build/auto_import/script
if [ -d ${SCRIPT_LOCATOIN} ] ; then
	mkdir -p ${SCRIPT_LOCATION}
fi
SCRIPT_FILE=${SCRIPT_LOCATION}/SampleAldImport.sh

LOG_FILE=/tmp/build/postbuild_$$.log
if [ -e ${LOG_FILE} ]
then
	rm -f ${LOG_FILE}
	echo `date` > ${LOG_FILE}
fi

echo  
echo "User:$USR"; 
echo "User: $USR"  >> ${LOG_FILE}
GAR="${GROUP}/${APP}/${REL_B}($VERSION)";
echo "Build Release GAR: ${GAR}" 
echo "Build Release GAR: ${GAR}" >>${LOG_FILE} 
#
################################################################################
# User Customized code for doing some other things. like compile/build project.
#  
################################################################################
# Compile the project ...

# Copy the output to a tmp folder

# mkdir -p /tmp/build/bin  >> ${LOG_FILE} 2>&1
# rm -rf /tmp/build/bin/* >> ${LOG_FILE} 2>&1
# cp /projectxxx/bin/*.jar /tmp/build/bin >> ${LOG_FILE} 2>&1
# cp /projectxxx/bin/*.war /tmp/build/bin >> ${LOG_FILE} 2>&1


# 
################################################################################
# Calling the script to import the build .war/.jar back to 
# Group/App/BuildRelease(x.x.x) 
################################################################################
if [ ! -f ${SCRIPT_FILE} ] ; then
	echo ${SCRIPT_FILE} does not exist! Please check again.
	exit 3
fi
su - ${USR} -s /bin/bash ${SCRIPT_FILE} $GAR $USR $PWD >> ${LOG_FILE} 2>&1
