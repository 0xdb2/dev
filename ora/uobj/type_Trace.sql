/*
$Header$
*/

SET ECHO ON;
SET VERIFY OFF;

DEFINE MaxLineSize=1999;

--DROP TYPE type_Trace;
--DROP TYPE type_TraceBase;

-- should executed AS DBA!!!
/*
    SET LINESIZE 32000;

BEGIN
    :SID# := SYS_CONTEXT('USERENV', 'SESSIONID');
    :TERM := SYS_CONTEXT('USERENV', 'TERMINAL');
END;
/

    SELECT * FROM V$SESSION WHERE AUDSID > 0;


---

    SELECT * FROM ALL_OBJECTS WHERE OBJECT_NAME = 'UTL_FILE' AND OBJECT_TYPE = 'PACKAGE';
    SELECT * FROM ALL_OBJECTS WHERE OBJECT_NAME = 'UTL_FILE' AND OBJECT_TYPE = 'SYNONYM';

    GRANT EXECUTE ON UTL_FILE TO PUBLIC;

    SELECT * FROM ALL_GRANTS WHERE
    OBJECT_NAME = 'UTL_FILE' AND OBJECT_TYPE = 'SYNONYM';
    ROWNUM=1;

    SELECT *  FROM all_tab_privs WHERE table_name = 'LOG_DIR';
    SELECT *
    from all_tab_privs
    where table_name in (
        select directory_name
        from dba_directories
        )
    ;

    DROP DIRECTORY LOG_DIR;

    CREATE DIRECTORY LOG_DIR AS '/vol/db/oralog';
    GRANT READ,WRITE ON DIRECTORY LOG_DIR TO PUBLIC;

    CREATE DIRECTORY LOG_DIR AS '/vol/db/oralog/hr';
    GRANT READ,WRITE ON DIRECTORY LOG_DIR TO HR;
  :s///gc

    SELECT * FROM ALL_OBJECTS WHERE OBJECT_TYPE = 'DIRECTORY';
    SELECT * FROM ALL_DIRECTORIES;
    SELECT DIRECTORY_NAME, SUBSTRB(DIRECTORY_PATH, 1, 60) path, SUBSTRB(o.OWNER, 1, 10) Owner, CREATED, STATUS FROM ALL_DIRECTORIES d, ALL_OBJECTS o
    WHERE OBJECT_TYPE = 'DIRECTORY'
    AND DIRECTORY_NAME = OBJECT_NAME;

    SELECT OBJECT_TYPE,  COUNT(1) FROM ALL_OBJECTS GROUP BY OBJECT_TYPE ORDER BY OBJECT_TYPE;

    UPDATE Tab SET ....
    SHOW  EXITCOMMIT
*/

--DROP TYPE type_FileFlags;

--###########################################################################
--CREATE OR REPLACE TYPE type_FileFlags AS OBJECT (
--###########################################################################
--$Revision$
--
--    CDate DATE,
--    flush CHAR, --Y/N
--    CONSTRUCTOR FUNCTION type_FileFlags RETURN self AS RESULT
--) --END type_FileFlags
--/
--L
--SHOW ERRORS;

--###########################################################################
--CREATE OR REPLACE TYPE BODY type_FileFlags AS
--###########################################################################
--$Revision$
--
--    CONSTRUCTOR FUNCTION type_FileFlags RETURN self AS RESULT IS
--    BEGIN
--        self.CDate := NULL;
--        flush := ''; --Y/N
--
--    END type_FileFlags;
--END;
--/
--L
--SHOW ERRORS;

--###########################################################################
--DROP PACKAGE packFile;
CREATE OR REPLACE PACKAGE packFile AS
--###########################################################################

    SUBTYPE FileHndlType IS UTL_FILE.FILE_TYPE;
    TYPE FileType IS RECORD (
        Path VARCHAR2(80),
        Name VARCHAR2(40),
        Hndl FileHndlType,
        Bytes NUMBER,
        MaxBytes NUMBER,
        AutoFlush BOOLEAN,
        CDate DATE
        );
    FUNCTION getFile (fileId IN OUT NUMBER) RETURN FileType;
    PROCEDURE setFile (fileId IN NUMBER, file IN FileType);

    PROCEDURE setHndlr(hndlr IN type_TraceHndlr);
    --PROCEDURE setHndlr(hndlr IN OUT NOCOPY type_TraceHndlr);
    FUNCTION getHndlr RETURN type_TraceHndlr;
