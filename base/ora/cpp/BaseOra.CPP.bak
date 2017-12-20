
/* Result Sets Interface */
#ifndef SQL_CRSR
#  define SQL_CRSR
  struct sql_cursor
  {
    unsigned int curocn;
    void *ptr1;
    void *ptr2;
    unsigned int magic;
  };
  typedef struct sql_cursor sql_cursor;
  typedef struct sql_cursor SQL_CURSOR;
#endif /* SQL_CRSR */

/* Thread Safety */
typedef void * sql_context;
typedef void * SQL_CONTEXT;

/* Object support */
struct sqltvn
{
  unsigned char *tvnvsn; 
  unsigned short tvnvsnl; 
  unsigned char *tvnnm;
  unsigned short tvnnml; 
  unsigned char *tvnsnm;
  unsigned short tvnsnml;
};
typedef struct sqltvn sqltvn;

struct sqladts
{
  unsigned int adtvsn; 
  unsigned short adtmode; 
  unsigned short adtnum;  
  sqltvn adttvn[1];       
};
typedef struct sqladts sqladts;

static struct sqladts sqladt = {
  1,1,0,
};

/* Binding to PL/SQL Records */
struct sqltdss
{
  unsigned int tdsvsn; 
  unsigned short tdsnum; 
  unsigned char *tdsval[1]; 
};
typedef struct sqltdss sqltdss;
static struct sqltdss sqltds =
{
  1,
  0,
};

/* File name & Package Name */
struct sqlcxp
{
  unsigned short fillen;
           char  filnam[10];
};
static const struct sqlcxp sqlfpn =
{
    9,
    "BaseOra.C"
};


static unsigned int sqlctx = 35075;


static struct sqlexd {
   unsigned int   sqlvsn;
   unsigned int   arrsiz;
   unsigned int   iters;
   unsigned int   offset;
   unsigned short selerr;
   unsigned short sqlety;
   unsigned int   occurs;
      const short *cud;
   unsigned char  *sqlest;
      const char  *stmt;
   sqladts *sqladtp;
   sqltdss *sqltdsp;
            void  **sqphsv;
   unsigned int   *sqphsl;
            int   *sqphss;
            void  **sqpind;
            int   *sqpins;
   unsigned int   *sqparm;
   unsigned int   **sqparc;
   unsigned short  *sqpadto;
   unsigned short  *sqptdso;
   unsigned int   sqlcmax;
   unsigned int   sqlcmin;
   unsigned int   sqlcincr;
   unsigned int   sqlctimeout;
   unsigned int   sqlcnowait;
              int   sqfoff;
   unsigned int   sqcmod;
   unsigned int   sqfmod;
            void  *sqhstv[6];
   unsigned int   sqhstl[6];
            int   sqhsts[6];
            void  *sqindv[6];
            int   sqinds[6];
   unsigned int   sqharm[6];
   unsigned int   *sqharc[6];
   unsigned short  sqadto[6];
   unsigned short  sqtdso[6];
} sqlstm = {12,6};

// Prototypes
extern "C" {
  void sqlcxt (void **, unsigned int *,
               struct sqlexd *, const struct sqlcxp *);
  void sqlcx2t(void **, unsigned int *,
               struct sqlexd *, const struct sqlcxp *);
  void sqlbuft(void **, char *);
  void sqlgs2t(void **, char *);
  void sqlorat(void **, unsigned int *, void *);
}

// Forms Interface
static const int IAPSUCC = 0;
static const int IAPFAIL = 1403;
static const int IAPFTL  = 535;
extern "C" { void sqliem(unsigned char *, signed int *); }

typedef struct { unsigned short len; unsigned char arr[1]; } VARCHAR;
typedef struct { unsigned short len; unsigned char arr[1]; } varchar;

