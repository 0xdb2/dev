#ifndef _BaseVfs_H
#define _BaseVfs_H

static char BaseVfs_H [] = "@(#) db $Header: http://belov.homenet.org/svn/trunk/base/vfs/BaseVfs.H 232 2013-10-21 14:57:30Z db $";

/*
   $Log: not supported by cvs2svn $
   Revision 1.6  2000/11/01 20:14:02  db
   tuning

   Revision 1.5  2000/10/31 01:13:23  db
   with switch of file

   Revision 1.4  2000/10/29 22:25:11  db
   jet another streams test

   Revision 1.3  2000/10/25 23:31:33  db
   some tests

   Revision 1.2  2000/10/24 22:03:23  db
   added class BaseVfs

   Revision 1.1  2000/10/24 20:45:46  db
 *** empty log message ***

*/

#include <iostream>
#include <fstream>
#include <ext/stdio_filebuf.h> // db@121027: __gnu_cxx::stdio_filebuf

#include <BaseObj.H>
#include <BaseTime.H>

using namespace std; // db@101012

#define OPEN_NEW_FILE (true) 

typedef __gnu_cxx::stdio_filebuf<char> fdbuf; // db@121027: new

class BaseVfsBuf : 	public BaseObj,
    //public filebuf
    public fdbuf // db@121027
{
    public:
        BaseVfsBuf(void);
        BaseVfsBuf(const char * fn, int ms);
        virtual ~BaseVfsBuf();
        void Open(const char * fn, int ms);
        char const * ClassName() const;
    protected:
        //virtual filebuf* open(const char * fn, int om, int prot=filebuf::openprot); 
        virtual filebuf* open(const char * fn, int om); // db@121017
        virtual int overflow(int c = EOF);
        //virtual streamsize sys_write(const char*, streamsize); // db@121020
        virtual streamsize xsputn(const char*, streamsize);
        virtual void switchfile(bool newfile=false);
        virtual void link(void); 
        //virtual int fd(void) { return _fd; }
        //int writefile(char *p, int len);
    private:
        int maxsize;
        int cursize;
        char * fname;
        //int _fd; // db@121017
}; 
// BaseVfsBuf end


class BaseVfs: 	public BaseObj, 
    public ostream
{
    public:
        BaseVfs(void);
        BaseVfs(const char * fn, int ms);
        ~BaseVfs();
        void Open(const char * fn, int ms)
        {
            buf.Open(fn, ms);
        }
        char const * ClassName() const;
    private:

        // private data
        BaseVfsBuf  buf;
}; // End  BaseVfs

#endif // _BaseVfs_H
