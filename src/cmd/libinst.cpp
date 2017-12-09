#include "config.hpp"

#include "cbc.hpp"

#include "../lib/consume_whitespace.tpp"
#include "../lib/ensure_input.tpp"
#include "../lib/process_array.tpp"
#include "../lib/process_false.tpp"
#include "../lib/process_null.tpp"
#include "../lib/process_number.tpp"
#include "../lib/process_object.tpp"
#include "../lib/process_string.tpp"
#include "../lib/process_true.tpp"
#include "../lib/process_value.tpp"
#include "../lib/seqcmp.tpp"


template json2lua::pointer_pair<char32_t>
json2lua::process_value<cbc>
(
 json2lua::pointer_pair<char32_t>,
 cbc &
);
