#include "process_true.hpp"

#include "check.hpp"
#include "seqcmp.hpp"


namespace json2lua {
	template<
	 typename CallbacksClass
	>
	pointer_pair<char32_t>
	process_true
	(
	 pointer_pair<char32_t> ends,
	 CallbacksClass & cbo
	)
	//noexcept( traits<CallbacksClass>::noexcept_read && traits<CallbacksClass>::noexcept_write )
	{
		static_assert(check<CallbacksClass>());
		
		const unsigned char true_str[] = { u8't', u8'r', u8'u', u8'e' };
		
		ends = seqcmp(ends, true_str, cbo);
		
		cbo.write(true_str, sizeof(true_str));
		
		return ends;
	}
}
