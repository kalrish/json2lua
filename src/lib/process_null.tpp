#include "process_null.hpp"

#include "check.hpp"
#include "seqcmp.hpp"


namespace json2lua {
	template<
	 typename CallbacksClass
	>
	pointer_pair<char32_t>
	process_null
	(
	 pointer_pair<char32_t> ends,
	 CallbacksClass & cbo
	)
	//noexcept( traits<CallbacksClass>::noexcept_read && traits<CallbacksClass>::noexcept_write )
	{
		static_assert(check<CallbacksClass>());
		
		const unsigned char null_str[] = { u8'n', u8'u', u8'l', u8'l' };
		const unsigned char nil_str[] = { u8'n', u8'i', u8'l' };
		
		ends = seqcmp(ends, null_str, cbo);
		
		cbo.write(nil_str, sizeof(nil_str));
		
		return ends;
	}
}