END packFile;
/
L
SHOW ERRORS;

--###########################################################################
CREATE OR REPLACE PACKAGE BODY packFile AS
--###########################################################################

    TYPE FileTabType IS TABLE OF FileType INDEX BY PLS_INTEGER;
    FileTab FileTabType;
    myHndlr type_TraceHndlr;

    FUNCTION getFile (fileId IN OUT NUMBER) RETURN FileType IS
    BEGIN
        --DBMS_OUTPUT.PUT_LINE ('pack_File.getFile(' || fileId || ')');

        IF fileId IS NULL THEN
           fileId := FileTab.COUNT + 1;
           FileTab(fileId).Bytes := 0;
           FileTab(fileId).MaxBytes := 1024;
           FileTab(fileId).AutoFlush := FALSE;
        END IF;

        RETURN FileTab(fileId);
    END getFile;

    PROCEDURE setFile (fileId IN NUMBER, file IN FileType) IS
    BEGIN
        FileTab(fileId) := file;
    END;

    PROCEDURE setHndlr(hndlr IN type_TraceHndlr) IS
    BEGIN
        myHndlr := hndlr;
    END;

    FUNCTION getHndlr RETURN type_TraceHndlr IS
    BEGIN
        RETURN NVL(myHndlr, type_TraceHndlr('SQLP'));
    END;

END packFile;
/
L
SHOW ERRORS;

--QUIT;

--DROP TYPE type_Trace;
--DROP TYPE type_Trace;
--DROP TYPE type_TraceHndlr;
--DROP TYPE type_TraceBase;

--###########################################################################
CREATE OR REPLACE TYPE type_TraceStack FORCE AS TABLE OF VARCHAR2(40);
/
--###########################################################################

--###########################################################################
CREATE OR REPLACE TYPE type_TraceBase FORCE AS OBJECT (
--###########################################################################
--$Revision$
------- Attributes
    typeName VARCHAR2(30),
    fileId NUMBER,

------- Methods
    FINAL STATIC FUNCTION processFile (
        actionType IN VARCHAR2,
        fileId IN OUT NUMBER,
        inPath IN VARCHAR2 DEFAULT 'LOG_DIR',
        inFileName IN VARCHAR2  DEFAULT NULL,
        data IN VARCHAR2 DEFAULT NULL
       ) RETURN VARCHAR,
    NOT INSTANTIABLE MEMBER FUNCTION getName (Func IN VARCHAR2) RETURN VARCHAR2
) NOT FINAL NOT INSTANTIABLE; --END SPEC type_BaseTrace SPEC
/
L
SHOW ERRORS;

