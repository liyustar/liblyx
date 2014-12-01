#include "lyxThread.h"
#include "lyxMutex.h"
#include "lyxException.h"
#include "lyxThreadLocal.h"
#include "lyxAtomicCounter.h"
#include <sstream>

namespace lyx {

Thread::Thread():
    _id(uniqueId()),
    _name(makeName()),
    _pTLS(0),
    _event(true),
    _mutex(true)
{
}

Thread::Thread(const std::string& name):
    _id(uniqueId()),
    _name(name),
    _pTLS(0),
    _event(true),
    _mutex(true)
{
}

Thread::~Thread() {
    delete _pTLS;
}

void Thread::setPriority(Priority prio) {
    setPriorityImpl(prio);
}

Thread::Priority Thread::getPriority() const {
    return Priority(getPriorityImpl());
}

void Thread::start(Runnable& target) {
    startImpl(target);
}

void Thread::start(Callable target, void* pData) {
    startImpl(target, pData);
}

void Thread::join() {
    joinImpl();
}

void Thread::join(long milliseconds) {
    if (!joinImpl(milliseconds))
        throw TimeoutException();
}

bool Thread::tryJoin(long milliseconds) {
    return joinImpl(milliseconds);
}

void Thread::wakeUp() {
    _event.set();
}

ThreadLocalStorage& Thread::tls() {
    if (!_pTLS)
        _pTLS = new ThreadLocalStorage;
    return *_pTLS;
}

void Thread::clearTLS() {
    if (_pTLS) {
        delete _pTLS;
        _pTLS = 0;
    }
}

std::string Thread::makeName() {
    std::ostringstream name;
    name << '#' << _id;
    return name.str();
}

int Thread::uniqueId() {
    static lyx::AtomicCounter counter;
    return ++counter;
}

void Thread::setName(const std::string& name) {
    Mutex::ScopedLock lock(_mutex);
    _name = name;
}

} // namespace lyx