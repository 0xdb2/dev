static char BaseTime_C [] = "@(#) db $Header: http://belov.homenet.org/svn/trunk/base/time/BaseTime.C 250 2014-01-22 00:54:26Z db $";

#include <BaseTime.H>

/*
   $Log: not supported by cvs2svn $
   Revision 1.1  2000/10/31 01:20:50  db
 *** empty log message ***

*/

#define SIZE 256

//============= BaseTime members =============

BaseTime::BaseTime()
{
    format = 0;
}

BaseTime::BaseTime(const char * fmt)
{
    format = new char[strlen(fmt)+1];
    strcpy(format, fmt);
}

BaseTime::~BaseTime()
{
    if (format)
    {
        delete format;
    }
}

char const * BaseTime::Now(void) 
{
    now();
    return time_buf;
}

const coretime_t& BaseTime::GetTime(void) 
{
    now();
    return time;
}

void BaseTime::now(void) 
{
    struct tm *tm_time;

    //gettimeofday( &sys_time, NULL);
    //coretime_now(time);
    time.now();
    tm_time = localtime( &time.tv_sec);
    if (format)
    {
        //strftime (time_buf, SIZE, format, tm_time); 
        // "_%y%m%d_%H%M%S.%.3"; %.n - tv_usec rounded to n-digit 
        char * usecfmt = strstr(format, "%.");
        if (usecfmt)
        {
            char * tmp = strndup(format, usecfmt-format);
            //cout << "usec=" << sys_time.tv_usec << ",usecfmt=" << usecfmt << '(' << float(sys_time.tv_usec)/1000000 << ')' <<endl;
            int len = strftime (time_buf, SIZE, tmp, tm_time); 
            //sprintf(tmp, "%.*f%s", atoi(usecfmt+2), float(sys_time.tv_usec)/1000000, usecfmt+3); 
            sprintf(tmp, "%.*f%s", atoi(usecfmt+2), float(time.tv_nsec)/1000000000LL, usecfmt+3); 
            //strncat(time_buf, tmp+2, 6); // without '0.'
            strcat(time_buf, tmp+2); // without '0.'
        } 
        else
        {
            strftime (time_buf, SIZE, format, tm_time);
        }
    }
}
struct xtm
{
    unsigned int year, mon, day, hour, min, sec;
};

/* struct tm
 * tm_sec   int     seconds after the minute 0-60*
 * tm_min   int     minutes after the hour   0-59
 * tm_hour  int     hours since midnight     0-23
 * tm_mday  int     day of the month         1-31
 * tm_mon   int     months since January     0-11
 * tm_year  int     years since 1900        
 * tm_wday  int     days since Sunday        0-6
 * tm_yday  int     days since January 1     0-365
 * tm_isdst int     Daylight Saving Time flag       
*/

ostream& operator << (ostream& os, const tm& tm) { 

    //ios_base::fmtflags fmtflags = os.flags();
    //os.width(2);
    //os.fill('x');
    char buf[100];
    snprintf(buf, sizeof(buf), "%d.%02d.%02d %02d:%02d:%02d", tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    os << "tm=" << buf; 
    //os << tm.year << '.' << tm.mon << '.' << tm.day << ' ' << tm.hour << ':' << tm.min << ':' << tm.sec; 
    //os.flags(fmtflags);
    return os; 
}

struct tm uts2tm(unsigned long long unixtime)
{
    struct tm tm;
    static const int days_befor_epoch_since_ca = 719499;
    /* First take out the hour/minutes/seconds - this part is easy. */
    tm.tm_sec = unixtime % 60;
    unixtime = unixtime / 60;
    tm.tm_min = unixtime % 60;
    unixtime = unixtime / 60;
    tm.tm_hour = unixtime % 24;
    unixtime = unixtime / 24;

    // unixtime is seconds since 01.01.1970 UTC -> Rebaseline to the Common Era (CA) 01.01.1970 = 01.01.0001 + 719499 days   
    long numdays = (unixtime) + days_befor_epoch_since_ca;

    /* Roll forward looking for the year.  
    *  This could be done more efficiently but this will do.  
    *  We have to start at 1969 because the year we calculate here runs from March 
    *  - so January and February 1970 will come out as 1969 here.
    */
    for ( tm.tm_year = 1969; numdays > YEAR_TO_DAYS(tm.tm_year + 1) + 30; tm.tm_year++);

    /* OK we have our "year", so subtract off the days accounted for by full years. */
    numdays -= YEAR_TO_DAYS(tm.tm_year);

    /* numdays is now number of days we are into the year (remembering that March 1 is the first day of the "year" still). */
    /* Roll forward looking for the month.  1 = March through to 12 = February. */
    for (tm.tm_mon = 1; tm.tm_mon < 12 && numdays > 367*(tm.tm_mon+1)/12; tm.tm_mon++);

    /* Subtract off the days accounted for by full months */
    numdays -= 367*tm.tm_mon/12;
    tm.tm_mday = numdays;
    /* numdays is now number of days we are into the month */

    /* Adjust the month/year so that 1 = January, and years start where we usually expect them to. */
    tm.tm_mon += 2;
    if (tm.tm_mon > 12)
    {
        tm.tm_mon -= 12;
        tm.tm_year++;
    }
    return tm;
}

//============= BaseTime end ==========
