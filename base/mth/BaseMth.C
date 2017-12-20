static char BaseTpl_C [] = "@(#) $Id: BaseMth.C 236 2013-10-27 14:51:43Z db $";

//#include <fcntl.h>

#include <BaseMth.H>

EXCDEF(BaseMth_unexpected);

int BaseMth::thcnt = 0;
const thread::id main_thid = this_thread::get_id();

#ifndef __RUNNABLE

#else
//ostream& operator<< (ostream& os, const mth_& ts)
//{
//    os << "name=" << ts.name;
//    return os; 
//}

//============= BaseMth members ==========
BaseMth::BaseMth() :
    thid(0)
{
    // struct init
}

BaseMth::~BaseMth()
{
    if (running())
    {
        cout << ClassName() << ": thid=" << thid << endl;
    }
}

bool BaseMth::create()
{
    if (running())
    {
        EXCRAISE(BaseMth_unexpected, "alredy running");
    }

    //BaseMth::set;
    thcnt++;
    thid = thcnt;
    return true;
}

#endif // __RUNNABLE
//void set(const int, const void*, int);
//{
//        EXCRAISE(BaseMth_unexpected, strerror(errno));
//}

//const BaseMth& BaseMth::operator << (const BaseStr& s) const
//{
//    if (! BaseMth::send(s))
//    {
//        EXCRAISE("BaseMth::send", strerror(errno));
//    }
//    return *this;
//}

//const BaseMth& BaseMth::operator >> (BaseStr& s) const
//{
//    if (BaseMth::recv(s) == -1)
//    {
//        EXCRAISE("BaseMth::recv", (errno? strerror(errno) : "unknown errror"));
//    }
//    return *this;
//}

//============= BaseMth end =============

