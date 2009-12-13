#!/bin/bash 
premake4 clean
premake4 gmake linux
if [ ! -d build_logs ]; then
	mkdir build_logs
fi

make config=debug test.unit | tee ./build_logs/unit_gnu_debug.log
make config=release test.unit | tee ./build_logs/unit_gnu_release.log
premake4 clean
