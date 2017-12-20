// $Header$

#include <signal.h>

#include <BasePth.H>
//#include <BaseSock.H>

static EXCDEF(bad_logic);
static EXCDEF(unrealised_pth);

void signal_handler (int sig)
{
    cerr << "signal_handler: sig=" << sig << endl;
    EXCRAISE("signal_handler", "unexpected signal");
}

BaseStr myFun1(int sec, string&& str)
{
    cout << BaseTime("%H%M%S.%.3").Now() << ": myFun1 start for " << sec << " sec with str=" << str << " ..." << endl;
    sleep(sec);
    cout << BaseTime("%H%M%S.%.3").Now() << ": myFun1 end now" << endl;
    return "OK:" + str;
}

class My
{
    public:
        My(): str() {};
        My(const char * s): str(s) { cout << "My::My:  const char=" << s << endl; };
        ~My() { cout << "~My::My: str=" << str << endl; };
        My(const My&) = delete;
        My& operator=(const My&) = delete;
        My& operator=(const char * s) { this->get() = s; return *this; };
        My(My&& other): str() { cout << "My::My: moved other=" << other.get() << endl; swap(str, other.str); };
        //My& operator=(My &&) = default;
        My& operator=(My&& other)
        {
            cout << "My::operator=: moved other=" << other.get() << endl;
            swap(str, other.str);
            return *this;
        };
        string& get(void) { return str; };
    private:
        string str;
};

//void myFun2(int sec, BaseSockSrv&& sock)
My myFun2(int sec, My&& str)
{
    EXCFUNC(myFun2);
    cout << TS << ": myFun2 start for " << sec << " sec with str=" << str.get() << endl;
    //cout << BaseTime("%H%M%S.%.3").Now() << ": myFun2 start for " << sec << " sec with sock=" << sock << endl;
    sleep(sec);
    cout << TS << ": myFun2 end now" << endl;
    EXCFUNCEND;
    //str = "moved " + str;
    return move(str);
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
#if false // ok
    {
        thread th1(myFun1, 100, "test string");
        //thread th2(myFun2, 200);

        //sleep(1);

        th1.join();
        //th2.join();
    }
#elif false // ok
    {
/*        BasePth th1;
        BasePth th2;
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
*/
    }
#elif true
    {
        //BasePth th1;
        BasePth<BaseStr> th1;
        string str1("test string");
        th1.task(myFun1, 2, move(str1));
        th1.get().detach();
        //sleep(3);
        cout << TS << "WAIT: th1=" << th1 << endl;
        //BaseStr ret1 = th1.wait();
        while (! th1.ready())
        {
            sleep(1);
        }
        BaseStr ret1 = th1.getresult();
        cout << TS << "RET: th1=" << th1 << ",ret=" << ret1 << endl;

        BasePth<My> th2;
        My str2("test string 2");

        //th2.start(myFun2, 1, move(str2));
        //th2.wait();
        //cout << TS << "STOP: th2=" << th2 << endl;

        //th2.task(myFun2, 1, "test string 2");
        //th2.task(myFun2, 1, str2.get().data());
        th2.task(myFun2, 1, move(str2)); // ok
        //th2.task(myFun2, 1, "test string 2");
        th2.get().detach();
        cout << TS << "WAIT: th2=" << th2 << endl;
        while (! th2.ready())
        {
            sleep(1);
        }
        My ret2 = th2.getresult();
        cout << TS << "RET: th2=" << th2 << ",ret=" << ret2.get() << endl;
    }
#elif false
    class MyThread
    {
        public:
            MyThread(): _thread() { };
            //MyThread(MyThread&& other) /*: _thread(move(other._thread)) */ { };
            //MyThread& operator=(MyThread&& other) {
                //swap(_thread, other._thread);
                //_thread = move(other._thread);
                //return *this;
            //};
        private:
            BasePth<My> _thread;
    };


    MyThread _thread;
    {
        _thread = MyThread();
    }

#elif false
    {
        using namespace std::placeholders;
        My str = "test *** string";
        //cout << TS << "result=" << myFun2(1, move(str)) << endl;
        //auto fun = bind(myFun2, 1, "test *** string");
        auto fun = bind(myFun2, 1, _1);
        My result = fun(move(str));
        cout << TS << "result=" << result.get() << endl;
    }
#else
    {
        EXCRAISE("uncatchable_logic_error", "else uncompleted");
    }
#endif

    cout << TS << (myName + ':') << EXCHDLR << ": ############## END" << endl;
    //EXCRAISE(bad_logic, "endless loop");

    EXCEPTION;
    WHEN bad_logic THEN
        cerr << _act_exc_handler << ": catched exc=" << BaseExc::ActExc() << '/' << (void*)BaseExc::ActExc() << '(' << exc.what() << ')' << endl;
        //Action = false;
        //reraise
        BaseExc::ActExc() = unrealised_pth;
        EXCRERAISE;
    END;
    EXCEND;

    EXCFUNCEND;

    EXCTERM;
    cout << (myName + "': ENDOK") << endl;
    exit (0);
} // END -- :!pthtst; echo "$_=$?"

