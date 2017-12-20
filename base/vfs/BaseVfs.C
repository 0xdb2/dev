static char BaseVfs_C [] =
"@(#) db $Header: http://belov.homenet.org/svn/trunk/base/vfs/BaseVfs.C 480 2016-07-08 16:21:48Z db $";

#include <BaseVfs.H>

/*
   $Log: not supported by cvs2svn $
   Revision 1.9  2000/11/04 23:57:12  db
 *** empty log message ***

 Revision 1.7  2000/11/01 20:14:02  db
 tuning

 Revision 1.6  2000/10/31 01:13:23  db
 with switch of file

 Revision 1.5  2000/10/29 22:25:11  db
 jet another streams test

 Revision 1.4  2000/10/25 23:31:33  db
 some tests

 Revision 1.3  2000/10/24 22:52:38  db
 test overflow

 Revision 1.2  2000/10/24 22:03:23  db
 added class BaseVfs

 Revision 1.1  2000/10/24 20:45:46  db
 *** empty log message ***

*/

//#include <iostream>
//#include <fstream>

//============= BaseVfsBuf members ==========
//BStorable::BStorable() : Bobj()
//{
//}
BaseVfsBuf::BaseVfsBuf(void) :
    fdbuf(2, ios::out | ios::app),
    maxsize(0),
    cursize(0),
    fname(NULL)

{
    //attach(2);
    //int fd = dup(2); // db@121017
    //fdbuf fdb(fd, ios::app); // ????
    //std::streambuf::rdbuf(&fdb);
}

BaseVfsBuf::BaseVfsBuf(const char * fn, int ms) :
    maxsize(ms),
    cursize(0),
    fname(NULL)
{
    open(fn, ios::app);
}

void BaseVfsBuf::Open(const char * fn, int ms)
{
    if (fname)
    {
        delete fname;
    }
    fname = new char[strlen(fn) + 1];
    strcpy(fname, fn);
    maxsize = ms;
    cursize = 0;

    open(fn, ios::out | ios::app);
}

//filebuf *BaseVfsBuf::open(const char * fn, int omode, int openprot)
filebuf *BaseVfsBuf::open(const char * fn, int om) // db@121017
{
    switchfile(OPEN_NEW_FILE);

    return this;
}

BaseVfsBuf::~BaseVfsBuf()
{
    if (fname)
    {
        delete fname;
        fname = NULL;
    }
}

int BaseVfsBuf::overflow(int ch)
{
    // db-20160708 (int) char* loses precision on 64bit -> intptr_t
    streamsize  strlen = pptr () - pbase ();
    cerr << "BaseVfsBuf::overflow:" << "ch=" << ch << ",strlen=" << strlen << ",base(ptr)=" << (intptr_t)pbase() << "(" << (intptr_t)pptr() << ")" << ",cursize=" << cursize << endl;
    //int ret = filebuf::overflow(ch);
    int ret = fdbuf::overflow(ch);
    int wrote = strlen - (pptr () - pbase ());
    cerr << "fdbuf::overflow=" << ret << ":ch=" << ch << ",wrote=" << wrote << ",base(ptr)=" << (intptr_t)pbase() << "(" << (intptr_t)pptr() << ")" << ",fd=" << fd() << endl;
    if (fd() == 1) // when the file is opened
    {
        if (wrote > 0)
        {
            cursize += wrote;

        }

        if (cursize >= maxsize)
        {
            cursize = 0;
            switchfile();
            // the last buf is already wrote throw recursiv call of sys_write (sync() in switchfile())
            // we must return only size (?) and should't nothing write to the stream at this point
            return ret;
        }

        if (cursize > 100)
        {
            cerr << "BaseVfsBuf::overflow: throw 77"  << endl;
            throw 77;
        }
    }
    return ret;
}

