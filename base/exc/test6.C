#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include <new.h>
#include <PsiSdExc.H>

EXCDEF(TEST_EXC);


class Bruch
{

public:

    class NennerIstNull
    {
    };

    Bruch(int z)
    {
        if ( z == 0 )
        {
            throw NennerIstNull();
        }
        else
        {
            cout << "Nenner = " << z << endl;
        }

    }

    void TestExec(int i)
    {
        
        EXCBEGIN;

        if ( i != 0 )
        {
            cout << "Wert=" << i << endl;
        }
        else
        {
            EXCRAISE(TEST_EXC);
        }

        EXCEPTIONS "TestExec";

        EXCEND;

    }

};




int main(int, char* argv[])
{
    EXCINITIALIZE;
    EXCBEGIN;

    cout << "Hello World!" << endl;

    
    Bruch x(1);
    int   i;

    x.TestExec(atoi(argv[1]));

    cin >> i;

//    try
//    {
//        Bruch x(0);
//    }
//    catch (Bruch::NennerIstNull)
//    {
//        cout << "Eingabefehler: Nenner darf nicht Null sein!" << endl;
//    }


    EXCEPTIONS "main";

//    catch (...)
//    {
//	cout << "bad_exception" << endl;
//    }

//    SELECT
//        WHEN TEST_EXC THEN
//        cout << "i = 0!" << endl;
//    ENDSELECT;


//     EXCEND;   

 if (!PsiSdExc::Cancel) throw pExc; }

    catch (...)
    {
	cout << "bad_exception" << endl;
    }
} ;

     EXCTERMINATE;

     return 0;
}



