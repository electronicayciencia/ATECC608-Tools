#!/bin/bash 

#
# Rotate a key from a parent key and a randon nonce.
#

set -u # stop if any variable is undefined
set -e # stop if any command fails

nonce_in=0000000000000000000000000000000000000000

############################################
# Check parameters
############################################

if [ $# -ne 1 ]
then
    echo "Rotate a key from a parent key and a randon nonce"
    echo "Usage $0 KeyID"
    echo "Ex.: $0 6"
    echo ""
    echo "Note that in case target key requires authorization, you need to run"
    echo "authorize command first."
    exit 1
fi

read -p "Input parent key (32 hex bytes): " parent

key=$1

############################################
# Get serial number
############################################
sn=`./serial`
sn01=${sn:0:4}
sn23=${sn:4:4}
sn47=${sn:8:8}
sn8=${sn:16:2}


############################################
# Get random nonce, data and device MAC
############################################
nonce_out=`./nonce_rand $nonce_in`    # create nonce
echo "Nonce: $nonce_out"


############################################
# Calculate local MAC
############################################
opcode=1c
param1=00
param2=$(printf "%02x00" $key)
mac_msg=$parent$opcode$param1$param2$sn8$sn01

mac=$(echo $mac_msg | xxd -r -p | sha256sum | cut -d ' ' -f 1)


############################################
# Call Derive Key
############################################
./derivekey $key 0 $mac


