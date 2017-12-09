stderr="$("$@" 2>&1)"
[[ $? = 1 && $stderr = *"invalid escape sequence"* ]] && exit 0 || exit 1
