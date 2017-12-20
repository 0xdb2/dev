/*
 * $Header$
 */

/* ##   sqlplus /nolog  # CONNECT / AS SYSDBA # SET LINESIZE 32000;

    CREATE OR REPLACE LIBRARY Shell AS '/home/db/rel/.lib/shell.so';
    DROP LIBRARY Shell;

    CREATE OR REPLACE LIBRARY SockSrv AS '/home/db/rel/.lib/socksrv.so';
    DROP LIBRARY HR.SockSrv;

    CREATE OR REPLACE LIBRARY HR.Socket AS '/home/db/rel/.lib/libSocket.so';
    --DROP LIBRARY HR.Socket;

    COL FILE_SPEC FOR A60;
    SELECT * FROM ALL_LIBRARIES WHERE LIBRARY_NAME LIKE 'S%';

    DROP PACKAGE Extproc;

    oracle@mpsrv:/vol/db/app/oracle/product/11.2.0/home/hs/admin$ view extproc.ora
    SET EXTPROC_DLLS=/home/db/prj/.lib/shell.so:/home/db/prj/.lib/socksrv.so

*/

--###########################################################################
CREATE OR REPLACE PACKAGE Extproc AUTHID DEFINER AS
--###########################################################################
--$Revision$

PROCEDURE System (
    Command IN VARCHAR2,
    Msg     OUT VARCHAR2
    )
    AS LANGUAGE C
    LIBRARY Shell NAME "shell"
    --WITH CONTEXT
    --    PARAMETERS(CONTEXT, Command STRING, Msg STRING);
        PARAMETERS(Command STRING, Msg STRING);

PROCEDURE SockSrv (
    Port IN BINARY_INTEGER,
    Snd  IN VARCHAR2,
    Rcv  OUT VARCHAR2
    )
    AS LANGUAGE C
    LIBRARY HR.Socket NAME "SockSrv"
    --WITH CONTEXT
    --    PARAMETERS(CONTEXT, Command STRING, Msg STRING);
        PARAMETERS(Port INT, Snd STRING, Rcv STRING);

END Extproc;
/
L
SHOW ERRORS;

--CREATE OR REPLACE PUBLIC SYNONYM Extproc FOR HR.Extproc;
--GRANT EXECUTE ON Extproc TO xxxxx;

--PROMPT NOTHING TO DO #################

-- AS SYSDBA
--DROP PUBLIC SYNONYM SOCKSRV;
--CREATE OR REPLACE LIBRARY HR.SockSrv AS '/home/db/rel/.lib/socksrv.so';
--SQL> col FILE_SPEC for A40
--SQL> SELECT * FROM ALL_LIBRARIES WHERE LIBRARY_NAME like 'S%';
--OWNER                          LIBRARY_NAME                   FILE_SPEC                                D STATUS
-------------------------------- ------------------------------ ---------------------------------------- - -------
--SYS                            SOCKSRV                        /home/db/rel/.lib/socksrv.so             Y VALID

QUIT;
