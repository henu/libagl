#ifndef AGL_ZLIB_INFLATOR_HPP
#define AGL_ZLIB_INFLATOR_HPP

#include "../Stream.hpp"

namespace Agl
{

namespace Zlib
{

class Inflator : public Stream
{

public:

	Inflator();
	virtual ~Inflator();

private:

	void* zstrm;

	virtual void newDataAvailable(uint64_t amount, bool end_of_data);

};

}

}

#endif
