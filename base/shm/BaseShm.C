static char BaseTpl_C [] = "@(#) $Id: BaseShm.C 233 2013-10-21 21:18:19Z db $";

//#include <fcntl.h>

#include <BaseShm.H>

EXCDEF(BaseShm_unexpected);

//ostream& operator<< (ostream& os, const shm_& ts)
//{
//    os << "name=" << ts.name;
//    return os; 
//}

//============= BaseShm members ==========
BaseShm::BaseShm() :
    shm(0)
{
    // struct init
}

BaseShm::~BaseShm()
{
    if (valid())
    {
        cout << ClassName() << ": shm=" << shm << endl;
    }
}

bool BaseShm::create()
{
    if (valid())
    {
        EXCRAISE(BaseShm_unexpected, "already exists");
    }

    //BaseShm::set;
    shm = 1;
    return true;
}

//void set(const int, const void*, int);
//{
//        EXCRAISE(BaseShm_unexpected, strerror(errno));
//}

//const BaseShm& BaseShm::operator << (const BaseStr& s) const
//{
//    if (! BaseShm::send(s))
//    {
//        EXCRAISE("BaseShm::send", strerror(errno));
//    }
//    return *this;
//}

//const BaseShm& BaseShm::operator >> (BaseStr& s) const
//{
//    if (BaseShm::recv(s) == -1)
//    {
//        EXCRAISE("BaseShm::recv", (errno? strerror(errno) : "unknown errror"));
//    }
//    return *this;
//}

//============= BaseShm end =============

