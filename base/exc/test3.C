#if defined __linux__
#print "linux"
#else
#error "Undefined"
#endif

#include <BObj.H> 
// #include <BTime.H> 
// #include <stdexcept> 

class exc : public BObj	 
{
	string what;
public: 	
	exc(const string& e):
				  what(e)
	{
		cerr << "exc::exc() " << *this << endl;   
	}
	~exc(void)
	{
		cerr << "~exc::exc() " << *this << endl;   
	}
};

void func (void) 
{
	static const string _act_func ("func");
	try
	{
		cout << _act_func << " reise exc at line " << __LINE__ << endl;
		throw exc ("test_exception");
    }
	catch ( exc& e ) 
	{
		cerr << _act_func << " catched at line "   << __LINE__  << " exc:" << e << endl;   
	    // throw;
	} 
}


int main(int, char* argv[])
{ 
	static string _act_func ("main");

	try
	{
		cout << _act_func << ": call to func()" << endl;
		func ();
	}
	
	catch (exc& e)
	{
	  	cout << _act_func << "_exc_handler: " << " " << e << endl;
	    exit (1);
	}
	catch (...)
	{
	  	cout << "main_default_exc_handler: " << "unknownn type of exception" << endl;
	    exit (1);
	}
	
	exit (0);

}
