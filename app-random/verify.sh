#!/bin/bash 

set -u

key=0000000000000000000000000000000000000000000000000000000000000000
nonce_in=0000000000000000000000000000000000000000

nonce_out=c54606fb3d2bed6f91724284428e8a248521cdeafaa7f79fa5136e91b1a58acf
opcode=16
mode=00
param2=00

tempkey=$(echo $nonce_out$nonce_in$opcode$mode$param2 | xxd -r -p | sha256sum | cut -d ' ' -f 1)


opcode=08
mode=41
keyid=0600
zeros1=0000000000000000
zeros2=000000
sn8=ee
sn47=3d783966
sn01=0123
sn23=116f

mac=$(echo $key$tempkey$opcode$mode$keyid$zeros1$zeros2$sn8$sn47$sn01$sn23 | xxd -r -p | sha256sum | cut -d ' ' -f 1)


echo Expected MAC: $mac

