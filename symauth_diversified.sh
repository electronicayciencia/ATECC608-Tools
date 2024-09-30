#!/bin/bash 

#
# Verify an Original EyC component using a diversified key.
# Use an authenticated read to retrieve Brand and manufacturing date.
#

set -u # stop if any variable is undefined
set -e # stop if any command fails

parentkey=0000000000000000000000000000000000000000000000000000000000000000
keyslot=6
dataslot=8

#nonce_in=0000000000000000000000000000000000000000
nonce_in=`cat /dev/random | head -c 20 | sha256sum | head -c 40`

echo "EyC Original component verifier. Diversified key."
echo 

echo "Parent key:  $parentkey"

############################################
# Get serial number
############################################
sn=`./serial`
sn01=${sn:0:4}
sn23=${sn:4:4}
sn47=${sn:8:8}
sn8=${sn:16:2}
echo "Serial:      $sn"

############################################
# Local-calculate derived key
############################################

# Tempkey seed for diversified key is SN + 00h x 23 bytes

opcode=1c
param1=00
param2=$(printf "%02x00" $keyslot)
zeros=00000000000000000000000000000000000000000000000000
tempkey=${sn}0000000000000000000000000000000000000000000000
tempkey_msg=$parentkey$opcode$param1$param2$sn8$sn01$zeros$tempkey

key=$(echo $tempkey_msg | xxd -r -p | sha256sum | cut -d ' ' -f 1)

echo "Derived key: $key"
echo "Using slot:  $keyslot"




############################################
# Get random nonce, data and device MAC
############################################
nonce_out=`./nonce_rand $nonce_in`    # create nonce
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
param2=$(printf "%02x00" $dataslot)
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
    echo "Verdict:     It is original.  :-)"
    echo
    echo "Authenticated read data in slot $dataslot:"
    echo $slotdata | xxd -r -p
    echo
    echo

else
    echo "Verdict:     Counterfeit."
fi

