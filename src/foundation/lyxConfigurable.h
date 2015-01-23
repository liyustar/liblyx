#ifndef LIBLYX_FOUNDATION_LYXCONFIGURABLE_H_
#define LIBLYX_FOUNDATION_LYXCONFIGURABLE_H_

#include <string>

namespace lyx {

class Configurable {
    public:
        Configurable();
        virtual ~Configurable();
        virtual void setProperty(const std::string& nane, const std::string& value) = 0;
        virtual std::string getProperty(const std::string& name) const = 0;
};

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXCONFIGURABLE_H_
