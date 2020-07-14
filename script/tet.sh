#!/bin/bash
fun()
{	

echo "OK "
echo "$1" 
TEST="$1" 
echo "TEST: ${TEST}"
}
# calling the function with one parm
fun "par1"
