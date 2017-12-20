#ifndef _BaseTime_H
#define _BaseTime_H

static char BaseTime_H [] = "@(#) dbE $Header: http://belov.homenet.org/svn/trunk/base/time/BaseTime.H 247 2013-12-30 07:34:12Z db $";

/*
   $Log: not supported by cvs2svn $
   Revision 1.2  2000/10/31 01:24:49  db
 *** empty log message ***

*/

#define SIZE 256 

#define TS BaseTime("%H%M%S.%.3:").Now()
#define DT BaseTime("%y%m%d_%H%M%S:").Now()

#include <BaseObj.H>

class BaseTime: public BaseObj
{
    public:
        BaseTime(void);
        BaseTime(const char * fmt);
        virtual ~BaseTime();
        char const* Now(void);
        const coretime_t&  GetTime(void);
        char const * ClassName(void) const { return "BaseTime"; }
    protected:
        virtual void now(void); 
    private:
        coretime_t time;
        char * format;
        char time_buf[SIZE];
}; 
// BaseTime end

#define YEAR_TO_DAYS(y) ((y)*365 + (y)/4 - (y)/100 + (y)/400)
struct tm uts2tm(unsigned long long unixtime);
ostream& operator << (ostream& os, const tm& tm);

#endif // _BaseTime_H