--###########################################################################
CREATE OR REPLACE TYPE BODY type_TraceBase AS
--###########################################################################
--$Revision$

    FINAL STATIC FUNCTION processFile (
        actionType IN VARCHAR2,
        fileId IN OUT NUMBER,
        inPath IN VARCHAR2 DEFAULT 'LOG_DIR',
        inFileName IN VARCHAR2  DEFAULT NULL,
        data IN VARCHAR2 DEFAULT NULL
    ) RETURN VARCHAR AS

        --Path VARCHAR2(80);
        --fileName VARCHAR2(40);
        --FileHandle UTL_FILE.FILE_TYPE;
        file packFile.FileType := packFile.getFile(fileId);
        Ret VARCHAR2(10) := 'UNDEF';

        FUNCTION getFileName (file IN packFile.FileType) RETURN VARCHAR2 IS
        BEGIN
            RETURN file.Name || '_' || TO_CHAR(file.CDate, 'YYMMDD_HH24MISS') || '.trc';
        END getFileName;

    BEGIN
        --DBMS_OUTPUT.PUT_LINE ('type_TraceBase.processFile(' || actionType ||') file(' || fileId || ')=' || file.Path || '/' || file.Name);

        CASE actionType
        WHEN 'INIT' THEN
            file.Name := inFileName;
            file.Path := inPath;
            packFile.setFile(fileId, file);
            Ret := 'OK';
        WHEN 'IS_OPEN' THEN
            IF UTL_FILE.IS_OPEN(file.Hndl) THEN
                Ret := 'Y';
            ELSE
                Ret := 'N';
            END IF;
        WHEN 'OPEN' THEN
            <<OPEN>>
            DECLARE
                fileName VARCHAR(80);
            BEGIN
                file.CDate := SYSDATE;
                fileName := getFileName(file);
                file.Hndl := UTL_FILE.FOPEN (file.Path, fileName, 'a');
                packFile.setFile(fileId, file);
                Ret := 'OK';
                DBMS_OUTPUT.PUT_LINE ('Trace.Open()=' || Ret || ':file=' || file.Path || '/' || fileName);
            EXCEPTION
            WHEN OTHERS THEN
                RAISE_APPLICATION_ERROR(-20000, 'TraceBase.processFile-OPEN: file=' || file.Path || '/' || fileName || '(' || SQLERRM(SQLCODE) || ')');
            END;
        WHEN 'PUTL' THEN
            <<PUTL>>
            DECLARE
                TStamp TIMESTAMP(3) := SYSTIMESTAMP;
                TimeStampPrefix VARCHAR2(20) := TO_CHAR(TStamp, 'HH24:MI:SSXFF3');
                data VARCHAR2(1999);
                DataLen NUMBER := LENGTHB(processFile.data) + 1;
            BEGIN
                IF ((file.Bytes + DataLen) > file.MaxBytes) OR (TRUNC(TStamp) > TRUNC(file.CDate)) THEN
                <<SwitchTraceFile>>
                BEGIN
                    UTL_FILE.PUT_LINE(file.Hndl, TimeStampPrefix || ' ' || ' ****** closed ******', AutoFlush=>TRUE);
                    UTL_FILE.FCLOSE (file.Hndl);
                    file.CDate := SYSDATE;
                    file.Bytes := 0;
                    file.Hndl := UTL_FILE.FOPEN (file.Path, getFileName(file), 'a');
                    UTL_FILE.PUT_LINE(file.Hndl, TimeStampPrefix || ' ' || ' ****** opened ******', AutoFlush=>TRUE);
                END SwitchTraceFile; END IF;

                Putl.data := TimeStampPrefix || ' ';
--                IF self.stackIdx > 0 THEN
--                    Putl.data := Putl.data || LPAD( '|  ', self.stackIdx * 3, '|  ' ) || stack(self.stackIdx) || ': ';
--                END IF;
                Putl.data := Putl.data || processFile.data;

                Putl.data := TimeStampPrefix || ' ' || processFile.data || '(' || file.Bytes || ')';
                UTL_FILE.PUT_LINE(file.Hndl, Putl.data, file.AutoFlush);
                file.Bytes := file.Bytes + LENGTHB(Putl.data);
                packFile.setFile(fileId, file);
                Ret := 'OK';
            EXCEPTION
            WHEN OTHERS THEN
                RAISE_APPLICATION_ERROR(-20000, 'TraceBase.processFile-PUTL: file=' || file.Path || '/' || file.Name || '(' || SQLERRM(SQLCODE) || ')');
            END PUTL;

        WHEN 'FLUSH' THEN
            <<FLUSH>>
            BEGIN
                --DBMS_OUTPUT.PUT_LINE ('TraceBase.processFile(' || actionType ||',data=' || data || ')');
                CASE UPPER(data)
                WHEN 'ON' THEN
                    file.AutoFlush := TRUE;
                    packFile.setFile(fileId, file);
                WHEN 'OFF' THEN
                    file.AutoFlush := FALSE;
                    packFile.setFile(fileId, file);
                ELSE NULL;
                END CASE;
                IF file.AutoFlush OR data IS NULL THEN UTL_FILE.FFLUSH(file.Hndl); END IF;
                Ret := CASE file.AutoFlush WHEN TRUE THEN 'ON' ELSE 'OFF' END;
            END FLUSH;
        WHEN 'CLOSE' THEN
            UTL_FILE.FCLOSE (file.Hndl);
        ELSE
            RAISE_APPLICATION_ERROR(-20000, 'TraceBase.processFile: INVALID_OPERATION actionType=' || actionType);
        END CASE;

        --DBMS_OUTPUT.PUT_LINE ('type_TraceBase.processFile(' || actionType ||')=' || Ret || ' file(' || fileId || ')=' || file.Path || '/' || file.Name || '(' || file.Bytes || ')');

        RETURN Ret;

    END processFile;

