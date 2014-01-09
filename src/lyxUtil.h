#ifndef LYXUTIL_H
#define LYXUTIL_H

#include <string>

namespace lyx {
	class Util {
		public:
		static int writeToFile(const std::string &s, const std::string &filename);
	};
}

#endif // LYXUTIL_H


