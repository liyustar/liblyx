#include "lyxUtil.h"

#include <fstream>

using namespace std;

namespace lyx {
	int Util::writeToFile(const string &s, const string &filename) {
		ofstream fout(filename.c_str(), ios::out);
		fout << s;
		fout.flush();
		fout.close();
		return 0;
	}
}

#ifdef LYXUTIL_TEST_

int main() {
	string s("xxx");
	lyx::Util::writeToFile(s, "t.tt");
	return 0;
}

#endif
