#include "process_number.hpp"

#include "check.hpp"


namespace json2lua {
	template<
	 typename CallbacksClass
	>
	pointer_pair<char32_t>
	process_number
	(
	 pointer_pair<char32_t> ends,
	 CallbacksClass & cbo
	)
	noexcept( traits<CallbacksClass>::noexcept_read && traits<CallbacksClass>::noexcept_write )
	{
		static_assert(check<CallbacksClass>());
		
		const unsigned char zero_comma[2] = { u8'0', u8'.' };
		
		unsigned char c;
		
		ends = ensure_input(ends, cbo);
		
		switch ( *ends.beg )
		{
			case u8'-':
			case u8'1':
			case u8'2':
			case u8'3':
			case u8'4':
			case u8'5':
			case u8'6':
			case u8'7':
			case u8'8':
			case u8'9':
				c = *ends.beg;
				cbo.write(&c, 1);
				++ends.beg;
				break;
			case u8'0':
				cbo.write(zero_comma, sizeof(zero_comma));
				++ends.beg;
				ends = ensure_input(ends, cbo);
				++ends.beg;
				break;
			case u8'.':
				cbo.write(zero_comma, sizeof(zero_comma));
				++ends.beg;
				break;
			default:
				return ends;
		}
		
again_inside:
		ends = ensure_input(ends, cbo);
		
		switch ( *ends.beg )
		{
			case u8'0':
			case u8'1':
			case u8'2':
			case u8'3':
			case u8'4':
			case u8'5':
			case u8'6':
			case u8'7':
			case u8'8':
			case u8'9':
				c = *ends.beg;
				cbo.write(&c, 1);
				++ends.beg;
				goto again_inside;
			case u8'.':
				cbo.write(u8'.');
				++ends.beg;
				goto again_fractional;
			case u8'e':
			case u8'E':
				cbo.write(u8'e');
				++ends.beg;
				goto again_exponent;
			default:
				return ends;
		}
		
again_fractional:
		ends = ensure_input(ends, cbo);
		
		switch ( *ends.beg )
		{
			case u8'0':
			case u8'1':
			case u8'2':
			case u8'3':
			case u8'4':
			case u8'5':
			case u8'6':
			case u8'7':
			case u8'8':
			case u8'9':
				c = *ends.beg;
				cbo.write(&c, 1);
				++ends.beg;
				goto again_inside;
			case u8'e':
			case u8'E':
				cbo.write(u8'e');
				++ends.beg;
				goto again_exponent;
			default:
				return ends;
		}
		
again_exponent:
		ends = ensure_input(ends, cbo);
		
		switch ( *ends.beg )
		{
			case u8'+':
				++ends.beg;
				goto again_exponent_inside;
			case u8'-':
				cbo.write(u8'-');
				++ends.beg;
				goto again_exponent_inside;
			case u8'0':
			case u8'1':
			case u8'2':
			case u8'3':
			case u8'4':
			case u8'5':
			case u8'6':
			case u8'7':
			case u8'8':
			case u8'9':
				c = *ends.beg;
				cbo.write(&c, 1);
				++ends.beg;
				goto again_exponent_inside;
			default:
				return ends;
		}
		
again_exponent_inside:
		ends = ensure_input(ends, cbo);
		
		switch ( *ends.beg )
		{
			case u8'0':
			case u8'1':
			case u8'2':
			case u8'3':
			case u8'4':
			case u8'5':
			case u8'6':
			case u8'7':
			case u8'8':
			case u8'9':
				c = *ends.beg;
				cbo.write(&c, 1);
				++ends.beg;
				goto again_exponent_inside;
			default:
				return ends;
		}
	}
}
