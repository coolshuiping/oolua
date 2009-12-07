
--sets options for the different os's
function configure_for_os()

	configuration("Debug")
		defines { "DEBUG", "_DEBUG" }
		flags {"Symbols", "ExtraWarnings"}
		
	configuration("Release")
		defines{ "NDEBUG", "RELEASE"}
		flags {"Optimize", "ExtraWarnings"}
				
	configuration { "windows" }
		defines{ "PLATFORM_CHECKED", "WINDOWS_BUILD" }
					
	configuration { "vs*" }
		defines{ "WIN32" }
		flags { "No64BitChecks"}
		links{"lua51"}
		
	configuration{"vs*","Debug"}
		buildoptions {"/Gm","/Zi"}

--[[		
	configuration { "vs2008" }
		buildoptions {"/analyze"}
--]]
			
	configuration{"vs*","Release"}
		buildoptions {"/GL"}

	configuration { "windows","codeblocks" }
		linkoptions { "lua" }
		buildoptions{ "-W -Wall -ansi -pedantic -std=c++98 -Wno-long-long" }
		
	configuration { "macosx" }
		defines { "PLATFORM_CHECKED", "MAC_BUILD" }
		buildoptions { "-W -Wall -ansi -pedantic -std=c++98 -Wno-long-long" }
		linkoptions{ "-llua" }
		
	configuration { "linux" }
		defines{ "PLATFORM_CHECKED" , "LINUX_BUILD" }
		buildoptions { "-W -Wall -ansi -pedantic -std=c++98 -Wno-long-long" }
		linkoptions{ "-llua" }
		
	configuration { "linux","codeblocks"  }
		defines{ "PLATFORM_CHECKED" , "LINUX_BUILD" }
		buildoptions { "-W -Wall -ansi -pedantic -std=c++98 -Wno-long-long" }
		linkoptions{ "-llua" }
		
	configuration("gmake")
		defines{ "PLATFORM_CHECKED" , "LINUX_BUILD" }
		buildoptions { "-W -Wall -ansi -pedantic -std=c++98 -Wno-long-long" }
		linkoptions{ "-llua" }

end

function create_package(name,path_to_root,kind_)
	local root = path_to_root or "./"
	local proj = project(name)
	proj.language ="C++"
  proj.kind = kind_ or "SharedLib"

	if(kind_ == "StaticLib" or kind_ =="SharedLib" )then
		configuration { "Debug" }
		targetname(proj.name.."_d")
	end

  configuration{"Debug"}
    targetdir(root.. "bin" .."/Debug")
 		objdir(root.. "obj" .."/Debug")

  configuration{"Release"}
    targetdir(root.. "bin"  .."/Release")
    objdir(root.. "obj" .."/Release")
    
	configure_for_os()

	includepaths = { root.."include" }
end