/* cud (compilation unit data) array */
static const short sqlcud0[] =
{12,4130,873,0,0,
5,0,0,0,0,0,27,76,0,0,4,4,0,1,0,1,9,0,0,1,10,0,0,1,10,0,0,1,10,0,0,
36,0,0,2,278,0,6,110,0,0,6,6,0,1,0,1,9,0,0,2,9,0,0,1,9,0,0,1,9,0,0,2,9,0,0,2,9,
0,0,
75,0,0,3,0,0,29,138,0,0,0,0,0,1,0,
90,0,0,4,0,0,31,149,0,0,0,0,0,1,0,
105,0,0,5,0,0,32,160,0,0,0,0,0,1,0,
};


#line 1 "BaseOra.C"
static char BaseTpl_C [] = "@(#) $Id$";

//#include <fcntl.h>

#include <BaseOra.H>

/* EXEC SQL include sqlca.h;
 */ 
#line 1 "/vol/db/app/oracle/product/11.2.0/home/precomp/public/sqlca.h"
/*
 * $Header: sqlca.h 24-apr-2003.12:50:58 mkandarp Exp $ sqlca.h 
 */

/* Copyright (c) 1985, 2003, Oracle Corporation.  All rights reserved.  */
 
/*
NAME
  SQLCA : SQL Communications Area.
FUNCTION
  Contains no code. Oracle fills in the SQLCA with status info
  during the execution of a SQL stmt.
NOTES
  **************************************************************
  ***                                                        ***
  *** This file is SOSD.  Porters must change the data types ***
  *** appropriately on their platform.  See notes/pcport.doc ***
  *** for more information.                                  ***
  ***                                                        ***
  **************************************************************

  If the symbol SQLCA_STORAGE_CLASS is defined, then the SQLCA
  will be defined to have this storage class. For example:
 
    #define SQLCA_STORAGE_CLASS extern
 
  will define the SQLCA as an extern.
 
  If the symbol SQLCA_INIT is defined, then the SQLCA will be
  statically initialized. Although this is not necessary in order
  to use the SQLCA, it is a good pgming practice not to have
  unitialized variables. However, some C compilers/OS's don't
  allow automatic variables to be init'd in this manner. Therefore,
  if you are INCLUDE'ing the SQLCA in a place where it would be
  an automatic AND your C compiler/OS doesn't allow this style
  of initialization, then SQLCA_INIT should be left undefined --
  all others can define SQLCA_INIT if they wish.

  If the symbol SQLCA_NONE is defined, then the SQLCA variable will
  not be defined at all.  The symbol SQLCA_NONE should not be defined
  in source modules that have embedded SQL.  However, source modules
  that have no embedded SQL, but need to manipulate a sqlca struct
  passed in as a parameter, can set the SQLCA_NONE symbol to avoid
  creation of an extraneous sqlca variable.
 
MODIFIED
    lvbcheng   07/31/98 -  long to int
    jbasu      12/12/94 -  Bug 217878: note this is an SOSD file
    losborne   08/11/92 -  No sqlca var if SQLCA_NONE macro set 
  Clare      12/06/84 - Ch SQLCA to not be an extern.
  Clare      10/21/85 - Add initialization.
  Bradbury   01/05/86 - Only initialize when SQLCA_INIT set
  Clare      06/12/86 - Add SQLCA_STORAGE_CLASS option.
*/
 
#ifndef SQLCA
#define SQLCA 1
 
struct   sqlca
         {
         /* ub1 */ char    sqlcaid[8];
         /* b4  */ int     sqlabc;
         /* b4  */ int     sqlcode;
         struct
           {
           /* ub2 */ unsigned short sqlerrml;
           /* ub1 */ char           sqlerrmc[70];
           } sqlerrm;
         /* ub1 */ char    sqlerrp[8];
         /* b4  */ int     sqlerrd[6];
         /* ub1 */ char    sqlwarn[8];
         /* ub1 */ char    sqlext[8];
         };

#ifndef SQLCA_NONE 
#ifdef   SQLCA_STORAGE_CLASS
SQLCA_STORAGE_CLASS struct sqlca sqlca
#else
         struct sqlca sqlca
