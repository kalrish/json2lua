if TESTING then
	generate_test_rules{
		"arrays",
		"boolean",
		"empty_array",
		"empty_object",
		"escape_sequences",
		"example",
		"numbers",
		"top_level-array",
		"top_level-boolean",
		"top_level-number",
		"top_level-object",
		"top_level-string",
		"unicode"
	}
	
	tup.definerule{
		inputs={
			"../<cmd>"
		},
		command="bash --norc --noprofile -- incomplete.bash %<cmd> incomplete.json incomplete.lua",
		outputs={
			"incomplete.lua"
		}
	}
	
	tup.definerule{
		inputs={
			"../<cmd>"
		},
		command="bash --norc --noprofile -- invalid_escape_sequence.bash %<cmd> invalid_escape_sequence.json invalid_escape_sequence.lua",
		outputs={
			"invalid_escape_sequence.lua"
		}
	}
end
