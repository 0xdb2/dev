
#include <BaseSock.H>

int main (int argc, char * args[]) 
{
    EXCINIT;
    EXCFUNC("sockclt::main");

    BaseSockClt sock("mpsrv.local.net", 30000);   // 12405=11000001110101
                                            // 30000=101110111000

    BaseStr data, clt(args[1]), replay;

    EXCBLK("Connect");
    while (true)
    {
        //for (int step=0; step ;step++) 
        if (replay == "")
            data="CONN"; 
        else if (replay(0, 3) == "ACK")
            data="REQ";
        else if (replay(0, 3) == "END")
            data="CLOSE";
        else
            data="KEEPALIVE";

        sock << data + "=" + clt;
        sock >> replay;
        cout << "-- server respond:" << endl << replay << endl;
        if (replay == "CLOSE") break;
        sleep(1);
    }
    EXCEPTION;
    EXCEND;

    EXCEPTION;
    EXCEND;

    EXCTERM;
    exit (0);
} // !sockclt; echo $_=$?

