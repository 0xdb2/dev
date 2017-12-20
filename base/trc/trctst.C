// $Id: makefile 241 2013-11-08 14:18:08Z db $

#include <signal.h>

#include <BaseTrc.H>

static EXCDEF(bad_logic);
static EXCDEF(unrealised_trc);

typedef unsigned short flags_t;
enum flags {
    undefined  = 0x0000,
    created    = 0x0001,
    bounded    = 0x0002,
    listened   = 0x0004,
    connected  = 0x0008,
    faulty     = 0x8000
};

void signal_handler (int sig)
{
    cerr << "signal_handler: sig=" << sig << endl;
    EXCRAISE("signal_handler", "unexpected signal");
}

int main (int argc, char *argv[])
{
    static const BaseStr myName = argv[0] + BaseStr("::main"); 
    EXCINIT;

    EXCFUNC(myName);

    signal(SIGINT, signal_handler);
    signal(SIGALRM, signal_handler);

    BaseTrc trc;

    bool Action = true;
    while (Action)
    {

        EXCBLK(Action);
        // Action
        cout << TS << EXCHDLR << ": ############## START ..." << endl;
        //Action = false;
        EXCRAISE(bad_logic, "endless loop");
   

        EXCEPTION;
        WHEN bad_logic THEN
            cerr << _act_exc_handler << ": catched exc=" << BaseExc::ActExc() << '/' << (void*)BaseExc::ActExc() << '(' << exc.what() << ')' << endl;
            Action = false;
            //reraise
            BaseExc::ActExc() = unrealised_trc;
            EXCRERAISE;
        END;
        EXCEND;
    }

    EXCFUNCEND;

    EXCTERM;
    cout << (myName + "': ENDOK") << endl;
    exit (0);
} // END -- :!trctst; echo "$_=$?"

