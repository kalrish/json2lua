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
		
		/*
		switch ( static_cast<int>(ends.beg == ends.end) )
		{
			case 1:
				return cbo.read();
			default:
				return ends;
		}
		*/
		
		/*
		switch ( ends.end - ends.beg )
		{
			case 0:
				return cbo.read();
			default:
				return ends;
		}
		*/
	}
	
	/*
	template<
	 typename CallbacksClass
	>
	pointer_pair<char32_t>
	ensure_input
	(
	 pointer_pair<char32_t> ends,
	 std::size_t n,
	 CallbacksClass & cbo
	)
	noexcept( traits<CallbacksClass>::noexcept_read )
	{
		static_assert(check<CallbacksClass>());
		
		ends = ensure_input(ends, cbo);
		
		do {
			++ends.beg;
			ends = ensure_input(ends, cbo);
			
			--n;
		} while ( n > 0 );
		
		return ends;
	}
	*/
}
