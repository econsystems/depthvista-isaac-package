#!/bin/bash

if [ -z "$1" ]
then
echo -n 'Enter the absolute path of Isaac SDK (Eg: /home/user/isaac) : '
read input
else
input=$1
fi

while [[ "${input}" == "" ]] || [[ ! -d "${input}" ]] 
do

if [ "${input}" != "" ]
then
echo -e ${COLOR_RED}"[error] ${COLOR_DEFAULT}Invalid path: ${input}"
echo ""
fi

read -p "Enter the absolute path of Isaac SDK (Eg: /home/user/isaac) : " input
done

cp -r packages/ ${input}/sdk
cp -r apps/ ${input}/sdk
cp -r third_party/ ${input}/sdk
cp WORKSPACE ${input}/sdk

echo "Deployed successfully!"
