#include "seqcmp.hpp"

#include "check.hpp"
#include "unexpected_token.hpp"


namespace json2lua {
	template<
	 typename CallbacksClass,
	 std::size_t N
	>
	pointer_pair<char32_t>
	seqcmp
	(
	 pointer_pair<char32_t> ends,
	 const unsigned char (& str)[N],
	 CallbacksClass & cbo
	)
	//noexcept( noexcept(ensure_input(ends, cbo)) )
	{
		static_assert(check<CallbacksClass>());
		
		for ( const auto c : str )
		{
			if ( *ends.beg != c )
			{
				unexpected_token e;
				e.token = *ends.beg;
				throw e;
			}
			
			++ends.beg;
			ends = ensure_input(ends, cbo);
		}
		
		return ends;
	}
}
