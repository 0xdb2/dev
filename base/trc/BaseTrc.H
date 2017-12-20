#ifndef _BaseTrc_H
#define _BaseTrc_H

static char BaseTpl_H [] = "@(#) db $Id: makefile 241 2013-11-08 14:18:08Z db $";

//#ifndef __GXX_EXPERIMENTAL_CXX0X__
//#define __GXX_EXPERIMENTAL_CXX0X__
//#endif
//#include <type>

#include <BaseExc.H>
#include <BaseStr.H>
#include <BaseTime.H>

static const int MAX = 4 * 1024;

EXCDEFEXT(BaseTrc_unexpected);

class BaseTrc: 
    public BaseObj //, 
    //public type
{
    public:
        BaseTrc();
        BaseTrc(const BaseTrc&) = delete;
        BaseTrc(BaseTrc&&) = default;
        virtual ~BaseTrc();
        BaseTrc& operator= (const BaseTrc&) = delete;
        virtual char const * ClassName() const { return "BaseTrc"; };

        bool create();
        //void set(const int, const void*, int);
        
        //virtual const BaseTrc& operator << (const BaseStr&) const=0;
        //virtual const BaseTrc& operator >> (BaseStr&) const=0;

        bool valid() const { return trc; }
        //friend ostream& operator<< (ostream& os, const struct&);
        virtual ostream& onPrint(ostream& os) const { 
            os << "trc=" << trc; 
            return os; 
        };
    private:
        int trc;
        // struct trc_;
}; 
// BaseTrc end

//ostream& operator<< (ostream&, const struct trc_&);

//======= Masros
//======= 
#endif // _BaseTrc_H
