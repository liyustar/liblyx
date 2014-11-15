#include "lyxException.h"
#include <typeinfo>

namespace lyx {

Exception::Exception(int code) :
    _pNested(0),
    _code(code)
{
}

Exception::Exception(const std::string& msg, int code) :
    _msg(msg),
    _pNested(0),
    _code(code)
{
}

Exception::Exception(const std::string& msg, const std::string& arg, int code) :
    _msg(msg),
    _pNested(0),
    _code(code)
{
    if (!arg.empty()) {
        _msg.append(": ");
        _msg.append(arg);
    }
}

Exception::Exception(const std::string& msg, const Exception& nested, int code) :
    _msg(msg),
    _pNested(nested.clone()),
    _code(code)
{
}

Exception::Exception(const Exception& exc) :
    std::exception(exc),
    _msg(exc._msg),
    _code(exc._code)
{
    _pNested = exc._pNested ? exc._pNested->clone() : 0;
}

Exception::~Exception() throw() {
    delete _pNested;
}

Exception& Exception::operator = (const Exception& exc) {
    if (&exc != this) {
        Exception* newPNested = exc._pNested ? exc._pNested->clone() : 0;
        delete _pNested;
        _msg = exc._msg;
        _pNested = newPNested;
        _code = exc._code;
    }
    return *this;
}

const char* Exception::name() const throw() {
    return "Exception";
}

const char* Exception::className() const throw() {
    return typeid(*this).name();
}

const char* Exception::what() const throw() {
    return name();
}

std::string Exception::displayText() const {
    std::string txt = name();
    if (!_msg.empty()) {
        txt.append(": ");
        txt.append(_msg);
    }
    return txt;
}

void Exception::extendedMessage(const std::string& arg) {
    if (!arg.empty()) {
        if (!_msg.empty()) {
            _msg.append(": ");
        }
        _msg.append(arg);
    }
}

Exception* Exception::clone() const {
    return new Exception(*this);
}

void Exception::rethrow() const {
    throw *this;
}

LYX_IMPLEMENT_EXCEPTION(LogicException, Exception, "Logic exception")
LYX_IMPLEMENT_EXCEPTION(AssertionViolationException, LogicException, "Assertion violation")
LYX_IMPLEMENT_EXCEPTION(NullPointerException, LogicException, "Null pointer")
LYX_IMPLEMENT_EXCEPTION(NullValueException, LogicException, "Null value")
LYX_IMPLEMENT_EXCEPTION(BugcheckException, LogicException, "Bugcheck")
LYX_IMPLEMENT_EXCEPTION(InvalidArgumentException, LogicException, "Invalid argument")
LYX_IMPLEMENT_EXCEPTION(NotImplementedException, LogicException, "Not implemented")
LYX_IMPLEMENT_EXCEPTION(RangeException, LogicException, "Out of range")
LYX_IMPLEMENT_EXCEPTION(IllegalStateException, LogicException, "Illegal state")
LYX_IMPLEMENT_EXCEPTION(InvalidAccessException, LogicException, "Invalid access")
LYX_IMPLEMENT_EXCEPTION(SignalException, LogicException, "Signal received")
LYX_IMPLEMENT_EXCEPTION(UnhandledException, LogicException, "Unhandled exception")

LYX_IMPLEMENT_EXCEPTION(RuntimeException, Exception, "Runtime exception")
LYX_IMPLEMENT_EXCEPTION(NotFoundException, RuntimeException, "Not found")
LYX_IMPLEMENT_EXCEPTION(ExistsException, RuntimeException, "Exists")
LYX_IMPLEMENT_EXCEPTION(TimeoutException, RuntimeException, "Timeout")
LYX_IMPLEMENT_EXCEPTION(SystemException, RuntimeException, "System exception")
LYX_IMPLEMENT_EXCEPTION(RegularExpressionException, RuntimeException, "Error in regular expression")
LYX_IMPLEMENT_EXCEPTION(LibraryLoadException, RuntimeException, "Cannot load library")
LYX_IMPLEMENT_EXCEPTION(LibraryAlreadyLoadedException, RuntimeException, "Library already loaded")
LYX_IMPLEMENT_EXCEPTION(NoThreadAvailableException, RuntimeException, "No thread available")
LYX_IMPLEMENT_EXCEPTION(PropertyNotSupportedException, RuntimeException, "Property not supported")
LYX_IMPLEMENT_EXCEPTION(PoolOverflowException, RuntimeException, "Pool overflow")
LYX_IMPLEMENT_EXCEPTION(NoPermissionException, RuntimeException, "No permission")
LYX_IMPLEMENT_EXCEPTION(OutOfMemoryException, RuntimeException, "Out of memory")
LYX_IMPLEMENT_EXCEPTION(DataException, RuntimeException, "Data error")

LYX_IMPLEMENT_EXCEPTION(DataFormatException, DataException, "Bad data format")
LYX_IMPLEMENT_EXCEPTION(SyntaxException, DataException, "Syntax error")
LYX_IMPLEMENT_EXCEPTION(CircularReferenceException, DataException, "Circular reference")
LYX_IMPLEMENT_EXCEPTION(PathSyntaxException, SyntaxException, "Bad path syntax")
LYX_IMPLEMENT_EXCEPTION(IOException, RuntimeException, "I/O error")
LYX_IMPLEMENT_EXCEPTION(ProtocolException, IOException, "Protocol error")
LYX_IMPLEMENT_EXCEPTION(FileException, IOException, "File access error")
LYX_IMPLEMENT_EXCEPTION(FileExistsException, FileException, "File exists")
LYX_IMPLEMENT_EXCEPTION(FileNotFoundException, FileException, "File not found")
LYX_IMPLEMENT_EXCEPTION(PathNotFoundException, FileException, "Path not found")
LYX_IMPLEMENT_EXCEPTION(FileReadOnlyException, FileException, "File is read-only")
LYX_IMPLEMENT_EXCEPTION(FileAccessDeniedException, FileException, "Access to file denied")
LYX_IMPLEMENT_EXCEPTION(CreateFileException, FileException, "Cannot create file")
LYX_IMPLEMENT_EXCEPTION(OpenFileException, FileException, "Cannot open file")
LYX_IMPLEMENT_EXCEPTION(WriteFileException, FileException, "Cannot write file")
LYX_IMPLEMENT_EXCEPTION(ReadFileException, FileException, "Cannot read file")
LYX_IMPLEMENT_EXCEPTION(UnknownURISchemeException, RuntimeException, "Unknown URI scheme")


LYX_IMPLEMENT_EXCEPTION(ApplicationException, Exception, "Application exception")
LYX_IMPLEMENT_EXCEPTION(BadCastException, RuntimeException, "Bad cast exception")


} // namespace lyx
