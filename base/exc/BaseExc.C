static char BaseExc_C [] = "@(#) db $Header: http://belov.homenet.org/svn/trunk/base/exc/BaseExc.C 247 2013-12-30 07:34:12Z db $";

//---------------------------------------------
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

//----------- begin defines --------  

#include <BaseExc.H>

void dummy(void) { 
    LOG << "dummy" << endl;
    LOGL("dummy");
}

//----------- end defines ----------   
//----------- globals
char const * const null_exception = "null_exception";
char const * const std_exception = "std_exception";
const string _act_exc_handler = "undefined";
const string _myName = "undefined";

//============= BaseExc members =============

// template <class T> 
// BaseExc::BaseExc()  default Ctor not alowed 
// {
// }

BaseExc::BaseExc(const string& w) :
    strwhat(w)
{
    //	cerr << "BaseExc::BaseExc" << *this << endl;
}

BaseExc::~BaseExc() throw() //db@121110: throw added
{
    //	active_exc = null_exception;
    //	cerr << "~BaseExc::BaseExc " << *this << endl;
}

// static members
terminate_handler  BaseExc::old_terminate  = 0; 
unexpected_handler BaseExc::old_unexpected = 0;

const char * BaseExc::actexc = null_exception;
bool BaseExc::canceled = false;

BaseExc::BaseExcStack BaseExc::ExcStack;  

// static functions
void BaseExc::Init(void)
{
    old_terminate  = set_terminate  ( BaseExc::terminate );
    old_unexpected = set_unexpected ( BaseExc::unexpected );
}

void BaseExc::Throw ( const char * name,  const char * text,  const char * file, int line ) throw (exception) //db@121110: exeption pro BaseExc
{
    actexc = name;

    cerr << "Exception "  << name
        << "(" << text   << ")"
        << " thrown in " << file
        << " at line "   << line
        << endl;

    ExcStack.push(BaseExcHandler( "RAISER: BaseExc::Throw", file, line )); // db@131115: pro push_front
    throw BaseExc(text); 
}

void BaseExc::terminate()
{
    // static terminate_handler  old_terminate; 
    cerr << "BaseExc::terminate called" << endl;
    exit(1);
}

void BaseExc::unexpected(void) 
{
    // static unexpected_handler old_unexpected;
    cerr << "unexpected exception: bad_exception will throun" << endl;
    throw bad_exception();
}

char const * BaseExc::ClassName() const
{
    return("BaseExc");
}

void BaseExc::OnUnwind(const string& handler, const string& file, const int& line)
{
    if ( actexc == null_exception )
    {
        // thrown another class as BaseExc, can be only standard exception
        // all exception should have a valid name 
        actexc = std_exception;
    }

    ExcStack.push(BaseExcHandler( handler, file, line )); // db@131115: pro push_front
}

void BaseExc::PrintStack( const exception& e, ostream& os )
{
    os 	<< "________________________________________________________________________" << endl;
    os 	<<"UNHANDLED Exception "<< actexc <<" of type: "
        << typeid (e).name()
        <<"("<< e.what() <<")"<< endl;
    os	<<"TIME: " << BaseTime("%d.%m.%y %H:%M:%S.%.3ms").Now()
        << endl; 
    while (!ExcStack.empty())
    {
        // os << ExcStack.back() << endl;
        //ExcStack.pop_back();
        // db@131115
        os << ExcStack.front() << endl; 
        ExcStack.pop();
    }
}

//const BaseExc& operator= (exception& e, const BaseExc& o) throw () // db@131021
//{
//    cout << "BaseExc::operator=(" << other << ')' << endl;
//    e = other; 
//    return e;
//}

//============= BaseExc end ==========

//============= BaseExcHandler ==========

ostream& BaseExcHandler::onPrint(ostream& os) const     
{
    os << (HandlerName.substr(0,6) == "RAISER" ? "" : "HANDLER: ") << HandlerName 
        << " in file " << FileName 
        << " at line " << Line
        ;
    return os;
}

//============= BaseExcHandler end ==========
