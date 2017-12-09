#include <cstddef>


const char32_t * decode_utf8
(
 const unsigned char *,
 std::size_t,
 char32_t *
) noexcept;

std::size_t encode_utf8
(
 char32_t,
 unsigned char[4]
) noexcept;
