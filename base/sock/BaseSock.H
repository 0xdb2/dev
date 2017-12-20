#ifndef _BaseSock_H
#define _BaseSock_H

static char BaseSock_H [] = "@(#) db $Header: http://belov.homenet.org/svn/trunk/base/sock/BaseSock.H 269 2014-04-18 10:46:44Z db $";

#include <sys/socket.h>
#include <arpa/inet.h>

#ifdef PTHREAD
//#include <queue>
#include <list>
#include <future>
#include <BasePth.H>
#endif

#include <BaseExc.H>
#include <BaseStr.H>
#include <BaseTime.H>
#include <BaseMap.H>

static const int MAXRECV = 4 * 1024;

EXCDEFEXT(socket_shutdown);

//class BaseSockInf;

typedef unsigned short sockflag_t;
enum flags {
    undefined  = 0x0000,
    created    = 0x0001, //
    bounded    = 0x0002,
    listened   = 0x0004, //
    connected  = 0x0008,
    enqueued   = 0x0010,
    dequeued   = 0x0020,
    faulty     = 0x8000
};

/*
char buf[100];
        flags_t flags = undefined | faulty;
        flags |= connected;
        sprintf(buf, "0x%04x", flags);
        cout << "flags=" << buf << endl;

operator const char* () (const flags f)
{
    switch (f)
        case undefined
        case created
        case bounded
        case listened
        case connected
        case faulty
        default:     return "default";
} */

struct traf_t
{
    traf_t(): recv(0), sent(0) { };
    unsigned int recv;
    unsigned int sent;
};

class BaseSock:
    public BaseObj //,
    //public string
{
    public:
        BaseSock() noexcept;
        //BaseSock(const int sd) noexcept : SockFd(sd) { }; // for tests only
        virtual ~BaseSock();
        //virtual char const * ClassName() const { return "BaseSock"; };
        //virtual void onPrint ( ostream& ) const;

        BaseSock(const BaseSock& other) = delete;
        //BaseSock(const BaseSock& other) {
        //    cout << "BaseSock::BaseSock(" << other << ")" << endl;
        //    this->SockFd = other.SockFd;
        //    this->SockAddr = other.SockAddr;
        //};
        BaseSock& operator= (const BaseSock& other) = delete;
        BaseSock(BaseSock&& other) noexcept: SockFlag(other.SockFlag), SockFd(other.SockFd), SockAddr(other.SockAddr) {
            cout << "BaseSock::BaseSock(&&); moved " << other << endl;
            other.SockFlag = undefined;
        };
        //virtual char const * ClassName() const { return "BaseSock"; }; // abstract class
        bool create();
        void setSockOpt(const int OptNam, const void* OptVal, int OptLen);
        // Server initialization
        bool bind (const int Port);
        bool listen();
        bool accept(BaseSock&) const;
        // Client initialization
        bool connect( const BaseStr& Host, const int Port);
        // Data Transimission
        bool send (const BaseStr&);
        int recv (BaseStr&);
        struct traf_t& gettraf() { return bytes; }
        virtual const BaseSock& operator << (const BaseStr&);
        virtual const BaseSock& operator >> (BaseStr&);

        void setNonBlock (const bool);
        void close(); // dont needed
        bool valid() const { return SockFlag != undefined; }
        friend ostream& operator<< (ostream& os, const sockaddr_in& sa);
        //friend BaseSockInf& BaseSockInf::operator= (const BaseSock& sock);
        virtual ostream& onPrint(ostream& os) const {
            char flags[10];
            snprintf(flags, 10,"0x%04x", SockFlag);
            //os << "sock=";
            //ios_base::fmtflags sflags = os.flags();
            //os << hex << showbase << setw(4) << SockFlag << ':'; os.flags(sflags);
            os << "sock=" << flags << ':' << SockFd << ':' << inet_ntoa(SockAddr.sin_addr) << ':' << ntohs(SockAddr.sin_port);
            return os;
        };

        void clear() {
            SockFlag = undefined;
            SockFd = -1;
            memset(&SockAddr, 0, sizeof(SockAddr));
        };

    private:
        sockflag_t SockFlag;
        int SockFd;
        sockaddr_in SockAddr;
        traf_t bytes;
};
// BaseSock end

