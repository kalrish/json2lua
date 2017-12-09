#include "process_object.hpp"

#include "check.hpp"
#include "consume_whitespace.hpp"
#include "ensure_input.hpp"
#include "process_string.hpp"
#include "process_value.hpp"
#include "unexpected_token.hpp"


namespace json2lua {
	template<
	 typename CallbacksClass
	>
	pointer_pair<char32_t>
	process_object
	(
	 pointer_pair<char32_t> ends,
	 CallbacksClass & cbo
	)
	//noexcept( traits<CallbacksClass>::noexcept_read && traits<CallbacksClass>::noexcept_write )
	{
		static_assert(check<CallbacksClass>());
		
		const unsigned char bracket_close_and_equal_sign[] = { u8']', u8'=' };
		
		cbo.write(u8'{');
		
		ends = consume_whitespace(ends, cbo);
		
		if ( *ends.beg == u8'}' )
		{
			cbo.write(u8'}');
			
			++ends.beg;
			
			return ends;
		}
		else
		{
again:
			if ( *ends.beg != u8'"' )
			{
				unexpected_token e;
				e.token = *ends.beg;
				throw e;
			}
			
			++ends.beg;
			ends = ensure_input(ends, cbo);
			
			cbo.write(u8'[');
			
			ends = process_string(ends, cbo);
			
			cbo.write(bracket_close_and_equal_sign, sizeof(bracket_close_and_equal_sign));
			
			ends = consume_whitespace(ends, cbo);
			
			if ( *ends.beg != u8':' )
			{
				unexpected_token e;
				e.token = *ends.beg;
				throw e;
			}
			
			++ends.beg;
			
			ends = process_value(ends, cbo);
			
			ends = consume_whitespace(ends, cbo);
			
			switch ( *ends.beg )
			{
				case u8',':
					cbo.write(u8',');
					++ends.beg;
					ends = consume_whitespace(ends, cbo);
					goto again;
				case u8'}':
					cbo.write(u8'}');
					++ends.beg;
					return ends;
				default:
					{
						unexpected_token e;
						e.token = *ends.beg;
						throw e;
					}
			}
		}
	}
}
