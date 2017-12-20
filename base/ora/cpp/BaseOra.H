#ifndef _BaseOra_H
#define _BaseOra_H

static char BaseOra_H [] = "@(#) db $Id$";

//#ifndef __GXX_EXPERIMENTAL_CXX0X__
//#define __GXX_EXPERIMENTAL_CXX0X__
//#endif
//#include <type>

#include <BaseExc.H>
#include <BaseStr.H>
#include <BaseTime.H>

static const int MAX = 4 * 1024;

EXCDEFEXT(BaseOra_unexpected);

class BaseOra: 
    public BaseObj //, 
    //public type
{
    public:
        BaseOra();
        BaseOra(const BaseOra&) = delete;
        BaseOra(BaseOra&&) = default;
        virtual ~BaseOra();
        BaseOra& operator= (const BaseOra&) = delete;
        virtual char const * ClassName() const { return "BaseOra"; };

        // basic Connect Commit Rollback Disconnect
        bool Connect(const BaseStr& _connstr);
        const BaseStr& Execute(const BaseStr&, const BaseStr&, const BaseStr&);
        void Commit(void) const;
        void Rollback(void) const;
        void Disconnect(void);

        //void set(const int, const void*, int);
        static void ErrHandler(const BaseStr&);
        
        //virtual const BaseOra& operator << (const BaseStr&) const=0;
        //virtual const BaseOra& operator >> (BaseStr&) const=0;

        bool connected(void) const { return _connected; };
        void connected(bool setval) { _connected = setval; }
        //friend ostream& operator<< (ostream& os, const struct&);
        virtual ostream& onPrint(ostream& os) const { 
            os << "connected=" << _connected << " connstr=" << _connstr; 
            return os; 
        };
        //static ErrHandler(void);
    private:
        bool _connected;
        BaseStr _connstr;
        struct { 
            BaseStr Ret;
            BaseStr OutPar; 
            BaseStr Msg;
        } pls;
}; 
// BaseOra end

//ostream& operator<< (ostream&, const struct ora_&);

//======= Masros
//======= 
#endif // _BaseOra_H
