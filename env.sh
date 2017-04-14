#!/bin/sh

env_dir=$(pwd)
export LD_LIBRARY_PATH="$env_dir/libso:$env_dir/libuart"
