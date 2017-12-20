/* $Revision: 232 $
*/

#include <BExc.H>


int main (int argc, char *argv[])
{
	BStream vlog;
	char buf[100];

	vlog << "test: open log" << endl;
	vlog.Open("test.out", 512);
	//fb.open("test.out", ios::app);
	cerr = (ostream&) vlog;	
	
	cout << "cout here" << endl;
	cerr << "cerr here" << endl;
	clog << "clog here" << endl;
	vlog << "vlog here" << endl;
	
	while(1)
	{
		cerr << "Enter: " << flush;
		cin >> buf;
		if (strcmp( buf, "EOF") == 0 ) break;

		cerr <<  buf << endl; 
	}

	cerr << "End of execution" << endl; 
	return 0;
}

