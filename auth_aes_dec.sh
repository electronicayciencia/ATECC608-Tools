#!/bin/bash 

#
# Decrypt a block of data using a secret symmetric key that requires Authorization
#

set -u # stop if any variable is undefined
#set -e # stop if any command fails

############################################
# Check parameters
############################################

if [ $# -ne 3 ]
then
    echo "Decrypt a block of data using a secret symmetric key that requires Authorization"
    echo "Usage $0 <SecretId> <AuthId> <data>"
    echo "Where:"
    echo "    <SecretId> is the secret AES key Id"
    echo "    <AuthId> is the authorization key Id"
    echo "    <data> is 16 hex bytes"
    echo "Ex.: $0 12 13 00000000000000000000000000000000"
    echo 
    echo "This script prompts for a password."
    echo "The authorization key value must be equal to the SHA256 of this password."
    exit 1
fi

SECRET_AES_KEY_ID=$1
AUTHORIZATION_KEY_ID=$2
DATA=$3

IV=00000000000000000000000000000000

read -s -p "Password: " pass

shapass=`echo -n "$pass" | sha256sum  | cut -d' ' -f 1`

./authorize.sh $AUTHORIZATION_KEY_ID $shapass > /dev/null

if [ $? -ne 0 ] 
then
    echo 
    echo "Authentication failure." 
    exit 1
else
    echo "Ok"
fi

set -e

./load_tempkey $IV > /dev/null
./gendig $SECRET_AES_KEY_ID > /dev/null
./aes_decrypt TEMPKEY $DATA





