#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include <signal.h>
#include <unistd.h>
#include <exception>
#include <stdexcept>

void terminate_new_handler(void)
{
    cerr << "terminate_handler" << endl;
	exit (1);
}

void sigint_handler( int sig )
{
    try
    {
        throw sig;
    }
    catch( int i )
    {
        cout << "Exc_handler in sigint_handler=" << i << endl;
        throw ;
    }
}

void sigint2( int sig )
{
    try
    {
        throw out_of_range("Test");
    }
    catch( out_of_range& i )
    {
        cout << "Exc_handler in sigint2=" << i.what() << endl;
        throw ;
    }
}


void fun ( int sig )
{
    try
    {
        throw sig;
    }
    catch( int i )
    {
        cout << "Exc_handler in fun=" << i << endl;
        throw ;
    }
}

int main(int, char* argv[])
{

    set_terminate(terminate_new_handler);
    signal(SIGINT, sigint_handler);
    try
    {
        sleep(50);
        // fun(3);
    }
    catch( int i )
    {
        cout << "Exc handler in main=" << i << endl;
    }
    catch( ... )
    {
        cout << "Exc handler in main=unbekannt" << endl;
    }

    return 0;
}



