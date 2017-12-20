static char BaseTpl_C [] = "@(#) $Id: BaseTpl.C 232 2013-10-21 14:57:30Z db $";

//#include <fcntl.h>

#include <BaseTpl.H>

EXCDEF(BaseTpl_unexpected);

//ostream& operator<< (ostream& os, const tpl_& ts)
//{
//    os << "name=" << ts.name;
//    return os; 
//}

//============= BaseTpl members ==========
BaseTpl::BaseTpl() :
    tpl(0)
{
    // struct init
}

BaseTpl::~BaseTpl()
{
    if (valid())
    {
        cout << ClassName() << ": tpl=" << tpl << endl;
    }
}

bool BaseTpl::create()
{
    if (valid())
    {
        EXCRAISE(BaseTpl_unexpected, "already exists");
    }

    //BaseTpl::set;
    tpl = 1;
    return true;
}

//void set(const int, const void*, int);
//{
//        EXCRAISE(BaseTpl_unexpected, strerror(errno));
//}

//const BaseTpl& BaseTpl::operator << (const BaseStr& s) const
//{
//    if (! BaseTpl::send(s))
//    {
//        EXCRAISE("BaseTpl::send", strerror(errno));
//    }
//    return *this;
//}

//const BaseTpl& BaseTpl::operator >> (BaseStr& s) const
//{
//    if (BaseTpl::recv(s) == -1)
//    {
//        EXCRAISE("BaseTpl::recv", (errno? strerror(errno) : "unknown errror"));
//    }
//    return *this;
//}

//============= BaseTpl end =============

