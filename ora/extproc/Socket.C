// $Header$
//#include <signal.h>
#include <BaseSock.H>
#include <BaseVfs.H>

/* NOK ld error
#    c++ -std=c++11 -Wall -Wno-unused-variable -fPIC -DSHARED_OBJECT -I ../../.inc -c SockSrv.C
#    ld -shared -o socksrv.so SockSrv.o
#    cp -p  socksrv.so $HOME/rel/.lib/;
*/
// or
/* OK
#    gcc -std=c++11 -Wall -Wno-unused-variable -fPIC -I ../../.inc -c SockSrv.c
#    gcc -shared -static-libgcc -o socksrv.so SockSrv.o -L ../../.lib -lBase -lrt -lstdc++
#    cp -p  socksrv.so $HOME/rel/.lib/;
*/
// or
/* OK
#    # BaseSockSrv must be compiled with CFLAGS := $(CFLAGS) -DNOPTHREAD !!! (otherwise infinite loop)
#    c++ -std=c++11 -Wall -Wno-unused-variable -fPIC -DSHARED_OBJECT -I ../../.inc -c SockSrv.C
#    gcc -v -shared -o SockSrv.so SockSrv.o -L ../../.lib -lBase -lrt -static-libgcc -static-libstdc++ -lstdc++
#    cp -p  socksrv.so $HOME/rel/.lib/;
*/

//void signal_handler (int sig)
//{
//    cerr << TS << "signal_handler: sig=" << sig << endl;
//    EXCRAISE("signal_handler", "unexpected signal");
//}

static BaseVfs vlog;
static bool loginit = true;

extern "C" {
void SockSrv(int par, char* snd, char* rcv)
{
    EXCINIT;
    EXCFUNC(extproc::SockSrv);
    //FILE* fp = fopen("/vol/db/oralog/hr/SOCKSRV.trc", "a");
    //fprintf(fp, "%s: extproc::SockSrv: BEGIN\n", BaseTime("%y%m%d_%H%M%S").Now());
    //fflush(fp);
    if (loginit)
    {
        vlog.Open("/vol/db/oralog/hr/SockSrv", 10*1024*1024);
        //vlog.Open("log/SockSrv", 10*1024*1024);
        loginit = false;
    }

    LOGL("##### BEGIN par=" << par << ",snd=" << snd);

//    signal(SIGPIPE, signal_handler);
//    signal(SIGINT, signal_handler);
//    signal(SIGALRM, signal_handler);
//    signal(SIGSEGV, signal_handler);

    //fflush(NULL);
    // Create the socket
    LOGL("##### srv-socket port=" << par);
    static BaseSockSrv server(par); //OK
    static BaseSockSrv sock;
    LOGL("opened srv-socket server=" << server << "(" << sock << ")");
    //BaseSockClt client("localhost", 30000); //NOK
    //BaseSock sock; //NOK
    //BasePth<void> th1; //OK

    //char* chr;
    //strcpy(chr, "error segv"); OK -> SIGSEGV
    //throw 1; OK -> Exception of unknown type was thrown

    //static BaseSockClt sock("mpsrv.local.net", 30000); //NOK
/*
    static BaseStr stir;
    if (str.empty())
    {
        str = "1 first static";
    }
    else
    {
        str = "2 second static";
    }
    strcpy(msg, str.data());
    LOGL("static str=" << str << ",msg=" << msg);
    //EXCRAISE("test_exception", "test exception");
*/

//*
//
    BaseStr data;
    if (sock.valid())
    {
        data = snd;
        LOGL("send data=" << data);
        sock << data;
        if (data != "CLOSE")
        {
            sock >> data;
            LOGL("recived data=" << data);
        }
        else
        {
            sock.Close();
            data = "closed";
        }
        strcpy(rcv, data.data());
    }
    else
    {
        //bool Accept = true;
        //while (Accept)
        //{
        EXCBLK(WaitOnAccept);

        LOGL("############## Accept on port=30000 ...");
        server.Accept (sock);

        LOG << "START: sock=" << sock << endl;
        sock >> data;
        LOGL("recived data=" << endl << data);
        strcpy(rcv, data.data());

        EXCEPTION;
            ERR << "catched ActExc=" << BaseExc::ActExc() << '(' << exc.what() << ')';
        WHEN socket_shutdown THEN
            sock.Close();
        END;
        EXCEND;
        //}
    }

// */
    EXCEPTION;
    EXCEND;

    EXCTERM;
    LOGL("END");
}
} // extern C

/*
BaseSockInf SockHandler(BaseSockSrv&& sock)
{
    //BaseSockSrv sock = _sock;
    BaseSockInf ret;
    EXCFUNC(socksrv::SockHandler);
    LOG << "START: sock=" << sock << endl;
    while ( true )
    {
        BaseStr data;
        sock >> data;

        cout << "-- data recieved:" << endl << data << endl;
        if (data(0, 5) == "CLOSE")
        {
            sock << data(0, 5);
            //sock << "WAIT";
            //Accept = false;
            break;
        }
        else if (data(0, 3) == "REQ")
        {
            sock << "END: request from " + data.substr(4) + " denied";
            continue;
        }
        else if (data(0, 3) == "EXC")
        {
            EXCRAISE("socksrv::SockHandler", "data with EXC");
        }
        sleep(1);
        sock << "ACK: " + data;
    }
    //BaseStr sent = to_string(sock.gettraf().sent);
    //BaseStr recv = to_string(sock.gettraf().recv);
    //ret.set("Flags=0xff|SockHdlr=SockHandler|BytesSent=" + sent + "|BytesRecv=" + recv + "|");
    ret.set("Flags=0xff|SockHdlr=SockHandler|BytesSent=" + to_string(sock.gettraf().sent) + "|BytesRecv=" + to_string(sock.gettraf().recv) + "|");
    LOG << "END: sock=" << sock << endl;
    //EXCRAISE("thread_faulty", "test propagate of exception");
    EXCEPTION;
    EXCEND;
    return ret;
}

int main (void)
{
    //cout << "socksrv::main: " << endl;

    EXCINIT;
    EXCFUNC(socksrv::main);

    signal(SIGPIPE, signal_handler);
    signal(SIGINT, signal_handler);
    signal(SIGALRM, signal_handler);
    signal(SIGSEGV, signal_handler);

    // Create the socket
    BaseSockSrv server(30000);

    // netstat -na -ttcp
    //tcp        0      0 0.0.0.0:30000           0.0.0.0:*               LISTEN      14519/socksrv
    //tcp       48      0 127.0.0.1:33596         127.0.0.1:30000         ESTABLISHED 14521/sockclt
    //tcp        0      0 127.0.0.1:30000         127.0.0.1:33596         ESTABLISHED 14519/socksrv

    bool Accept = true;
    while (Accept)
    {
        BaseSockSrv sock;
        //cout << TS << EXCHDLR << ": new " << sock << endl;

        EXCBLK(WaitOnAccept);

        cout <<  TS << EXCHDLR << ": ############## Accept on port=30000 (max 5 sec) ..." << endl;
        server.Accept (sock, 5);

        //SockHandler(move(sock));
        sock.Execute(SockHandler);
        //cout << TS << EXCHDLR << ": moved " << sock << endl;

        EXCEPTION;
            cerr << "catched ActExc=" << BaseExc::ActExc() << '(' << exc.what() << ')';
        //    << ": socket_shutdown=" << socket_shutdown << '/' << &socket_shutdown << endl;
        WHEN socket_shutdown THEN
            Accept = false;
        END;
        EXCEND;
    }
    EXCEPTION;
    EXCEND;

    EXCTERM;
    exit (0);
}
*/

