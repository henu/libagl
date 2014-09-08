#include "Zlib/Deflator.hpp"

#include <zlib.h>

namespace Agl
{

namespace Zlib
{

Deflator::Deflator(Level level)
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

	int zlevel;
	switch (level) {
	case NO_COMPRESSION:
		zlevel = Z_NO_COMPRESSION;
		break;
	case FAST:
		zlevel = Z_BEST_SPEED;
		break;
	case DEFAULT_COMPRESSION:
		zlevel = Z_DEFAULT_COMPRESSION;
		break;
	case BEST:
		zlevel = Z_BEST_COMPRESSION;
		break;
	}

	int err = deflateInit(z_streamp(zstrm), zlevel);
	if (err == Z_MEM_ERROR) {
		throw std::bad_alloc();
	}
	if (err == Z_STREAM_ERROR) {
		throw std::runtime_error("Invalid compression level!");
	}
	if (err == Z_VERSION_ERROR) {
		throw std::runtime_error("Invalid zlib version!");
	}
}

Deflator::~Deflator()
{
	deflateEnd(z_streamp(zstrm));
	delete z_streamp(zstrm);
}

void Deflator::newDataAvailable(uint64_t amount, bool end_of_data)
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
		int err = deflate(z_streamp(zstrm), end_of_data ? Z_FINISH : Z_NO_FLUSH);
		if (err == Z_STREAM_ERROR) {
			throw std::runtime_error("Stream error in zlib deflate()!");
		}
		if (err == Z_BUF_ERROR) {
			throw std::runtime_error("Buffer error in zlib deflate()!");
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
