#include "process_array.hpp"

#include "check.hpp"
#include "consume_whitespace.hpp"
#include "process_value.hpp"
#include "unexpected_token.hpp"


namespace json2lua {
	template<
	 typename CallbacksClass
	>
	pointer_pair<char32_t>
	process_array
	(
	 pointer_pair<char32_t> ends,
	 CallbacksClass & cbo
	)
	//noexcept( traits<CallbacksClass>::noexcept_read && traits<CallbacksClass>::noexcept_write )
	{
		static_assert(check<CallbacksClass>());
		
		cbo.write(u8'{');
		
		ends = consume_whitespace(ends, cbo);
		
		if ( *ends.beg == u8']' )
		{
			cbo.write(u8'}');
			
			++ends.beg;
			
			ends = ensure_input(ends, cbo);
			
			return ends;
		}
		else
		{
again:
			ends = process_value(ends, cbo);
			
			ends = consume_whitespace(ends, cbo);
			
			switch ( *ends.beg )
			{
				case u8',':
					cbo.write(u8',');
					++ends.beg;
					goto again;
				case u8']':
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
