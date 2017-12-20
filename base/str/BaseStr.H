#ifndef _BaseStr_H
#define _BaseStr_H

static char BaseStr_H [] = "@(#) db $Header: http://belov.homenet.org/svn/trunk/base/str/BaseStr.H 247 2013-12-30 07:34:12Z db $";

/*
  $Log: not supported by cvs2svn $
  Revision 1.1  2000/12/13 23:46:10  db
  *** empty log message ***

*/

#include <BaseObj.H>

class BaseStr: 
    public BaseObj,
    public string
{
    public:
	BaseStr(): string() {};
	BaseStr(const BaseStr& o) : string(o) {};
	BaseStr(const char * s) : string(s) {};
	BaseStr(char c) : string(1, c) {};
	BaseStr(const string s) : string(s) {};
	virtual ~BaseStr() {};
	virtual char const * ClassName() const { return "BaseStr"; };
	virtual ostream& onPrint ( ostream& ) const;

        operator const char*() const { return data(); };
        BaseStr operator+ (const char* s) { return BaseStr(*this).append(s); };
        BaseStr operator+ (const int& i) { return BaseStr(*this).append(to_string(i)); };
        BaseStr operator+ (const BaseStr& s) {  return BaseStr(*this).append(s); };
        bool operator== (const BaseStr& o) { return compare(o) == 0; };
        bool operator== (const char* s) { return compare(s) == 0; };
        const char& operator[] (int pos) const { return at(pos); };
        char& operator[] (int pos) { return at(pos); };
        BaseStr operator() (size_t pos, size_t len) const { return BaseStr().assign(substr(pos, len)); };
   
//private:
	// private member
}; 
// BaseStr end

extern const BaseStr operator+ (const char*, const BaseStr&);

//======= Masros
//======= 
#endif // _BaseStr_H