class BaseSockInf: public BaseObj
{
    public:
        BaseSockInf (): stat() { };
        BaseSockInf(const BaseSockInf& other): stat(other.stat) { };
        //BaseSockInf(const stat_t& newstat) { stat = newstat; };
        BaseSockInf(BaseSockInf&&) = default;
        virtual ~BaseSockInf() { };
        virtual char const * ClassName() const { return "BaseSockInf"; };
        BaseSockInf& operator= (const BaseSockInf& sock)
        {
            this->stat = sock.stat;
            return *this;
        }
        BaseSockInf& operator= (BaseSockInf&&) = default;
        //set() { };
        virtual ostream& onPrint(ostream& os) const
        {
            char flags[5];
            unsigned char tmp;
            memcpy(&tmp, &stat.flags, 1);
            snprintf(flags, 5, "0x%02x", tmp);
            os << "SockInf=" << flags << ':' << stat.threadid << ':' << stat.SockHdlr << ":sent/recv=" << stat.BytesSent << '/' << stat.BytesRecv;
            return os;

        }

        struct stat_t
        {
            stat_t(): flags(), SockHdlr(), BytesSent(0), BytesRecv(0), threadid(0) {};
            struct flags_t
            {
                flags_t(): ok(0), nok(0), reserved(0), faulty(0) {};
                flags_t(const int fl) { memcpy(this, &fl, 1); };
                unsigned char ok:        1;
                unsigned char nok:       1;
                unsigned char reserved:  5;
                unsigned char faulty:    1;
                //operator(const int fl) { }
            } flags;
            BaseStr SockHdlr;
            unsigned long long BytesSent;
            unsigned long long BytesRecv;
#ifdef PTHREAD
            thread::id threadid;
#else
            int threadid;
#endif
        };
        void set(const BaseMap&);
        void set(const BaseStr&);
        BaseSockInf& operator=(const stat_t& other)
        {
            return *this;
        };
    private:
        void clear()
        {
            memset(&stat.flags, 0, sizeof(stat.flags));
        };
        stat_t stat;
};

ostream& operator<< (ostream& os, const sockaddr_in& sa);

class BaseSockSrv : private BaseSock
{
    public:
        BaseSockSrv (){};
        BaseSockSrv ( int port );
        BaseSockSrv(BaseSockSrv&& other) = default; //
        //BaseSockSrv(const BaseSockSrv& other) {  }; //
        virtual ~BaseSockSrv() {};
        virtual char const * ClassName() const { return "BaseSockSrv"; };

        const BaseSockSrv& operator << ( const BaseStr& );
        const BaseSockSrv& operator >> ( BaseStr& );
        friend ostream& operator<< (ostream& os, const BaseSockSrv&);

        void Accept (BaseSockSrv&, int timeout = 0);
        void Close(void) { return BaseSock::close(); };
        bool valid(void) { return BaseSock::valid(); };
        struct traf_t& gettraf() { return BaseSock::gettraf(); }
#ifdef PTHREAD
        void Execute(BaseSockInf(*) (BaseSockSrv&&));
#endif
    private:
        static void timeouted (int);
        BaseSockSrv& get (void) { return *this; };
        static int timeout;
#ifdef PTHREAD
        bool ready(void);
        //BaseSockInf getresult() { return thread.getresult(); };
        BaseSockInf getresult() { return result.get(); };
        //void setresult() {};
        static list<BaseSockSrv> executed;
        //BasePth<BaseSockInf> thread;
        future<BaseSockInf> result;
#endif
};

ostream& operator<< (ostream& os, const BaseSockSrv&);

class BaseSockClt : private BaseSock
{
    public:

        BaseSockClt (BaseStr host, int port);
        virtual ~BaseSockClt(){};
        virtual char const * ClassName() const { return "BaseSockClt"; };

        const BaseSockClt& operator << ( const BaseStr& );
        const BaseSockClt& operator >> ( BaseStr& );
};

//======= Masros
//=======
#endif // _BaseSock_H
