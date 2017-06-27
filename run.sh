#!/bin/sh

env_dir=$(pwd)
#if [ ! -d "$env_dir/lib" ];then
#       mkdir "$env_dir/lib"
#fi

export LD_LIBRARY_PATH="/opt/mycode/lib"

if [ -f "/opt/mycode/core_file" ]; then
 ulimit -c unlimited
fi

if [ -f "/opt/mycode/bin/process_main" ]; then
 /opt/mycode/bin/process_main 3
fi
