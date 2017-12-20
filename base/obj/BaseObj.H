#ifndef _BaseObj_H
#define _BaseObj_H

static char BaseObj_H [] = "@(#) db $Header: http://belov.homenet.org/svn/trunk/base/obj/BaseObj.H 247 2013-12-30 07:34:12Z db $";

/*
   $Log: not supported by cvs2svn $
   Revision 1.10  2000/12/14 00:42:28  db
   inStream -> onPrint

   Revision 1.9  2000/10/19 22:30:36  db
   onPrint adjusted

   Revision 1.8  2000/10/18 21:28:16  db
   without abstract functions

   Revision 1.7  2000/10/18 21:14:53  db
   new interface for ostream insertion operator

   Revision 1.6  2000/10/17 23:15:19  db
   operator << as friend

   Revision 1.5  2000/10/17 22:47:10  db
   operator << added
   */

#include <BaseCore.H>

using namespace std;

class BaseObj
{
    public:
        BaseObj();
        BaseObj(const BaseObj& other)
        { 
            cout << "BaseObj::BaseObj(" << other << ")" << endl;
            timestamp = other.timestamp;
            instance_num=other.instance_num; 
        }
        virtual ~BaseObj();
        //virtual char const * ClassName() const;
        virtual char const * ClassName() const = 0; //db@121110: pure virtual now 
        virtual  const BaseObj&  operator= (BaseObj const &other) const // avoid copy BaseObj private members
        {
            cout << "BaseObj::operator=(" << other << ")" << endl;
            return *this;
        };
        bool operator== (const BaseObj& other)
        {
            return (
                this->instance_num == other.instance_num &&
                this->timestamp.tv_sec == other.timestamp.tv_sec &&
                this->timestamp.tv_nsec == other.timestamp.tv_nsec // db@131217 instead tv_usec
                );
        };
        const coretime_t& GetCreated() const; 
        const BaseObj& GetInstance() const; 
        virtual ostream& onPrint ( ostream& ) const;
        friend ostream& operator<<( ostream&, BaseObj const& ); 
    private:
        coretime_t timestamp;			
        unsigned long instance_num;
        static unsigned long instance_count;
        char signature[sizeof(timeval) + 1 + sizeof(long) + 1];  
}; 
// BaseObj end

#endif // _BaseObj_H
