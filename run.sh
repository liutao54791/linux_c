#!/bin/sh

env_dir=$(pwd)
#if [ ! -d "$env_dir/lib" ];then
#       mkdir "$env_dir/lib"
#fi

export LD_LIBRARY_PATH="$env_dir/lib"

if [ -f "core_file" ]; then
 ulimit -c unlimited
fi

if [ -f "process_main" ]; then
 ./process_main 3
fi
