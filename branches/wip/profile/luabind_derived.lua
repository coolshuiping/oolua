local N = 10
local ave = 0
local times = 1000000


for i = 0, N do

	local obj = ProfileBase()
	local increment_me = ProfileDerived()


	local t0 = os.clock()
	for i=1,times do
		obj:increment_a_base(increment_me)
	end
	local dt = os.clock()-t0
	if i~=0 then
	 ave = ave + dt
	end

end

print('Luabind passing derived to a function that wants a base  (average elapsed time):',ave/N)



ave =0
for i = 0, N do

	local obj = ProfileBase()
	local increment_me = ProfileMultiBases()


	local t0 = os.clock()
	for i=1,times do
		obj:increment_a_base(increment_me)
	end
	local dt = os.clock()-t0
	if i~=0 then
	 ave = ave + dt
	end

end

print('Luabind passing derived with multiple bases to a function that wants a base (average elapsed time):',ave/N)
