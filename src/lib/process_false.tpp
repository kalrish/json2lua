#include "process_false.hpp"

#include "check.hpp"
#include "seqcmp.hpp"


namespace json2lua {
	template<
	 typename CallbacksClass
	>
	pointer_pair<char32_t>
	process_false
	(
	 pointer_pair<char32_t> ends,
	 CallbacksClass & cbo
	)
	//noexcept( traits<CallbacksClass>::noexcept_read && traits<CallbacksClass>::noexcept_write )
	{
		static_assert(check<CallbacksClass>());
		
		const unsigned char false_str[] = { u8'f', u8'a', u8'l', u8's', u8'e' };
		
		ends = seqcmp(ends, false_str, cbo);
		
		cbo.write(false_str, sizeof(false_str));
		
		return ends;
	}
}
