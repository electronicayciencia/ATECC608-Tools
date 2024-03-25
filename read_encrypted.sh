#!/bin/bash 

#
# Read encrypted data from a slot.
#

set -u # stop if any variable is undefined
set -e # stop if any command fails

############################################
# Check parameters
############################################

if [ $# -ne 3 ]
then
    echo "Read encrypted data from a slot"
    echo "Usage $0 SlotID KeyID Key"
    echo "Ex.: $0 2 7 0000000000000000000000000000000000000000000000000000000000000000"
    exit 1
fi

nonce_in=0000000000000000000000000000000000000000
dataslot=$1
keyslot=$2
key=$3

echo "Reading data from slot: $dataslot"
echo "Read key in slot: $keyslot"
echo

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
out=`./gendig $keyslot`          # incorporate key Digest to TempKey
slotdata=`./read_slot $dataslot` # Get encrypted data

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
# Update Tempkey using slot data
############################################
opcode=15
param1=02
param2=$(printf "%02x00" $keyslot)
zeros=00000000000000000000000000000000000000000000000000
tempkey_msg=$key$opcode$param1$param2$sn8$sn01$zeros$tempkey
tempkey=$(echo $tempkey_msg | xxd -r -p | sha256sum | cut -d ' ' -f 1)


############################################
# Decrypt data using calculated TempKey
############################################
cleartext=`./xor $tempkey $slotdata`

echo "Data: $cleartext"




