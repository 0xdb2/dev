// $Id: shmtst.C 233 2013-10-21 21:18:19Z db $

#include <signal.h>

#include <BaseShm.H>

static EXCDEF(bad_logic);
static EXCDEF(unrealised_shm);

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

    BaseShm shm;

    bool Action = true;
    while (Action)
    {

        EXCBLK(Action);
        // Action
        cout << (myName + ':') << _act_exc_handler << ": ############## START ..." << endl;
        //Action = false;
        EXCRAISE(bad_logic, "endless loop");
   
        EXCEPTION;
        WHEN bad_logic THEN
            cerr << _act_exc_handler << ": catched exc=" << BaseExc::ActExc() << '/' << &BaseExc::ActExc() << '(' << exc.what() << ')' << endl;
            Action = false;
            //reraise
            BaseExc::ActExc() = unrealised_shm;
        END;
        EXCEND;
    }

    EXCEPTION;
    EXCEND;

    EXCTERM;
    cout << (myName + "': ENDOK") << endl;
    exit (0);
} // END -- :!shmtst; echo "$_=$?"




