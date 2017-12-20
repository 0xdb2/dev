// $Id: makefile 234 2013-10-23 10:38:48Z db $

#include <signal.h>

#include <BaseMth.H>
//#include <BaseSock.H>

static EXCDEF(bad_logic);
static EXCDEF(unrealised_mth);

void signal_handler (int sig)
{
    cerr << "signal_handler: sig=" << sig << endl;
    EXCRAISE("signal_handler", "unexpected signal");
}

void myFun1(int sec, string&& str) 
{
    cout << BaseTime("%H%M%S.%.3").Now() << ": myFun1 start for " << sec << " sec with str=" << str << " ..." << endl; 
    sleep(sec);
    cout << BaseTime("%H%M%S.%.3").Now() << ": myFun1 end now" << endl; 
}

class My
{
    public:
        My(): str() {};
        My(const char * s): str(s) { };
        ~My() { cout << "~My::My:" << str << endl; };
        My(const My&) = delete;
        My& operator=(const My&) = delete;
        My& operator=(const char * s) { this->get() = s; return *this; };
        My(My&& rs): str(rs.get()) { cout << "My::My: moved rs=" << rs.get() << endl; rs.get() = "";}; 
        string& get(void) { return str; };
    private:
        string str;
};

void myFun2(int sec, My&& str)
//void myFun2(int sec, BaseSockSrv&& sock)
{
    cout << BaseTime("%H%M%S.%.3").Now() << ": myFun2 start for " << sec << " sec with str=" << str.get() << endl; 
    //cout << BaseTime("%H%M%S.%.3").Now() << ": myFun2 start for " << sec << " sec with sock=" << sock << endl; 
    sleep(sec);
    cout << BaseTime("%H%M%S.%.3").Now() << ": myFun2 end now" << endl; 
}

int main (int argc, char *argv[])
{
    static const BaseStr myName = argv[0] + BaseStr("::main"); 
    EXCINIT;

    EXCFUNC(myName);

    signal(SIGINT, signal_handler);
    signal(SIGALRM, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGCHLD, signal_handler);

    EXCBLK(Action);
        // Action
    cout << TS << (myName + ':') << EXCHDLR << ": ############## START ..." << endl;
    if (false) // ok
    {
        thread th1(myFun1, 100, "test string");
        //thread th2(myFun2, 200);
        
        //sleep(1); 
        
        th1.join();
        //th2.join();
    }
    else if (true)
    {
        BaseMth th1;
        BaseMth th2;
        cout << TS << "START: th1=" << th1 << ",th2=" << th2 << endl;
        string str1("test string 1");
        th1.start(myFun1, 2, move(str1));
        cout << "string moved str1=" << str1 << endl; 
        //th1.get().detach();
        //BaseSockSrv sock(3000);
        My str2("test string 2");
        th2.start(myFun2, 1, move(str2));
        //th2.start(myFun2, 1, move(sock));
        str2="";
        //sock.clear();
        cout << "string moved str2=" << str2.get() << endl; 
        //th1.start(myFun1, 3, "calling");
        //th2.start(myFun2, 5, "calling");
        //sleep(1);
        
        cout << TS << "WAIT: th1=" << th1 << ",th2=" << th2 << endl;
        th2.wait();
        th1.wait();
        cout << TS << "STOP: th1=" << th1 << ",th2=" << th2 << endl;
    }
    else
    {
        EXCRAISE("uncatchable_logic_error", "else uncompleted");
    }

    cout << TS << (myName + ':') << EXCHDLR << ": ############## END" << endl;
    //EXCRAISE(bad_logic, "endless loop");
   
    EXCEPTION;
    WHEN bad_logic THEN
        cerr << _act_exc_handler << ": catched exc=" << BaseExc::ActExc() << '/' << (void*)BaseExc::ActExc() << '(' << exc.what() << ')' << endl;
        //Action = false;
        //reraise
        BaseExc::ActExc() = unrealised_mth;
        EXCRERAISE;
    END;
    EXCEND;

    EXCFUNCEND;

    EXCTERM;
    cout << (myName + "': ENDOK") << endl;
    exit (0);
} // END -- :!mthtst; echo "$_=$?"

