-- publish
-- :!sqlplus db/myoradb\$db @orderman

--################################################################### create tables

--DROP TABLE Customers CASCADE CONSTRAINTS;
--
--CREATE TABLE Customers (
--    CustNo NUMBER(3) NOT NULL,
--    CustName VARCHAR2(30) NOT NULL,
--    Street VARCHAR2(20) NOT NULL,
--    City VARCHAR2(20) NOT NULL,
--    State CHAR(2) NOT NULL,
--    Zip VARCHAR2(10) NOT NULL,
--    Phone VARCHAR2(12),
--    PRIMARY KEY (CustNo)
--);
--

--################################################################### publishing

/*
:DB db/myoradb\\$db

VARIABLE CompiledCnt NUMBER;
CALL DBMS_JAVA.compile_class('orderman') INTO :CompiledCnt;
PRINT CompiledCnt;

    SELECT * FROM user_objects WHERE object_name = dbms_java.shortname('orderman') OR object_name = UPPER('orderman');

    SELECT cast(object_name as varchar2(20)), object_type FROM user_objects;

    COL object_name format a30
    COL object_type format a15
    SELECT OBJECT_NAME,	OBJECT_ID, OBJECT_TYPE,	CREATED, LAST_DDL_TIME, TIMESTAMP, STATUS, NAMESPACE
    FROM user_objects WHERE object_type LIKE 'JAVA%' ORDER BY object_type, object_name;

    select * from javasnm;

*/

CREATE OR REPLACE PACKAGE Form AS
    PROCEDURE init(logfile VARCHAR2) AS LANGUAGE JAVA
    NAME 'FormDb.init(java.lang.String)'
    ;

    PROCEDURE setDataSet(sqlStmt VARCHAR2) AS LANGUAGE JAVA
    NAME 'FormDb.setDataSet(java.lang.String)'
    ;

    PROCEDURE getOutput AS LANGUAGE JAVA
    NAME 'FormDb.getOutput()'
    ;

END form;
/
SHOW ERRORS;

--CREATE OR REPLACE PACKAGE BODY Form AS
--    PROCEDURE init(logfile VARCHAR2) AS LANGUAGE JAVA
--    NAME 'FormDb.init(java.lang.String)'
--    ;
--
--    PROCEDURE setDataSet(sql VARCHAR2) AS LANGUAGE JAVA
--    NAME 'FormDb.setDataSet(java.lang.String)'
--    ;
--
--END form;
--/
--SHOW ERRORS;

QUIT;
