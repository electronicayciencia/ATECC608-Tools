#!/bin/bash 

#
# Verify an Original EyC component and retrieve verified data from a slot.
#

set -u # stop if any variable is undefined
set -e # stop if any command fails

nonce_in=0000000000000000000000000000000000000000
key=0000000000000000000000000000000000000000000000000000000000000000
keyslot=6
dataslot=8

echo "EyC Original component verifier"
echo 

echo "Secret key:   Key in slot $keyslot"


############################################
# Get serial number
############################################
sn=`./serial`
sn01=${sn:0:4}
sn23=${sn:4:4}
sn47=${sn:8:8}
sn8=${sn:16:2}
echo "Serial:       $sn"


############################################
# Get random nonce, data and device MAC
############################################
nonce_out=`./nonce $nonce_in`    # create nonce
out=`./gendig $dataslot`         # incorporate data Digest to TempKey
device_mac=`./mac $keyslot`      # create mac with the key
slotdata=`./read_slot $dataslot` # Get cleartext data

#echo "Nonce:        $nonce_out"
#echo "Device MAC:   $device_mac"


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
param2=0800
sn8=ee
sn01=0123
zeros=00000000000000000000000000000000000000000000000000
tempkey_msg=$slotdata$opcode$param1$param2$sn8$sn01$zeros$tempkey
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

#echo "Expected MAC: $local_mac"


############################################
# Result
############################################
if [ "$local_mac" = "$device_mac" ]; then
    echo "Verdict:      Original. MAC matches  :-)"
else
    echo "Verdict:      Counterfeit.   :-S"
fi

echo
echo "Data in slot $dataslot:"
echo $slotdata | xxd -r -p
echo
echo

