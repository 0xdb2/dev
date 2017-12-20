#ifndef _BaseTpl_H
#define _BaseTpl_H

static char BaseTpl_H [] = "@(#) db $Id: BaseTpl.H 241 2013-11-08 14:18:08Z db $";

//#ifndef __GXX_EXPERIMENTAL_CXX0X__
//#define __GXX_EXPERIMENTAL_CXX0X__
//#endif
//#include <type>

#include <BaseExc.H>
#include <BaseStr.H>
#include <BaseTime.H>

static const int MAX = 4 * 1024;

EXCDEFEXT(BaseTpl_unexpected);

class BaseTpl: 
    public BaseObj //, 
    //public type
{
    public:
        BaseTpl();
        BaseTpl(const BaseTpl&) = delete;
        BaseTpl(BaseTpl&&) = default;
        virtual ~BaseTpl();
        BaseTpl& operator= (const BaseTpl&) = delete;
        virtual char const * ClassName() const { return "BaseTpl"; };

        bool create();
        //void set(const int, const void*, int);
        
        //virtual const BaseTpl& operator << (const BaseStr&) const=0;
        //virtual const BaseTpl& operator >> (BaseStr&) const=0;

        bool valid() const { return tpl; }
        //friend ostream& operator<< (ostream& os, const struct&);
        virtual ostream& onPrint(ostream& os) const { 
            os << "tpl=" << tpl; 
            return os; 
        };
    private:
        int tpl;
        // struct tpl_;
}; 
// BaseTpl end

//ostream& operator<< (ostream&, const struct tpl_&);

//======= Masros
//======= 
#endif // _BaseTpl_H
