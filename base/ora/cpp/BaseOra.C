static char BaseTpl_C [] = "@(#) $Id$";

//#include <fcntl.h>

#include <BaseOra.H>

EXEC SQL include sqlca.h;

EXEC SQL WHENEVER SQLERROR   DO BaseOraErrHandler("SQLERROR");
EXEC SQL WHENEVER SQLWARNING DO BaseOraErrHandler("SQLWARNING");
//EXEC SQL WHENEVER SQLMESSAGE call PsiSdDbErrhd_Msg;

EXCDEF(BaseOra_unexpected);

//ostream& operator<< (ostream& os, const ora_& ts)
//{
//    os << "name=" << ts.name;
//    return os; 
//}

static 

//============= BaseOra members ==========
//

void BaseOraErrHandler(const BaseStr& msg)
{
    BaseOra::ErrHandler(msg);
}

void BaseOra::ErrHandler(const BaseStr& msg)
{
    BaseStr oraerr;
    //ERR << msg << ":" << sqlca.sqlerrm.sqlerrml << endl << sqlca.sqlerrm.sqlerrmc << "sqlwarn(" << sqlca.sqlwarn << ')' << endl;
    //Rollback(); TODO
    oraerr.assign(sqlca.sqlerrm.sqlerrmc, sqlca.sqlerrm.sqlerrml-1); 
    EXCRAISE(BaseOra_unexpected, oraerr); 
}

BaseOra::BaseOra() :
    _connected(false)
{
    // struct init
}

BaseOra::~BaseOra()
{
    EXCFUNC(BaseOra::~BaseOra);
    if (connected())
    {
        //cout << ClassName() << ": connected=" << _connected << " to '" << _connstr << "'" << endl;
        Disconnect();
    }
    EXCFUNCEND;
}

bool BaseOra::Connect(const BaseStr& connstr)
{
    EXCFUNC(BaseOra::Connect);
    
    if (connected())
    {
        EXCRAISE(BaseOra_unexpected, "already connected");
    }

    _connstr = connstr;

    EXEC SQL BEGIN DECLARE SECTION;
        varchar ConnStr[81];
    EXEC SQL END DECLARE SECTION;

    ConnStr.len = strlen(strcpy((char*)ConnStr.arr, _connstr.data()));
    //ConnStr.len = _connstr.length();    

    LOGL(*this << ": CONNECT ...");
    EXEC SQL CONNECT :ConnStr; 
    LOGL("OK");

    //BaseOra::set;
    connected(true);

    EXCFUNCRET(connected());
    //return connected();
}

const BaseStr& BaseOra::Execute(
        const BaseStr& Package, 
        const BaseStr& FuncName, 
        const BaseStr& InPar 
        )
{
    EXCFUNC(BaseOra::Execute);

    EXEC SQL BEGIN DECLARE SECTION;
        static varchar sqlRet[11];
        static varchar sqlPackage[21];
        static varchar sqlFuncName[41];
        static varchar sqlInPar[1001];
        static varchar sqlOutPar[1001];
        static varchar sqlMsg[401];
        //static varchar sqlActFunc[41];
    EXEC SQL END DECLARE SECTION;
    
    sqlPackage.len  = strlen(strcpy((char*)sqlPackage.arr,  Package.data()));
    sqlFuncName.len = strlen(strcpy((char*)sqlFuncName.arr, FuncName.data()));
    sqlInPar.len    = strlen(strcpy((char*)sqlInPar.arr,    InPar.data()));
    //sql.len = strlen(strcpy((char*)sql.arr, .data())); // TODO macro

    LOGL(*this << ": EXECUTE ...");
    EXEC SQL EXECUTE
        DECLARE
            Stmt VARCHAR2(200);
        BEGIN
            Stmt := 'BEGIN :Ret := ' || :sqlPackage || '.Exec(:FuncName, :InPar, :OutPar, :Msg); END;';
            /* trc.trc('EXECUTE IMMEDIATE Stmt=' || Stmt); */
            EXECUTE IMMEDIATE Stmt USING OUT :sqlRet, IN :sqlFuncName, IN :sqlInPar, OUT :sqlOutPar, OUT :sqlMsg;
        EXCEPTION WHEN OTHERS THEN
            /* trc.trc( :sqlActFunc || ': EXCEPTION! ' || SQLERRM(SQLCODE)); */
            RAISE;
        END;
    END-EXEC;

    pls.Ret.assign((char *)   sqlRet.arr,    sqlRet.len); // TODO macro
    pls.OutPar.assign((char *)sqlOutPar.arr, sqlOutPar.len);
    pls.Msg.assign((char *)   sqlMsg.arr,    sqlMsg.len);

    LOGL("Ret=" << pls.Ret << ": OutPar=" << pls.OutPar << ",Msg=" << pls.Msg);

    EXCFUNCEND;
    return pls.Ret;
}

void BaseOra::Commit(void) const
{
    EXCFUNC(BaseOra::Commit);

    LOGL(*this << ": COMMIT ...");   
    EXEC SQL COMMIT;
    LOGL("OK");

    EXCFUNCEND;
}

void BaseOra::Rollback(void) const
{
    EXCFUNC(BaseOra::Rollback);

    LOGL(*this << ": ROLLBACK ...");
    EXEC SQL ROLLBACK;
    LOGL("OK");

    EXCFUNCEND;
}

void BaseOra::Disconnect(void)
{
    EXCFUNC(BaseOra::Disconnect);

    LOGL(*this << ": ROLLBACK RELEASE ...");
    EXEC SQL ROLLBACK RELEASE;
    LOGL("OK");
    connected(false);

    EXCFUNCEND;
}

//void set(const int, const void*, int);
//{
//        EXCRAISE(BaseOra_unexpected, strerror(errno));
//}

//const BaseOra& BaseOra::operator << (const BaseStr& s) const
//{
//    if (! BaseOra::send(s))
//    {
//        EXCRAISE("BaseOra::send", strerror(errno));
//    }
//    return *this;
//}

//const BaseOra& BaseOra::operator >> (BaseStr& s) const
//{
//    if (BaseOra::recv(s) == -1)
//    {
//        EXCRAISE("BaseOra::recv", (errno? strerror(errno) : "unknown errror"));
//    }
//    return *this;
//}

//============= BaseOra end =============

