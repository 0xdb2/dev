static char BaseSock_C [] = "@(#) db $Header: http://belov.homenet.org/svn/trunk/base/sock/BaseSock.C 269 2014-04-18 10:46:44Z db $";

#include <netdb.h>
#include <fcntl.h>
#include <signal.h>

#include <BaseSock.H>

EXCDEF(socket_shutdown);

int BaseSockSrv::timeout = 0;
#ifdef PTHREAD
    list<BaseSockSrv> BaseSockSrv::executed;
#endif

ostream& operator<< (ostream& os, const sockaddr_in& sa)
{
    os << inet_ntoa(sa.sin_addr) << ':' << ntohs(sa.sin_port);
    return os;
}

//============= BaseSock members ==========
BaseSock::BaseSock() noexcept
{
    clear();
}

BaseSock::~BaseSock()
{
    if (valid())
    {
        ::shutdown(SockFd, 2);
        ::close(SockFd);
        SockFlag = (SockFlag & ~connected);
        cout << "BaseSock::~BaseSock: socket " << *this << " was closed" << endl;
        //clear();
    }
}

void BaseSock::close()
{
    if (SockFlag & connected)
    {
        ::shutdown(SockFd, 2);
        SockFlag = (SockFlag & ~connected);
    }
    ::close(SockFd);
    cout << "BaseSock::close: socket " << *this << " was closed" << endl;
    clear();
}

bool BaseSock::create()
{
    SockFd = ::socket(AF_INET, SOCK_STREAM, 0);
    SockFlag = (SockFd != -1 ) ? created : undefined;
    if (! valid())
    {
        return false;
    }
    int ReuseAddr = 1;
    BaseSock::setSockOpt (SO_REUSEADDR, &ReuseAddr, sizeof(ReuseAddr));
    return true;
}

void BaseSock::setSockOpt(const int OptNam, const void* OptVal, int OptLen)
{
    if (::setsockopt(SockFd, SOL_SOCKET, OptNam, &OptVal, OptLen) == -1)
    {
        EXCRAISE("BaseSock::SetSockOpt", strerror(errno));
    }
}

bool BaseSock::connect( const BaseStr& Host, const int Port)
{
    int Rtc;
    if (!valid()) { return false; }

    SockAddr.sin_family = AF_INET;
    SockAddr.sin_port = htons(Port);

    Rtc = ::inet_pton(AF_INET, Host, &SockAddr.sin_addr);
    if (Rtc == -1) { return false; }
    else if (Rtc == 0)
    {
        struct hostent* HostInfo;
        if ((HostInfo = ::gethostbyname(Host)) == NULL)
        {
            cerr << "failed: gethostbyname(" << Host << ")" << endl;
            EXCRAISE("ExcConnect", strerror(errno));
        }
        SockAddr.sin_addr.s_addr = ((struct in_addr *)(HostInfo->h_addr))->s_addr;
    }

    //cout << "Connecting Host=" << Host << ",Ip=" << inet_ntoa(SockAddr.sin_addr) << ",Port=" << SockAddr.sin_port << " ..." << endl;
    cout << "BaseSock::connect: connecting " << *this << " ..." << endl;
    Rtc = ::connect(SockFd, reinterpret_cast<sockaddr*>(&SockAddr), sizeof (SockAddr));
    //cout << "Host=" << Host << ",Ip=" << inet_ntoa(SockAddr.sin_addr) << ",Port=" << SockAddr.sin_port
    //    << ": connected=" << (Rtc==0 ? "OK" : "NOK ") << (Rtc==-1 ? strerror(errno) : "") << endl;
    cout << "BaseSock::connect: connected=" << (Rtc==0 ? "OK" : "NOK ") << (Rtc==-1 ? strerror(errno) : "") << endl;
    SockFlag |= (Rtc == 0 ) ? connected : faulty;
    return (Rtc == 0 );
}

bool BaseSock::bind (const int Port)
{
    if (!valid()) { return false; }

    SockAddr.sin_family = AF_INET;
    SockAddr.sin_addr.s_addr = INADDR_ANY;
    SockAddr.sin_port = htons(Port);

    int Rtc = ::bind(SockFd, reinterpret_cast<struct sockaddr*>(&SockAddr), sizeof(SockAddr));
    SockFlag |= (Rtc == 0 ) ? bounded : faulty;
    return (Rtc != -1);
}

bool BaseSock::listen()
{
    if (! valid()) { return false; }

    int Rtc = ::listen(SockFd, 10);
    SockFlag |= (SockFd != -1 ) ? listened : faulty;
    return (Rtc != -1);
}

