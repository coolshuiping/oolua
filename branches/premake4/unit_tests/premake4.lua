--OOLua test.unit
local root = "../"
create_package("test.unit",root,"ConsoleApp")


configuration {}

files 
		{ 
    	"./**.h",
    	"./**.cpp"
		}
includedirs 
		{
			"include/cppunit",
			"include/gmock",
			"include/lua",
			"include/",
			root .. "include/",
			"/usr/local/include",
			"/usr/include",
			"./bind_classes",
			"./cpp_classes",
			"./test_classes"
		} 
					
defines 
		{
			"USING_CPPUNIT",
			"USING_GMOCK",
			"OOLUA_STORE_ERROR"
		}
links
	{
		"oolua"
	}

	configuration { "vs*"}
		postbuildcommands { "$(TargetPath)" }
		links{"lua51"}
		
	configuration { "vs*","Debug"}
		links{ "cppunitd" , "gmockd" }

	configuration { "vs*","Release"}
		links{ "cppunit" , "gmock" }
		
	configuration {"codeblocks" }
		postbuildcommands { "$(TARGET_OUTPUT_FILE)"}
				
	configuration {"gmake or codeblocks or xcode3","linux or macosx" }
		libdirs {"-Lusr/local/lib","-Lusr/lib", "L"..root.."bin/Debug/"}
		links{ "cppunit", "lua" }
		linkoptions{"`gmock-config --cxxflags --ldflags --libs`"}

	configuration {"windows","codeblocks","Debug" }
		links{ "lua", "cppunitd" , "gmockd" }
		
	configuration {"windows","codeblocks","Release" }	
		links{ "lua", "cppunit" , "gmock" }
		
	configuration {"gmake or xcode3","Debug"}	
		postbuildcommands  { root .. "bin/Debug/test.unit" }
		
	configuration {"gmake or xcode3","Release"}	
		postbuildcommands { root .. "bin/Release/test.unit" }


