#!/bin/bash

if [ ! -d "tmp" ];then
    mkdir tmp
fi

for test_file_in in `ls tests/refs | grep in`
do
    test_file_out=`echo $test_file_in | sed 's/in/out/'`
    n=`wc -w tests/refs/$test_file_in | cut -d " " -f 1`
    echo $test_file_in
    bin/compileBST $n tests/refs/$test_file_in > tmp/$test_file_out
done

exit 0
