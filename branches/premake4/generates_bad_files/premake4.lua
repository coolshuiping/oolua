--oolua premake4 file

dofile("helper4.lua")

if _ACTION == "clean" then
  os.rmdir("obj")
  os.rmdir("bin")
end



solution("oolua")
   configurations { "Debug", "Release" }


dofile("./oolua4.lua")
dofile("./test.unit4.lua")
