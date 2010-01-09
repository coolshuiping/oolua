OOLua Cross platform C++ and Lua binding framework
===================================================
http://code.google.com/p/oolua/

Bug reports are welcome. Please use the GoogleCode bug tracking system at 
http://code.google.com/p/oolua/issues/list

Library Help
------------
Basic usage can be seen : http://code.google.com/p/oolua/wiki/CheatSheet
Mailing List : http://groups.google.com/group/oolua-user
Twitter Account : http://twitter.com/OOLua


Requirements
------------
Makefiles or IDE Projects
(version 4.2) http://industriousone.com/premake 
Unit Tests
http://sourceforge.net/apps/mediawiki/cppunit/
http://code.google.com/p/googlemock/

Compile Scripts
---------------
Run a bash file (.sh) or bat (.bat) file of your choice in the root directory, these will produce
output saved to disk located in a directory create (build_logs). This is the compiler output, and 
UnitTests results if selected.


Building Makefiles or IDE projects.
-----------------------------------
OOLUA's source and header files can added to your project or be compiled as a static library.
To compile as a static library or to run the UnitTests, Premake version 4 is the recommended 
method to generate make files/IDE projects. This can either be accomplish using the following 
details or using the pre-existing bash or bat files(Compile Scripts). 


Premake Format
--------------
premake4 [make or IDE] [target operation system]

==Makefile==

premake4 gmake []

    * macosx
    * linux 

==Xcode==

premake4 xcode3 macosx

macosx is required


==Visual Studio==

premake4 vs[] windows

windows is required

    * 2005
    * 2008
    * 2010 

==CodeBlocks==

premake4 codeblocks []

    * windows
    * linux
    * macosx 

For further Premake details see http://industriousone.com/premake or type

premake4 --help