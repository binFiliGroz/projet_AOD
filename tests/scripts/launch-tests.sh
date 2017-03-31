#!/bin/bash

if [ ! -d "tmp" ];then
    mkdir tmp
fi

for test_file_in in `ls tests/refs | grep in`
do
    test_file_out=`echo $test_file_in | sed 's/in/out/'`
    n=`wc -w tests/refs/$test_file_in | cut -d " " -f 1`
    bin/compileBST $n tests/refs/$test_file_in > tmp/$test_file_out

    diff  tmp/$test_file_out tests/refs/$test_file_out > /dev/null
    if [ $? -ne 0 ]
    then 
	echo "Test failed"
    else 
	echo "Test OK"
    fi
done

exit 0
