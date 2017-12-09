#include "consume_whitespace.hpp"

#include "ensure_input.hpp"


namespace json2lua {
	template<
	 typename CallbacksClass
	>
	pointer_pair<char32_t>
	consume_whitespace
	(
	 pointer_pair<char32_t> ends,
	 CallbacksClass & cbo
	)
	noexcept( traits<CallbacksClass>::noexcept_read )
	{
		static_assert(check<CallbacksClass>());
		
again:
		ends = ensure_input(ends, cbo);
		
		switch ( *ends.beg )
		{
			case u8' ':
			case u8'\t':
			case u8'\r':
			case u8'\n':
				++ends.beg;
				goto again;
			default:
				return ends;
		}
	}
}
