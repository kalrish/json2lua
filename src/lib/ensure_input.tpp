#include "ensure_input.hpp"

#include "check.hpp"


namespace json2lua {
	template<
	 typename CallbacksClass
	>
	pointer_pair<char32_t>
	ensure_input
	(
	 const pointer_pair<char32_t> ends,
	 CallbacksClass & cbo
	)
	noexcept( traits<CallbacksClass>::noexcept_read )
	{
		static_assert(check<CallbacksClass>());
		
		return ends.beg == ends.end ? cbo.read() : ends;
	}
}
