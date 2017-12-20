#ifndef _BaseIpc_H
#define _BaseIpc_H

static char BaseTpl_H [] = "@(#) db $Id: makefile 241 2013-11-08 14:18:08Z db $";

//#ifndef __GXX_EXPERIMENTAL_CXX0X__
//#define __GXX_EXPERIMENTAL_CXX0X__
//#endif
//#include <type>

#include <BaseExc.H>
#include <BaseStr.H>
#include <BaseTime.H>

static const int MAX = 4 * 1024;

EXCDEFEXT(BaseIpc_unexpected);

class BaseIpc: 
    public BaseObj //, 
    //public type
{
    public:
        BaseIpc();
        BaseIpc(const BaseIpc&) = delete;
        BaseIpc(BaseIpc&&) = default;
        virtual ~BaseIpc();
        BaseIpc& operator= (const BaseIpc&) = delete;
        virtual char const * ClassName() const { return "BaseIpc"; };

        bool create();
        //void set(const int, const void*, int);
        
        //virtual const BaseIpc& operator << (const BaseStr&) const=0;
        //virtual const BaseIpc& operator >> (BaseStr&) const=0;

        bool valid() const { return ipc; }
        //friend ostream& operator<< (ostream& os, const struct&);
        virtual ostream& onPrint(ostream& os) const { 
            os << "ipc=" << ipc; 
            return os; 
        };
    private:
        int ipc;
        // struct ipc_;
}; 
// BaseIpc end

//ostream& operator<< (ostream&, const struct ipc_&);

//======= Masros
//======= 
#endif // _BaseIpc_H