#endif
 
#ifdef  SQLCA_INIT
         = {
         {'S', 'Q', 'L', 'C', 'A', ' ', ' ', ' '},
         sizeof(struct sqlca),
         0,
         { 0, {0}},
         {'N', 'O', 'T', ' ', 'S', 'E', 'T', ' '},
         {0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0}
         }
#endif
         ;
#endif
 
#endif
 
/* end SQLCA */

#line 8 "BaseOra.C"

/* EXEC SQL WHENEVER SQLERROR   DO BaseOraErrHandler("SQLERROR"); */ 
#line 9 "BaseOra.C"

/* EXEC SQL WHENEVER SQLWARNING DO BaseOraErrHandler("SQLWARNING"); */ 
#line 10 "BaseOra.C"

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

    /* EXEC SQL BEGIN DECLARE SECTION; */ 
#line 68 "BaseOra.C"

        /* varchar ConnStr[81]; */ 
struct { unsigned short len; unsigned char arr[81]; } ConnStr;
#line 69 "BaseOra.C"

    /* EXEC SQL END DECLARE SECTION; */ 
#line 70 "BaseOra.C"


    ConnStr.len = strlen(strcpy((char*)ConnStr.arr, _connstr.data()));
    //ConnStr.len = _connstr.length();    

    LOGL(*this << ": CONNECT ...");
    /* EXEC SQL CONNECT :ConnStr; */ 
#line 76 "BaseOra.C"

{
#line 76 "BaseOra.C"
    struct sqlexd sqlstm;
#line 76 "BaseOra.C"
    sqlstm.sqlvsn = 12;
#line 76 "BaseOra.C"
    sqlstm.arrsiz = 4;
#line 76 "BaseOra.C"
    sqlstm.sqladtp = &sqladt;
#line 76 "BaseOra.C"
    sqlstm.sqltdsp = &sqltds;
#line 76 "BaseOra.C"
    sqlstm.iters = (unsigned int  )10;
#line 76 "BaseOra.C"
    sqlstm.offset = (unsigned int  )5;
#line 76 "BaseOra.C"
    sqlstm.cud = sqlcud0;
#line 76 "BaseOra.C"
    sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 76 "BaseOra.C"
    sqlstm.sqlety = (unsigned short)4352;
#line 76 "BaseOra.C"
    sqlstm.occurs = (unsigned int  )0;
#line 76 "BaseOra.C"
    sqlstm.sqhstv[0] = (         void  *)&ConnStr;
#line 76 "BaseOra.C"
    sqlstm.sqhstl[0] = (unsigned int  )83;
#line 76 "BaseOra.C"
    sqlstm.sqhsts[0] = (         int  )83;
#line 76 "BaseOra.C"
    sqlstm.sqindv[0] = (         void  *)0;
#line 76 "BaseOra.C"
    sqlstm.sqinds[0] = (         int  )0;
#line 76 "BaseOra.C"
    sqlstm.sqharm[0] = (unsigned int  )0;
#line 76 "BaseOra.C"
    sqlstm.sqadto[0] = (unsigned short )0;
#line 76 "BaseOra.C"
    sqlstm.sqtdso[0] = (unsigned short )0;
#line 76 "BaseOra.C"
    sqlstm.sqphsv = sqlstm.sqhstv;
#line 76 "BaseOra.C"
    sqlstm.sqphsl = sqlstm.sqhstl;
#line 76 "BaseOra.C"
    sqlstm.sqphss = sqlstm.sqhsts;
#line 76 "BaseOra.C"
    sqlstm.sqpind = sqlstm.sqindv;
#line 76 "BaseOra.C"
    sqlstm.sqpins = sqlstm.sqinds;
#line 76 "BaseOra.C"
    sqlstm.sqparm = sqlstm.sqharm;
#line 76 "BaseOra.C"
    sqlstm.sqparc = sqlstm.sqharc;
#line 76 "BaseOra.C"
    sqlstm.sqpadto = sqlstm.sqadto;
#line 76 "BaseOra.C"
    sqlstm.sqptdso = sqlstm.sqtdso;
#line 76 "BaseOra.C"
    sqlstm.sqlcmax = (unsigned int )100;
#line 76 "BaseOra.C"
    sqlstm.sqlcmin = (unsigned int )2;
#line 76 "BaseOra.C"
    sqlstm.sqlcincr = (unsigned int )1;
#line 76 "BaseOra.C"
    sqlstm.sqlctimeout = (unsigned int )0;
#line 76 "BaseOra.C"
    sqlstm.sqlcnowait = (unsigned int )0;
#line 76 "BaseOra.C"
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 76 "BaseOra.C"
    if (sqlca.sqlcode < 0) BaseOraErrHandler("SQLERROR");
#line 76 "BaseOra.C"
    if (sqlca.sqlwarn[0] == 'W') BaseOraErrHandler("SQLWARNING");
#line 76 "BaseOra.C"
}