END;
/
L
SHOW ERRORS;

--###########################################################################
CREATE OR REPLACE TYPE type_TraceHndlr FORCE UNDER type_TraceBase (
--###########################################################################
--$Revision$

------- Attributes
    stack type_TraceStack,
    stackIdx NUMBER,
    Line VARCHAR2(1999),
    Ret VARCHAR(10),

------- Methods
    --NOT INSTANTIABLE MEMBER FUNCTION getName RETURN VARCHAR2,
    --MEMBER FUNCTION getName RETURN VARCHAR2,
    CONSTRUCTOR FUNCTION type_TraceHndlr RETURN self AS RESULT,
    CONSTRUCTOR FUNCTION type_TraceHndlr(ApplName IN VARCHAR2) RETURN self AS RESULT,
    OVERRIDING MEMBER FUNCTION getName (Func IN VARCHAR2 DEFAULT NULL) RETURN VARCHAR2,
    MEMBER FUNCTION  Opened RETURN BOOLEAN,
    MEMBER PROCEDURE Open,
    MEMBER PROCEDURE msg(data IN VARCHAR2),
    MEMBER PROCEDURE put(data IN VARCHAR2, Id IN NUMBER DEFAULT 0),
    MEMBER FUNCTION push(self IN OUT type_TraceHndlr, Name IN VARCHAR2, data IN VARCHAR2) RETURN NUMBER,
    MEMBER PROCEDURE pop(data IN VARCHAR2, Id NUMBER DEFAULT 0),
    MEMBER PROCEDURE flush(setAuto IN VARCHAR2),
    MEMBER PROCEDURE Close
) FINAL; -- END type_Trace SPEC
/
L
SHOW ERRORS;

