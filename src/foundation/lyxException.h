#ifndef LIBLYX_FOUNDATION_LYXEXCEPTION_H_
#define LIBLYX_FOUNDATION_LYXEXCEPTION_H_

#include <exception>
#include <string>

namespace lyx {

class Exception : public std::exception {
    public:
        Exception(const std::string& msg, int code = 0);
        Exception(const std::string& msg, const std::string& arg, int code = 0);
        Exception(const std::string& msg, const Exception& nested, int code = 0);
        Exception(const Exception& exc);
        ~Exception() throw();

        Exception& operator = (const Exception& exc);

        virtual const char* name() const throw();
        virtual const char* className() const throw();
        virtual const char* what() const throw();

        const Exception* nested() const;
        const std::string& message() const;
        int code() const;

        std::string displayText() const;

        virtual Exception* clone() const;
        virtual void rethrow() const;

    protected:
        Exception(int code = 0);
        void message(const std::string& msg);
        void extendedMessage(const std::string& arg);

    private:
        std::string _msg;
        Exception*  _pNested;
        int         _code;
};

inline const Exception* Exception::nested() const {
    return _pNested;
}

inline const std::string& Exception::message() const {
    return _msg;
}

inline void Exception::message(const std::string& msg) {
    _msg = msg;
}

inline int Exception::code() const {
    return _code;
}

#define LYX_DECLARE_EXCEPTION_CODE(CLS, BASE, CODE) \
    class CLS : public BASE {                                                       \
        public:                                                                     \
            CLS(int code = CODE);                                                   \
            CLS(const std::string& msg, int code = CODE);                           \
            CLS(const std::string& msg, const std::string& arg, int code = CODE);   \
            CLS(const std::string& msg, const Exception& exc, int code = CODE);     \
            CLS(const CLS& exc);                                                    \
            ~CLS() throw();                                                         \
            CLS& operator = (const CLS& exc);                                       \
            const char* name() const throw();                                       \
            const char* className() const throw();                                  \
            Exception* clone() const;                                               \
            void rethrow() const;                                                   \
    };

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXEXCEPTION_H_
