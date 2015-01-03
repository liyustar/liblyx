#include "lyxSocket.h"
#include "lyxStreamSocketImpl.h"
#include <cstring>
#include <sys/epoll.h>

namespace lyx {

Socket::Socket():
    _pImpl(new StreamSocketImpl)
{
}

Socket::Socket(SocketImpl* pImpl) {
    _pImpl.reset(pImpl);
}

Socket::Socket(const Socket& socket):
    _pImpl(socket._pImpl)
{
}

Socket& Socket::operator = (const Socket& socket) {
    if (&socket != this) {
        _pImpl = socket._pImpl;
    }
    return *this;
}

Socket::~Socket() {
}

int Socket::select(SocketList& readList, SocketList& writeList, SocketList& exceptList, const Timespan& timeout) {
    int epollSize = readList.size() + writeList.size() + exceptList.size();
    if (epollSize == 0) return 0;

    int epollfd = -1;
    {
        struct epoll_event eventsIn[epollSize];
        std::memset(eventsIn, 0, sizeof(eventsIn));
        struct epoll_event* eventLast = eventsIn;

        for (SocketList::iterator it = readList.begin(); it != readList.end(); ++it) {
            int sockfd = it->sockfd();
            if (sockfd != -1) {
                struct epoll_event* e = eventsIn;
                for (; e != eventLast; ++e) {
                    if (reinterpret_cast<Socket*>(e->data.ptr)->sockfd() == sockfd)
                        break;
                }
                if (e == eventLast) {
                    e->data.ptr = &(*it);
                    ++eventLast;
                }
                e->events |= EPOLLIN;
            }
        }

        for (SocketList::iterator it = writeList.begin(); it != writeList.end(); ++it) {
            int sockfd = it->sockfd();
            if (sockfd != -1) {
                struct epoll_event* e = eventsIn;
                for (; e != eventLast; ++e) {
                    if (reinterpret_cast<Socket*>(e->data.ptr)->sockfd() == sockfd)
                        break;
                }
                if (e == eventLast) {
                    e->data.ptr = &(*it);
                    ++eventLast;
                }
                e->events |= EPOLLOUT;
            }
        }

        for (SocketList::iterator it = exceptList.begin(); it != exceptList.end(); ++it) {
            int sockfd = it->sockfd();
            if (sockfd != -1) {
                struct epoll_event* e = eventsIn;
                for (; e != eventLast; ++e) {
                    if (reinterpret_cast<Socket*>(e->data.ptr)->sockfd() == sockfd)
                        break;
                }
                if (e == eventLast) {
                    e->data.ptr = &(*it);
                    ++eventLast;
                }
                e->events |= EPOLLERR;
            }
        }

        epollSize = eventLast - eventsIn;
        epollfd = epoll_create(epollSize);
        if (epollfd < 0) {
            char buf[1024];
            strerror_r(errno, buf, sizeof(buf));
            SocketImpl::error(std::string("Can't create epoll queue: ") + buf);
        }

        for (struct epoll_event* e = eventsIn; e != eventLast; ++e) {
            int sockfd = reinterpret_cast<Socket*>(e->data.ptr)->sockfd();
            if (sockfd != -1) {
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, e) < 0) {
                    char buf[1024];
                    strerror_r(errno, buf, sizeof(buf));
                    ::close(epollfd);
                    SocketImpl::error(std::string("Can't insert socket to epoll queue: ") + buf);
                }
            }
        }
    }

    struct epoll_event eventsOut[epollSize];
    std::memset(eventsOut, 0, sizeof(eventsOut));

    Timespan remainingTime(timeout);
    int rc;
    do {
        Timestamp start;
        rc = epoll_wait(epollfd, eventsOut, epollSize, remainingTime.totalMilliseconds());
        if (rc < 0 && SocketImpl::lastError() == EINTR) {
            Timestamp end;
            Timespan waited = end - start;
            if (waited < remainingTime)
                remainingTime -= waited;
            else
                remainingTime = 0;
        }
    } while (rc < 0 && SocketImpl::lastError() == EINTR);

    ::close(epollfd);
    if (rc < 0) SocketImpl::error();

    SocketList readyReadList;
    SocketList readyWriteList;
    SocketList readyExceptList;
    for (int n = 0; n < rc; ++n) {
        if (eventsOut[n].events & EPOLLERR)
            readyExceptList.push_back(*reinterpret_cast<Socket*>(eventsOut[n].data.ptr));
        if (eventsOut[n].events & EPOLLIN)
            readyReadList.push_back(*reinterpret_cast<Socket*>(eventsOut[n].data.ptr));
        if (eventsOut[n].events & EPOLLOUT)
            readyWriteList.push_back(*reinterpret_cast<Socket*>(eventsOut[n].data.ptr));
    }
    std::swap(readList, readyReadList);
    std::swap(writeList, readyWriteList);
    std::swap(exceptList, readyExceptList);
    return readList.size() + writeList.size() + exceptList.size();
}

} // namespace lyx
