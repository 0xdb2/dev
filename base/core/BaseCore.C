static char BaseCore_C [] = "@(#) $Id$";

//#include <fcntl.h>

#include <BaseCore.H>

void coretime_now(coretime_t& coretime)
{
    clock_gettime(CLOCK_REALTIME, &coretime);
}

ostream& operator<< (ostream& os, coretime_t& time) 
{ 
    static char timebuf[21], decbuf[12];
    if (time.autots) time.now();
    int len = strftime (timebuf, sizeof(timebuf), "%Y%m%d_%H%M%S", localtime(&time.tv_sec));
    //return os << timebuf << '(' << time.tv_sec << " sec)." << (time.tv_nsec/1000000); 
    //return os << timebuf << "." << (time.tv_nsec/1000000);
    sprintf(decbuf, "%.*f", time.width, float(time.tv_nsec)/1000000000LL);
    //return os << timebuf << (decbuf+1) << '(' << float(time.tv_nsec)/1000000000LL << " " << time.tv_nsec << " nsec)."; 
    return os << timebuf << (decbuf+1); 
}
//============= BaseCore end =============

