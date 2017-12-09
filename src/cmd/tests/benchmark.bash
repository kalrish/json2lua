print_inner_structure() {
	echo "\"$1\""
	echo ': {
        "title": "a simple string",
		"escape sequences": "a string enclosed in \" with many escape sequences, which start with a backslash \\, such as \b, \n, \f, \r, etc",
		"unicode": "el ñoño murciélago hindú comía álamos, die Straße heißt dadada",
		"boolean": {
			"true": true,
			"false": false
		},
		"numbers": [
			1,
			12,
			12e1,
			-7
		]
    }'
}

print_json() {
	echo '{'
	
	for (( i=0 ; i<10000 ; i++ )) ; do
		print_inner_structure $i
		echo ','
	done
	
	print_inner_structure end
	
	echo '}'
}

print_json | time ../json2lua.exe >/dev/null

exit $?