--###########################################################################
CREATE OR REPLACE TYPE BODY type_TraceHndlr AS
--###########################################################################
--$Revision$
--SUBTYPE _RowType IS TabName%ROWTYPE;
    CONSTRUCTOR FUNCTION type_TraceHndlr RETURN self AS RESULT IS
    BEGIN
        self.stackIdx := 0;
        self.stack := type_TraceStack();
        self.Line := 'empty';
        RETURN;
    END type_TraceHndlr;

    CONSTRUCTOR FUNCTION type_TraceHndlr(ApplName IN VARCHAR2) RETURN self AS RESULT IS
    BEGIN
        self.stackIdx := 0;
        self.stack := type_TraceStack();
        self.Line := 'empty';
        --self.fileName := fileName;
        Ret := type_TraceBase.processFile('INIT', fileId, inFileName=>ApplName);
        --DBMS_OUTPUT.PUT_LINE ('type_TraceHndlr(' || ApplName || ') fileId=' || fileId);
        packFile.setHndlr(self);
        RETURN;
    END type_TraceHndlr;

    OVERRIDING MEMBER FUNCTION getName (Func IN VARCHAR2 DEFAULT NULL) RETURN VARCHAR2 IS
    BEGIN
        RETURN CASE Func WHEN NULL THEN 'type_TraceHndlr' ELSE 'Trace.' || Func END;
    END getName;

    MEMBER FUNCTION Opened RETURN BOOLEAN IS
        fileId NUMBER := self.fileId;
    BEGIN
        RETURN (type_TraceBase.processFile('IS_OPEN', fileId) = 'Y');
    EXCEPTION
        WHEN UTL_FILE.INVALID_OPERATION THEN
            RETURN(FALSE);
    END Opened;

    MEMBER PROCEDURE Open IS
        myName CONSTANT VARCHAR2(40) := 'Trace.Open';
    BEGIN
        Ret := type_TraceBase.processFile('OPEN', fileId);
    EXCEPTION
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20000, myName || ':' || SQLERRM(SQLCODE));
    END Open;

    MEMBER FUNCTION push(self IN OUT type_TraceHndlr, Name IN VARCHAR2, data IN VARCHAR2) RETURN NUMBER IS
        myName VARCHAR2(40) := getName('push');
    BEGIN
        --msg('type_TraceHndlr.push(' || Name || ') StackId=' || stackIdx);
        stack.EXTEND;
        stackIdx := stack.COUNT; -- ++
        stack(stackIdx) := Name;
        packFile.setHndlr(self);
        put(data, stackIdx);
        --msg('type_TraceHndlr.stack(' || stackIdx || ')=' || stack(stackIdx) || '.');
        RETURN stackIdx; --stack(stackIdx);
    EXCEPTION
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20000, myName || ':' || SQLERRM(SQLCODE));
    END push;

    MEMBER PROCEDURE put(data IN VARCHAR2, Id IN NUMBER DEFAULT 0) IS
    BEGIN
    <<My>>
    DECLARE
        --myName VARCHAR2(40) := self.push('put');
        Name CONSTANT VARCHAR2(40) := 'TraceBase.put';
        data VARCHAR2(1999);
    BEGIN
        --msg('put(Id=' || Id ||') stackIdx=' || stackIdx);
        IF NOT self.Opened THEN self.Open; END IF;

        IF stackIdx > Id THEN
            stack.DELETE(Id+1, stackIdx);
            stackIdx := Id;
            packFile.setHndlr(self);
        END IF;

        IF stackIdx > 1 THEN
            My.data := LPAD( '|  ', (self.stackIdx-1) * 3, '|  ' ) || stack(self.stackIdx) || ':' || put.data;
        ELSIF stackIdx = 1 THEN
            My.data := stack(self.stackIdx) || ':' || put.data;
        ELSE
            My.data := put.data;
        END IF;
        Ret := type_TraceBase.processFile('PUTL', fileId, data=>My.data);

    EXCEPTION
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20000, My.Name || ':' || SQLERRM(SQLCODE));
    END My;
    END put;

    MEMBER PROCEDURE msg(data IN VARCHAR2) IS --TODO implement in base
        myName CONSTANT VARCHAR2(40) := 'Trace.msg';
    BEGIN
        DBMS_OUTPUT.PUT_LINE(msg.data);
    EXCEPTION
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20000, myName || ':' || SQLERRM(SQLCODE));
    END msg;

    MEMBER PROCEDURE pop(data IN VARCHAR2, Id NUMBER DEFAULT 0) IS
        myName VARCHAR2(40) := getName() || '.' || 'pop';
    BEGIN
        --msg('type_TraceHndlr.pop(' || Id || ') stackIdx=' || stackIdx);
        IF stackIdx > Id THEN
            stack.DELETE(Id+1, stackIdx);
            stackIdx := Id;
            packFile.setHndlr(self);
        END IF;

        put(data, Id);

        IF stackIdx > 0 THEN
            stack.DELETE(stackIdx);
            stackIdx := stackIdx - 1;
            packFile.setHndlr(self);
        ELSE
            RAISE_APPLICATION_ERROR(-20000, myName || ': empty stack! stackIdx=' || stackIdx || ',stack.COUNT=' || stack.COUNT);
        END IF;
    EXCEPTION
    WHEN OTHERS THEN

        RAISE_APPLICATION_ERROR(-20000, myName || ':' || SQLERRM(SQLCODE));
    END pop;

    MEMBER PROCEDURE flush(setAuto IN VARCHAR2) IS
        myName VARCHAR2(40) := getName('flush');
    BEGIN
        Ret := type_TraceBase.processFile('FLUSH', fileId, data=>setAuto);
    --EXCEPTION
    --WHEN OTHERS THEN
    --    RAISE_APPLICATION_ERROR(-20000, myName || ':' || SQLERRM(SQLCODE));
    END flush;

    MEMBER PROCEDURE Close IS
    BEGIN
        Ret := type_TraceBase.processFile('CLOSE', fileId);
    END Close;

