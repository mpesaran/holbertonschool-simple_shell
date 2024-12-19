#!/bin/bash

# Test with empty PATH
echo -e "\nTesting with empty PATH:"
PATH="" ./hsh << EOF
ls
EOF

# Test with modified PATH
echo -e "\nTesting with /bin at end of PATH:"
PATH="/usr/local/bin:/usr/bin:/bin" ./hsh << EOF
ls
EOF

# Test with no PATH
echo -e "\nTesting with no PATH:"
unset PATH
echo "ls" | ./hsh
