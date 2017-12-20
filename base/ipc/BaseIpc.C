static char BaseTpl_C [] = "@(#) $Id: makefile 241 2013-11-08 14:18:08Z db $";

//#include <fcntl.h>

#include <BaseIpc.H>

EXCDEF(BaseIpc_unexpected);

//ostream& operator<< (ostream& os, const ipc_& ts)
//{
//    os << "name=" << ts.name;
//    return os; 
//}

//============= BaseIpc members ==========
BaseIpc::BaseIpc() :
    ipc(0)
{
    // struct init
}

BaseIpc::~BaseIpc()
{
    if (valid())
    {
        cout << ClassName() << ": ipc=" << ipc << endl;
    }
}

bool BaseIpc::create()
{
    if (valid())
    {
        EXCRAISE(BaseIpc_unexpected, "already exists");
    }

    //BaseIpc::set;
    ipc = 1;
    return true;
}

//void set(const int, const void*, int);
//{
//        EXCRAISE(BaseIpc_unexpected, strerror(errno));
//}

//const BaseIpc& BaseIpc::operator << (const BaseStr& s) const
//{
//    if (! BaseIpc::send(s))
//    {
//        EXCRAISE("BaseIpc::send", strerror(errno));
//    }
//    return *this;
//}

//const BaseIpc& BaseIpc::operator >> (BaseStr& s) const
//{
//    if (BaseIpc::recv(s) == -1)
//    {
//        EXCRAISE("BaseIpc::recv", (errno? strerror(errno) : "unknown errror"));
//    }
//    return *this;
//}

//============= BaseIpc end =============