#line 76 "BaseOra.C"
 
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

    /* EXEC SQL BEGIN DECLARE SECTION; */ 
#line 94 "BaseOra.C"

        static /* varchar sqlRet[11]; */ 
struct { unsigned short len; unsigned char arr[11]; } sqlRet;
#line 95 "BaseOra.C"

        static /* varchar sqlPackage[21]; */ 
struct { unsigned short len; unsigned char arr[21]; } sqlPackage;
#line 96 "BaseOra.C"

        static /* varchar sqlFuncName[41]; */ 
struct { unsigned short len; unsigned char arr[41]; } sqlFuncName;
#line 97 "BaseOra.C"

        static /* varchar sqlInPar[1001]; */ 
struct { unsigned short len; unsigned char arr[1001]; } sqlInPar;
#line 98 "BaseOra.C"

        static /* varchar sqlOutPar[1001]; */ 
struct { unsigned short len; unsigned char arr[1001]; } sqlOutPar;
#line 99 "BaseOra.C"

        static /* varchar sqlMsg[401]; */ 
struct { unsigned short len; unsigned char arr[401]; } sqlMsg;
#line 100 "BaseOra.C"

        //static varchar sqlActFunc[41];
    /* EXEC SQL END DECLARE SECTION; */ 
#line 102 "BaseOra.C"

    
    sqlPackage.len  = strlen(strcpy((char*)sqlPackage.arr,  Package.data()));
    sqlFuncName.len = strlen(strcpy((char*)sqlFuncName.arr, FuncName.data()));
    sqlInPar.len    = strlen(strcpy((char*)sqlInPar.arr,    InPar.data()));
    //sql.len = strlen(strcpy((char*)sql.arr, .data())); // TODO macro

    LOGL(*this << ": EXECUTE ...");
    /* EXEC SQL EXECUTE
        DECLARE
            Stmt VARCHAR2(200);
        BEGIN
            Stmt := 'BEGIN :Ret := ' || :sqlPackage || '.Exec(:FuncName, :InPar, :OutPar, :Msg); END;';
            /o trc.trc('EXECUTE IMMEDIATE Stmt=' || Stmt); o/
            EXECUTE IMMEDIATE Stmt USING OUT :sqlRet, IN :sqlFuncName, IN :sqlInPar, OUT :sqlOutPar, OUT :sqlMsg;
        EXCEPTION WHEN OTHERS THEN
            /o trc.trc( :sqlActFunc || ': EXCEPTION! ' || SQLERRM(SQLCODE)); o/
            RAISE;
        END;
    END-EXEC; */ 
#line 121 "BaseOra.C"

