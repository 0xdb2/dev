#ifndef _BaseCore_H
#define _BaseCore_H

static char BaseCore_H [] = "@(#) db $Id$";

//#ifndef __GXX_EXPERIMENTAL_CXX0X__
//#define __GXX_EXPERIMENTAL_CXX0X__
//#endif
//#include <type>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>

#include <unistd.h>
#include <time.h>
//#include <sys/time.h>
#include <uuid/uuid.h>
#include <iostream>
#include <string>

using namespace std;

//=========== Types 
//typedef time_t Date;
//typedef timeval Date; // db@130926; gettimeofday 
//struct timeval {
//time_t      tv_sec;     /* seconds */
//suseconds_t tv_usec;    /* microseconds */
//};

//typedef struct timespec systime_type; // clock_gettime
//class coretime_t;
//extern void coretime_now(coretime_t&);
class coretime_t: public timespec
{
    public:
    coretime_t(): autots(false), width(3) { tv_sec = 0; tv_nsec = 0; };
    coretime_t(const timeval& other): autots(false), width(3) { tv_sec = other.tv_sec; tv_nsec = other.tv_usec; };
    coretime_t(const bool _auto): autots(_auto), width(3) { now(); };
    operator timeval* () { static timeval tv{tv_sec, tv_nsec * 1000 }; return &tv; };
    friend ostream& operator<< (ostream&, coretime_t&);

    void now(void)
    {
        //coretime_now(*this);
        clock_gettime(CLOCK_REALTIME, this);
    };
    private:
    bool autots;
    unsigned int width; 
};

ostream& operator<< (ostream&, coretime_t&);

// struct timespec {
//    time_t tv_sec;        /* seconds */
//    long   tv_nsec;       /* nanoseconds */
//};

//operator systime_type(const timeval& tv) { };

class Uuid
{
    public:
        Uuid()
        { 
            str[0] = '\0';
            safe = uuid_generate_time_safe(uuid);
            unparse(); 
        };
        ~Uuid() { clear(); }
        void clear(void)
        {
            uuid_clear(uuid);
        };
        char * get(void) { return str; };
        char * gettime(const char* fmt="%y%m%d_%H%M%S")
        {
            timeval t;
            static char timebuf [21];
            uuid_time(uuid, &t);
            int len = strftime (timebuf, sizeof(timebuf), fmt, localtime(&t.tv_sec));
            sprintf (timebuf + len, ".%03ld", t.tv_usec/1000);
            return timebuf;
        };
        void unparse(bool upper=true)
        {
            if (upper)
                uuid_unparse_upper(uuid, str);
            else
                uuid_unparse_lower(uuid, str);
        };
    private:
        uuid_t uuid;
        int safe;
        char str[37]; 
};

//=========== Globals
//static const int MAX = 4 * 1024;

//=========== Functions 

//extern void coretime_now(coretime_t&);

//=========== Macros

#define MAIN(nam) int main(int _argn, char** _args) \
{ \
    string _myName(#nam); \
    coretime_t _time_stamp(true); \
    cout << _time_stamp << ':' << _myName << ": ############## START" << endl;  

#define MAINEND(ret) \
    cout << _time_stamp << ':' << _myName << ": ############## END ret=" << ret << endl; \
    exit(ret); \
}

#endif // _BaseCore_H