bool BaseSock::accept(BaseSock& NewSock) const
{
    //struct sockaddr SockAddr;
    socklen_t AddrLen = (socklen_t)sizeof(NewSock.SockAddr);
    NewSock.SockFd = ::accept(SockFd, reinterpret_cast<struct sockaddr*>(&NewSock.SockAddr), &AddrLen);
    if (NewSock.SockFd == -1)
    {
        NewSock.SockFlag |= faulty;
        cout << TS << "BaseSock::accept: " << strerror(errno) << endl;
        return false;
    }
    else
    {
        NewSock.SockFlag |= created | connected;
        cout << TS << "BaseSock::accept: accepted NewSock=" << NewSock << endl;
        return true;
    }
}

bool BaseSock::send(const BaseStr& s)
{
    //cerr << "error s=" << s << endl;
    cout << "BaseSock::Send: try send(" << s << ')' << endl;
    int BytesSent = ::send(SockFd, s.data(), s.length(), 0);
    cout << "BaseSock::Send: BytesSent=" << BytesSent << '(' << s.length() << ')' << endl;
    //BaseSock::BytesSent += BytesSent;
    bytes.sent += BytesSent;
    return (BytesSent == (int)s.length());
}

int BaseSock::recv (BaseStr& r)
{
    static char buf[MAXRECV];
    memset(&buf, 0, MAXRECV);

    r = "";

    memset(buf, 0, sizeof(buf));
    int BytesRecv = ::recv(SockFd, buf, sizeof(buf) - 1, 0);

    if (BytesRecv > 0)
    {
        r = buf;
    }
    else if (BytesRecv == 0)
    {
        cerr << "peer has socket shutdown throw=" << socket_shutdown << '/' << &socket_shutdown << endl;
        EXCRAISE(socket_shutdown, "peer has performed socket shutdown");
    }
    else // <0
    {
        EXCRAISE("BaseSock::Recv", strerror(errno));

    }
    bytes.recv += BytesRecv;

    return BytesRecv;
}

void BaseSock::setNonBlock (const bool Block)
{
    int Opts = fcntl (SockFd, F_GETFL);
    if ( Opts < 0 ) { return; }

    if (Block)
    {
        Opts = (Opts | O_NONBLOCK);
    }
    else
    {
        Opts = (Opts & ~O_NONBLOCK);
    }

    fcntl (SockFd, F_SETFL, Opts);
}

const BaseSock& BaseSock::operator << (const BaseStr& s)
{
    int bytessent = send(s);
    if (!bytessent)
    {
        EXCRAISE("BaseSock::send", strerror(errno));
    }
    return *this;
}

const BaseSock& BaseSock::operator >> (BaseStr& s)
{
    int bytesrecv = recv(s);
    if (bytesrecv == -1)
    {
        EXCRAISE("BaseSock::recv", (errno? strerror(errno) : "peer has connection shutdown"));
    }
    return *this;
}

const BaseSockSrv& BaseSockSrv::operator << (const BaseStr& s)
{
    //if (! BaseSock::send(s))
    //{
    //    EXCRAISE("BaseSock::Send", strerror(errno));
    //}
    BaseSock::operator<<(s);
    return *this;
}

const BaseSockSrv& BaseSockSrv::operator >> (BaseStr& s)
{
    //if (BaseSock::recv(s) == -1)
    //{
    //    EXCRAISE("BaseSock::Recv", (errno? strerror(errno) : "peer has connection shutdown"));
    //}
    BaseSock::operator>>(s);
    return *this;
}

const BaseSockClt& BaseSockClt::operator << (const BaseStr& s)
{
    //if (! BaseSock::send(s))
    //{
    //    EXCRAISE("BaseSock::Send", strerror(errno));
    //}
    BaseSock::operator<<(s);
    return *this;
}

const BaseSockClt& BaseSockClt::operator >> (BaseStr& s)
{
    //if (! BaseSock::recv(s))
    //{
    //    EXCRAISE("BaseSock::Recv", strerror(errno));
    //}
    BaseSock::operator>>(s);
    return *this;
}

BaseSockSrv::BaseSockSrv (int port)
{
    if ( ! BaseSock::create() )
    {
        EXCRAISE("BaseSock::Create: Could not create server socket", strerror(errno));
    }

    if ( ! BaseSock::bind(port) )
    {
        EXCRAISE("BaseSock::Bind: Could not bind server socket", strerror(errno));
    }

    if ( ! BaseSock::listen() )
    {
        EXCRAISE("BaseSock::Listen: Could not listen to socket", strerror(errno));
    }
}

void BaseSockSrv::Accept ( BaseSockSrv& sock, int to)
{
    if (to > 0)
    {
        ::signal(SIGALRM, BaseSockSrv::timeouted);
        timeout = to;
        ::alarm(to);
    }

    if ( ! BaseSock::accept ( sock ) )
    {
        EXCRAISE("BaseSock::accept: Could not accept socket", strerror(errno));
    }

    if (timeout > 0) ::alarm(0);
}

