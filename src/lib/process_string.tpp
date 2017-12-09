#include "process_string.hpp"

#include <cstdlib>
//#include <type_traits>
#include "check.hpp"


namespace json2lua {
	template<
	 typename CallbacksClass
	>
	void
	output_code_unit
	(
	 const unsigned char code_unit,
	 CallbacksClass & cbo
	)
	noexcept( traits<CallbacksClass>::noexcept_write_s )
	{
		unsigned char output[4] = { u8'\\', u8'0', u8'0', u8'0' };
		unsigned char * output_p = output + sizeof(output) - 1;
		std::div_t result;
		
		result.quot = code_unit;
		
		do {
			result = std::div(result.quot, 10);
			
			*output_p-- = result.rem + u8'0';
		} while ( result.quot > 0 );
		
		cbo.write(output, sizeof(output));
	}
	
	template<
	 typename CallbacksClass
	>
	void
	write_escaped_char
	(
	 const unsigned char code_point,
	 CallbacksClass & cbo
	)
	noexcept( traits<CallbacksClass>::noexcept_write_s )
	{
		const unsigned char output[] = { u8'\\' , code_point };
		
		cbo.write(output, sizeof(output));
	}
	
	template<
	 typename CallbacksClass
	>
	void
	output_code_point
	(
	 const char32_t code_point,
	 CallbacksClass & cbo
	)
	noexcept( traits<CallbacksClass>::noexcept_encode && traits<CallbacksClass>::noexcept_write_c && noexcept(output_code_unit(static_cast<unsigned char>(0), cbo)) )
	{
		auto ends = cbo.encode_string_code_point(code_point);
		
		do {
			const auto c = *ends.beg;
			if ( c >= 32 && c <= 126 )
			//if ( static_cast<std::make_unsigned_t<decltype(c)>>(c - 32) < 95 )
				cbo.write(c);
			else
				output_code_unit(c, cbo);
			
			++ends.beg;
		} while ( ends.beg != ends.end );
	}
	
	template<
	 typename CallbacksClass
	>
	pointer_pair<char32_t>
	output_hex
	(
	 pointer_pair<char32_t> ends,
	 CallbacksClass & cbo
	)
	noexcept( traits<CallbacksClass>::noexcept_read && noexcept(output_code_point(static_cast<char32_t>(0), cbo)) )
	{
		char str[5];
		
		ends = ensure_input(ends, cbo);
		
		str[0] = *ends.beg;
		
		++ends.beg;
		ends = ensure_input(ends, cbo);
		
		str[1] = *ends.beg;
		
		++ends.beg;
		ends = ensure_input(ends, cbo);
		
		str[2] = *ends.beg;
		
		++ends.beg;
		ends = ensure_input(ends, cbo);
		
		str[3] = *ends.beg;
		
		str[4] = 0;
		
		output_code_point(std::strtoull(str, nullptr, 16), cbo);
		
		return ends;
	}
	
	enum string_parsing_mode
	{
		literal,
		escaped
	};
	
	template<
	 typename CallbacksClass
	>
	pointer_pair<char32_t>
	process_string
	(
	 pointer_pair<char32_t> ends,
	 CallbacksClass & cbo
	)
	noexcept( traits<CallbacksClass>::noexcept_read && traits<CallbacksClass>::noexcept_write )
	{
		static_assert(check<CallbacksClass>());
		
		enum string_parsing_mode mode = literal;
		
		cbo.write(u8'"');
		
again:
		ends = ensure_input(ends, cbo);
		
		switch ( mode )
		{
			case literal:
				switch ( *ends.beg )
				{
					case u8'"':
						cbo.write(u8'"');
						++ends.beg;
						return ends;
					case u8'\\':
						mode = escaped;
						break;
					default:
						output_code_point(*ends.beg, cbo);
						break;
				}
				break;
			case escaped:
				switch ( *ends.beg )
				{
					case u8'/':
						cbo.write(u8'/');
						break;
					case u8'"':
					case u8'\\':
					case u8'b':
					case u8'f':
					case u8't':
					case u8'r':
					case u8'n':
						write_escaped_char(*ends.beg, cbo);
						break;
					case u8'u':
						++ends.beg;
						ends = output_hex(ends, cbo);
						break;
					default:
						invalid_escape_sequence e;
						e.escape = *ends.beg;
						throw e;
				}
				mode = literal;
				break;
		}
		
		++ends.beg;
		
		goto again;
	}
}
