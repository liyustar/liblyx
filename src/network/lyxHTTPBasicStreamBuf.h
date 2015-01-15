#ifndef LIBLYX_NETWORK_LYXHTTPBASICSTREAMBUF_H_
#define LIBLYX_NETWORK_LYXHTTPBASICSTREAMBUF_H_

#include "lyxBufferedStreamBuf.h"
#include "lyxHTTPBufferAllocator.h"

namespace lyx {

typedef lyx::BasicBufferedStreamBuf<char, std::char_traits<char>, HTTPBufferAllocator> HTTPBasicStreamBuf;

} // namespace lyx

#endif // LIBLYX_NETWORK_LYXHTTPBASICSTREAMBUF_H_
