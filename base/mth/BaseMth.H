#ifndef _BaseMth_H
#define _BaseMth_H

static char BaseTpl_H [] = "@(#) db $Id: BaseMth.H 238 2013-10-29 13:37:27Z db $";

//#ifndef __GXX_EXPERIMENTAL_CXX0X__
//#define __GXX_EXPERIMENTAL_CXX0X__
//#endif
// must compiled with -std=c++11 -ptread 
#include <atomic>
#include <functional> 
#include <thread>

#include <BaseExc.H>
#include <BaseTime.H>
#include <BaseStr.H>

static const int MAX = 4 * 1024;

EXCDEFEXT(BaseMth_unexpected);

#ifndef __RUNNABLE
class BaseMth:
    public BaseObj 
{
public:
    BaseMth(): _thread() {};
    virtual ~BaseMth()  { 
        /* excepktion, when running!? */ 
        if (running())
            EXCRAISE(BaseMth_unexpected, "thread running jet");
    };
    virtual char const * ClassName() const { return "BaseMth"; };
    bool running () const { return _thread.joinable(); }
    
    template <typename Callable, typename ... Args>
    void start(Callable&& fn, Args&& ... args) { 
        EXCBLK(BaseMth::start);
            //running = true; 
            cout << EXCHDLR << ": running=" << running() << endl;
            //_thread = thread(fn, args ...);
            _thread = thread{ forward<Callable>(fn), forward<Args>(args) ... };
            //_thread.detach();
        EXCEPTION;
        EXCEND;
    }

    thread& get() { return _thread; }  

    void wait() { 
        if (running())
            _thread.join();
        else 
            cout << "BaseMth::wait: not joinable this=" << *this << endl;
        //_running = false; 
    }

    virtual ostream& onPrint(ostream& os) const { 
        os << "running=" << running() <<'(';
        BaseObj::onPrint(os); 
        os << ')';
        return os; 
    };

protected:
    //atomic<bool> running; 

private:
    static int thcnt;
    static const thread::id main_thid;
    thread _thread;
};

#else // false approach

class BaseMthRunnable:
    public BaseObj 
{
public:
    BaseMthRunnable(): _running(false), _thread() {};
    virtual ~BaseMthRunnable()  {};
    void start() { 
        EXCBLK(BaseMthRunnable::start);
            _running = true; 
            cout << "BaseMthRunnable::start: running=" << _running << endl;
            _thread = thread(&BaseMthRunnable::run, this); 
        EXCEPTION;
        EXCEND;
    }
    void wait() { 
        _thread.join(); 
        _running = false; 
    }
protected:
    virtual void run() = 0;
    atomic<bool> _running; 
private:
    thread _thread;
};

class BaseMth: 
    public BaseMthRunnable
{
    public:
        BaseMth();
        virtual ~BaseMth();
        virtual char const * ClassName() const { return "BaseMth"; };

        bool create();
        //void start(Fn&& fn, Args&&... args) { _thread = thread(fn, args); };

        //{
        //    thread(fn, args);
        //}
        //void set(const int, const void*, int);
        
        //virtual const BaseMth& operator << (const BaseStr&) const=0;
        //virtual const BaseMth& operator >> (BaseStr&) const=0;

        bool running() const { return _running; }
        //friend ostream& operator<< (ostream& os, const struct&);
        //BaseMth::run: Instance: BaseMth (1382639644,442451#4@0xbfa5774c)thid=0(0x80534e4): mthcnt=48
        //th2=thid=0Instance: BaseMth (1382645430,426756#4@0xbf84d3cc)(0x80535a4)
        virtual ostream& onPrint(ostream& os) const { 
            os << "running=" << _running <<'(';
            BaseObj::onPrint(os); 
            os << ')';
            return os; 
        };
        void run() 
        { 
            cout << TS << "BaseMth::run: start this=" << *this << endl;
            do
            { 
                /* some stuff here */ 
                thcnt ++;
                cout << TS << "BaseMth::run: " << *this  << ": thcnt=" << thcnt << endl;
                sleep(1);
            } 
            while (thcnt <= 5);
            _running = false; 
            cout << TS << "BaseMth::run: stoped this=" << *this << endl;
        };  
    protected:
    private:
        int thid; //thread thr;
        static int thcnt;
        // struct mth_;
}; 

#endif // __RUNNABLE
// BaseMth end

//ostream& operator<< (ostream&, const struct mth_&);

//======= Masros
//======= 
#endif // _BaseMth_H
