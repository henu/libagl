#include "Zlib/Inflator.hpp"

#include <zlib.h>

namespace Agl
{

namespace Zlib
{

Inflator::Inflator()
{
	zstrm = new z_stream;
	// Tune allocation of zstream
	z_streamp(zstrm)->zalloc = Z_NULL;
	z_streamp(zstrm)->zfree = Z_NULL;
	// Disable buffers at first
	z_streamp(zstrm)->next_in = Z_NULL;
	z_streamp(zstrm)->avail_in = 0;
	z_streamp(zstrm)->next_out = Z_NULL;
	z_streamp(zstrm)->avail_out = 0;

	int err = inflateInit(z_streamp(zstrm));
	if (err == Z_MEM_ERROR) {
		throw std::bad_alloc();
	}
	if (err == Z_VERSION_ERROR) {
		throw std::runtime_error("Invalid zlib version!");
	}
}

Inflator::~Inflator()
{
	inflateEnd(z_streamp(zstrm));
	delete z_streamp(zstrm);
}

void Inflator::newDataAvailable(uint64_t amount, bool end_of_data)
{
	(void)amount;

	Bytes bytes;
	readInputData(bytes);

	size_t OUTPUT_BUF_SIZE = 16 * 1024;
	uint8_t output_buf[OUTPUT_BUF_SIZE];

	z_streamp(zstrm)->next_in = &bytes[0];
	z_streamp(zstrm)->avail_in = bytes.size();
	z_streamp(zstrm)->next_out = output_buf;
	z_streamp(zstrm)->avail_out = OUTPUT_BUF_SIZE;

	while (true) {
		int err = inflate(z_streamp(zstrm), end_of_data ? Z_FINISH : Z_NO_FLUSH);
		if (err == Z_DATA_ERROR) {
			throw std::runtime_error("Corrupted data!");
		}
		if (err == Z_STREAM_ERROR) {
			throw std::runtime_error("Stream error in zlib inflate()!");
		}
		if (err == Z_BUF_ERROR) {
			throw std::runtime_error("Buffer error in zlib inflate()!");
		}

		// Read everything from output buffer
		writeOutputData(output_buf, z_streamp(zstrm)->next_out);

		if ((!end_of_data && z_streamp(zstrm)->avail_in == 0) ||
		    (end_of_data && err == Z_STREAM_END)) {
			break;
		}

		// Reset output buffer
		z_streamp(zstrm)->next_out = output_buf;
		z_streamp(zstrm)->avail_out = OUTPUT_BUF_SIZE;
	}

}

}

}
