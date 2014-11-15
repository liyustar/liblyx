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

#define LYX_DECLARE_EXCEPTION(CLS, BASE) \
    LYX_DECLARE_EXCEPTION_CODE(CLS, BASE, 0)

#define LYX_IMPLEMENT_EXCEPTION(CLS, BASE, NAME) \
    CLS::CLS(int code) : BASE(code) {}                                                              \
    CLS::CLS(const std::string& msg, int code) : BASE(msg, code) {}                                 \
    CLS::CLS(const std::string& msg, const std::string& arg, int code) : BASE(msg, arg, code) {}    \
    CLS::CLS(const std::string& msg, const Exception& exc, int code) : BASE(msg, exc, code) {}      \
    CLS::CLS(const CLS& exc) : BASE(exc) {}                                                         \
    CLS::~CLS() throw() {}                                                                          \
    CLS& CLS::operator = (const CLS& exc) {                                                         \
        BASE::operator = (exc);                                                                     \
        return *this;                                                                               \
    }                                                                                               \
    const char* CLS::name() const throw() { return NAME; }                                          \
    const char* CLS::className() const throw() { return typeid(*this).name(); }                     \
    Exception* CLS::clone() const { return new CLS(*this); }                                        \
    void CLS::rethrow() const { throw * this; }

// 具体Exception类声明
LYX_DECLARE_EXCEPTION(LogicException, Exception)
LYX_DECLARE_EXCEPTION(AssertionViolationException, LogicException)
LYX_DECLARE_EXCEPTION(NullPointerException, LogicException)     // 空指针
LYX_DECLARE_EXCEPTION(NullValueException, LogicException)
LYX_DECLARE_EXCEPTION(BugcheckException, LogicException)
LYX_DECLARE_EXCEPTION(InvalidArgumentException, LogicException) // 无效参数
LYX_DECLARE_EXCEPTION(NotImplementedException, LogicException)  // 未实现
LYX_DECLARE_EXCEPTION(RangeException, LogicException)           // 边界出错
LYX_DECLARE_EXCEPTION(IllegalStateException, LogicException)    // 状态非法
LYX_DECLARE_EXCEPTION(InvalidAccessException, LogicException)
LYX_DECLARE_EXCEPTION(SignalException, LogicException)
LYX_DECLARE_EXCEPTION(UnhandledException, LogicException)

LYX_DECLARE_EXCEPTION(RuntimeException, Exception)
LYX_DECLARE_EXCEPTION(NotFoundException, RuntimeException)
LYX_DECLARE_EXCEPTION(ExistsException, RuntimeException)
LYX_DECLARE_EXCEPTION(TimeoutException, RuntimeException)
LYX_DECLARE_EXCEPTION(SystemException, RuntimeException)
LYX_DECLARE_EXCEPTION(RegularExpressionException, RuntimeException)
LYX_DECLARE_EXCEPTION(LibraryLoadException, RuntimeException)
LYX_DECLARE_EXCEPTION(LibraryAlreadyLoadedException, RuntimeException)
LYX_DECLARE_EXCEPTION(NoThreadAvailableException, RuntimeException)
LYX_DECLARE_EXCEPTION(PropertyNotSupportedException, RuntimeException)
LYX_DECLARE_EXCEPTION(PoolOverflowException, RuntimeException)
LYX_DECLARE_EXCEPTION(NoPermissionException, RuntimeException)
LYX_DECLARE_EXCEPTION(OutOfMemoryException, RuntimeException)
LYX_DECLARE_EXCEPTION(DataException, RuntimeException)

LYX_DECLARE_EXCEPTION(DataFormatException, DataException)
LYX_DECLARE_EXCEPTION(SyntaxException, DataException)
LYX_DECLARE_EXCEPTION(CircularReferenceException, DataException)
LYX_DECLARE_EXCEPTION(PathSyntaxException, SyntaxException)
LYX_DECLARE_EXCEPTION(IOException, RuntimeException)
LYX_DECLARE_EXCEPTION(ProtocolException, IOException)
LYX_DECLARE_EXCEPTION(FileException, IOException)
LYX_DECLARE_EXCEPTION(FileExistsException, FileException)
LYX_DECLARE_EXCEPTION(FileNotFoundException, FileException)
LYX_DECLARE_EXCEPTION(PathNotFoundException, FileException)
LYX_DECLARE_EXCEPTION(FileReadOnlyException, FileException)
LYX_DECLARE_EXCEPTION(FileAccessDeniedException, FileException)
LYX_DECLARE_EXCEPTION(CreateFileException, FileException)
LYX_DECLARE_EXCEPTION(OpenFileException, FileException)
LYX_DECLARE_EXCEPTION(WriteFileException, FileException)
LYX_DECLARE_EXCEPTION(ReadFileException, FileException)
LYX_DECLARE_EXCEPTION(UnknownURISchemeException, RuntimeException)

LYX_DECLARE_EXCEPTION(ApplicationException, Exception)
LYX_DECLARE_EXCEPTION(BadCastException, RuntimeException)

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXEXCEPTION_H_