{
#line 110 "BaseOra.C"
    struct sqlexd sqlstm;
#line 110 "BaseOra.C"
    sqlstm.sqlvsn = 12;
#line 110 "BaseOra.C"
    sqlstm.arrsiz = 6;
#line 110 "BaseOra.C"
    sqlstm.sqladtp = &sqladt;
#line 110 "BaseOra.C"
    sqlstm.sqltdsp = &sqltds;
#line 110 "BaseOra.C"
    sqlstm.stmt = "declare Stmt VARCHAR2 ( 200 ) ; BEGIN Stmt := 'BEGIN :Re\
t := ' || :sqlPackage || '.Exec(:FuncName, :InPar, :OutPar, :Msg); END;' ; EX\
ECUTE IMMEDIATE Stmt USING OUT :sqlRet , IN :sqlFuncName , IN :sqlInPar , OUT\
 :sqlOutPar , OUT :sqlMsg ; EXCEPTION WHEN OTHERS THEN RAISE ; END ;";
#line 110 "BaseOra.C"
    sqlstm.iters = (unsigned int  )1;
#line 110 "BaseOra.C"
    sqlstm.offset = (unsigned int  )36;
#line 110 "BaseOra.C"
    sqlstm.cud = sqlcud0;
#line 110 "BaseOra.C"
    sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 110 "BaseOra.C"
    sqlstm.sqlety = (unsigned short)4352;
#line 110 "BaseOra.C"
    sqlstm.occurs = (unsigned int  )0;
#line 110 "BaseOra.C"
    sqlstm.sqhstv[0] = (         void  *)&sqlPackage;
#line 110 "BaseOra.C"
    sqlstm.sqhstl[0] = (unsigned int  )23;
#line 110 "BaseOra.C"
    sqlstm.sqhsts[0] = (         int  )0;
#line 110 "BaseOra.C"
    sqlstm.sqindv[0] = (         void  *)0;
#line 110 "BaseOra.C"
    sqlstm.sqinds[0] = (         int  )0;
#line 110 "BaseOra.C"
    sqlstm.sqharm[0] = (unsigned int  )0;
#line 110 "BaseOra.C"
    sqlstm.sqadto[0] = (unsigned short )0;
#line 110 "BaseOra.C"
    sqlstm.sqtdso[0] = (unsigned short )0;
#line 110 "BaseOra.C"
    sqlstm.sqhstv[1] = (         void  *)&sqlRet;
#line 110 "BaseOra.C"
    sqlstm.sqhstl[1] = (unsigned int  )13;
#line 110 "BaseOra.C"
    sqlstm.sqhsts[1] = (         int  )0;
#line 110 "BaseOra.C"
    sqlstm.sqindv[1] = (         void  *)0;
#line 110 "BaseOra.C"
    sqlstm.sqinds[1] = (         int  )0;
#line 110 "BaseOra.C"
    sqlstm.sqharm[1] = (unsigned int  )0;
#line 110 "BaseOra.C"
    sqlstm.sqadto[1] = (unsigned short )0;
#line 110 "BaseOra.C"
    sqlstm.sqtdso[1] = (unsigned short )0;
#line 110 "BaseOra.C"
    sqlstm.sqhstv[2] = (         void  *)&sqlFuncName;
#line 110 "BaseOra.C"
    sqlstm.sqhstl[2] = (unsigned int  )43;
#line 110 "BaseOra.C"
    sqlstm.sqhsts[2] = (         int  )0;
#line 110 "BaseOra.C"
    sqlstm.sqindv[2] = (         void  *)0;
#line 110 "BaseOra.C"
    sqlstm.sqinds[2] = (         int  )0;
#line 110 "BaseOra.C"
    sqlstm.sqharm[2] = (unsigned int  )0;
#line 110 "BaseOra.C"
    sqlstm.sqadto[2] = (unsigned short )0;
#line 110 "BaseOra.C"
    sqlstm.sqtdso[2] = (unsigned short )0;
#line 110 "BaseOra.C"
    sqlstm.sqhstv[3] = (         void  *)&sqlInPar;
#line 110 "BaseOra.C"
    sqlstm.sqhstl[3] = (unsigned int  )1003;
#line 110 "BaseOra.C"
    sqlstm.sqhsts[3] = (         int  )0;
#line 110 "BaseOra.C"
    sqlstm.sqindv[3] = (         void  *)0;
#line 110 "BaseOra.C"
    sqlstm.sqinds[3] = (         int  )0;
#line 110 "BaseOra.C"
    sqlstm.sqharm[3] = (unsigned int  )0;
#line 110 "BaseOra.C"
    sqlstm.sqadto[3] = (unsigned short )0;
#line 110 "BaseOra.C"
    sqlstm.sqtdso[3] = (unsigned short )0;
#line 110 "BaseOra.C"
    sqlstm.sqhstv[4] = (         void  *)&sqlOutPar;
#line 110 "BaseOra.C"
    sqlstm.sqhstl[4] = (unsigned int  )1003;
#line 110 "BaseOra.C"
    sqlstm.sqhsts[4] = (         int  )0;
#line 110 "BaseOra.C"
    sqlstm.sqindv[4] = (         void  *)0;
#line 110 "BaseOra.C"
    sqlstm.sqinds[4] = (         int  )0;
#line 110 "BaseOra.C"
    sqlstm.sqharm[4] = (unsigned int  )0;
#line 110 "BaseOra.C"
    sqlstm.sqadto[4] = (unsigned short )0;
#line 110 "BaseOra.C"
    sqlstm.sqtdso[4] = (unsigned short )0;
#line 110 "BaseOra.C"
    sqlstm.sqhstv[5] = (         void  *)&sqlMsg;
#line 110 "BaseOra.C"
    sqlstm.sqhstl[5] = (unsigned int  )403;
#line 110 "BaseOra.C"
    sqlstm.sqhsts[5] = (         int  )0;
#line 110 "BaseOra.C"
    sqlstm.sqindv[5] = (         void  *)0;
#line 110 "BaseOra.C"
    sqlstm.sqinds[5] = (         int  )0;
#line 110 "BaseOra.C"
    sqlstm.sqharm[5] = (unsigned int  )0;
#line 110 "BaseOra.C"
    sqlstm.sqadto[5] = (unsigned short )0;
#line 110 "BaseOra.C"
    sqlstm.sqtdso[5] = (unsigned short )0;
#line 110 "BaseOra.C"
    sqlstm.sqphsv = sqlstm.sqhstv;
#line 110 "BaseOra.C"
    sqlstm.sqphsl = sqlstm.sqhstl;
#line 110 "BaseOra.C"
    sqlstm.sqphss = sqlstm.sqhsts;
#line 110 "BaseOra.C"
    sqlstm.sqpind = sqlstm.sqindv;
#line 110 "BaseOra.C"
    sqlstm.sqpins = sqlstm.sqinds;
#line 110 "BaseOra.C"
    sqlstm.sqparm = sqlstm.sqharm;
#line 110 "BaseOra.C"
    sqlstm.sqparc = sqlstm.sqharc;
#line 110 "BaseOra.C"
    sqlstm.sqpadto = sqlstm.sqadto;
#line 110 "BaseOra.C"
    sqlstm.sqptdso = sqlstm.sqtdso;
#line 110 "BaseOra.C"
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 110 "BaseOra.C"
    if (sqlca.sqlcode < 0) BaseOraErrHandler("SQLERROR");
#line 110 "BaseOra.C"
    if (sqlca.sqlwarn[0] == 'W') BaseOraErrHandler("SQLWARNING");
#line 110 "BaseOra.C"
}

