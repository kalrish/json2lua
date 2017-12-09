stderr="$("$@" 2>&1)"
[[ $? = 1 && $stderr = *"incomplete JSON"* ]] && exit 0 || exit 1
