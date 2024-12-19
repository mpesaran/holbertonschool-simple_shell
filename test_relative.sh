#!/bin/bash

# First create necessary directories and copy ls
mkdir -p ../parent
mkdir -p ../../grandparent
cp /bin/ls ./hbtn_ls
cp /bin/ls ../parent/hbtn_ls
cp /bin/ls ../../grandparent/hbtn_ls
chmod +x ./hbtn_ls
chmod +x ../parent/hbtn_ls
chmod +x ../../grandparent/hbtn_ls

# Test various relative paths
echo -e "\nTesting ./hbtn_ls:"
echo "./hbtn_ls /var" | ./hsh

echo -e "\nTesting ../hbtn_ls:"
echo "../parent/hbtn_ls /var" | ./hsh

echo -e "\nTesting ../../hbtn_ls:"
echo "../../grandparent/hbtn_ls /var" | ./hsh

echo -e "\nTesting ./.././../hbtn_ls:"
echo "./.././../hbtn_ls /var" | ./hsh
