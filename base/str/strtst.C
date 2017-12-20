#include <BaseTime.H>
#include <BaseStr.H>
#include <BaseExc.H>
#include <BaseStore.H>

#include <sstream> // std::stringstream
#include <memory>

class StrStore: 
    public BaseStorable, 
    public string 
{  
    public:
        StrStore (const char *s) : string(s) { };
        virtual const char* ClassName() const { return "StrStore"; };
        virtual BaseStorable* Copy() const { return (new StrStore(*this)); };
};

ostream& operator << (ostream& os, const StrStore& s) { os << s.data(); s.onPrint(os); return os; };

/*
class My: public BaseObj
{
    public:
        My() { };
        My(const char* s) { str = s; }
        virtual ~My() { cout << "~My: str=" << str << endl; };
        virtual const char* ClassName() const { return "My"; };
        friend ostream& operator<< (ostream& os, const My& my);
    private:
        BaseStr str;
};
*/

// ostream& operator<< (ostream& os, const My& my) { os << my.str << '='; my.onPrint(os); return os; }

int main (void) 
{
    EXCINIT;
    EXCBLK("strtst::main")
    
#if false

    BaseStr str1("abc"), str2="def";
    str1 = str2 + 999;
    LOG << "str1=" << str1 << ",str2=" << str2 << endl;

#endif

#if true

    string str1("abc"), str2("123");
    BaseStr Str1 = "abc", Str2 = "123";
    cout << "main: str1=" << str1 << ",str2=" << str2 << ",Str1=" << Str1 << ",Str2=" << Str2 << endl;
   
    str1 = "def", str2 = "456";
    Str1 = "def", Str2 = "456";
    cout << "main: str1=" << str1 << ",str2=" << str2 << ",Str1=" << Str1 << ",Str2=" << Str2 << endl;

    str1 = str2;
    Str1 = Str2;
    cout << "main: str1=" << str1 << ",str2=" << str2 << ",Str1=" << Str1 << ",Str2=" << Str2 << endl;

    str1 = str2 + "abc";
    Str1 = Str2 + "abc";
    cout << "main: str1=" << str1 << ",str2=" << str2 << endl << "      Str1=" << Str1 << ",Str2=" << Str2 << endl;

    str1 += " abc";
    Str1 += " abc";
    cout << "main: str1=" << str1 << ",str2=" << str2 << endl << "      Str1=" << Str1 << ",Str2=" << Str2 << endl;

    //str1 = "zzz " + str2;
    Str1 = 
        (
        "zzz " 
        + Str2
        );
    cout << "main: str1=" << str1 << ",str2=" << str2 << endl << "      Str1=" << Str1 << ",Str2=" << Str2 << endl;
   
    str1 = str2 + str1;
    Str1 = Str2 + Str1;
    cout << "main: str1=" << str1 << ",str2=" << str2 << endl << "      Str1=" << Str1 << ",Str2=" << Str2 << endl;
   
    str1 = str2;
    Str1 = Str2;
    cout << "main: str1=" << str1 << ",str2=" << str2 << "==" << (str1==str2) << endl << "      Str1=" << Str1 << ",Str2=" << Str2 << "==" << (Str1==Str2) << endl;
    
    str1 = 'x';
    Str1 = 'x';
    cout << "main: str1=" << str1 << ",str2=" << str2 << "==" << (str1==str2) << endl << "      Str1=" << Str1 << ",Str2=" << Str2 << "==" << (Str1==Str2) << endl;
    
    str1 = str2[1];
    Str1 = Str2[1];
    cout << "main: str1=" << str1 << ",str2=" << str2 << "==" << (str1==str2) << endl << "      Str1=" << Str1 << ",Str2=" << Str2 << "==" << (Str1==Str2) << endl;

    str1 = str2.substr(0,2);
    Str1 = Str2(0,2);
    cout << "main: str1=" << str1 << ",str2=" << str2 << "==" << (str1==str2) << endl << "      Str1=" << Str1 << ",Str2=" << Str2 << "==" << (Str1==Str2) << endl;
#endif

    EXCEPTION
    EXCEND;
    EXCTERM;

    exit(0);
}

/*****************************************************************************    
    {
    //ostringstream stras, strns;
    //stra.BaseObj::onPrint(stras); 
    //strn.BaseObj::onPrint(strns);
 
    StrStore stra = "abc", strn = "123";
    BaseStore store; 
    store.add(stra);   
    store.add(strn);  

    for (int i=0; i< store.nstored(); i++)
        cout << "store[" << i << "]=" << store.get(i) << endl;
    }

    My* myptr;
    char cstr[401];
    {
        unique_ptr<My> mystrp1(new My("god"));
        My* mystrp2 = new My("bad");
        cout << "mystrp1=" << *mystrp1 << " mystrp2=" << *mystrp2 << endl;
        myptr = mystrp2;  
        delete mystrp2;
    }
    cout << "myptr=" << myptr << endl;
    My my("add");
    cout << "my=" << my << "=" << &my << endl;
    myptr = &my; 
    cout << "myptr=" << *myptr << "=" << myptr << endl;
    exit (0);
    return 0;
}

*****************************************************************************/    
