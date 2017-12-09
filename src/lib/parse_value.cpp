#include "pointer_pair.hpp"

struct cbc final
{
	json2lua_pointer_pair_unicode (* reader)
		(
		 void *
		);
	void * reader_argument;
	int (* writer)
		(
		 const unsigned char *,
		 std::size_t,
		 void *
		);
	void * writer_argument;
	
	json::pointer_pair<char32_t>
		read
		( );
};

json::pointer_pair<char32_t>
cbc::read
( )
{
	const auto ends = this->reader(this->reader_argument);
	return ends.beg ? ends : throw 1;
}

void
cbc::write
(
 const unsigned char c
)
{
	const auto rv = this->writer(&c, 1, this->writer_argument);
	if ( !rv )
	{
		throw 2;
	}
}

extern "C"
json2lua_pointer_pair_unicode
json2lua_parse_value
(
 json2lua_pointer_pair_unicode ends,
 json2lua_pointer_pair_unicode (* const reader)
 (
  void *
 ),
 void * const reader_argument,
 int (* const writer)
 (
  const unsigned char *,
  std::size_t,
  void *
 ),
 void * const writer_argument
)
{
	cbc cbo;
	cbo.reader = reader;
	cbo.reader_argument = reader_argument;
	cbo.writer = writer;
	cbo.writer_argument = writer_argument;
	
	try {
		json2lua::process_value(ends, cbo);
	}
	catch ( ... )
	{
	}
}