#ifdef PTHREAD
//template <typename Ret>
void BaseSockSrv::Execute(BaseSockInf (*sockhdlr) (BaseSockSrv&&))
{
    EXCFUNC(BaseSockSrv::Execute);
    BasePth<BaseSockInf> thread;
    //BaseSockSrv&& rref = move(*this);
    thread.task(sockhdlr, move(*this));
    thread.get().detach();
    result = thread.moveresult();
    LOG << "executed " << *this << "(result.valid=" << result.valid() << ")" << endl;
    executed.push_back(move(*this));
    SockFlag |= enqueued;
    //clear();
    //LOG << "valid=" << thread.valid() << endl;
    EXCFUNCEND;
}

bool BaseSockSrv::ready(void)
{
    bool ret = false;
    EXCFUNC(BaseSockSrv::ready);
    //ret = result.wait_for(chrono::seconds(0)) == future_status::ready;
    //BaseSockInf result = thread.wait();
    cout << TS << EXCHDLR << ": valid=" << result.valid() << endl;
    //ret = thread.ready();
    ret = result.valid();
    EXCFUNCEND;
    return ret;
};

void BaseSockSrv::timeouted (int)
{
    EXCFUNC(BaseSockSrv::timeouted);
    //timeouted = true;
    LOG << "timeouted=" << timeout << " pending executed=" << executed.size() << endl;
    list<BaseSockSrv>::iterator SrvConn;
    for( SrvConn = executed.begin(); SrvConn != executed.end();)
    {
        //cout << *iter << endl;
        if (SrvConn->ready())
        {
            //cout << TS << EXCHDLR << ": will be erased sock=" << *SrvConn << ",result=" << SrvConn->getresult() << ",thread=" << SrvConn->thread << endl;
            LOG << "will be erased sock=" << *SrvConn << ",result=" << SrvConn->getresult() << endl;
            SrvConn = executed.erase(SrvConn);
            //cout << TS << EXCHDLR << ": thread from executed erased" << endl;
        }
        else
        {
            SrvConn++;
        }
    }

    //::signal(SIGALRM, SIG_DEF);

    LOG << "timeouted=" << timeout << " still executed=" << executed.size() << endl;
    ::alarm(timeout);
    EXCFUNCEND;
};
#else

void BaseSockSrv::timeouted (int)
{
    EXCFUNC(BaseSockSrv::timeouted);
    LOG << "timeouted=" << timeout << " pending (nothing-todo)" << endl;
    ::alarm(timeout);
    EXCFUNCEND;
};

#endif

void BaseSockInf::set(const BaseStr& sp)
{
    set(BaseMap(sp));
}

void BaseSockInf::set(const BaseMap& mp)
    //const stat_t::flags_t& f=stat_t::flags_t(),
    //const BaseStr& hdlr="",
    //unsigned long long bs=0,
    //unsigned long long br=0
    //)
{
    EXCFUNC(BaseSockInf::set(BaseMap));
    //stat.flags = f;
    //stat.SockHdlr = hdlr;
    //stat.BytesSent = bs;
    //stat.BytesRecv = br;
    //stat.threadid = this_thread::get_id();
    //"Flags=|SoickHdlr=|BytesSent=|BytesRecv=|"
    LOG << mp << endl;
    for (auto pair : mp)
    {
        BaseStr key = pair.first, val = pair.second;
        if      (key == "Flags")
        {
            //LOG << mp << ":key=" << key << ",val=" << val << " stoi(" << stoi(val, nullptr, 16) << ')' << endl;
            stat.flags = stoi(val, nullptr, 16);
        }
        else if (key == "SockHdlr")  stat.SockHdlr = val;
        else if (key == "BytesSent") stat.BytesSent = stoi(val);
        else if (key == "BytesRecv") stat.BytesRecv = stoi(val);
        else ERR << "Key=" << key << ": not exists" << endl;
    }
#ifdef PTHREAD
    stat.threadid = this_thread::get_id();
#endif
    EXCFUNCEND;
};

BaseSockClt::BaseSockClt (BaseStr host, int port )
{
    if ( ! BaseSock::create() )
    {
        EXCRAISE("BaseSock::create", strerror(errno));
    }

    if ( ! BaseSock::connect (host, port) )
    {
        EXCRAISE("BaseSock::connect", strerror(errno));
    }
}

ostream& operator<< (ostream& os, const BaseSockSrv& sock)
{
    return sock.onPrint(os);
}

//void BaseSock::onPrint(ostream& os) const
//{
//     os.write (data (), length ());
//}
//============= BaseSock end =============

