#ifndef _BaseMap_H
#define _BaseMap_H

static char BaseMap_H [] = "@(#) db $Id$";

//#ifndef __GXX_EXPERIMENTAL_CXX0X__
//#define __GXX_EXPERIMENTAL_CXX0X__
//#endif
//#include <type>

#include <BaseExc.H>
#include <BaseStr.H>
#include <BaseTime.H>
#include <map>

//static const int MAX = 4 * 1024;

EXCDEFEXT(BaseMap_unexpected);

class BaseMap: 
    public BaseObj, public map<BaseStr, BaseStr>
{
    public:
        BaseMap();
        //BaseMap(const BaseStr&);
        BaseMap(const char* mappedstr) { strtomap(mappedstr); };
        BaseMap(const BaseStr& mappedstr) { 
            const static string _myName = "BaseMap::BaseMaip(BaseStr)";
            LOG << "mappedstr=" << mappedstr << endl; 
            strtomap(mappedstr); 
        }
        BaseMap(const BaseMap&) = delete;
        BaseMap(BaseMap&&) = default;
        virtual ~BaseMap();
        BaseMap& operator= (const BaseMap&) =delete;
        BaseMap& operator= (const BaseStr&);
        BaseMap& operator= (const char* mappedstr);
        operator BaseStr() const; 
        virtual char const * ClassName() const { return "BaseMap"; };

        bool create();
        //void set(const int, const void*, int);
        
        //virtual const BaseMap& operator << (const BaseStr&) const=0;
        //virtual const BaseMap& operator >> (BaseStr&) const=0;

        bool valid() const { return map; }
        friend ostream& operator<< (ostream& os, const BaseMap&);
        virtual ostream& onPrint(ostream& os) const { 
            os << "map=" << map; 
            return os; 
        };
    private:
        void strtomap(const BaseStr& mappedstr);
        int map;
        // struct map_;
}; 
// BaseMap end

typedef BaseMap::iterator BaseMapIter;

//ostream& operator<< (ostream&, const struct map_&);

//======= Masros
//======= 
#endif // _BaseMap_H
