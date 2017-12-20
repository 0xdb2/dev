// $Revision: 270 $
#include <BaseVfs.H>

int main (void) // db@121020: ::main must return int
{
	BaseVfs vlog;
	//filebuf fb;
	//ostream os(&fb);
	char buf[100];
	//os.open("test.out", ios::app);
	//os.attach(2);
        try {
	cout << "cout here" << endl;
	cerr << "cerr here" << endl;
	clog << "clog here" << endl;
	vlog << "vlog here" << endl;

	vlog << "test:Open log" << endl;
	vlog.Open("/tst0.log", 128);
	//fb.open("test.out", ios::app);

	cout << "cout here" << endl;
	cerr << "cerr here" << endl;
	clog << "clog here" << endl;
	vlog << "vlog here" << endl;

	while(1)
	{
		cerr << "Enter: ";
		cin >> buf;
		if (strcmp( buf, "EOF") == 0 ) break;

		vlog <<  buf << endl;
	}

	vlog << "End of execution" << endl;
        } catch(int exc) { cerr << "cught Ecxception exc=" << exc << endl; }

//	os << 1234          << endl;
}

