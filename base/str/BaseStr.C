static char BaseStr_C [] = "@(#) db $Header: http://belov.homenet.org/svn/trunk/base/str/BaseStr.C 247 2013-12-30 07:34:12Z db $";

#include <BaseStr.H>
#include <BaseExc.H>

/*
  $Log: not supported by cvs2svn $
  Revision 1.1  2000/12/13 23:46:10  db
  *** empty log message ***

*/

//============= BaseStr members ==========

ostream& BaseStr::onPrint(ostream& os) const
{
     os.write (data (), length ());
     return os;
}
//============= BaseStr end =============

const BaseStr operator+ (const char* s, const BaseStr& o) { BaseStr ret(s); ret.append(o); return move(ret); };

