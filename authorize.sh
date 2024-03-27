#!/bin/bash 

#
# Performs a Check MAC in order to tell the device we know that key.
#

set -u # stop if any variable is undefined
set -e # stop if any command fails

############################################
# Check parameters
############################################

if [ $# -ne 2 ]
then
    echo "Performs a Check MAC with Nonce and key in order to tell the device we know that key"
    echo "Usage $0 KeyID Key"
    echo "Ex.: $0 7 0000000000000000000000000000000000000000000000000000000000000000"
    exit 1
fi

nonce_in=0000000000000000000000000000000000000000
keyslot=$1
key=$2

#echo "Reading data from slot: $dataslot"
#echo "Read key in slot: $keyslot"
#echo

############################################
# Get serial number
############################################
sn=`./serial`
sn01=${sn:0:4}
sn23=${sn:4:4}
sn47=${sn:8:8}
sn8=${sn:16:2}
#echo "Serial:       $sn"


############################################
# Get random nonce, data and device MAC
############################################
nonce_out=`./nonce $nonce_in`    # create nonce
#echo "Nonce:        $nonce_out"


############################################
# Calculate local TempKey
############################################
opcode=16
mode=00
param2=00
tempkey_msg=$nonce_out$nonce_in$opcode$mode$param2
tempkey=$(echo $tempkey_msg | xxd -r -p | sha256sum | cut -d ' ' -f 1)


############################################
# Calculate local MAC
############################################
opcode=08
mode=41
keyid=$(printf "%02x00" $keyslot)
zeros1=0000000000000000
zeros2=000000
mac_msg=$key$tempkey$opcode$mode$keyid$zeros1$zeros2$sn8$sn47$sn01$sn23
local_mac=$(echo $mac_msg | xxd -r -p | sha256sum | cut -d ' ' -f 1)

#echo "Calculated MAC: $local_mac"


############################################
# Call Check MAC
############################################
otherdata=$opcode$mode$keyid$zeros2$sn47$sn23
./check_mac $keyslot $local_mac $otherdata



