
// $Header: http://belov.homenet.org/svn/trunk/base/store/store.C 232 2013-10-21 14:57:30Z db $
#include <BaseStore.H>
#include <iostream>

using namespace std;

class My1: public BaseStorable 
{
    public:
        My1() : num(1) { cout << "My1: num=" << num << endl; };	
        My1(int i) 
        { 
            num = i; 
            cout << "My1(int): num=" << num << ' ' << *this << endl; 
        };
        ~My1(void) { cout << "~My1: " << *this << endl; };
        virtual BaseStorable* Copy() const;
        virtual void restore(void) { };
        virtual const char* ClassName() const { return "My1"; };
        friend ostream & operator << (ostream& os, const My1& b);
        int get() { return this->num; };
    private:
        int num;
};

class My2: public BaseStorable
{
    public:
        My2() : num(2) { cout << "My2: num=" << num << endl; };	
        My2(int i) 
        { 
            num = i; 
            cout << "My2(int): num=" << num << ' ' << *this << endl;
        };
        ~My2(void) { cout << "~My2: " << *this << endl; };
        virtual BaseStorable* Copy() const;
        virtual const char* ClassName() const { return "My2"; };
        friend ostream & operator << (ostream& os, const My2& b);
        virtual const My2&  operator= (const My2 &other) 
        {
            cout << "My2::operator=(" << other << ")" << endl;
            //num = other.num;
            this->num = other.num;
            return *this;
        }  
        int get() { return this->num; };
    private:
        int num;
};

BaseStorable* My1::Copy() const
{
    return (new My1(*this));
}

BaseStorable* My2::Copy() const
{
    return (new My2(*this));
}

ostream & operator << (ostream& os, const My1& b) { os << b.ClassName() << ".num=" << b.num << ' '; b.onPrint(os); return os; };
ostream & operator << (ostream& os, const My2& b) { os << b.ClassName() << ".num=" << b.num << ' '; b.onPrint(os); return os; };

int main(void)
{
    {
        My1 my1(1);
        My2 my2(2);
        My2 my3(3);
        BaseStore mystore;
        cout << "##### main (after init): " << my1 << ' ' << my2 << ' ' << mystore << endl; 

        mystore.add(my1); 
        mystore.add(my2); 
        cout << "##### main (after save): " << my1 << ' ' << my2 << ": nstored="<< mystore.nstored() << endl; 

        my1 = 11;
        my2 = 22;
        my3 = 33;
        cout << "##### main (after mode1): " << my1 << ' ' << my2 << ": nstored="<< mystore.nstored() << endl; 

        cout << "##### main: nstored="<< mystore.nstored() << endl;
        for (int i = 0; i < mystore.nstored(); i++ )
        {
            cout << "mystore[" << i << "]=" << *mystore.get(i) << endl;
        }
        cout << "##### Back ..." << endl;
        //my1 = (My1&)*mystore.get(my1.getIndex);
        //my2 = (My2&)*mystore.get(my2.getIndex);
        my1 = (My1&)mystore.back(my1);
        my2 = (My2&)mystore.back(my2);
        my3 = (My2&)mystore.back(my3);
        cout << "##### main (after back): " << my1 << ' ' << my2 << ' ' << my3 << endl; 
        my1 = 111;
        my2 = 222;
        my3 = 333;
        cout << "##### main (after mode2): " << my1 << ' ' << my2 << ' ' << mystore << endl; 
        cout << "##### DelStore ..." << endl;
        mystore = BaseStore();
        cout << "##### main (after DelStore): " << my1 << ' ' << my2 << ' ' << mystore << endl; 
        

    }
    exit(0);
} 
