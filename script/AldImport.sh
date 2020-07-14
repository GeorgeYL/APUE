#!/bin/bash

ALDCS_CFG="";
V_LMeSrv=''
#-----------------------------------------------------------------------
# Start detecting whether there is the aldcs.conf exist.  
#-----------------------------------------------------------------------
echo "### Detecting the aldcs.conf..."
if [ -e /etc/aldcs.conf ] 
then
	ALDCS_CFG=/etc/aldcs.conf
	echo "### aldcs.conf was found[${ALDCS_CFG}]."
elif [ -e /opt/aldon/aldonlmc/current/etc/aldcs.conf ]
then
	ALDCS_CFG=/opt/aldon/aldonlmc/current/etc/aldcs.conf
	echo "!!! aldcs.conf was found[${ALDCS_CFG}]."
else
	echo "!!! Could not find the file aldcs.conf.Please check again"
	exit
fi

flgLMe="FALSE"
#cat ${ALDCS_CFG} | while read line
while read line
do 
    if [ $flgLMe == "FALSE" ]
    then
        if [[ -n "$line" && ${line:1:3} == "LMe" ]]
        then 
            flgLMe="TRUE"
        fi
    else
        V_LMeSrv=${line}
        flgLMe="FALSE"
        echo "### Found the server..."
        break
    fi
done < ${ALDCS_CFG}

array=(${V_LMeSrv//=/ })
#for var in ${array[@]}
#do 
#	echo $var
#done
V_LMeSrv=${array[0]}
#echo ${V_LMeSrv} 

# set path to client
export PATH=$PATH:/opt/aldon/aldonlmc/current/bin

#-----------------------------------------------------------------------
# Start with a clean log file.  
# The $$ includes you the process id (pid) of this shell script.
#-----------------------------------------------------------------------
# set the path to the ${BUILD_LOCATION}
BUILD_LOCATION=/tmp/build
mkdir /tmp/build > /dev/null 2>&1

LOG_FILE=/tmp/build/build_$$.log
if [ -e ${LOG_FILE} ]
then
    rm -f ${LOG_FILE}
    echo `date` > ${LOG_FILE}
fi

#-----------------------------------------------------------------------
# Specify the folder which included the files to be imported
#-----------------------------------------------------------------------
echo '----------------------------------------------------------------'
echo "Please specify the folder path which included the file(s) you " 
echo "want to import."

read -p "[Press Enter] To Use current path by default." FLD_IMP 
echo '----------------------------------------------------------------'
if [ -z ${FLD_IMP} ] ; then
	FLD_IMP=.
	echo "Your input is empty. Take current folder by default."
fi
cd ${FLD_IMP}

#-----------------------------------------------------------------------
# Check if the .aldlme file exist
#-----------------------------------------------------------------------
if [[ -d .aldlme && -f .aldlme/config ]]
then    
    while read line
    do
        echo $line
    done < .aldlme/config 

    RESET_GAR="Y"
    echo "Would you like to reuse the existed configration? [Yes/No]:"

    while read Option
    do
        if [[ $Option == "Yes" || $Option == "YES" 
        || $Option == "Y" || $Option == "y" ]]
        then
            echo "Your input is: ["$Option"]"
            RESET_GAR="N"
            break
        elif [[ $Option == "No" || $Option == "NO"
        || $Option == "N" || $Option == "n" ]]
        then
            echo "Your input is: ["$Option"]"
            RESET_GAR="Y"
            break
        else 
            echo "Please input [Yes/No]:"
        fi
    done 
fi
if [[ ${RESET_GAR} == "Y" ]] ; then
    echo '----------------------------------------------------------------'
    echo "Please specify the Group/Application/Release(version)::PD structure."
    echo "[eg: MyGroup/MyApp/MyBuildRel(1.0.0)::\$1]:"
    read GAR
    echo '----------------------------------------------------------------'
    echo "Your input GAR is: ${GAR} " >> ${LOG_FILE} 2>&1
    echo '----------------------------------------------------------------'
    
    # set the release details and perform the import
    ald initialize "${V_LMeSrv}:${GAR}" >> ${LOG_FILE} 2>&1 
    ald initialize -l >> ${LOG_FILE} 2>&1 
fi
# Modify here if using EncryptedSignon 
echo '----------------------------------------------------------------'
while read -p  "Please input your username:" ALD_USR
do
if [ -z ${ALD_USR} ] ; then
	echo "User name should not be empty! Please input your username:"
else 
	break
fi
done

read -s -p "Please input your password:" ALD_PWD
ald signon ${ALD_USR} -p ${ALD_PWD} -q 
#ald signon  ${ALD_USR} -p ${ALD_PWD} >> ${LOG_FILE} 2>&1

# do some process for Build
echo "Import run starting..." >> ${LOG_FILE}
    
ald import ${FLD_IMP}/*.*
echo '----------------------------------------------------------------'
echo "### Import Finished ###"
echo '----------------------------------------------------------------'
ald signoff >> ${LOG_FILE} 2>&1

#ENDOFTheFile########



