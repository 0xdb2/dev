static char BaseTpl_C [] = "@(#) $Id: makefile 241 2013-11-08 14:18:08Z db $";

//#include <fcntl.h>

#include <BaseTrc.H>

EXCDEF(BaseTrc_unexpected);

//ostream& operator<< (ostream& os, const trc_& ts)
//{
//    os << "name=" << ts.name;
//    return os; 
//}

//============= BaseTrc members ==========
BaseTrc::BaseTrc() :
    trc(0)
{
    // struct init
}

BaseTrc::~BaseTrc()
{
    if (valid())
    {
        cout << ClassName() << ": trc=" << trc << endl;
    }
}

bool BaseTrc::create()
{
    if (valid())
    {
        EXCRAISE(BaseTrc_unexpected, "already exists");
    }

    //BaseTrc::set;
    trc = 1;
    return true;
}

//void set(const int, const void*, int);
//{
//        EXCRAISE(BaseTrc_unexpected, strerror(errno));
//}

//const BaseTrc& BaseTrc::operator << (const BaseStr& s) const
//{
//    if (! BaseTrc::send(s))
//    {
//        EXCRAISE("BaseTrc::send", strerror(errno));
//    }
//    return *this;
//}

//const BaseTrc& BaseTrc::operator >> (BaseStr& s) const
//{
//    if (BaseTrc::recv(s) == -1)
//    {
//        EXCRAISE("BaseTrc::recv", (errno? strerror(errno) : "unknown errror"));
//    }
//    return *this;
//}

//============= BaseTrc end =============

