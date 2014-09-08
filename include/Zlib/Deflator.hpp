#ifndef AGL_ZLIB_DEFLATOR_HPP
#define AGL_ZLIB_DEFLATOR_HPP

#include "../Stream.hpp"

namespace Agl
{

namespace Zlib
{

class Deflator : public Stream
{

public:

	enum Level {
		NO_COMPRESSION,
		FAST,
		DEFAULT_COMPRESSION,
		BEST
	};

	Deflator(Level level = DEFAULT_COMPRESSION);
	virtual ~Deflator();

private:

	void* zstrm;

	virtual void newDataAvailable(uint64_t amount, bool end_of_data);

};

}

}

#endif
