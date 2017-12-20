#if defined __linux__
#print "linux"
#else
#error "Undefined"
#endif

#include <BExc.H> 
#include <BTime.H> 
#include <list> 
#include <stdexcept> 

// BExc Exc ("test_exception");

class StackWalker: public BObj
{
    int Level;
	string HandlerName;
	string FileName;
    int Line;
public:	
	StackWalker() { }
	StackWalker(const string& hn, const string& fn, const int& ln) :
	HandlerName(hn),
	Level(0),
	FileName(fn),
	Line(ln)
	{ }
	~StackWalker() {}
    virtual void onPrint(ostream& os) const; 
};

list<StackWalker> ProgramStack;

void StackWalker::onPrint(ostream& os) const
{
     os << "Exception handler " << HandlerName << " in file " << FileName << " at line " << Line;
}  
	 
void  PrintStack(void)
{
  	while (!ProgramStack.empty())
	{
		cout << ProgramStack.back() << endl;
		ProgramStack.pop_back();
	}
}

// int StackWalker::Level = 0;


void funt (void) throw(bad_exception)
{
	static const string _act_func ("funt");
	
	//try
	//{
        // BExc* Exc = new BExc("test_exception"); 
		cout << _act_func <<"() will throw exception at " <<  BTime("%y%m%d_%H%M%S").Now() << endl;
		// cout << "to throw " << *Exc <<  " " << Exc << endl;
		// throw  new BExc("test_exception");
		//throw BExc ("test_exception");
		throw 1;
	//}
	//catch (BExc& e)
	//{
	//	cout << _act_func << "_exc_handler: " << e.what() << " " << e << endl;
	//	ProgramStack.push_front(StackWalker( _act_func, __FILE__, __LINE__ ));
	//	throw; 
	//}
}

void func (void)
{
	static string _act_func ("func");
	try
	    {
			cout << _act_func << endl;
			funt ();
  		}
		catch (BExc& e)
		{
		    cout << _act_func << "_exc_handler: " << e.what() << " " << e << endl;
		    ProgramStack.push_front(StackWalker( _act_func, __FILE__, __LINE__ ));
		    throw;
		} 
}

int main(int, char* argv[])
{ 

EXCINIT;

	static string _act_func ("main");
	try
	{
		cout << _act_func << ": __DATE__ __TIME__ " <<  __DATE__ <<  __TIME__  << endl;
		cout << _act_func << ": call to func()" << endl;
		func ();
	}
	catch (BExc& e)
	{
		cout << _act_func << "_exc_handler: " << e.what() << " " << e << endl;
		ProgramStack.push_front(StackWalker( "main", __FILE__, __LINE__ ));
		PrintStack();
		exit (1);
	}
	catch (exception& e)
	{
		cout << "main_standard_exc_handler: " <<  e.what() << endl;
		ProgramStack.push_front(StackWalker( "main" , __FILE__, __LINE__ ));
		PrintStack();
		exit (1);
	}
	catch (...)
	{
		cout << "main_idefault_exc_handler: " << "unknownn type of exception" << endl;
		ProgramStack.push_front(StackWalker( "main" , __FILE__, __LINE__ ));
		PrintStack();
		exit (1);
	}
	exit (0);

EXCTERM;	
}
