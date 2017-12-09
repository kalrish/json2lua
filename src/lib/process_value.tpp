#include "process_value.hpp"

#include "check.hpp"
#include "consume_whitespace.hpp"
#include "process_array.hpp"
#include "process_false.hpp"
#include "process_null.hpp"
#include "process_number.hpp"
#include "process_object.hpp"
#include "process_string.hpp"
#include "process_true.hpp"
#include "unexpected_token.hpp"


namespace json2lua {
	template<
	 typename CallbacksClass
	>
	pointer_pair<char32_t>
	process_value
	(
	 pointer_pair<char32_t> ends,
	 CallbacksClass & cbo
	)
	//noexcept( traits<CallbacksClass>::noexcept_read && traits<CallbacksClass>::noexcept_write )
	{
		static_assert(check<CallbacksClass>());
		
		ends = consume_whitespace(ends, cbo);
		
		switch ( *ends.beg )
		{
			case u8'{': // object
				++ends.beg;
				return process_object(ends, cbo);
			case u8'[': // array
				++ends.beg;
				return process_array(ends, cbo);
			case u8'"': // string
				++ends.beg;
				return process_string(ends, cbo);
			case u8'-':
			case u8'0':
			case u8'1':
			case u8'2':
			case u8'3':
			case u8'4':
			case u8'5':
			case u8'6':
			case u8'7':
			case u8'8':
			case u8'9':
			case u8'.':
				return process_number(ends, cbo);
			case u8't': // true  - boolean
				return process_true(ends, cbo);
			case u8'f': // false - boolean
				return process_false(ends, cbo);
			case u8'n': // null
				return process_null(ends, cbo);
			default:
				{
					unexpected_token e;
					e.token = *ends.beg;
					throw e;
				}
		}
	}
}