#line 121 "BaseOra.C"


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
    /* EXEC SQL COMMIT; */ 
#line 138 "BaseOra.C"

{
#line 138 "BaseOra.C"
    struct sqlexd sqlstm;
#line 138 "BaseOra.C"
    sqlstm.sqlvsn = 12;
#line 138 "BaseOra.C"
    sqlstm.arrsiz = 6;
#line 138 "BaseOra.C"
    sqlstm.sqladtp = &sqladt;
#line 138 "BaseOra.C"
    sqlstm.sqltdsp = &sqltds;
#line 138 "BaseOra.C"
    sqlstm.iters = (unsigned int  )1;
#line 138 "BaseOra.C"
    sqlstm.offset = (unsigned int  )75;
#line 138 "BaseOra.C"
    sqlstm.cud = sqlcud0;
#line 138 "BaseOra.C"
    sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 138 "BaseOra.C"
    sqlstm.sqlety = (unsigned short)4352;
#line 138 "BaseOra.C"
    sqlstm.occurs = (unsigned int  )0;
#line 138 "BaseOra.C"
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 138 "BaseOra.C"
    if (sqlca.sqlcode < 0) BaseOraErrHandler("SQLERROR");
#line 138 "BaseOra.C"
}

#line 138 "BaseOra.C"

    LOGL("OK");

    EXCFUNCEND;
}

