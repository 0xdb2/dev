// $Revision: 232 $

#include <string.h>
#include <iostream>
#include <fstream>

using namespace std;

int main (void)
{
	filebuf fb;
	//ostream vlog(&fb);
	ostream vlog(cerr.rdbuf());
	//ofstream vlog;
	//cerr.tie(&vlog);
	//vlog.tie(&cerr);
	char buf[100];

	cout << "cout here" << endl;
	cerr << "cerr here" << endl;
	clog << "clog here" << endl;
	vlog << "vlog here" << endl;

	cerr << "##### open vlog" << endl;
	fb.open("tst1.log", ios::out);
	cerr << "vlog: isopen=" << fb.is_open() << endl;
	//iostream os(&fb);
	//vlog.tie(&os);
	vlog.rdbuf(&fb);

	cout << "cout here" << endl;
	cerr << "cerr here" << endl;
	clog << "clog here" << endl;
	vlog << "vlog here" << endl;
	
	while(1)
	{
		cerr << "Enter: ";
		cin >> buf;
		if (strcmp( buf, "EOF") == 0 ) break;

		vlog << buf << endl; 
	}

	vlog << "End of execution" << endl;
	//fb.sync(); // is protected
        fb.close();	
}

