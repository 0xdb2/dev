#if defined __linux__
#print "linux"
#else
#error "Undefined"
#endif

// #include <BObj.H> 
// #include <BTime.H> 
// #include <exception> 
#include <typeinfo> 
#include <iostream> 
#include <string> 

class excbase 
{
public: 	
	excbase(void)
	{
		cerr << "excbase() " << this << endl;   
	}
	~excbase(void)
	{
		cerr << "~excbase() " << this << endl;   
	}
    virtual const char* what () const { return typeid(this).name(); }
};

class objbase 
{
    int id;	
public: 	
	objbase(int i)
	{ 
	    id = i;
		cerr << "objbase() " << this << endl;   
	}
	~objbase(void)
	{
		cerr << "~objbase() " << this << endl;   
	}
    virtual int Id (void) { return 0; }
};

class exc : 
		public excbase,
		public objbase	 
{
	string _what;
public: 	
	exc(const string& e, int i):
	            objbase(i),
				  _what(e)
	{
		cerr << "exc() " << this << endl;   
	}
	~exc(void)
	{
		cerr << "~exc::exc() " << this << endl;   
	}
    virtual const char* what () const { return _what.c_str (); }
    // virtual int GetId () const { return id; }
		
};



void func (void ) 
{
	static const string _act_func ("func");
	

	try
	{
		exc e("test_exception", 100);
		//exc* ep = &e;

		cout << _act_func << " reise exc at line " << __LINE__ << endl;
		throw e; // exc("test_exception");
    }
	catch ( excbase& e ) 
	{
		cerr << _act_func << ": "<< typeid(e).name() 
		     << "(" << e.what() << ")" << " catched at line "   << __LINE__  << endl;   
		// delete (ep);
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
	
	catch (excbase& e)
	{
	  	cout << _act_func << "_exc_handler" << endl;
	    exit (1);
	}
	catch (...)
	{
	  	cout << "main_default_exc_handler: " << "unknownn type of exception" << endl;
	    exit (1);
	}
	
	exit (0);

}
