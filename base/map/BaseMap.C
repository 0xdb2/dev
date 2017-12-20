static char BaseTpl_C [] = "@(#) $Id: makefile 241 2013-11-08 14:18:08Z db $";

//#include <fcntl.h>

#include <BaseMap.H>
#include <sstream>

EXCDEF(BaseMap_unexpected);

ostream& operator<< (ostream& os, const BaseMap& map)
{
    os << "map(" << map.size() << ")=<" << BaseStr(map) << ">";
    return os; 
}

//============= BaseMap members ==========
BaseMap::BaseMap() :
    map(0)
{
    // struct init
}

BaseMap::~BaseMap()
{
    if (valid())
    {
        cout << ClassName() << ": map=" << map << endl;
    }
}

BaseMap::operator BaseStr() const // map -> str
{ 
    BaseStr mapstr;
    // "BaseMap::operator BaseStr()");
    //map<map::key_type, map::mapped_type>::iterator iter;
    //for (iter=this->begin(); iter!=this->end(); iter++)
    for (auto& pair: *this)
    {
        mapstr += pair.first + "=" + pair.second + "|";
    }
    return mapstr;
};

//BaseMap::BaseMap(const BaseStr& mappedstr)
//{
//    const static string _myName = "BaseMap::BaseMap(BaseStr)";
//    LOG << "mappedstr=" << mappedstr << endl; 
//    //BaseMap::operator=(mappedstr); // str -> map 
//    strtomap(mappedstr);
//}

BaseMap& BaseMap::operator= (const BaseStr& mappedstr) // str -> map 
{
    const static string _myName = "BaseMap::operator=(BaseStr)";
    LOG << "mappedstr=" << mappedstr << endl; 
    strtomap(mappedstr);
    return *this; 
}

BaseMap& BaseMap::operator= (const char* mappedstr) // str -> map 
{
    const static string _myName = "BaseMap::operator=(char*)";
    LOG << "mappedstr=" << mappedstr << endl; 
    strtomap(BaseStr(mappedstr));
    return *this;
}

void BaseMap::strtomap(const BaseStr& mappedstr)
{ 
    const static string _myName = "BaseMap::strtomap";
    //LOG << "mappedstr=" << mappedstr << endl; 

    //this->emplace(BaseStr("Key1"), BaseStr("Val1")); not supported yet in gcc 4.7.2 
    //this->operator[]("Key1") = "Val1"; // Ok 

    clear(); 
    stringstream ms(mappedstr);
    string pair;
    while (getline(ms, pair, '|')) 
    {
        auto pos = pair.find("=");
        this->insert(make_pair(pair.substr(0, pos), pair.substr(pos+1)));
    }
}

bool BaseMap::create()
{
    if (valid())
    {
        EXCRAISE(BaseMap_unexpected, "already exists");
    }

    //BaseMap::set;
    map = 1;
    return true;
}

//void set(const int, const vo*, int);
//{
//        EXCRAISE(BaseMap_unexpected, strerror(errno));
//}

//const BaseMap& BaseMap::operator << (const BaseStr& s) const
//{
//    if (! BaseMap::send(s))
//    {
//        EXCRAISE("BaseMap::send", strerror(errno));
//    }
//    return *this;
//}

//const BaseMap& BaseMap::operator >> (BaseStr& s) const
//{
//    if (BaseMap::recv(s) == -1)
//    {
//        EXCRAISE("BaseMap::recv", (errno? strerror(errno) : "unknown errror"));
//    }
//    return *this;
//}

//============= BaseMap end =============

