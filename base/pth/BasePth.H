#ifndef _BasePth_H
#define _BasePth_H

static char BaseTpl_H [] = "@(#) db $Id: BasePth.H 246 2013-12-14 04:18:20Z db $";

//#ifndef __GXX_EXPERIMENTAL_CXX0X__
//#define __GXX_EXPERIMENTAL_CXX0X__
//#endif
// must compiled with -std=c++11 -ptread 
#include <atomic>
#include <functional> 
#include <thread>
#include <future>

#include <BaseExc.H>
#include <BaseTime.H>
#include <BaseStr.H>

static const int MAX = 4 * 1024;

EXCDEFEXT(BasePth_unexpected);

#if !defined _CODE
//##############################################

template <class Result>
class BasePth:
    public BaseObj 
{
//typedef typename std::result_of<Callable(Args...)>::type Ret;
public:
    //BasePth() = delete;
    BasePth(): _thread() {};
    BasePth(BasePth&& other): _thread(move(other._thread)), result(move(other.result)) { }; 
    BasePth& operator=(BasePth&& other) { swap(_thread, other._thread); swap(result, other.result); return *this; };
    //BasePth(Callable, Args...): _thread() {};
    virtual ~BasePth()  { 
        if (running())
            EXCRAISE(BasePth_unexpected, "thread running jet");
    };
    virtual char const * ClassName() const { return "BasePth"; };
    bool running () const { return _thread.joinable(); }
    
    template <class Callable, class ... Args>
    void start(Callable&& fn, Args&& ... args) // obsolete, use task() with Result=void instead
    { 
        EXCBLK(BasePth::start);
            //running = true; 
            cout << EXCHDLR << ": running=" << running() << endl;
            //_thread = thread(fn, args ...);
            _thread = thread{ forward<Callable>(fn), forward<Args>(args) ... };
            //_thread.detach();
        EXCEPTION;
        EXCEND;
    }

    template <class Callable, class ... Args>
    //std::future<typename std::result_of<Function(Args...)>::type> 
    void task(Callable&& fn, Args&&... args)
    {
        using namespace std::placeholders;
        EXCBLK(BasePth::task);
        //cout << TS << EXCHDLR << ": fn=" << fn << ",args=" << args[0] << endl;
        cout << TS << EXCHDLR << ": fn=" << fn << endl;
        //packaged_task<Ret()> _task(bind(forward<Callable>(fn), forward<Args>(args)...)); // bind use copy ctor 
        //packaged_task<Ret()> _task(bind(forward<Callable>(fn), _1)); 
        packaged_task<Result(Args...)> _task(fn); 
        //future<Result> fut = _task.get_future();
        //result = move(fut);
        LOG << "1 get result.valid=" << result.valid() << endl;
        result = _task.get_future();
        LOG << "2 get result.valid=" << result.valid() << endl;
        _thread = thread(move(_task), forward<Args>(args)...);
        LOG << "3 get result.valid=" << result.valid() << endl;
            
        EXCEPTION;
        EXCEND;
    }

    thread& get() { return _thread; }  
    //bool result_valid(void) { return result.valid(); };
    //Result& operator= (future<Result>&& result) { result.get(); };
    //Result& operator() (future<Result>&& result) { result.get(); };
    Result getresult()
    {  
        return result.get();  
        //return move(result);  
    };

    future<Result> moveresult() { return move(result); };

    bool ready() 
    { 
        bool ret = false; 
       
        if (running())
        {
            _thread.join();
            ret = true;
        }
        else 
        {
            cout << "BasePth::wait: not joinable this=" << *this << endl;
            ret = false;
        }
        if (result.valid())
        {
            ret = result.wait_for(chrono::seconds(0)) == future_status::ready;
            //return result.get();  
        }
        else
        {
            cout << "BasePth::wait: result is not yet valid" << endl;
        }
   
        return ret; 
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
    thread _thread;
    future<Result> result;
    //static int threadcnt;
    //static const thread::id thid;
};
//##############################################
#elif _CODE==NONTPL
class BasePth:
    public BaseObj 
{
public:
    BasePth(): _thread() {};
    virtual ~BasePth()  { 
        /* exception, when running!? */ 
        if (running())
            EXCRAISE(BasePth_unexpected, "thread running jet");
    };
    virtual char const * ClassName() const { return "BasePth"; };
    bool running () const { return _thread.joinable(); }
    
    //template <typename Callable, typename ... Args>
    template <class Callable, class ... Args>
    void start(Callable&& fn, Args&& ... args) { 
        EXCBLK(BasePth::start);
            //running = true; 
            cout << EXCHDLR << ": running=" << running() << endl;
            //_thread = thread(fn, args ...);
            _thread = thread{ forward<Callable>(fn), forward<Args>(args) ... };
            //_thread.detach();
        EXCEPTION;
        EXCEND;
    }
    template <class Callable, class ... Args>
    //std::future<typename std::result_of<Function(Args...)>::type> 
    bool task(Callable&& fn, Args&&... args)
    {
        typedef typename std::result_of<Callable(Args...)>::type Ret;
        EXCBLK(BasePth::task);

        //cout << TS << EXCHDLR << ": fn=" << fn << ",args=" << args[0] << endl;
        cout << TS << EXCHDLR << ": fn=" << fn << endl;
        packaged_task<Ret()> _task(bind(fn, args...)); 
        future<Ret> fut = _task.get_future();
        ret = move(fut);
        _thread = thread(move(_task));
            
        EXCEPTION;
        EXCEND;
        return true;
    }

    //template <class Ret>
    int getret(void) { return ret.get(); };
    thread& get() { return _thread; }  
    void wait() { 
        if (running())
            _thread.join();
        else 
            cout << "BasePth::wait: not joinable this=" << *this << endl;
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
    future<int> ret;
    static int thcnt;
    //static const thread::id thid;
    thread _thread;
};
#elif _CODE==RUNNABLE
// false approach
class BasePthRunnable:
    public BaseObj 
{
public:
    BasePthRunnable(): _running(false), _thread() {};
    virtual ~BasePthRunnable()  {};
    void start() { 
        EXCBLK(BasePthRunnable::start);
            _running = true; 
            cout << "BasePthRunnable::start: running=" << _running << endl;
            _thread = thread(&BasePthRunnable::run, this); 
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

class BasePth: 
    public BasePthRunnable
{
    public:
        BasePth();
        virtual ~BasePth();
        virtual char const * ClassName() const { return "BasePth"; };

        bool create();
        //void start(Fn&& fn, Args&&... args) { _thread = thread(fn, args); };

        //{
        //    thread(fn, args);
        //}
        //void set(const int, const void*, int);
        
        //virtual const BasePth& operator << (const BaseStr&) const=0;
        //virtual const BasePth& operator >> (BaseStr&) const=0;

        bool running() const { return _running; }
        //friend ostream& operator<< (ostream& os, const struct&);
        //BasePth::run: Instance: BasePth (1382639644,442451#4@0xbfa5774c)thid=0(0x80534e4): pthcnt=48
        //th2=thid=0Instance: BasePth (1382645430,426756#4@0xbf84d3cc)(0x80535a4)
        virtual ostream& onPrint(ostream& os) const { 
            os << "running=" << _running <<'(';
            BaseObj::onPrint(os); 
            os << ')';
            return os; 
        };
        void run() 
        { 
            cout << TS << "BasePth::run: start this=" << *this << endl;
            do
            { 
                /* some stuff here */ 
                thcnt ++;
                cout << TS << "BasePth::run: " << *this  << ": thcnt=" << thcnt << endl;
                sleep(1);
            } 
            while (thcnt <= 5);
            _running = false; 
            cout << TS << "BasePth::run: stoped this=" << *this << endl;
        };  
    protected:
    private:
        int thid; //thread thr;
        static int thcnt;
        // struct pth_;
}; 
#else
# error "unsuppspected _CODE"
#endif // __RUNNABLE
// BasePth end

//ostream& operator<< (ostream&, const struct pth_&);

//======= Masros
//======= 
#endif // _BasePth_H
