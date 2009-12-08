#!/bin/bash 
premake clean gmake linux
make CONFIG=Debug  | tee unit_gnu_debug.log
make CONFIG=Release | tee unit_gnu_release.log
premake clean
