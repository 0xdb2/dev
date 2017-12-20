#ifndef _BaseExc_H
#define _BaseExc_H

static char BaseExc_H [] = 
"@(#) dbE $Header: http://belov.homenet.org/svn/trunk/base/exc/BaseExc.H 254 2014-02-03 15:17:49Z db $";

//-----------------------------------------------
//  $Log: not supported by cvs2svn $
//  Revision 1.8  2001/07/19 00:12:35  db
//  *** empty log message ***
//
//  Revision 1.7  2001/07/18 22:11:24  db
//  *** empty log message ***
//
//  Revision 1.6  2001/07/11 23:53:46  db
//  backup only
//
//  Revision 1.5  2001/07/04 00:10:13  db
//  test of exceptions
//
//  Revision 1.4  2001/06/26 00:22:26  db
//  *** empty log message ***
//
//  Revision 1.3  2001/06/19 22:25:24  db
//  at work
//
//  Revision 1.2  2000/12/14 00:04:12  db
//  at work
//
//  Revision 1.1  2000/11/08 00:47:36  db
//  initial revision
//-----------------------------------------------

//----------- begin defines --------
#include <exception>
#include <typeinfo>
//#include <list>
#include <queue> // db@131115

#include <BaseObj.H>
#include <BaseTime.H> 

//----------- end defines --------

extern char const * const null_exception ;
extern char const * const std_exception ;
extern const string _act_exc_handler;
extern const string _myName;

//----------- begin macros --------


#define EXCHDLR  _act_exc_handler
#define LOGHDLR \
    (_act_exc_handler != "undefined" ? (_act_exc_handler + ":") : (_myName != "undefined" ? (_myName + ":") : string()))
#define LOG cout << TS << (LOGHDLR)
#define LOGL(line) cout << TS << (LOGHDLR) << line << endl;
#define ERR cerr << TS << (LOGHDLR + "ERROR: ")


#define EXCINIT BaseExc::Init();\
    try {\

#define EXCTERM     }\
    catch( exception& e )\
{\
    BaseExc::OnUnwind( "EXCTERM", __FILE__, __LINE__ );\
    BaseExc::PrintStack(e, cerr );\
    terminate();\
}\
catch(...)\
{\
    cerr << "Exception of unknown type was thrown"  \
    << endl; \
    terminate();\
}

// db@131017 pro static 
#define EXCDEF(e)\
    const char * (e) = #e

#define EXCDEFEXT(e)\
    extern const char * (e)	

#define EXCBLK(exc_handler)\
    static const string EXCHDLR (#exc_handler);\
try {

#define EXCFUNC(fn) EXCBLK(fn) 

#define EXCEPTION }\
    catch ( exception& exc ) {\
        BaseExc::OnUnwind( EXCHDLR, __FILE__, __LINE__ );

//BaseExc::OnUnwind( _act_exc_handler, __FILE__, __LINE__ );
//#define EXCEND if (BaseExc::ActExc() == null_exception)
#define EXCEND if (BaseExc::Canceled()) \
        { \
            BaseExc::Clear(); \
        } \
        else \
        { \
            throw; \
        }\
    }

#define EXCFUNCEND \
    EXCEPTION; \
    EXCEND;

#define EXCFUNCRET(Ret) \
    cerr << TS << LOGHDLR << (#Ret) << "=" << (Ret) << endl; \
    EXCEPTION; \
    EXCEND; \
    return (Ret);


#define EXCRAISE(name, text) BaseExc::Throw(name, text, __FILE__, __LINE__ ); // , throw BaseExc (text);
#define EXCRERAISE BaseExc::Canceled() = false; // , throw BaseExc (text);

//  (const string& n) 
//cerr << "Exception " << n << trown 
//throw BaseExc ("test_exception");   


#define WHEN if ((BaseExc::ActExc() == 
#define OR ) || (BaseExc::ActExc() == 
//#define THEN )) { BaseExc::ActExc() = null_exception;
#define THEN )) { BaseExc::Canceled() = true;
#define END }


//----------- end macros --------

class BaseExcHandler;

class BaseExc: 
    public exception, 
    public BaseObj
{
    public:
        // destructors, constructor
        // BaseExc(void); default Ctor not alowed
        BaseExc(const string& /*, T*/ );
        BaseExc(exception& exc) {
        
        }
       ~BaseExc() throw(); //db@121110: throw added
        // public member functions
        char const * ClassName() const;
        // from <exception>
        virtual const char* what () const throw () { return strwhat.data(); } //db@121110: throw added
        // = operator comming soon
        const BaseExc& operator= (const BaseExc& other) throw () { // db@131021
            cout << "BaseExc::operator=(" << other << ')' << endl;
            strwhat = other.what(); 
            return *this;
        };  
        //friend const BaseExc& operator= (exception&, const BaseExc&); // db@131021
        // static functions
        static void Init( void );
        static void terminate( void );
        static void unexpected( void );
        static void Throw( const char * name,  const char * text,  const char * file, int line) throw (exception); //db@121110: exeption pro BaseExc 
        static void OnUnwind( const string& hn, const string& fn, const int& ln);
        static void Clear(void) { 
            //ExcStack.clear(); 
            BaseExcStack().swap(ExcStack); // db@131115 
            canceled = false; 
            actexc = null_exception; 
            };
        static void PrintStack( const exception& e, ostream& os ); 
        static const char*& ActExc( void ) { return actexc; }; 
        static bool& Canceled(void) { return canceled; }; 
    private:
        // from <exception>
        string strwhat;
        // private data
        static const char * actexc;
        static bool canceled;
        static terminate_handler  old_terminate; 
        static unexpected_handler old_unexpected; 
        //typedef list<BaseExcHandler> BaseExcStack;
        typedef queue<BaseExcHandler> BaseExcStack; // db@131115
        static BaseExcStack ExcStack;
}; // End  BaseExc

//const BaseExc& operator= (exception&, const BaseExc&); // db@131021

class BaseExcHandler: public BaseObj
{
    public:
        char const * ClassName() const { return "BaseExcHandler"; };
        BaseExcHandler() : HandlerName(0), FileName(0), Line(0) { };
        BaseExcHandler(const string& hn, const string& fn, const int& ln) : HandlerName(hn), FileName(fn), Line(ln) { };
        ~BaseExcHandler() { };
        virtual ostream& onPrint(ostream& os) const;    
    private:
        string HandlerName;
        string FileName;
        int Line;
}; 

#endif // _BaseExc_H
