#pragma once

namespace lyx {

    class INoCopy {
        private:
            INoCopy(const INoCopy&);
            INoCopy& operator=(const INoCopy&);
    };

}
