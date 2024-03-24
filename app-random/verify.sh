#!/bin/bash 

# Verify an Original EyC component.
#

set -u

nonce_in=0000000000000000000000000000000000000000
key=0000000000000000000000000000000000000000000000000000000000000000
slot=6

echo "Secret key:   $key (slot $slot)"

# Get serial
sn=`./serial`
sn01=${sn:0:4}
sn23=${sn:4:4}
sn47=${sn:8:8}
sn8=${sn:16:2}


# Get random nonce and device MAC
nonce_out=`./nonce $nonce_in`
device_mac=`./mac $slot`

echo "Nonce:        $nonce_out"
echo "Device MAC:   $device_mac"


# Calculate local TempKey
opcode=16
mode=00
param2=00

tempkey_msg=$nonce_out$nonce_in$opcode$mode$param2
tempkey=$(echo $tempkey_msg | xxd -r -p | sha256sum | cut -d ' ' -f 1)

# Calculate local MAC
opcode=08
mode=41
keyid=$(printf "%02x00" $slot)
zeros1=0000000000000000
zeros2=000000

mac_msg=$key$tempkey$opcode$mode$keyid$zeros1$zeros2$sn8$sn47$sn01$sn23
local_mac=$(echo $mac_msg | xxd -r -p | sha256sum | cut -d ' ' -f 1)

echo "Expected MAC: $local_mac"

# Result
if [ "$local_mac" = "$device_mac" ]; then
    echo "Verdict:      EyC original.  :-)"
else
    echo "Verdict:      Counterfeit.   :-S"
fi

