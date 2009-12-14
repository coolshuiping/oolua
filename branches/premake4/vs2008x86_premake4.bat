:: build the solution
premake4.exe clean 
premake4.exe vs2008
:: start devenv
CALL "C:\Program Files\Microsoft Visual Studio 9.0\VC\vcvarsall.bat" x86
:: build the project
devenv  %~dp0oolua.sln /build debug /project %~dp0test.unit.vcproj >unit_vs2008_debug.log
devenv  %~dp0oolua.sln /build release /project %~dp0test.unit.vcproj >unit_vs2008.log
devenv  %~dp0oolua.sln /build debug /project %~dp0tests_may_fail.vcproj >may_fail_vs2008_debug.log
devenv  %~dp0oolua.sln /build release /project %~dp0tests_may_fail.vcproj >may_fail_vs2008.log
premake4 clean
