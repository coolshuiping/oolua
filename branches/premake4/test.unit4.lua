--OOLua test.unit
local root = "./"
create_package("test.unit",root,"ConsoleApp")


configuration {}

files 
		{ 
    	"unit_tests/**.h",
    	"unit_tests/**.cpp"
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
			"unit_tests/bind_classes",
			"unit_tests/cpp_classes",
			"unit_tests/test_classes"
		} 
					
defines 
		{
			"USING_CPPUNIT",
			"USING_GMOCK",
			"OOLUA_STORE_ERROR"
		}


	configuration { "vs*"}
		postbuildcommands { "$(TargetPath)" }
		libdirs {root .. "lib"}
		links{"oolua"}
		--linkoptions{ "lua51" }
		
	configuration { "vs*","Debug"}
		links{ "cppunitd" , "gmockd" }

	configuration { "vs*","Release"}
		links{ "cppunit" , "gmock" }
		
	configuration {"codeblocks" }
		links{"oolua"}
		--linkoptions{ "-llua" }
		postbuildcommands { "$(TARGET_OUTPUT_FILE)"}
		libdirs {"-L".. root .. "lib"}
				
	configuration {"linux","codeblocks" }
		libdirs {"-Lusr/local/lib","-Lusr/lib"}
		links{ "cppunit" }
		linkoptions{"`gmock-config --cxxflags --ldflags --libs`"}

	configuration {"windows","codeblocks","Debug" }
		links{ "cppunitd" , "gmockd" }
		
	configuration {"windows","codeblocks","Release" }	
		links{ "cppunit" , "gmock" }
		
	configuration {"gmake"}
		libdirs{"-L" ..root .."lib","-Lusr/local/lib","-Lusr/lib"}
		links { "oolua","lua","cppunit" }
		linkoptions {"`gmock-config --cxxflags --ldflags --libs`"}
		
	configuration {"gmake","Debug"}	
		postbuildcommands = { root .. "bin/Debug/test.unit" }
		
	configuration {"gmake","Release"}	
		postbuildcommands = { root .. "bin/Release/test.unit" }