/*
   char *b, *s, *p, *e;

   b = base();
   s = pbase ();
   p = pptr ();
   e = epptr();

   if (pptr () == epptr())
   {
   if (allocate() == 0)
   {
   return EOF;
   }
   }
   b = base();
   s = pbase ();
   p = pptr ();
   e = epptr();

   streamsize n = pptr () - pbase ();
   return 0;

   streamsize n = pptr () - pbase ();
   int ret = filebuf::overflow(ch);
   int ret = writefile(pbase(), n);
   return 0;
   */

streamsize BaseVfsBuf::xsputn(const char* buf, streamsize size)
{
    //streamsize ret = filebuf::xsputn(buf, size);
    streamsize ret = fdbuf::xsputn(buf, size);
    //cerr << "BaseVfsBuf::xsputn: throw 99" << endl;
    //throw 99;
    return ret;
}

/* db@121020
   streamsize BaseVfsBuf::sys_write(const char* buf, streamsize size)
   {
   if (fd() == 1) // when the file is not opened
   {
   if ((cursize + size) >= maxsize)
   {
   cursize = 0;
   switchfile();
// the last buf is already wrote throw recursiv call of sys_write (sync() in switchfile())
// we must return only size (?) and should't nothing write to the stream at this point
return size;
}
}

streamsize wrote = filebuf::sys_write(buf, size);
cursize += wrote;
return wrote;
}
*/

void BaseVfsBuf::switchfile(bool newfile)
{
    int ret = 0;
    ret = ret;

    ret = BaseVfsBuf::sync();
    if (newfile)
    {
        cerr << "Opening log file: " << fname << endl;
    }
    else
    {
        cerr << "================this file is now closed=====================" << endl;
        //filebuf::sys_write(buf, strlen(buf));
    }

    fdbuf::close(); // filebuf member
    ret = ::close(1);
    ret = ::close(2);

    if (unlink(fname))
    {
        //perror("unlink");
        fprintf(fopen("/tmp/BaseVfsBuf_error", "a"), "%s: unlink: fname=%s err=%s\n", BaseTime("%y%m%d_%H%M%S").Now(), fname, strerror(errno));
        fcloseall();
    }

    //filebuf::open(fname, ios::app);
    fdbuf::open(fname, ios::out | ios::app);
    if (! fdbuf::is_open())
    {
        fprintf(fopen("/tmp/BaseVfsBuf_error", "a"), "%s: fdbuf::open: cannot open fname=%s\n", BaseTime("%y%m%d_%H%M%S").Now(), fname);
        exit(2);
    }

    if (fd() != 1)
    {
        //cerr << "open: wrong file descriptor. fd=" << fd() << endl;
        //return;
        fprintf(fopen("/tmp/BaseVfsBuf_error", "a"), "%s: wrong file descriptor fd=%d for fname=%s\n", BaseTime("%y%m%d_%H%M%S").Now(), fd(), fname);
        exit(1);
    }

    ret = dup2(1, 2);
    link();

    cursize = seekoff(0, ios::end);
}


void  BaseVfsBuf::link(void)
{
    char const * time = BaseTime("_%y%m%d_%H%M%S").Now();
    char * linkname = new char[ strlen(fname) + strlen(time) + 1 ];

    strcpy(linkname, fname);
    strcat(linkname, time);

    if (::link(fname, linkname))
    {
        cerr << "link:error:" << errno << ": " << strerror(errno) << endl; // db@121027: sys_errlist->strerror
    }

    delete linkname;
}


char const * BaseVfsBuf::ClassName() const
{
    return("BaseVfsBuf");
}
//============= BaseVfsBuf end ==============

//============= BaseVfs members =============

BaseVfs::BaseVfs()
{
    init(&buf);
    //rdbuf(&buf);
    exceptions(ios::badbit|ios::failbit);
}

BaseVfs::BaseVfs(const char * fn, int ms):
    buf(fn, ms)
{
    init(&buf);
    //rdbuf(&buf);
    exceptions(ios::badbit|ios::failbit);
}

BaseVfs::~BaseVfs()
{
}

char const * BaseVfs::ClassName() const
{
    return("BaseVfs");
}

//============= BSream end ==========
