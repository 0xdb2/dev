/*
$Id:  $
*/

--DROP PACKAGE Bas;

--###########################################################################
CREATE OR REPLACE PACKAGE Bas AUTHID DEFINER AS
--###########################################################################
--$Revision$

FUNCTION Exec (
    FuncName IN VARCHAR2,
    InPar IN VARCHAR2,
    OutPar OUT VARCHAR2,
    Msg OUT VARCHAR2
    ) RETURN VARCHAR2; -- return-values (e.g. OK/NOK/ERR)

END Bas;
/
L
SHOW ERRORS;

--###########################################################################
CREATE OR REPLACE PACKAGE BODY Bas AS
--###########################################################################
--$Revision$

--SUBTYPE _RowType IS TabName%ROWTYPE;

-----------------------------------------------------------------------------

FUNCTION Exec (
    FuncName IN VARCHAR2,
    InPar IN VARCHAR2,
    OutPar OUT VARCHAR2,
    Msg OUT VARCHAR2
    ) RETURN VARCHAR2 IS -- return-values (e.g. OK/NOK/ERR)
-----------------------------------------------------------------------------

    Ret VARCHAR2(20) := 'UNDEF';

BEGIN
    --trc.push('Bas.Exec', '##### FuncName=' || FuncName || ',InPar=' || InPar || ': TODO');

    --trc.pop;
    OutPar := 'OutKey=OutVal|';
    Msg    := 'Bas.Exec: FuncName=' || FuncName || '(InPar=' || InPar || ',OutPar=' || OutPar || ')=' || Ret;
    RETURN Ret;

EXCEPTION
WHEN OTHERS THEN
    --trc( 'EXCEPTION! ' || SQLERRM( SQLCODE ) );
    --trc.pop;
    RAISE;
END Exec;

END Bas;
/
L
SHOW ERRORS;

--CREATE OR REPLACE PUBLIC SYNONYM Bas FOR HR.Bas;
--GRANT EXECUTE ON Bas TO xxxxx;

--SET TIMING ON;
--SET LINESIZE 32000;
--COL Cmd FOR A60;
--COL Msg FOR A100;
--DEFINE Cmd='echo pid=$$($PPID) >> /tmp/extproc.tmp 2>\&1';
--VARIABLE Msg VARCHAR2(1024);
--exec :Cmd:='&Cmd'; Bas.System(:Cmd, :Msg);
--SELECT :Cmd "Cmd", :Msg "Msg" FROM DUAL;

QUIT;

