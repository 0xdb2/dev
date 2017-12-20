/*
$Header$
*/

--ALTER SESSION SET NLS_DATE_FORMAT='YYMMDD_HH24MISS';
--PROMPT NOTHING TO DO #################

--/* ################### Trace
SET SERVEROUTPUT ON;
SET LINESIZE 400;

CREATE OR REPLACE PACKAGE test_trc AS
    FUNCTION packFunc(msg IN VARCHAR2) RETURN VARCHAR2;
    PROCEDURE packProc(msg IN VARCHAR2);
END test_trc;
/
L
SHOW ERRORS;

CREATE OR REPLACE PACKAGE BODY test_trc AS

    FUNCTION packFunc(msg IN VARCHAR2) RETURN VARCHAR2 AS
        trc type_Trace := type_Trace('packFunc');
    BEGIN
        trc.put(msg || ': ret=OK');
        RETURN 'OK';
    END packFunc;

    PROCEDURE packProc(msg IN VARCHAR2) AS
        trc type_Trace := type_Trace('packProc');
    BEGIN
        trc.put(msg);
        raise_application_error(-20001, 'test_trc.packProc: exception');
    END packProc;

END test_trc;
/
L
SHOW ERRORS;

DECLARE

    trc_hndlr type_TraceHndlr := type_TraceHndlr('SQLP');
    trc type_Trace := type_Trace('anonymous');
    Ret VARCHAR(10);

    PROCEDURE Func3;
    PROCEDURE Func2;

    PROCEDURE Func1 IS
        --MyName VARCHAR2(40) := trc.push('Func1', 'begin func');
        trc type_Trace := type_Trace('Func1');
    BEGIN
        trc.put('in func');
        func2;
        trc.pop('end of func');
    EXCEPTION
    WHEN OTHERS THEN
        trc.pop('except in func ' || SQLERRM(SQLCODE));
        RAISE;
    END Func1;

    PROCEDURE Func2 IS
        --MyName VARCHAR2(40) := trc.push('Func2', 'begin func');
        trc type_Trace := type_Trace('Func2');
    BEGIN
        trc.put('in func');
        --DBMS_LOCK.SLEEP(1);
        func3;
        trc.pop('end of func');
    EXCEPTION
    WHEN OTHERS THEN
        --trc.pop('except in func ' || SQLERRM(SQLCODE));
        RAISE;
    END Func2;

    PROCEDURE Func3 IS
        --MyName VARCHAR2(40) := trc.push('Func3', 'begin func');
        trc type_Trace := type_Trace('Func3');
    BEGIN
        trc.put('in func');
        test_trc.packProc('in packaged proc');
        --trc.pop('end of func');
    EXCEPTION
    WHEN OTHERS THEN
        --trc.pop('except in func ' || SQLERRM(SQLCODE));
        RAISE;
    END Func3;

BEGIN
    --trc.Init('SQLP','file_Tst');
    --trc.Open();
    --trc.push

    trc.put('############### first trace line');
    --trc.put(CASE trc.Opened WHEN TRUE THEN 'opened:' ELSE 'closed:' END || 'my trace');
    trc.put('line 1 aaa bbb ccc');
    trc_hndlr.Flush(setAuto=>'on');
    FOR idx IN 2..20 LOOP
        trc.put('aaa bbb ccc line=' || idx);
    END LOOP;
    func1;
    ret := test_trc.packFunc('in packaged func');
    trc.put('############### my last trace line');
    EXCEPTION
    WHEN OTHERS THEN
        trc.pop('except in func ' || SQLERRM(SQLCODE));
END;
/

DROP PACKAGE test_trc;

/*
ORA-29283: invalid file operation

    EXEC  HR.pack_File.FileHandle := UTL_FILE.FOPEN ('LOG_DIR', 'test.trc', 'w');

SELECT * FROM V$PARAMETER WHERE NAME = 'utl_file_dir';

-- */

--################### Trace END */


QUIT;

