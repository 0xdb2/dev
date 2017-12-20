#include <BaseTime.H>

/** Konvertiert gegliederte UTC-Angaben in Unix-Zeit.
 * Parameter und ihre Werte-Bereiche:
 * - jahr [1970..2038]
 * - monat [1..12]
 * - tag [1..31]
 * - stunde [0..23]
 * - minute [0..59]
 * - sekunde [0..59]
long unixzeit(int jahr, int monat, int tag, int stunde, int minute, int sekunde)
string timetostr(long long unix_time)
{
    const short days_since_year[12] = {0,31,59,90,120,151,181,212,243,273,304,334};
 
    //long unix_zeit;
    //long years=year-1970;
    
    int leap_year=((year-1)-1968)/4 - ((year-1)-1900)/100 + ((year-1)-1600)/400;
 
    unix_time=second + (60*minute) + (60*60*hour) + 

    (leap_year[month-1]+day-1)*60*60*24 +

    (jahre*365+schaltjahre)*60*60*24;
 
    if ( (month>2) && (year%4==0 && (year%100!=0 || year%400==0)) )
        unix_time+=60*60*24; // +Schalttag wenn jahr Schaltjahr ist
 
    return unix_time;
*/

int main (void) 
{
    coretime_t time = BaseTime().GetTime();
    cout << "System Time: sec:" << time.tv_sec << " nsec:" << time.tv_nsec << endl;
    cout << "Time to string test >" << BaseTime("_%y%m%d_%H%M%S.%.4ms").Now() << "<" << endl;
    cout << "Time to string test >" << BaseTime("_%y%m%d_%H%M%S.%.5ms").Now() << "<" << endl;
    cout << "Time to string test >" << BaseTime("_%y%m%d_%H%M%S.%.6ms").Now() << "<" << endl;
    cout << "Time to string test >" << BaseTime("_%y%m%d_%H%M%S.%.7ms").Now() << "<" << endl;
    cout << "Time to string test >" << BaseTime("_%y%m%d_%H%M%S.%.10:ms").Now() << "<" << endl;
            
    exit(0);
    struct tm tm = uts2tm(time.tv_sec);
    cout << "ut2tm: " << tm << endl;
    for (int sec=time.tv_sec; sec<time.tv_sec+10; sec++)
    {
        tm = uts2tm(sec);
        cout << "ut2tm: sec=" << sec << " " << tm << endl;
    }
    unsigned long long sec[5] = { 0, 0x7FFFFFFFFFFFFFFFL, 0xFFFFFFFFFFFFFFFFL, 0x7FFFFFFFL, 0xFFFFFFFFL};
    for (int i=0; i<5; i++)
    {
        tm = uts2tm(sec[i]);
        cout << "ut2tm (max values): sec=" << sec[i] << " " << tm << endl;
    }

    return 0;
}