void BaseOra::Rollback(void) const
{
    EXCFUNC(BaseOra::Rollback);

    LOGL(*this << ": ROLLBACK ...");
    /* EXEC SQL ROLLBACK; */ 
#line 149 "BaseOra.C"

{
#line 149 "BaseOra.C"
    struct sqlexd sqlstm;
#line 149 "BaseOra.C"
    sqlstm.sqlvsn = 12;
#line 149 "BaseOra.C"
    sqlstm.arrsiz = 6;
#line 149 "BaseOra.C"
    sqlstm.sqladtp = &sqladt;
#line 149 "BaseOra.C"
    sqlstm.sqltdsp = &sqltds;
#line 149 "BaseOra.C"
    sqlstm.iters = (unsigned int  )1;
#line 149 "BaseOra.C"
    sqlstm.offset = (unsigned int  )90;
#line 149 "BaseOra.C"
    sqlstm.cud = sqlcud0;
#line 149 "BaseOra.C"
    sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 149 "BaseOra.C"
    sqlstm.sqlety = (unsigned short)4352;
#line 149 "BaseOra.C"
    sqlstm.occurs = (unsigned int  )0;
#line 149 "BaseOra.C"
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 149 "BaseOra.C"
    if (sqlca.sqlcode < 0) BaseOraErrHandler("SQLERROR");
#line 149 "BaseOra.C"
}

#line 149 "BaseOra.C"

    LOGL("OK");

    EXCFUNCEND;
}

void BaseOra::Disconnect(void)
{
    EXCFUNC(BaseOra::Disconnect);

    LOGL(*this << ": ROLLBACK RELEASE ...");
    /* EXEC SQL ROLLBACK RELEASE; */ 
#line 160 "BaseOra.C"

{
#line 160 "BaseOra.C"
    struct sqlexd sqlstm;
#line 160 "BaseOra.C"
    sqlstm.sqlvsn = 12;
#line 160 "BaseOra.C"
    sqlstm.arrsiz = 6;
#line 160 "BaseOra.C"
    sqlstm.sqladtp = &sqladt;
#line 160 "BaseOra.C"
    sqlstm.sqltdsp = &sqltds;
#line 160 "BaseOra.C"
    sqlstm.iters = (unsigned int  )1;
#line 160 "BaseOra.C"
    sqlstm.offset = (unsigned int  )105;
#line 160 "BaseOra.C"
    sqlstm.cud = sqlcud0;
#line 160 "BaseOra.C"
    sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 160 "BaseOra.C"
    sqlstm.sqlety = (unsigned short)4352;
#line 160 "BaseOra.C"
    sqlstm.occurs = (unsigned int  )0;
#line 160 "BaseOra.C"
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 160 "BaseOra.C"
    if (sqlca.sqlcode < 0) BaseOraErrHandler("SQLERROR");
#line 160 "BaseOra.C"
}

#line 160 "BaseOra.C"

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

