#!/bin/bash

# Test basic ls
echo "Testing basic ls command:"
echo "ls" | ./hsh

# Test ls with arguments
echo -e "\nTesting ls -l command:"
echo "ls -l" | ./hsh

# Test ls with path
echo -e "\nTesting /bin/ls command:"
echo "/bin/ls" | ./hsh

# Test ls with path and arguments
echo -e "\nTesting /bin/ls -l command:"
echo "/bin/ls -l" | ./hsh
