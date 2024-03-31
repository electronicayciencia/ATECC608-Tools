#!/bin/bash

#
# Decrypt data using AES-128-CBC mode.
#

set -u # stop if any variable is undefined

############################################
# Check parameters
############################################

if [ $# -ne 1 ]
then
    >&2 echo "Decrypt data using AES-128-CBC mode"
    >&2 echo ""
    >&2 echo "Usage $0 KeyID [IV]"
    >&2 echo "Where:"
    >&2 echo "    KeyID: is the key slot (or TEMPKEY)"
    >&2 echo "    IV: is the initialization vector (16 hex bytes) (0 is the default)"
    >&2 echo ""
    >&2 echo "Ex.: cat LICENSE.aes | ./cbc_decrypt.sh 3"
    exit 1
fi

SLOT=$1
IV=${2:-"00000000000000000000000000000000"}

############################################
# Main loop
############################################

last_encrypted_block=$IV

for current_encrypted_block in $(xxd -a -p -c 16)
do
    # Decrypt
    current_clear_block=$(./aes_decrypt $SLOT $current_encrypted_block)

    # Check for error
    if [ $? -ne 0 ]; then
        >&2 echo "Device error."
        exit 1
    fi
    
    # XOR (for CBC mode)
    current_clear_block=$(./xor $current_clear_block $last_encrypted_block)

    echo $current_clear_block | xxd -r -p
    
    last_encrypted_block=$current_encrypted_block
done

