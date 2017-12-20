// $Header: http://belov.homenet.org/svn/trunk/base/exc/exctst.C 234 2013-10-23 10:38:48Z db $
// $Id: exctst.C 234 2013-10-23 10:38:48Z db $
#if defined __linux__
//#pragma message "linux"
#else
#error "Undefined"
#endif

#include <stdexcept> 
#include <BaseExc.H> 
// #include <BaseTime.H> 

EXCDEF(test_exception);
EXCDEF(out_of_main_exception);

class exctst
{
    public:
        exctst(void)
        {
            cerr << "exctst() " << this << endl;
            EXCRAISE(out_of_main_exception, "what_out_of_main_exception")  
        }
        ~exctst(void)
        {
            cerr << "~exctst() " << this << endl;
        }
}; 

// exctst ExcTst;

void func (void) 
{
    EXCFUNC(func);
    cout << _act_exc_handler << " reise exc test_exception=" << test_exception << '/' << (void*)test_exception << " at next line " << __LINE__ << endl;
    EXCRAISE(test_exception, "test_exception raised")
        // throw bad_exception();

    EXCEPTION;
        const char * excref = BaseExc::ActExc();
        cerr << excref << " was catched ActExc=" << excref << '/' << (void*)excref
            << ": test_exception=" << test_exception << '/' << (void*)test_exception << endl;

    WHEN test_exception THEN
        //cerr << exc.what() << " was catched and reraised out_of_main_exception" << endl;
        //BaseExc::ActExc() = out_of_main_exception;
        //throw BaseExc("reraised out_of_main_exception"); 
        cerr << BaseExc::ActExc() << '(' << exc.what() << ") was catched:  raise out_of_main_exception ..." << endl;
        EXCRAISE(out_of_main_exception, "reraised out_of_main_exception");
        //cerr << BaseExc::ActExc() << '(' << exc.what() << ") was catched and will reraised ..." << endl;
        //EXCRERAISE;
    END;
    EXCEND;
}

int main(int, char* argv[])
{ 
    static string _act_exc_handler ("main");

    EXCINIT;
    EXCFUNC(exctst::main);


    cout << _act_exc_handler << ": call to func()" << endl;
    func ();
    
    EXCEPTION;
    WHEN out_of_main_exception THEN
        cerr << BaseExc::ActExc() << '(' << exc.what() << ") was catched and will reraised ..." << endl;
        EXCRERAISE;
    END;
    EXCEND;

    EXCTERM;
    exit (0);
}
