#include <iostream.h>
// #include <stdlib.h>

class Inner
{
    public:
        Inner();
        ~Inner();
        void fun();    
};

class Outer
{
    public:
        Outer();
        ~Outer();
        void fun();
    private:
};

Inner::Inner()
{
    cout << "Inner constructor\n";
}    

Inner::~Inner()
{
    cout << "Inner destructor\n";
}    

void Inner::fun()
{
    cout << "Inner fun\n";
    
    throw 1;
    cout << "This statement is not executed\n";
}    

Outer::Outer()
{
    cout << "Outer constructor\n";
}    

Outer::~Outer()
{
    cout << "Outer destructor\n";
}    

void Outer::fun()
{
    Inner
        in;

    cout << "Outer fun\n";
    in.fun();
}    


int main()
{
    int ret;
    Outer out;
    
    try
    {
      out.fun();
      return 0;
    }
    catch (...)
    {
        return 1;
    }
}



