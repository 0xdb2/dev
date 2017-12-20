static char coretst_C [] = "@(#) db $Id$";

#include <signal.h>

#include <BaseCore.H>

//typedef unsigned short flags_t;
//enum flags
//    undefined  = 0x0000,
//    created    = 0x0001,
//    bounded    = 0x0002,
//    listened   = 0x0004,
//    connected  = 0x0008,
//    faulty     = 0x8000
//};

//void signal_handler (int sig)
//{
//    cerr << "signal_handler: sig=" << sig << endl;
//    EXCRAISE("signal_handler", "unexpected signal");
//}

//int main (int argc, char *argv[])
//{
//    static const string _myName = "coretst::main"; 
MAIN(coretst::main)

    //signal(SIGINT, signal_handler);
    //signal(SIGALRM, signal_handler);
    //cout << time_stamp << ':' << _myName << ": ############## START" << endl;

    Uuid uuid;
    
    //time_t sec = uuid.gettime().tv_sec; 
    //struct tm *tm = localtime(&sec);
    //char time [20];
    for (int ii = 0; ii < 21; ii++)
    {
        Uuid uuid;
        cout << _myName << ": uuid=" << uuid.get() << ":time=" << uuid.gettime() << endl;
    }
    
//    cout << _myName << ": ############## ENDOK" << endl;
//    exit (0);
//} // END -- :!coretst; echo "$_=$?"
MAINEND(0);

