local TARGET = tup.getconfig("TARGET")
TESTING = TARGET == "" or TARGET == tup.getconfig("TUP_PLATFORM")

generate_test_rules = function(tests)
	local i = 1
	local test = tests[1]
	repeat
		local lua_output = test .. ".lua"
		
		tup.definerule{
			inputs={
				"../<cmd>"
			},
			command="%<cmd> " .. test .. ".json " .. lua_output,
			outputs={
				lua_output
			}
		}
		
		tup.definerule{
			inputs={
				lua_output
			},
			command="lua -- check.lua " .. test
		}
		
		i = i + 1
		test = tests[i]
	until test == nil
end
