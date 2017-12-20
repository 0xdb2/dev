static char BaseTpl_C [] = "@(#) $Id: BasePth.C 245 2013-12-02 13:18:18Z db $";

//#include <fcntl.h>

#include <BasePth.H>

EXCDEF(BasePth_unexpected);

//int BasePth::threadcnt = 0;
const thread::id main_thid = this_thread::get_id();

#ifndef __RUNNABLE

#else
//ostream& operator<< (ostream& os, const pth_& ts)
//{
//    os << "name=" << ts.name;
//    return os; 
//}

//============= BasePth members ==========
BasePth::BasePth() :
    thid(0)
{
    // struct init
}

BasePth::~BasePth()
{
    if (running())
    {
        cout << ClassName() << ": thid=" << thid << endl;
    }
}

bool BasePth::create()
{
    if (running())
    {
        EXCRAISE(BasePth_unexpected, "alredy running");
    }

    //BasePth::set;
    thcnt++;
    thid = thcnt;
    return true;
}

#endif // __RUNNABLE
//void set(const int, const void*, int);
//{
//        EXCRAISE(BasePth_unexpected, strerror(errno));
//}

//const BasePth& BasePth::operator << (const BaseStr& s) const
//{
//    if (! BasePth::send(s))
//    {
//        EXCRAISE("BasePth::send", strerror(errno));
//    }
//    return *this;
//}

//const BasePth& BasePth::operator >> (BaseStr& s) const
//{
//    if (BasePth::recv(s) == -1)
//    {
//        EXCRAISE("BasePth::recv", (errno? strerror(errno) : "unknown errror"));
//    }
//    return *this;
//}

//============= BasePth end =============