END;
/
L
SHOW ERRORS;

--###########################################################################
CREATE OR REPLACE TYPE type_Trace UNDER type_TraceBase (
--CREATE OR REPLACE TYPE type_Trace AS OBJECT (
--###########################################################################
--$Revision$

------- Attributes
    hndlr type_TraceHndlr,
    myName VARCHAR2(132),
    myId NUMBER,
    Line VARCHAR2(1999),
    Ret VARCHAR(10),

------- Methods
    --CONSTRUCTOR FUNCTION type_Trace RETURN self AS RESULT,
    CONSTRUCTOR FUNCTION type_Trace(funcName IN VARCHAR2) RETURN self AS RESULT,
    OVERRIDING MEMBER FUNCTION getName (Func IN VARCHAR2 DEFAULT NULL) RETURN VARCHAR2,
    MEMBER PROCEDURE msg(data IN VARCHAR2),
    MEMBER PROCEDURE put(data IN VARCHAR2),
    --MEMBER FUNCTION push(self IN OUT type_Trace, Name IN VARCHAR2, data IN VARCHAR2) RETURN VARCHAR2,
    MEMBER PROCEDURE pop(data IN VARCHAR2)
    --MEMBER PROCEDURE flush(setAuto IN VARCHAR2),
    --MEMBER PROCEDURE Close
) FINAL; -- END type_Trace SPEC
/
L
SHOW ERRORS;

--###########################################################################
CREATE OR REPLACE TYPE BODY type_Trace AS
--###########################################################################
--$Revision$

    CONSTRUCTOR FUNCTION type_Trace(funcName IN VARCHAR2) RETURN self AS RESULT IS
    BEGIN
        self.Line := 'empty';
        self.myName := funcName;
        self.hndlr := packFile.getHndlr();
        --hndlr.msg('type_Trace() Id=' || self.myId || '/' || hndlr.stackIdx);
        self.myId := self.hndlr.push(funcName, 'start');
        RETURN;
    END type_Trace;

    OVERRIDING MEMBER FUNCTION getName (Func IN VARCHAR2 DEFAULT NULL) RETURN VARCHAR2 IS
    BEGIN
        RETURN CASE Func WHEN NULL THEN 'type_Trace' ELSE 'Trace.' || Func END;
    END getName;

    MEMBER PROCEDURE put(data IN VARCHAR2) IS
    BEGIN
        --hndlr.msg('Trace2.put() Id=' || self.myId || '/' || hndlr.stackIdx);
        hndlr.put(data, myId);
    END put;

    MEMBER PROCEDURE msg(data IN VARCHAR2) IS
        myName CONSTANT VARCHAR2(40) := 'Trace.msg';
    BEGIN
        DBMS_OUTPUT.PUT_LINE(msg.data);
    EXCEPTION
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20000, myName || ':' || SQLERRM(SQLCODE));
    END msg;

   MEMBER PROCEDURE pop(data IN VARCHAR2) IS
        myName VARCHAR2(40) := getName() || '.' || 'pop';
    BEGIN
        hndlr.pop(data, myId);
    END pop;

END;
/
L
SHOW ERRORS;
--SET LINESIZE 1000;
--SET MAXDATA 60000;
--SET ARRAYSIZE 1;
--select line, RTRIM(LTRIM(text)) Source from user_source where type = 'TYPE' and name = UPPER('type_Trace');
--name = 'type_Trace';

--CREATE OR REPLACE SYNONYM type_Trace FOR hr.type_Trace;
--GRANT EXECUTE ON New TO xxxxx;

QUIT;

