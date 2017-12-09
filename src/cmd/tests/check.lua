local stderr = io.stderr
local log_error = function(...)
	stderr:write(...)
	stderr:write("\n")
end

local t1
do
	local file, error_message = io.open(arg[1] .. ".json", "rb")
	if file then
		local contents = file:read("*a")
		
		file:close()
		
		if contents then
			local pos
			t1, pos, error_message = require("check.dkjson").decode(contents, 1, nil)
			if error_message then
				log_error("couldn't parse JSON: ", error_message)
				t1 = nil
			end
		else
			log_error("couldn't read input")
			os.exit(false)
		end
	else
		if error_message then
			log_error("couldn't open input: ", error_message)
		else
			log_error("couldn't open input; no reason reported")
		end
		os.exit(false)
	end
end

local t2
do
	local error_message
	t2, error_message = loadfile(arg[1] .. ".lua", "t", {})
	if t2 then
		t2 = t2()
	else
		log_error("couldn't load Lua code: ", error_message)
	end
end

local compare
do
	local pairs = pairs
	local type = type
	
	local stderr = io.stderr
	local log_error = function(...)
		stderr:write(...)
		stderr:write("\n")
	end
	
	local tables_equal_basic = function(t1, t2)
		local equal = true
		
		for k, v1 in pairs(t1) do
			local v2 = t2[k]
			
			if v2 then
				if not compare(v1, v2) then
					equal = false
				end
				
				--[[
				if type(v1) == type(v2) then
					if type(v1) == "table" then
						if not tables_equal(v1, v2) then
							log_error("key ", k, ": tables don't match")
							
							equal = false
						end
					else
						if v1 ~= v2 then
							log_error("key ", k, ": 1=", v1, " , 2=", v2)
							
							equal = false
						end
					end
				else
					log_error("type mismatch for key ", k)
					
					equal = false
				end
				]]
			elseif type(v1) == "boolean" then
				-- our JSON library (dkjson) sets JSON false values to Lua nil
				if v2 ~= false then
					equal = false
				end
			else
				log_error("left side is missing key ", k)
				
				equal = false
			end
		end
		
		return equal
	end
	
	tables_equal = function(t1, t2)
		return tables_equal_basic(t1, t2) and tables_equal_basic(t2, t1)
	end
	
	local default_comparator = function(v1, v2)
		return v1 == v2
	end
	
	local comparators = {
		["boolean"] = default_comparator,
		["number"] = default_comparator,
		["string"] = default_comparator,
		["table"] = tables_equal
	}
	
	compare = function(v1, v2)
		local t = type(v1)
		if t == type(v2) then
			return comparators[t](v1, v2)
		else
			return false
		end
	end
end

if t1 and t2 then
	if not compare(t1, t2) then
		log_error("tables don't match")
		
		os.exit(false)
	end
else
	os.exit(false)
end
