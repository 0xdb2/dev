// $Header$
//

#include <iostream>
#include <string>
//#include <vector>
//#include <functional>

using namespace std;

string getName(void)
{
    return "my mame";
}

void print (const string& str)
{
    cout << "printl()=" << str << endl;
}

void print (string&& str)
{
    cout << "printr()=" << str << endl;
}

class MetaData
{
public:
    MetaData (int size, const std::string& name)
        : _name( name )
        , _size( size )
    {}

    // copy constructor
    MetaData (const MetaData& other)
        : _name( other._name )
        , _size( other._size )
    {}

    // move constructor
    MetaData (MetaData&& other)
        : _name( other._name )
        , _size( other._size )
    {}

    std::string getName () const { return _name; }
    int getSize () const { return _size; }
    private:
    std::string _name;
    int _size;
};

class ArrayWrapper
{
public:
    // default constructor produces a moderately sized array
    ArrayWrapper ()
        : _p_vals( new int[ 64 ] )
        , _metadata( 64, "ArrayWrapper" )
    {}

    ArrayWrapper (int n)
        : _p_vals( new int[ n ] )
        , _metadata( n, "ArrayWrapper" )
    {}

    // move constructor
    ArrayWrapper (ArrayWrapper&& other)
        : _p_vals( other._p_vals  )
        , _metadata( other._metadata )
    {
        other._p_vals = NULL;
    }

    // copy constructor
    ArrayWrapper (const ArrayWrapper& other)
        : _p_vals( new int[ other._metadata.getSize() ] )
        , _metadata( other._metadata )
    {
        for ( int i = 0; i < _metadata.getSize(); ++i )
        {
            _p_vals[ i ] = other._p_vals[ i ];
        }
    }
    ~ArrayWrapper ()
    {
        delete [] _p_vals;
    }
private:
    int *_p_vals;
    MetaData _metadata;
};

int x = 100;
int& getInt ()
{
    return x;
}

int && getRvalueInt ()
{
    return std::move( x );
}

void printAddress (const string& info, const int& v) // const ref to allow binding to rvalues
{
    cout << "printAddress" << "(" << info << ")" << "=" << reinterpret_cast<const void*>( & v ) << endl;
}

int main(void)
{
    cout << "##### maiin::exrc_rref ###### "<< endl;
    const string&  lrname = getName();
    string&& rrname = getName();

    cout << "lrname=" << lrname << " rrname=" << rrname << endl << endl;

    print(lrname);
    print(rrname);
    print(move(rrname));
    print(getName());

    cout << "##### main::exrc_move ###### "<< endl;

    ArrayWrapper aw1 = ArrayWrapper(1);
    ArrayWrapper aw2 = ArrayWrapper(2);
    ArrayWrapper&& refaw = move(aw2);

    {
        cout << "##### main::exrc_move_1 ###### "<< endl;

        printAddress( "val", x );
        printAddress( "rval", getInt() );
        printAddress( "refrval", getRvalueInt() );

        int&& xx = getRvalueInt();
        printAddress( "val xx", xx );
        xx++;
        cout << "x=" << x << " xx=" << xx << endl;
    }

};

