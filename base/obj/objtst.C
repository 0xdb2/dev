//#include <BaseDef.H>
#include <BaseObj.H>
#include <memory>

class My: public BaseObj
{
    public:
        My() { };
        My(const char* s) { str = s; }
        virtual ~My() { cout << "~My: str=" << str << endl; };
        virtual const char* ClassName() const { return "My"; };
        friend ostream& operator<< (ostream& os, const My& my);
    private:
        string str;
};

ostream& operator<< (ostream& os, const My& my)
{
    os << my.str << '='; my.onPrint(os);
    return os; 
}

int main(int, char**)
{
    //cout << "long=" << sizeof(long) << ",long long=" << sizeof(long long) << ",double=" << sizeof(double) << endl;
    //long long maxval=0x7FFFFFFFFFFFFFFFL;
    //long maxval=0x7FFFFFFFL;
    //string str;
    //str = to_string(maxval);
    //struct tm maxtime = *localtime(&maxval);
    //strftime(cstr, sizeof(cstr), "%Y.%m.%d %H:%M:%S", &maxtime);
    //snprintf(cstr, sizeof(cstr), "%lld years", maxval/60/60/24/365);
    My* myptr;
    char cstr[401];
    {
        //My mystr = "abc";
        unique_ptr<My> mystrp1(new My("god"));
        My* mystrp2 = new My("err");
        cout << "mystrp1=" << *mystrp1 << " mystrp2=" << *mystrp2 << endl;
        myptr = mystrp2;  
        delete mystrp2;
    }
    cout << "myptr=" << myptr << endl;
    My my("add");
    cout << "my=" << my << "=" << &my << " ****** crash there!!!" << endl;
    *myptr = my; 
    cout << "myptr=" << *myptr << "=" << myptr << endl;
    exit (0);
} 

