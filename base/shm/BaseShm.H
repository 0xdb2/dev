#ifndef _BaseShm_H
#define _BaseShm_H

static char BaseTpl_H [] = "@(#) db $Id: BaseShm.H 233 2013-10-21 21:18:19Z db $";

//#ifndef __GXX_EXPERIMENTAL_CXX0X__
//#define __GXX_EXPERIMENTAL_CXX0X__
//#endif
//#include <type>

#include <BaseExc.H>
#include <BaseStr.H>

static const int MAX = 4 * 1024;

EXCDEFEXT(BaseShm_unexpected);

class BaseShm: 
    public BaseObj //, 
    //public type
{
    public:
        BaseShm();
        virtual ~BaseShm();
        virtual char const * ClassName() const { return "BaseShm"; };

        bool create();
        //void set(const int, const void*, int);
        
        //virtual const BaseShm& operator << (const BaseStr&) const=0;
        //virtual const BaseShm& operator >> (BaseStr&) const=0;

        bool valid() const { return shm; }
        //friend ostream& operator<< (ostream& os, const struct&);
        virtual ostream& onPrint(ostream& os) const { 
            os << "shm=" << shm; 
            return os; 
        };
    private:
        int shm;
        // struct shm_;
}; 
// BaseShm end

//ostream& operator<< (ostream&, const struct shm_&);

//======= Masros
//======= 
#endif // _BaseShm_H
