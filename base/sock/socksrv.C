#include <signal.h>

#include <BaseSock.H>

void signal_handler (int sig)
{
    cerr << TS << "signal_handler: sig=" << sig << endl;
    EXCRAISE("signal_handler", "unexpected signal");
}

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
    BaseSockSrv server( 30000 );

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

#ifdef PTHREAD
        sock.Execute(SockHandler);
        //cout << TS << EXCHDLR << ": moved " << sock << endl;
#else
        SockHandler(move(sock));
#endif

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

