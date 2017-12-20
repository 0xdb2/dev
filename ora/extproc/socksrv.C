// $Header$
#include <signal.h>
#include <stdlib.h>
//#include <BaseVfs.H>
//#include <BaseExc.H>

/* OK
#    gcc -g socksrv.o -o socksrv -L ../../.lib -lBase -L ./ -lSockSrv
*/

void signal_handler (int sig)
{
    //cerr << TS << "signal_handler: sig=" << sig << endl;
    exit(sig);
//    EXCRAISE("signal_handler", "unexpected signal");
}

extern "C" { void SockSrv(int, char*); }

int main (void)
{

//    EXCINIT;
//    EXCFUNC(socksrv::main);
    //BaseVfs vlog;
    //vlog.Open("SOCKSRV", 10*1024*1024);
    //cout << "socksrv::main: BEGIN" << endl;
//    LOGL("BEGIN");

    signal(SIGPIPE, signal_handler);
    signal(SIGINT, signal_handler);
    signal(SIGALRM, signal_handler);
    signal(SIGSEGV, signal_handler);

    // Create the socket
    static char msg[1024+1];
    SockSrv(30000, msg);
    //cout << "socksrv::main: msg=" << msg << endl;
//    LOGL("END");

//    EXCEPTION;
//    EXCEND;

//    EXCTERM;
    //cout << "socksrv::main: END" << endl;
    exit (0);
}

