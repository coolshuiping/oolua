#!/bin/bash 
premake4 clean
premake4 codeblocks linux
codeblocks --build oolua.workspace --target="Debug"
codeblocks --build oolua.workspace --target="Release"
