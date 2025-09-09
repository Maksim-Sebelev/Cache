#!/bin/bash

# execute this script from directory '<project root dir>/build/'

if [ $# -ne 1 ]; then
    echo "expected 1 argument - name of files with test data."
    echo "exit 1"
    exit 1
fi

test_file="$1"

if ! [ -f $test_file ]; then
    echo "'$test_file' - didn\`t exists"
    echo "exit 1"
    exit 1
fi

cache_executable="cache"

if ! [ -f $cache_executable ]; then 
    echo "'$cache_executable' didn\`t exists"
    echo "exit 1"
    exit 1
fi

./$cache_executable ${test_file}

