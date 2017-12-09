#include <cstddef>
#include <type_traits>
#include <utility>


namespace json2lua {
	template<
	 typename T
	>
	constexpr bool
	check
	( )
	noexcept
	{
		static_assert( std::is_class_v<T> );
		static_assert( std::is_invocable_r_v<pointer_pair<char32_t>, decltype(&T::read), T *> );
		static_assert( std::is_same_v< decltype( std::declval<T>().write(static_cast<unsigned char>(0)) ) , void > );
		static_assert( std::is_same_v< decltype( std::declval<T>().write(static_cast<const unsigned char *>(nullptr), static_cast<std::size_t>(0)) ) , void > );
		static_assert( std::is_invocable_r_v<pointer_pair<unsigned char>, decltype(&T::encode_string_code_point), T *, char32_t> );
		
		return true;
	}
}
