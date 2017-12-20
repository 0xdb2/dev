static char BaseObj_C [] = 
"@(#) db $Header: http://belov.homenet.org/svn/trunk/base/obj/BaseObj.C 250 2014-01-22 00:54:26Z db $";

#include <BaseObj.H>

/*
  $Log: not supported by cvs2svn $
  Revision 1.12  2001/07/02 23:47:59  db
  *** empty log message ***

  Revision 1.11  2000/12/14 00:42:28  db
  inStream -> onPrint

  Revision 1.10  2000/10/20 00:01:33  db
  *** empty log message ***

  Revision 1.9  2000/10/19 22:30:36  db
  onPrint adjusted

  Revision 1.8  2000/10/18 22:43:31  db
  *** empty log message ***

  Revision 1.7  2000/10/18 22:12:53  db
  test

  Revision 1.6  2000/10/18 21:14:53  db
  new interface for ostream insertion operator

  Revision 1.5  2000/10/18 00:35:38  db
  misc. fixes

  Revision 1.4  2000/10/17 23:33:41  db
  debug

  Revision 1.3  2000/10/17 22:47:10  db
  operator << added
*/

//============= BaseObj members ==========
unsigned long BaseObj::instance_count = 0; 

BaseObj::BaseObj(): timestamp(false)
{
    //Created  = time(0);
    //gettimeofday(&timestamp, NULL); // db@130926
    //coretime_now(timestamp);
    instance_count++;
    instance_num = instance_count;
    //cout << "construct of " << this -> ClassName() << ':' << this  << endl;
    //cout << "BaseObj::BaseObj: " << *this << endl;
    snprintf(signature, sizeof(signature), "%ld,%6ld#%ld", timestamp.tv_sec, timestamp.tv_nsec, instance_num); // db@131217 instead timestamp.tv_usec
}

BaseObj::~BaseObj()
{
//    instance_num = 0;
}

//char const * BaseObj::ClassName() const //db@121110: pure virtual now
//{
//	return("BaseObj");
//}

const coretime_t& BaseObj::GetCreated() const
{
    return timestamp;
}

const BaseObj& BaseObj::GetInstance() const
{
   return *this;
}
 
ostream& BaseObj::onPrint(ostream& os) const
{
    return os << "Instance: " << ClassName()
    << " ("         << timestamp.tv_sec << ',' << timestamp.tv_nsec // db@131217 instead timestamp.tv_usec
    << "#"          << instance_num 
    << "@"          << this        << ")"; 
}

ostream &operator<<( ostream& os, BaseObj const &obj )
{
    obj.onPrint(os);
    return os;
}

//============= BaseObj end =============

