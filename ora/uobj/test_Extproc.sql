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

--PROMPT NOTHING TO DO #################

/*################### EXTPROC OK

--SET TIMING ON;
SET ESCAPE ON;
SET LINESIZE 32000;

COL Cmd FOR A60;
COL Msg FOR A120;

--DEFINE Cmd='echo pid=$$($PPID) >> /tmp/extproc.tmp 2>\&1';
--DEFINE Cmd=/bin/date;
--DEFINE Cmd='/bin/echo \"pid=$$($PPID)}\" 2>>/tmp/extproc.$$ >> /tmp/extproc.$$';


SET DEFINE OFF;

DEFINE Cmd='echo "$(/bin/date +%y%m%d_%H%M%S): CMD pid=$$($PPID)" 2>&1 >>/tmp/extproc.tmp';
--DEFINE Cmd='echo "pid=$$($PPID)" 2>&1 >>/tmp/extproc.tmp; /bin/sleep 60';

VARIABL Cmd VARCHAR2(256)
VARIABLE Msg VARCHAR2(1024);

SET DEFINE ON;
EXEC :Cmd := '&Cmd'; Extproc.System(:Cmd, :Msg);
SELECT SYSDATE "Date", :Cmd "Cmd", :Msg "Msg" FROM DUAL;

PROMPT sleeping for 1 min. ...
HOST sleep 60

--oracle   15060     1  0 14:02 ?        00:00:00 extprocmyoradb (DESCRIPTION=(LOCAL=YES)(ADDRESS=(PROTOCOL=BEQ)))
--oracle   15061 15060  0 14:02 ?        00:00:00 sh -c echo "pid=$$($PPID)" 2>&1 >>/tmp/extproc.tmp; /bin/sleep 60

--pid=7707(7706)
--140411_164935: command=echo "pid=7708(7706)" 2>&1 >>/tmp/extproc.tmp ret=0
--oracle    7706     1  0 16:49 ?        00:00:00 extprocmyoradb (DESCRIPTION=(LOCAL=YES)(ADDRESS=(PROTOCOL=BEQ)))

--
--pid=15061(15060)
--140405_140303: command=echo "pid=15073(15060)" 2>&1 >>/tmp/extproc.tmp; /bin/sleep 60 ret=0

--140411_175331: CMD pid=8165(8164)
--140411_175331: command=echo "140411_175331: CMD pid=8168(8164)" 2>&1 >>/tmp/extproc.tmp ret=0

--################### EXTPROC END */

/*################### EXTPROC SOCK

--SET TIMING ON;
SET LINESIZE 32000;

VARIABLE Msg VARCHAR2(1024);
COL Msg FOR A100;

DECLARE
    trc type_Trace := type_Trace();
    Snd VARCHAR2(1024);
    Rcv VARCHAR2(1024);
BEGIN
    trc.Init('SQLP','SockSrv');
  trc.Put('############### BEGIN Listen on port 30000');
    <<Accept>>
    LOOP
        Extproc.SockSrv(30000, 'ACCEPT', Rcv);
        trc.Put('recived Rcv=' || Rcv);
        IF SUBSTR(Rcv, 1, 4) = 'EXIT' THEN
            EXIT Accept;
        END IF;
        Extproc.SockSrv(0, 'END', Rcv);
        trc.Put('recived Rcv=' || Rcv);
        Extproc.SockSrv(0, 'CLOSE', Rcv);
        trc.Put('##### END Rcv=' || Rcv);
    END LOOP Accept;

    :Msg := Rcv;
    trc.Put('############### END Msg=' || :Msg);
END;
/

--PROMPT sleeping for 2 min. ...
--HOST sleep 120

SELECT SYSDATE "Date", :Msg "Msg" FROM DUAL;

--################### EXTPROC SOCK END */

QUIT;
